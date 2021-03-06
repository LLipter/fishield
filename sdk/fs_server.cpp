#include "fs_server.h"
#include "fs_dbmanager.h"

boost::asio::io_service service;
short _port = DEFAULT_SERV_PORT;
std::string rootdir = DEFAULT_ROOT_DIR;
std::string hidden_prefix = DEFAULT_HIDDEN_PREFIX;
std::string taskid_path = std::string(".") + SEPARATOR + DEFAULT_TASKID_FILE;
std::vector<server_ptr> clients;
std::map<int,fs::proto::Task> tasks;
std::mutex server_task_mutex;
std::mutex server_taskid_mutex;
std::map <std::string, fs::proto::User> token_map;
std::mutex token_mutex;


int task_id;
fs_server::fs_server():_sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    _is_stop = false;
    last_request = std::time(0);
}

boost::asio::ip::tcp::socket& fs_server::sock(){
    return _sock;
}

bool fs_server::is_stop(){
    return _is_stop;
}

void fs_server::set_stop(bool status){
    _is_stop = status;
}

bool fs_server::time_out(){
    return std::time(0) - last_request > DEFAULT_CLIENT_TIMEOUT;
}

void fs_server::init(){
    using namespace boost::filesystem;
    if(!exists(rootdir))
        create_directories(rootdir);
    if(!exists(taskid_path)){
        std::ofstream file(taskid_path);
        file << 0;
        file.close();
    }
    std::ifstream file(taskid_path);
    file >> task_id;
    file.close();

}

bool fs_server::receive_request(fs::proto::Request& request){
    using namespace fs::proto;

    // read length of request
    int len;
    boost::system::error_code err;
    boost::asio::read(_sock,boost::asio::buffer((char*)&len, 4), err);

    if(err == boost::asio::error::eof)  // connection closed by other side
        return false;
    if(err){
        std::cout << "receive_request() read len failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // read request
    char* buf = new char[len];
    boost::asio::read(_sock, boost::asio::buffer(buf, len), err);
    if(err){
        delete[] buf;
        std::cout << "receive_request() read request failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // deserialize request
    if(request.ParseFromArray(buf, len) == false){
        delete[] buf;
        std::cout << "receive_request() deserialize request failed : "
                  << std::endl;
        return false;
    }
    delete[] buf;

    std::cout << "receive_request() success : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;
    last_request = std::time(0);
    return true;
}

bool fs_server::send_response(const fs::proto::Response& response){
    using namespace fs::proto;
    int len = response.ByteSize();
    char* buf = new char[len+4];
    *(int*)buf = len;
    response.SerializeToArray(buf+4, len);
    boost::system::error_code err;
    boost::asio::write(_sock, boost::asio::buffer(buf,len+4), err);
    delete[] buf;
    if(err){
        std::cout << "send_response() failed : "
                  << err.message()
                  << std::endl;
        return false;
    }
    std::cout << "send_response() success : "
              << Response::ResponseType_Name(response.resp_type())
              << std::endl;
    return true;
}

void accept_thread() {
    typedef boost::asio::ip::tcp::acceptor acceptor;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    acceptor acptor(service, endpoint(boost::asio::ip::tcp::v4(), _port));
    while (true) {
        server_ptr serptr(new fs_server);
        acptor.accept(serptr->sock());
        clients.push_back(serptr);
        std::thread thd(communicate_thread, serptr);
        thd.detach();
    }
}


void remove_clients_thread() {
    while (true) {
        // have a sleep
        boost::this_thread::sleep(DEFAULT_REMOVE_SLEEP_TIME);

        // erase clients that are stoped
        clients.erase(std::remove_if(clients.begin(),
                                     clients.end(),
                                     boost::bind(&fs_server::is_stop,_1)),
                      clients.end());
        // erase clients that are timeout
        clients.erase(std::remove_if(clients.begin(),
                                     clients.end(),
                                     boost::bind(&fs_server::time_out,_1)),
                      clients.end());
        // erase tasks that are finished or timeout
        server_task_mutex.lock();
        std::vector<int> should_removed;
        for(auto it=tasks.begin();it!=tasks.end();it++){
            using namespace fs::proto;
            Task& task = it->second;
            if(is_timeout(task)
                    || task.task_status() == Task::UPLOADED
                    || task.task_status() == Task::DOWNLOADED
                    || task.task_status() == Task::CANCELED
                    || task.task_status() == Task::FAILED){
                should_removed.push_back(it->first);
                fs_DBManager manager;
                manager.update_task(task);
            }
        }
        for(int id : should_removed)
            tasks.erase(id);
        server_task_mutex.unlock();

        // erase timeout token
        token_mutex.lock();
        std::vector<std::string> tokens_remove;
        for(auto it=token_map.begin();it!=token_map.end();it++){
            fs::proto::User& user = it->second;
            if(std::time(0) - user.last_packet_time() > DEFAULT_TOKEN_TIMEOUT)
                tokens_remove.push_back(it->first);
        }
        for(std::string token : tokens_remove)
            token_map.erase(token);
        token_mutex.unlock();

    }
}


void getFilelist(const std::string& dirpath, fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string realpath_str = rootdir + dirpath;
    path realpath(realpath_str);
    if(!exists(realpath) || !is_directory(realpath)){
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    FileList* filelist = new FileList;
    filelist->set_path(dirpath);
    directory_iterator end_iter;
    for(directory_iterator iter(realpath);iter!=end_iter;iter++){
        if(boost::algorithm::starts_with(iter->path().filename().string(), hidden_prefix))
            // ignore hidden files
            continue;

        File* file = filelist->add_file();
        // set file name
        file->set_filename(iter->path().filename().string());

        // set file type
        if(is_directory(iter->path()))
            file->set_file_type(File::DIRECTORY);
        else if(is_regular_file(iter->path()))
            file->set_file_type(File::REGULAR);
        else if(is_symlink(iter->path()))
            file->set_file_type(File::SYMLINK);
        else
            file->set_file_type(File::OTHER);

        // set file size
        // if it's not a regular file, `static_cast<uintmax_t>(-1)` will returned by `file_size()`
        if(is_regular_file(iter->path()))
            file->set_size(file_size(iter->path()));

        // set last modified time
        file->set_mtime(last_write_time(iter->path()));

    }

    response.set_allocated_file_list(filelist);

}

void makedir(const std::string& ph, fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string path_str = rootdir + ph;
    boost::system::error_code err;
    create_directory(path_str, err);
    if(err){
        // something wrong here
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }
    response.set_resp_type(Response::SUCCESS);

}

void init_upload(int privilege,
                 const std::string& basepath,
                 const std::string& filename,
                 int packet_no,
                 fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    if(!((privilege >> UPLOAD_BIT) % 2)){
        // no right to upload
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }


    std::string base = rootdir + basepath;
    if(!exists(base)){
        // basepath doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    std::string newfile = base + SEPARATOR + filename;
    if(exists(newfile)){
        // newfile already exists
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    // remove hidden file
    std::string hiddedfile = base + SEPARATOR + hidden_prefix + filename;
    if(exists(hiddedfile))
        remove(hiddedfile);

    response.set_resp_type(Response::SUCCESS);
    server_taskid_mutex.lock();
    response.set_task_id(++task_id);
    server_taskid_mutex.unlock();

    Task task;
    task.set_task_id(task_id);
    task.set_remotebasepath(base);
    task.set_filename(filename);
    task.set_total_packet_no(packet_no);
    task.set_received_packet_no(0);
    task.set_last_packet_time(std::time(0));
    task.set_task_status(Task::UPLOADING);
    server_task_mutex.lock();
    tasks[task_id] = task;
    server_task_mutex.unlock();

    fs_DBManager manager;
    manager.insert_task(task);

}

bool load_task(int taskid){
    if(tasks.find(taskid) != tasks.end())
        return true;

    fs::proto::Task task;
    fs_DBManager manager;
    int ret = manager.get_task(taskid, task);
    if(ret == FS_E_NOSUCHTASK)
        return false;   // no such task

    server_task_mutex.lock();
    task.set_last_packet_time(std::time(0));
    tasks[taskid] = task;
    server_task_mutex.unlock();

    return true;
}

void receive_packet(int taskid, const fs::proto::Packet& packet, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    Task& task = tasks[taskid];
    if(packet.packet_id() != task.received_packet_no()){
        response.set_resp_type(Response::ILLEGALPACKETID);
        response.set_packet_id(task.received_packet_no());
        std::cout << "(packet.packet_id = " << packet.packet_id()
                  << ") (task.received_packet_no = "
                  << task.received_packet_no() << ")"
                  << std::endl;
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    task.set_received_packet_no(task.received_packet_no()+1);
    task.set_last_packet_time(std::time(0));

    // write data in file
    std::string filepath = task.remotebasepath()
                            + SEPARATOR
                            + DEFAULT_HIDDEN_PREFIX
                            + task.filename();
    std::ofstream file(filepath, std::ios_base::app | std::ios_base::binary);
    if(!file){
        // fetal error
        server_task_mutex.lock();
        task.set_task_status(Task::FAILED);
        server_task_mutex.unlock();
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }
    file << packet.data();
    file.close();

    // the last packet has been received
    if(task.total_packet_no() == task.received_packet_no()){
        // create an empty file
        if(task.total_packet_no() == 0)
            std::fstream ofile(filepath, std::ios_base::app | std::ios_base::binary);

        boost::filesystem::rename(filepath,
                                  task.remotebasepath() + SEPARATOR + task.filename());
        server_task_mutex.lock();
        task.set_task_status(Task::UPLOADED);
        server_task_mutex.unlock();
    }


}

void init_download(int privilege,
                   const std::string& basepath,
                   const std::string& filename,
                   fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;


    if(!((privilege >> DOWNLOAD_BIT) % 2)){
        // no right to download
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }


    std::string base_str = rootdir + basepath;
    std::string filepath_str = base_str + SEPARATOR + filename;
    path filepath(filepath_str);
    if(!exists(filepath)){
        // requested file doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }


    response.set_resp_type(Response::SUCCESS);
    server_taskid_mutex.lock();
    response.set_task_id(++task_id);
    server_taskid_mutex.unlock();

    Task task;
    task.set_task_id(task_id);
    task.set_remotebasepath(base_str);
    task.set_filename(filename);

    int size = file_size(filepath);
    int packet_no = size / PACKET_SIZE;
    if(size % PACKET_SIZE != 0)
        packet_no++;
    task.set_total_packet_no(packet_no);
    response.set_packet_no(packet_no);

    task.set_sent_packet_no(0);
    task.set_last_packet_time(std::time(0));
    task.set_task_status(Task::DOWNLOADING);
    server_task_mutex.lock();
    tasks[task_id] = task;
    server_task_mutex.unlock();

    fs_DBManager manager;
    manager.insert_task(task);

}

void send_packet(int taskid, int packetid, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    // generate packet
    Task& task = tasks[taskid];
    Packet* packet = response.mutable_packet();
    packet->set_packet_id(packetid);
    std::string filepath = task.remotebasepath() + SEPARATOR + task.filename();
    std::ifstream file(filepath, std::ios_base::binary);
    if(!file){
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }
    char* buf = new char[PACKET_SIZE];
    file.seekg(packetid * PACKET_SIZE);
    file.read(buf, PACKET_SIZE);
    packet->set_data(buf, file.gcount());
    file.close();
    delete[] buf;


    response.set_resp_type(Response::SUCCESS);
    task.set_sent_packet_no(task.sent_packet_no()+1);
    task.set_last_packet_time(std::time(0));
}

void confirm_download(int taskid, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    server_task_mutex.lock();
    tasks[taskid].set_task_status(Task::DOWNLOADED);
    server_task_mutex.unlock();
}

void remove_file(const std::string& ph, fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string path_str = rootdir + ph;
    boost::system::error_code err;
    remove_all(path_str,err);
    if(err){
        // something wrong here
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }
    response.set_resp_type(Response::SUCCESS);
}

void rename_file(const std::string& oldpath,
                 const std::string& newpath,
                 fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;


    std::string oldpath_str = rootdir + oldpath;
    std::string newpath_str = rootdir + newpath;

    boost::system::error_code err;
    rename(oldpath_str, newpath_str, err);

    if(err){
        // something wrong here
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
}

void cancel_task(int taskid, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    Task::TaskStatus status = tasks[taskid].task_status();
    server_task_mutex.lock();
    if(status == Task::UPLOADING || status == Task::DOWNLOADING)
        tasks[taskid].set_task_status(Task::CANCELED);
    else if(status == Task::UPLOADED || status == Task::DOWNLOADED){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    // remove temp file
    Task& task = tasks[taskid];
    std::string path = task.remotebasepath() + SEPARATOR + hidden_prefix + task.filename();
    if(boost::filesystem::exists(path))
        boost::filesystem::remove(path);

    server_task_mutex.unlock();
    response.set_resp_type(Response::SUCCESS);


}


std::string gen_token(){
    char buf[DEFAULT_TOKEN_LEN];
    for(int i=0;i<DEFAULT_TOKEN_LEN;i++)
        buf[i] = std::rand()%26 + 'A';
    return std::string(buf);
}

void verify_password(std::string username,
                     std::string password,
                     fs::proto::Response& response){
    using namespace fs::proto;
    fs_DBManager manager;
    User user;
    user.set_username(username);
    user.set_password(password);
    int ret = manager.login(user);
    if(ret == FS_E_NOSUCHUSER){
        response.set_resp_type(Response::NOSUCHUSER);
        return;
    }
    if(ret == FS_E_ILLEGALPASSWD){
        response.set_resp_type(Response::ILLEGALPASSWD);
        return;
    }

    std::string token = gen_token();
    response.set_token(token);
    user.set_last_packet_time(std::time(0));
    token_mutex.lock();
    token_map[token] = user;
    token_mutex.unlock();
    response.set_resp_type(Response::SUCCESS);
    response.set_privilege(user.privilege());

}


bool verify_token(const std::string& token, int& privilege){
    auto it = token_map.find(token);
    if(it != token_map.end()){
        it->second.set_last_packet_time(std::time(0));
        privilege = it->second.privilege();
        return true;
    }
    return false;
}

void get_disk_info(fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    space_info info = space("/");

    size_t occupied = 0;
    for(recursive_directory_iterator it(rootdir);
        it!=recursive_directory_iterator();
        ++it){
        if(!is_directory(*it))
            occupied += file_size(*it);
    }

    response.set_resp_type(Response::SUCCESS);
    response.set_avai_space(info.available);
    response.set_total_space(occupied + info.available);
}

void getUserList(int privilege, fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    UserList* userlist = new UserList;
    fs_DBManager manager;
    manager.getUserList(userlist);
    response.set_allocated_userlist(userlist);

}

void adduser(int privilege,
             const fs::proto::User& user,
             fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    fs_DBManager manager;
    int ret = manager.addUser(user);
    if(ret == FS_E_DUPLICATE_USER){
        response.set_resp_type(Response::DUPLICATEUSER);
        return;
    }

    response.set_resp_type(Response::SUCCESS);

}

void remove_user(int privilege,
                 std::string username,
                 fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    fs_DBManager manager;
    manager.removeUser(username);
    response.set_resp_type(Response::SUCCESS);
}

void get_ip_list(int privilege, fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    IPList* iplist = new IPList;
    fs_DBManager manager;
    manager.getIPList(iplist);
    response.set_allocated_iplist(iplist);
}

void add_ipaddr(int privilege,
                std::string ipaddr,
                fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    fs_DBManager manager;
    int ret = manager.addIPAddr(ipaddr);
    if(ret == FS_E_DUPLICATE_IPADDR){
        response.set_resp_type(Response::DUPLICATEIPADDR);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
}


void remove_ipaddr(int privilege,
                   std::string ipaddr,
                   fs::proto::Response& response){
    using namespace fs::proto;
    if(!((privilege >> ROOT_BIT) % 2)){
        // no right
        response.set_resp_type(Response::NOPRIVILEGE);
        return;
    }

    fs_DBManager manager;
    manager.removeIPAddr(ipaddr);
    response.set_resp_type(Response::SUCCESS);
}

void communicate_thread(server_ptr serptr){
    using namespace fs::proto;
    int ret_req;
    int ret_resp;

#ifdef DEBUG
    std::cout << "connection built with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;
#else
    std::ofstream ofile(DEFAULT_LOG_FILE, std::ios::app);
    time_t ticks = std::time(0);
    std::string time_str = std::ctime(&ticks);
    time_str = time_str.substr(0,time_str.length()-1);
    std::string addr_str = serptr->sock().remote_endpoint().address().to_string();
    ofile << time_str << " " << addr_str << std::endl;
#endif

    // verify ip address
    std::string ipaddr = serptr->sock().remote_endpoint().address().to_string();
    fs_DBManager manager;
    if(!manager.getIPAddr(ipaddr)){
        serptr->set_stop(true);
        return;
    }

    while(true){
        Request request;
        ret_req = serptr->receive_request(request);
        if(ret_req == false)
            break;
        Response response;
        int privilege;
#ifndef DEBUG
        ofile << "Received : " << Request::RequestType_Name(request.req_type()) << std::endl;
#endif

        if(request.req_type() == Request::LOGIN){
            verify_password(request.username(),
                            request.password(),
                            response);
        }else if(!verify_token(request.token(), privilege)){
            response.set_resp_type(Response::ILLEGALTOKEN);
        }else{
            switch (request.req_type()) {
            case Request::FILELIST:
                getFilelist(request.remote_path(), response);
                break;
            case Request::MKDIR:
                makedir(request.remote_path(), response);
                break;
            case Request::REMOVE:
                remove_file(request.remote_path(), response);
                break;
            case Request::RENAME:
                rename_file(request.remote_path(),
                            request.new_path(),
                            response);
                break;
            case Request::UPLOAD:
                init_upload(privilege,
                            request.remote_path(),
                            request.filename(),
                            request.packet_no(),
                            response);
                break;
            case Request::SEND_PACKET:
                receive_packet(request.task_id(),
                               request.packet(),
                               response);
                break;
            case Request::DOWNLOAD:
                init_download(privilege,
                              request.remote_path(),
                              request.filename(),
                              response);
                break;
            case Request::RECEIVE_PACKET:
                send_packet(request.task_id(),
                            request.packet_id(),
                            response);
                break;
            case Request::DOWNLOAD_CONFIRM:
                confirm_download(request.task_id(), response);
                break;
            case Request::CANCEL:
                cancel_task(request.task_id(), response);
                break;
            case Request::DISKSPACE:
                get_disk_info(response);
                break;
            case Request::USERLIST:
                getUserList(privilege, response);
                break;
            case Request::ADDUSER:
                adduser(privilege,
                        request.user(),
                        response);
                break;
            case Request::REMOVEUSER:
                remove_user(privilege,
                            request.username(),
                            response);
                break;
            case Request::IPLIST:
                get_ip_list(privilege, response);
                break;
            case Request::ADDIP:
                add_ipaddr(privilege,
                           request.ipaddress(),
                           response);
                break;
            case Request::REMOVEIP:
                remove_ipaddr(privilege,
                              request.ipaddress(),
                              response);
                break;
            default:
                response.set_resp_type(Response::ILLEGALREQUEST);
                break;
            }

        }
#ifndef DEBUG
        ofile << "Send : " << Response::ResponseType_Name(response.resp_type()) << std::endl;
#endif
        ret_resp = serptr->send_response(response);
        if(ret_resp == false)
            break;
    }

    serptr->set_stop(true);

#ifdef DEBUG
    std::cout << "connection lost with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;
#else
    ofile.close();
#endif

}


void save_thread(){
    while(true){
        boost::this_thread::sleep(DEFAULT_SAVE_SLEEP_TIME);
        std::ofstream file(taskid_path, std::ios::trunc);
        file << task_id;
        file.close();
    }
}

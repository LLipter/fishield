#include "fs_server.h"

boost::asio::io_service service;
short _port = DEFAULT_SERV_PORT;
std::string rootdir = DEFAULT_ROOT_DIR;
std::string hidden_prefix = DEFAULT_HIDDEN_PREFIX;
std::string taskid_path = std::string(".") + SEPARATOR + DEFAULT_TASKID_FILE;
std::vector<server_ptr> clients;
std::map<int,fs_task> tasks;

int task_id;
fs_server::fs_server():_sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    _is_stop = false;
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

}


bool fs_server::receive_request(fs::proto::Request& request){
    using namespace fs::proto;

    // read length of request
    int len;
    int* lenptr = &len;
    boost::system::error_code err;
    boost::asio::read(_sock,boost::asio::buffer((char*)lenptr,4),err);

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
    boost::asio::read(_sock,boost::asio::buffer(buf,len),err);
    if(err){
        delete[] buf;
        std::cout << "receive_request() read request failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // deserialize request
    if(request.ParseFromArray(buf,len) == false){
        delete[] buf;
        std::cout << "receive_request() deserialize request failed : "
                  << std::endl;
        return false;
    }
    delete[] buf;

    std::cout << "receive_request() success : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;
    return true;
}

bool fs_server::send_response(const fs::proto::Response& response){
    using namespace fs::proto;
    std::cout << "start send response : "
              << Response::ResponseType_Name(response.resp_type())
              << std::endl;
    int len = response.ByteSize();
    char* buf = new char[len+4];
    *(int*)buf = len;
    response.SerializeToArray(buf+4,len);
    boost::system::error_code err;
    boost::asio::write(_sock,boost::asio::buffer(buf,len+4), err);
    delete[] buf;
    if(err){
        std::cout << "send_response() failed : "
                  << err.message()
                  << std::endl;
        return false;
    }
    std::cout << "send_response() success : "
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
        std::thread thd(communicate_thread,serptr);
        thd.detach();
    }
}


void remove_clients_thread() {
    while (true) {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        // erase clients that are stoped
        clients.erase(std::remove_if(clients.begin(),
                                     clients.end(),
                                     boost::bind(&fs_server::is_stop,_1)),
                      clients.end());
        // TODO : REMOVE ALL TIMEOUT CLIENTS
        // TODO : REMOVE ALL TIMEOUT TASKS or finished tasks
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
        if(is_regular_file(iter->path()))
            file->set_size(file_size(iter->path()));

        // set last modified time
        file->set_mtime(last_write_time(iter->path()));

    }

    response.set_allocated_file_list(filelist);

}

void mkdir(const std::string& basepath,
           const std::string& dirname,
           fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string base_str = rootdir + basepath;
    path base(base_str);
    if(!exists(base)){
        // basepath doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    std::string newdir_str = base_str + SEPARATOR + dirname;
    path newdir(newdir_str);
    if(exists(newdir)){
        // newdir already exists
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    create_directory(newdir);
    response.set_resp_type(Response::SUCCESS);

}

void init_upload(const std::string& basepath,
                    const std::string& filename,
                    int packet_no,
                    fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string base_str = rootdir + basepath;
    path base(base_str);
    if(!exists(base)){
        // basepath doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    std::string newfile_str = base_str + SEPARATOR + filename;
    path newfile(newfile_str);
    if(exists(newfile)){
        // newfile already exists
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    response.set_task_id(++task_id);

    fs_task task;
    task.task_id = task_id;
    task.remotebasepath = base_str;
    task.filename = filename;
    task.total_packet_no = packet_no;
    task.received_packet_no = 0;
    task.last_packet_time = std::time(0);
    task.status = Task::UPLOADING;
    tasks[task_id] = task;

}


bool load_task(int taskid){
    if(tasks.find(taskid) != tasks.end())
        return true;
    // TODO : load task from database
    // if( find in database ) {
    //      load it to tasks
    //      return true;
    // }

    return false;
}


void receive_packet(int taskid, const fs::proto::Packet& packet, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    fs_task& task = tasks[taskid];
    if((int)packet.packet_id() != task.received_packet_no){
        response.set_resp_type(Response::ILLEGALPACKETID);
        response.set_packet_id(task.received_packet_no);
        std::cout << "(packet.packet_id = " << packet.packet_id()
                  << ") (task.received_packet_no = "
                  << task.received_packet_no << ")"
                  << std::endl;
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    task.received_packet_no++;
    task.last_packet_time = std::time(0);

    // write data in file
    std::string filepath = task.remotebasepath
                            + SEPARATOR
                            + DEFAULT_HIDDEN_PREFIX
                            + task.filename;
    std::ofstream file(filepath, std::ios_base::app | std::ios_base::binary);
    if(!file){
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }
    file << packet.data();
    file.close();

    // the last packet has been received
    if(task.total_packet_no == task.received_packet_no){
        boost::filesystem::rename(filepath,
                                  task.remotebasepath + SEPARATOR + task.filename);
        task.status = Task::UPLOADED;
    }


}

void init_download(const std::string& basepath,
                    const std::string& filename,
                    fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string base_str = rootdir + basepath;
    std::string filepath_str = base_str + SEPARATOR + filename;
    path filepath(filepath_str);
    if(!exists(filepath)){
        // requested file doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }


    response.set_resp_type(Response::SUCCESS);
    response.set_task_id(++task_id);

    fs_task task;
    task.task_id = task_id;
    task.remotebasepath = base_str;
    task.filename = filename;

    int size = file_size(filepath);
    int packet_no = size / PACKET_SIZE;
    if(size % PACKET_SIZE != 0)
        packet_no++;
    task.total_packet_no = packet_no;
    response.set_packet_no(packet_no);

    task.sent_packet_no = 0;
    task.last_packet_time = std::time(0);
    task.status = Task::DOWNLOADING;
    tasks[task_id] = task;

}

void send_packet(int taskid, int packetid, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    // generate packet
    fs_task& task = tasks[taskid];
    Packet* packet = response.mutable_packet();
    packet->set_packet_id(packetid);
    std::string filepath = task.remotebasepath + SEPARATOR + task.filename;
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
    task.sent_packet_no++;
    task.last_packet_time = std::time(0);


}


void confirm_download(int taskid, fs::proto::Response& response){
    using namespace fs::proto;
    if(!load_task(taskid)){
        response.set_resp_type(Response::ILLEGALTASKID);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    tasks[taskid].status = Task::DOWNLOADED;
}

void remove_file(const std::string& basepath,
                 const std::string& filename,
                 fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string base_str = rootdir + basepath;
    std::string filepath_str = base_str + SEPARATOR + filename;
    path filepath(filepath_str);
    if(!exists(filepath)){
        // file doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    if(is_directory(filepath) && !is_empty(filepath)){
        // refuse to remove a non-empty directory
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    remove(filepath);
    response.set_resp_type(Response::SUCCESS);
}


// TODO verify token
bool verify_token(const std::string& token){
    return true;
}

void communicate_thread(server_ptr serptr){
    using namespace fs::proto;
    int ret_req;
    int ret_resp;

    std::cout << "connection built with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;

    while(true){
        Request request;
        ret_req = serptr->receive_request(request);
        if(ret_req == false)
            break;
        Response response;
        if(request.req_type() == Request::LOGIN){
            // TODO connect to database to verify password&username
            // TODO generate a random token and stored it
            response.set_resp_type(Response::SUCCESS);
            response.set_token("ttttooookkkkeeeennnn");
        }else if(!verify_token(request.token())){
            response.set_resp_type(Response::ILLEGALTOKEN);
        }else{
            switch (request.req_type()) {
            case Request::FILELIST:
                getFilelist(request.remote_path(), response);
                break;
            case Request::MKDIR:
                mkdir(request.remote_path(), request.filename(), response);
                break;
            case Request::UPLOAD:
                init_upload(request.remote_path(),
                               request.filename(),
                               request.packet_no(),
                               response);
                break;
            case Request::SEND_PACKET:
                receive_packet(request.task_id(), request.packet(), response);
                break;
            case Request::DOWNLOAD:
                init_download(request.remote_path(),
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
            case Request::REMOVE:
                remove_file(request.remote_path(),
                            request.filename(),
                            response);
                break;
            default:
                response.set_resp_type(Response::ILLEGALREQUEST);
                break;
            }

        }

        ret_resp = serptr->send_response(response);
        if(ret_resp == false)
            break;
    }

    serptr->set_stop(true);
    std::cout << "connection lost with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;
}


void save_thread(){
    while(true){
        boost::this_thread::sleep(boost::posix_time::seconds(2));
        std::ofstream file(taskid_path,std::ios::trunc);
        file << task_id;
        file.close();
    }
}

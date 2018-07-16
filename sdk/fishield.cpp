#include "fishield.h"

extern boost::asio::ip::address _server_addr;
extern int _server_port;
extern boost::asio::io_service service;
extern std::string _token;
extern fs_fp_intdouble cb_progress;
extern fs_fp_int cb_success;
extern fs_fp_interror cb_failed;
extern fs_fp_tasks cb_report;

int fs_client_startup(const std::string& addr, const short port){
    using namespace boost::asio::ip;

    boost::system::error_code err;
    tcp::resolver resolver(service);
    tcp::resolver::query qry(addr, std::to_string(port));
    tcp::resolver::iterator it = resolver.resolve(qry, err);

    if(err){
        std::cout << err.message() << std::endl;
        return FS_E_ILLEGAL_VALUE;
    }

    tcp::resolver::iterator end;
    if(it != end){
        std::cout << addr << " : "
                  << (*it).endpoint().address().to_string()
                  << std::endl;
        _server_addr = (*it).endpoint().address();
        _server_port = port;
    }else
        return FS_E_UNKNOWN;

    fs_client::init();

    return 0;
}



void _fs_login(const std::string& username,
               const std::string& password,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    using namespace fs::proto;
    Request login_request;
    login_request.set_req_type(Request::LOGIN);
    login_request.set_username(username);
    login_request.set_password(password);

    // send request and receive response
    Response response;
    if(send_receive(login_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        _token = response.token();
        cb_success();
    }else
        cb_failed(response.resp_type());
}


void fs_login(const std::string& username,
              const std::string& password,
              fs_fp_void cb_success,
              fs_fp_error cb_failed){
    std::thread thd(_fs_login,
                    username,
                    password,
                    cb_success,
                    cb_failed);
    thd.detach();
}


void _fs_filelist(const std::string& dirpath,
                  fs_fp_filelist cb_success,
                  fs_fp_error cb_failed){
    using namespace fs::proto;
    Request filelist_request;
    filelist_request.set_req_type(Request::FILELIST);
    filelist_request.set_remote_path(dirpath);
    filelist_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(filelist_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS)
        cb_success(response.file_list());
    else
        cb_failed(response.resp_type());


}


void fs_filelist(const std::string& dirpath,
                 fs_fp_filelist cb_success,
                 fs_fp_error cb_failed){
    std::thread thd(_fs_filelist,
                    dirpath,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_mkdir(const std::string& path,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    using namespace fs::proto;
    Request mkdir_request;
    mkdir_request.set_req_type(Request::MKDIR);
    mkdir_request.set_remote_path(path);
    mkdir_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(mkdir_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS)
        cb_success();
    else
        cb_failed(response.resp_type());
}


void fs_mkdir(const std::string& path,
              fs_fp_void cb_success,
              fs_fp_error cb_failed){
    std::thread thd(_fs_mkdir,
                    path,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_remove(const std::string& path,
                fs_fp_void cb_success,
                fs_fp_error cb_failed){
    using namespace fs::proto;
    Request remove_request;
    remove_request.set_req_type(Request::REMOVE);
    remove_request.set_remote_path(path);
    remove_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(remove_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS)
        cb_success();
    else
        cb_failed(response.resp_type());
}

void fs_remove(const std::string& path,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    std::thread thd(_fs_remove,
                    path,
                    cb_success,
                    cb_failed);
    thd.detach();
}

bool fs_upload(const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename){

    using namespace boost::filesystem;
    using namespace fs::proto;

    // local file doesn't exist
    path local_path(localbasepath + SEPARATOR + filename);
    if(!exists(local_path))
        return false;

    // only support upload regular file
    if(is_directory(local_path))
        return false;


    // generate a task object
    fs::proto::Task task;
    task.set_localbasepath(localbasepath);
    task.set_remotebasepath(remotebasepath);
    task.set_filename(filename);
    int size = file_size(local_path);
    int packet_no = size / PACKET_SIZE;
    if(size % PACKET_SIZE != 0)
        packet_no++;
    task.set_total_packet_no(packet_no);
    task.set_sent_packet_no(0);
    task.set_task_status(Task::UPLOAD_INIT);


    fs_scheduler::instance()->add_task(task);

    return true;
}

bool fs_download(const std::string& localbasepath,
                 const std::string& remotebasepath,
                 const std::string& filename){
    using namespace boost::filesystem;
    using namespace fs::proto;

    // local file already exists
    path local_path(localbasepath + SEPARATOR + filename);
    if(exists(local_path))
        return false;

    // only support download regular file
    if(is_directory(local_path))
        return false;


    // generate a task object
    fs::proto::Task task;
    task.set_localbasepath(localbasepath);
    task.set_remotebasepath(remotebasepath);
    task.set_filename(filename);
    task.set_received_packet_no(0);
    task.set_task_status(Task::DOWNLOAD_INIT);


    fs_scheduler::instance()->add_task(task);

    return true;
}

void fs_register_task_callback(fs_fp_tasks cb_reprt,
                               fs_fp_int cb_succes,
                               fs_fp_interror cb_fal){
    cb_report = cb_reprt;
    cb_success = cb_succes;
    cb_failed = cb_fal;
}

void _fs_rename(const std::string& oldpath,
                const std::string& newpath,
                fs_fp_void cb_success,
                fs_fp_error cb_failed){
    using namespace fs::proto;

    Request rename_request;
    rename_request.set_req_type(Request::RENAME);
    rename_request.set_remote_path(oldpath);
    rename_request.set_new_path(newpath);
    rename_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(rename_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS)
        cb_success();
    else
        cb_failed(response.resp_type());
}


void fs_rename(const std::string& oldpath,
               const std::string& newpath,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    std::thread thd(_fs_rename,
                    oldpath,
                    newpath,
                    cb_success,
                    cb_failed);
    thd.detach();
}

extern std::mutex client_task_mutex;
void _fs_cancel(int task_id,
                fs_fp_int cb_success,
                fs_fp_interror cb_failed){
    using namespace fs::proto;


    Request cancel_request;
    cancel_request.set_req_type(Request::CANCEL);
    cancel_request.set_task_id(task_id);
    cancel_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(cancel_request,response) == false){
        cb_failed(task_id, Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        client_task_mutex.lock();
        Task& task = fs_scheduler::instance()->task_map_current[task_id];

        switch (task.task_status()) {
        case Task::UPLOADING:
        case Task::UPLOAD_PAUSING:
        case Task::DOWNLOADING:
        case Task::DOWNLOAD_PAUSING:
            task.set_task_status(Task::CANCELING);
            break;
        case Task::UPLOAD_PAUSED:
        case Task::UPLOAD_INIT:
        case Task::UPLOAD_RESUME:
        case Task::DOWNLOAD_PAUSED:
        case Task::DOWNLOAD_INIT:
        case Task::DOWNLOAD_RESUME:
            task.set_task_status(Task::CANCELED);
            break;
        case Task::UPLOADED:
        case Task::DOWNLOADED:
        case Task::CANCELING:
        case Task::CANCELED:
        case Task::FAILING:
        case Task::FAILED:
            // do nothing
            break;
        default:
            break;
        }

        // remove temp file
        std::string path = task.localbasepath() + SEPARATOR + DEFAULT_HIDDEN_PREFIX + task.filename();
        if(boost::filesystem::exists(path))
            boost::filesystem::remove(path);

        client_task_mutex.unlock();
        cb_success(task_id);
    }
    else
        cb_failed(task_id, response.resp_type());
}

void fs_cancel(int task_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    std::thread thd(_fs_cancel,
                    task_id,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_pause(int task_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    using namespace fs::proto;

    auto& tmap = fs_scheduler::instance()->task_map_current;
    if(tmap.find(task_id) == tmap.end()){
        cb_failed(task_id, Response::ILLEGALTASKID);
        return;
    }

    client_task_mutex.lock();
    Task& task = fs_scheduler::instance()->task_map_current[task_id];
    switch (task.task_status()) {
    case Task::UPLOADING:
        task.set_task_status(Task::UPLOAD_PAUSING);
        break;
    case Task::DOWNLOADING:
        task.set_task_status(Task::DOWNLOAD_PAUSING);
        break;
    default:
        // do nothing
        break;
    }
    client_task_mutex.unlock();
    cb_success(task_id);
}


void fs_pause(int task_id,
              fs_fp_int cb_success,
              fs_fp_interror cb_failed){
    std::thread thd(_fs_pause,
                    task_id,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_resume(int task_id,
                fs_fp_int cb_success,
                fs_fp_interror cb_failed){
    using namespace fs::proto;

    auto& tmap = fs_scheduler::instance()->task_map_current;
    if(tmap.find(task_id) == tmap.end()){
        cb_failed(task_id, Response::ILLEGALTASKID);
        return;
    }

    client_task_mutex.lock();
    Task& task = fs_scheduler::instance()->task_map_current[task_id];
    switch (task.task_status()) {
    case Task::UPLOAD_PAUSED:
        task.set_task_status(Task::UPLOAD_RESUME);
        break;
    case Task::DOWNLOAD_PAUSED:
        task.set_task_status(Task::DOWNLOAD_RESUME);
        break;
    default:
        // do nothing
        break;
    }
    client_task_mutex.unlock();
    cb_success(task_id);
}

void fs_resume(int task_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    std::thread thd(_fs_resume,
                    task_id,
                    cb_success,
                    cb_failed);
    thd.detach();
}

extern short _port;
void fs_server_startup(const short port){
    _port = port;
    fs_server::init();
    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(remove_clients_thread);
    threads.create_thread(save_thread);
    std::cout << "server startup ok" << std::endl;
    std::cout << "------------------------------------" << std::endl;
    threads.join_all();
}

void _fs_history_tasks(fs_fp_tasks cb_history_tasks){
    std::vector<fs::proto::Task> history_tasks;
    auto& tmap = fs_scheduler::instance()->task_map_finished;
    for(auto it=tmap.begin();it!=tmap.end();it++){
        history_tasks.push_back(it->second);
    }
    cb_history_tasks(history_tasks);
}

void fs_history_tasks(fs_fp_tasks cb_history_tasks){
    std::thread thd(_fs_history_tasks,
                    cb_history_tasks);
    thd.detach();
}


extern std::string tasks_finished_path;
extern std::string tasks_current_path;
void fs_cleaning_up(){
    using namespace fs::proto;

    client_task_mutex.lock();
    auto& tmap = fs_scheduler::instance()->task_map_current;
    for(auto it=tmap.begin();it!=tmap.end();it++){
        Task& task = it->second;
        if(task.task_status() == Task::UPLOADING)
            task.set_task_status(Task::UPLOAD_PAUSING);
        if(task.task_status() == Task::DOWNLOADING)
            task.set_task_status(Task::DOWNLOAD_PAUSING);
    }
    client_task_mutex.unlock();

    save_task_to_file(tasks_finished_path, fs_scheduler::instance()->task_map_finished);
    save_task_to_file(tasks_current_path, fs_scheduler::instance()->task_map_current);


    boost::this_thread::sleep(boost::posix_time::milliseconds(300));
}

void _fs_disk_space(fs_fp_ii cb_success,
                    fs_fp_error cb_failed){
    using namespace fs::proto;

    Request disk_request;
    disk_request.set_req_type(Request::DISKSPACE);
    disk_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(disk_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS)
        cb_success(response.avai_space(),response.total_space());
    else
        cb_failed(response.resp_type());
}


void fs_disk_space(fs_fp_ii cb_success,
                   fs_fp_error cb_failed){
    std::thread thd(_fs_disk_space,
                    cb_success,
                    cb_failed);
    thd.detach();
}

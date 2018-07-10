#include "fishield.h"

extern boost::asio::ip::address _server_addr;
extern int _server_port;
extern boost::asio::io_service service;
extern std::string _token;
extern fs_fp_intdouble cb_progress;
extern fs_fp_int cb_success;
extern fs_fp_interror cb_failed;

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

void fs_upload(int client_id,
               const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename){

    using namespace boost::filesystem;
    using namespace fs::proto;

    // local file doesn't exist
    path local_path(localbasepath + SEPARATOR + filename);
    if(!exists(local_path)){
        cb_failed(client_id, Response::ILLEGALPATH);
        return;
    }

    // only support upload regular file
    if(is_directory(local_path)){
        cb_failed(client_id, Response::ILLEGALPATH);
        return;
    }


    // generate a task object
    fs::proto::Task task;
    task.set_client_id(client_id);
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


}

void fs_download(int client_id,
                 const std::string& localbasepath,
                 const std::string& remotebasepath,
                 const std::string& filename){
    using namespace boost::filesystem;
    using namespace fs::proto;

    // local file already exists
    path local_path(localbasepath + SEPARATOR + filename);
    if(exists(local_path)){
        cb_failed(client_id, Response::ILLEGALPATH);
        return;
    }

    // only support download regular file
    if(is_directory(local_path)){
        cb_failed(client_id, Response::ILLEGALPATH);
        return;
    }


    // generate a task object
    fs::proto::Task task;
    task.set_client_id(client_id);
    task.set_localbasepath(localbasepath);
    task.set_remotebasepath(remotebasepath);
    task.set_filename(filename);
    task.set_received_packet_no(0);
    task.set_task_status(Task::DOWNLOAD_INIT);


    fs_scheduler::instance()->add_task(task);
}

void fs_register_task_callback(fs_fp_intdouble cb_prog,
                               fs_fp_int cb_succes,
                               fs_fp_interror cb_fal){
    cb_progress = cb_prog;
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
void _fs_cancel(int client_id,
                fs_fp_int cb_success,
                fs_fp_interror cb_failed){
    using namespace fs::proto;

    int taskid = get_taskid_by_clientid(client_id);
    if(taskid == FS_E_NOSUCHID)
        cb_failed(client_id, Response::ILLEGALCLIENTID);

    Request cancel_request;
    cancel_request.set_req_type(Request::CANCEL);
    cancel_request.set_task_id(taskid);
    cancel_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(cancel_request,response) == false){
        cb_failed(client_id, Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        client_task_mutex.lock();
        Task& task = fs_scheduler::instance()->task_map_current[taskid];

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

        client_task_mutex.unlock();
        cb_success(client_id);
    }
    else
        cb_failed(client_id, response.resp_type());
}

void fs_cancel(int client_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    std::thread thd(_fs_cancel,
                    client_id,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_pause(int client_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    using namespace fs::proto;

    int taskid = get_taskid_by_clientid(client_id);
    if(taskid == FS_E_NOSUCHID)
        cb_failed(client_id, Response::ILLEGALCLIENTID);

    client_task_mutex.lock();
    Task& task = fs_scheduler::instance()->task_map_current[taskid];
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
    cb_success(client_id);
}


void fs_pause(int client_id,
              fs_fp_int cb_success,
              fs_fp_interror cb_failed){
    std::thread thd(_fs_pause,
                    client_id,
                    cb_success,
                    cb_failed);
    thd.detach();
}

void _fs_resume(int client_id,
                fs_fp_int cb_success,
                fs_fp_interror cb_failed){
    using namespace fs::proto;

    int taskid = get_taskid_by_clientid(client_id);
    if(taskid == FS_E_NOSUCHID)
        cb_failed(client_id, Response::ILLEGALCLIENTID);

    client_task_mutex.lock();
    Task& task = fs_scheduler::instance()->task_map_current[taskid];
    switch (task.task_status()) {
    case Task::UPLOAD_PAUSED:
        task.set_task_status(Task::UPLOAD_RESUME);
        break;
    case Task::DOWNLOAD_PAUSED:
        task.set_task_status(Task::DOWNLOAD_RESUME);
        break;
    default:
        client_task_mutex.unlock();
        cb_failed(client_id, Response::ILLEGALCLIENTID);
        break;
    }
    client_task_mutex.unlock();
    cb_success(client_id);
}

void fs_resume(int client_id,
               fs_fp_int cb_success,
               fs_fp_interror cb_failed){
    std::thread thd(_fs_resume,
                    client_id,
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

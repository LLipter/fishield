#include "fishield.h"

extern boost::asio::ip::address _server_addr;
extern int _server_port;
extern boost::asio::io_service service;
extern std::string _token;
extern fs_fp_intdouble cb_progress;
extern fs_fp_int cb_success;
extern fs_fp_interror cb_failed;

int fs_client_startup(const std::string& addr, const short port){
    try{
        boost::asio::ip::tcp::resolver resolver(service);
        boost::asio::ip::tcp::resolver::query qry(addr, std::to_string(port));
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(qry);
        boost::asio::ip::tcp::resolver::iterator end;
        if(it != end){
            std::cout << addr << " : " << (*it).endpoint().address().to_string() << std::endl;
            _server_addr = (*it).endpoint().address();
            _server_port = port;
        }else
            return FS_E_UNKNOWN;
    }catch(std::exception& e){
        std::cout << e.what() << std::endl;
        return FS_E_ILLEGAL_VALUE;
    }
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

void _fs_mkdir(const std::string& basepath,
               const std::string& dirname,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    using namespace fs::proto;
    Request mkdir_request;
    mkdir_request.set_req_type(Request::MKDIR);
    mkdir_request.set_remote_path(basepath);
    mkdir_request.set_filename(dirname);
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


void fs_mkdir(const std::string& basepath,
              const std::string& dirname,
              fs_fp_void cb_success,
              fs_fp_error cb_failed){
    std::thread thd(_fs_mkdir,
                    basepath,
                    dirname,
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

void _fs_remove(const std::string& basepath,
               const std::string& filename,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    using namespace fs::proto;
    Request mkdir_request;
    mkdir_request.set_req_type(Request::REMOVE);
    mkdir_request.set_remote_path(basepath);
    mkdir_request.set_filename(filename);
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


void fs_remove(const std::string& basepath,
               const std::string& filename,
               fs_fp_void cb_success,
               fs_fp_error cb_failed){
    std::thread thd(_fs_remove,
                    basepath,
                    filename,
                    cb_success,
                    cb_failed);
    thd.detach();
}


void fs_register_task_callback(fs_fp_intdouble cb_prog,
                               fs_fp_int cb_succes,
                               fs_fp_interror cb_fal){
    cb_progress = cb_prog;
    cb_success = cb_succes;
    cb_failed = cb_fal;
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
    threads.join_all();
}

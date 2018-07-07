#include "fishield.h"

extern boost::asio::ip::address _server_addr;
extern int _server_port;
extern boost::asio::io_service service;
extern std::string _token;

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
    switch (response.resp_type()) {
    case Response::SUCCESS:
        _token = response.token();
        cb_success();
        break;
    case Response::ILLEGALPASSWD:
    case Response::NOSUCHUSER:
        cb_failed(response.resp_type());
        break;
    default:
        cb_failed(Response::UNKNOWN);
        break;
    }
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


void _fs_get_filelist(const std::string& dirpath,
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
    switch (response.resp_type()) {
    case Response::SUCCESS:
        cb_success(response.file_list());
        break;
    case Response::ILLEGALTOKEN:
    case Response::ILLEGALPATH:
        cb_failed(response.resp_type());
        break;
    default:
        cb_failed(Response::UNKNOWN);
        break;
    }


}


void fs_get_filelist(const std::string& dirpath,
                     fs_fp_filelist cb_success,
                     fs_fp_error cb_failed){
    std::thread thd(_fs_get_filelist,
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
    switch (response.resp_type()) {
    case Response::SUCCESS:
        cb_success();
        break;
    case Response::ILLEGALTOKEN:
    case Response::ILLEGALPATH:
        cb_failed(response.resp_type());
        break;
    default:
        cb_failed(Response::UNKNOWN);
        break;
    }
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

void _fs_upload(const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename,
               fs_fp_int cb_start_upload,
               fs_fp_void cb_progress,
               fs_fp_int cb_success,
               fs_fp_error cb_failed){

    using namespace boost::filesystem;
    using namespace fs::proto;

    path local_path(localbasepath + SEPARATOR + );
    if(!exists(local_path)){
        cb_failed(Response::ILLEGALTOKEN);
        return;
    }

    Request upload_request;
    upload_request.set_req_type(Request::UPLOAD);
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
    switch (response.resp_type()) {
    case Response::SUCCESS:
        cb_success();
        break;
    case Response::ILLEGALTOKEN:
    case Response::ILLEGALPATH:
        cb_failed(response.resp_type());
        break;
    default:
        cb_failed(Response::UNKNOWN);
        break;
    }

}


void fs_upload(const std::string& localbasepath,
               const std::string& remotebasepath,
               const std::string& filename,
               fs_fp_int cb_start_upload,
               fs_fp_void cb_progress,
               fs_fp_int cb_success,
               fs_fp_error cb_failed){
    std::thread thd(_fs_upload,
                    localbasepath,
                    remotebasepath,
                    filename,
                    cb_start_upload,
                    cb_progress,
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
    std::cout << "server startup ok" << std::endl;
    threads.join_all();
}

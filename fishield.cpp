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

void _fs_login(const std::string& username, const std::string& password, fs_fp_void cb_success, fs_fp_error cb_failed){
    using namespace fs::proto;
    Request login_request;
    login_request.set_req_type(Request::LOGIN);
    login_request.set_username(username);
    login_request.set_password(password);

    // connect to server
    fs_client client;
    if(client.connect() == false){
        std::cout << "connect() failed"
                  << std::endl;
        cb_failed(Response::NORESPONSE);
        return;
    }

    // send request
    if(client.send_request(login_request) == false){
        std::cout << "send_request() failed"
                  << std::endl;
        cb_failed(Response::NORESPONSE);
        return;
    }

    // receive response
    Response response;
    if(client.receive_response(response) == false){
        std::cout << "receive_response() failed"
                  << std::endl;
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


void fs_login(const std::string& username, const std::string& password, fs_fp_void cb_success, fs_fp_error cb_failed){
    std::thread thd(_fs_login, username, password, cb_success, cb_failed);
    thd.detach();
}


extern short _port;
void fs_server_startup(const short port){
    _port = port;

    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(remove_clients_thread);
    std::cout << "server startup ok" << std::endl;
    threads.join_all();
}

#include "fs_server.h"

boost::asio::io_service service;
short _port = DEFAULT_SERV_PORT;
std::vector<server_ptr> clients;

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


bool fs_server::receive_request(fs::proto::Request& request){
    using namespace fs::proto;
    std::cout << "start receive request : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;

    // read length of request
    int len;
    int* lenptr = &len;
    boost::system::error_code err;
    boost::asio::read(_sock,boost::asio::buffer((char*)lenptr,4),err);
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
    }
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
        ret_resp = serptr->send_response(response);
        if(ret_resp != 0)
            break;
    }

    serptr->set_stop(true);
    std::cout << "connection lost with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;
}


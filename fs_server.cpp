#include "fs_server.h"

boost::asio::io_service service;
short _port = DEFAULT_SERV_PORT;

fs_server::fs_server():_sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
}


boost::asio::ip::tcp::socket& fs_server::sock(){
    return _sock;
}


int fs_server::receive_request(){
    return 1;
}

int fs_server::send_response(){
    return 1;
}




void accept_thread() {
    typedef boost::asio::ip::tcp::acceptor acceptor;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    acceptor acptor(service, endpoint(boost::asio::ip::tcp::v4(), _port));
    while (true) {
        server_ptr serptr(new fs_server);
        acptor.accept(serptr->sock());
        std::thread thd(communicate_thread,serptr);
        thd.detach();
    }
}


void communicate_thread(server_ptr serptr){
    int ret_req;
    int ret_resp;

    std::cout << "connection built" << std::endl;

    while(true){
        ret_req = serptr->receive_request();
        if(ret_req != 0)
            break;
        ret_resp = serptr->send_response();
        if(ret_resp != 0)
            break;
    }
}


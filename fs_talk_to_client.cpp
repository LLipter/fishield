#include "fs_talk_to_client.h"

extern boost::asio::io_service service;

fs_talk_to_client::fs_talk_to_client() : _sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    is_login = false;
}

boost::asio::ip::tcp::socket& fs_talk_to_client::sock(){
    return _sock;
}

void fs_talk_to_client::login(){
    is_login = true;
}

std::string fs_talk_to_client::username(){
    return _username;
}

void fs_talk_to_client::stop(){
    boost::system::error_code err;
    _sock.close(err);
}

#include "fs_talk_to_client.h"

boost::asio::io_service service;

fs_talk_to_client::fs_talk_to_client() : _sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
}

boost::asio::ip::tcp::socket& fs_talk_to_client::sock(){
    return _sock;
}

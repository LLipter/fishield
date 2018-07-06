#include "fs_client.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>   // google::protobuf::io::ArrayOutputStream


extern boost::asio::io_service service;

boost::asio::ip::address _server_addr = DEFAULT_SERV_ADDR;
short _server_port = DEFAULT_SERV_PORT;

fs_client::fs_client():
    _sock(service),
    _ep(_server_addr, _server_port)
{
    memset(data_buffer, 0, sizeof(data_buffer));
}

bool fs_client::connect(){
    boost::system::error_code error;
    _sock.connect(_ep,error);
    if(!error)
        return true;
    std::cout << error.message() << std::endl;
    return false;
}





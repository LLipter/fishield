#include "fs_talk_to_client.h"
#include "fs_error.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>

extern boost::asio::io_service service;

fs_talk_to_client::fs_talk_to_client() : _sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    is_login = false;
    already_read = 0;
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

void fs_talk_to_client::read_request() {
    char buff[4] = {0};
    already_read = 0;
    if ( _sock.available()){
        (void)_sock.receive(boost::asio::buffer(buff, 4), boost::asio::ip::tcp::socket::message_peek );
        google::protobuf::uint32 size;
        google::protobuf::io::ArrayInputStream ais_hdr(buff,4);
        google::protobuf::io::CodedInputStream coded_input_hdr(&ais_hdr);
        coded_input_hdr.ReadVarint32(&size);//Decode the HDR and get the size

        already_read = _sock.read_some(boost::asio::buffer(data_buffer, 4+size));
        if(already_read <= 0)
            err_quit("fs_talk_to_client::read_request() - read_some() return negetive value");
        google::protobuf::io::ArrayInputStream ais(data_buffer,size+4);
        google::protobuf::io::CodedInputStream coded_input(&ais);
        //Read an unsigned integer with Varint encoding, truncating to 32 bits.
        coded_input.ReadVarint32(&size);
        //After the message's length is read, PushLimit() is used to prevent the CodedInputStream
        //from reading beyond that length.Limits are used when parsing length-delimited
        //embedded messages
        google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(size);
        //De-Serialize

        fs::proto::packet::Request request;
        request.ParseFromCodedStream(&coded_input);
        //Once the embedded message has been parsed, PopLimit() is called to undo the limit
        coded_input.PopLimit(msgLimit);
        std::cout << request.packet().user_name();
        std::cout << request.packet().token();
        client_req = request;
        //Print the message
    }else
        err_quit("fs_talk_to_client::read_request() error - _sock is not available");
}

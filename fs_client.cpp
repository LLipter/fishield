#include "fs_client.h"
#include <boost/asio.hpp>
#include <google/protobuf/io/zero_copy_stream_impl.h>   // google::protobuf::io::ArrayOutputStream


boost::asio::io_service service;
std::string _server_addr = "127.0.0.1";
int _server_port = SERV_PORT;
std::string _user_name = "no such user";
std::string _token = "no such token";

fs_client::fs_client():_sock(service),_ep(boost::asio::ip::address::from_string(_server_addr), _server_port){
    _already_read = 0;
}

bool fs_client::connect(){
    boost::system::error_code err;
    _sock.connect(_ep,err);
    if(err)
        return false;
    return true;
}

void fs_client::close(){
    _sock.close();
}


bool fs_client::send_request(fs::proto::packet::Request request){
    //https://stackoverflow.com/questions/9496101/protocol-buffer-over-socket-in-c
    int siz = request.ByteSize()+4;
    char *pkt = new char [siz];
    google::protobuf::io::ArrayOutputStream aos(pkt,siz);
    google::protobuf::io::CodedOutputStream *coded_output = new google::protobuf::io::CodedOutputStream(&aos);
    coded_output->WriteVarint32(request.ByteSize());
    request.SerializeToCodedStream(coded_output);

    boost::system::error_code err;
    _sock.write_some(boost::asio::buffer(pkt, siz), err);
    if(err) {
        delete[] pkt;
        return false;
    }
    //todo: read response
    delete[] pkt;
    return true;
}


int fs_client::read_reply(fs::proto::packet::Reply& reply) {
    char buff[4] = {0};
    _already_read = 0;
    try{
        (void) _sock.receive(boost::asio::buffer(buff, 4), boost::asio::ip::tcp::socket::message_peek);
        google::protobuf::uint32 size;
        google::protobuf::io::ArrayInputStream ais_hdr(buff, 4);
        google::protobuf::io::CodedInputStream coded_input_hdr(&ais_hdr);

        coded_input_hdr.ReadVarint32(&size);
        _already_read = read(_sock, boost::asio::buffer(_buffer, 4 + size));

        if(_already_read <= 0) {
            return -1;
        }
        google::protobuf::io::ArrayInputStream ais(_buffer, size+4);
        google::protobuf::io::CodedInputStream coded_input(&ais);
        coded_input.ReadVarint32(&size);
        google::protobuf::io::CodedInputStream::Limit msgLimit =
                coded_input.PushLimit(size);
        reply.ParseFromCodedStream(&coded_input);
        coded_input.PopLimit(msgLimit);
    }catch(std::exception& e) {
        return -1;
    }
    return 0;
}


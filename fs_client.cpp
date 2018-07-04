#include "fs_client.h"

boost::asio::io_service service;

fs_client::fs_client(boost::asio::ip::tcp::endpoint ep):_sock(service){
    _ep = ep;
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


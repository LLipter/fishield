#include "fs_client.h"

boost::asio::io_service service;

fs_client::fs_client(boost::asio::ip::tcp::endpoint ep):_sock(service){
    _ep = ep;
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
        int acc_read = _sock.receive(boost::asio::buffer(buff, 4), boost::asio::ip::tcp::socket::message_peek);
//        LOG_DEBUG << "acc_read is " << acc_read << std::endl;
        google::protobuf::uint32 size;
        google::protobuf::io::ArrayInputStream ais_hdr(buff, 4);
        google::protobuf::io::CodedInputStream coded_input_hdr(&ais_hdr);

        coded_input_hdr.ReadVarint32(&size);
//        LOG_DEBUG << "size of payload is " << size << std::endl;
        _already_read = read(_sock, boost::asio::buffer(_buffer, 4 + size),4 + size);

        if(_already_read <= 0) {
//            LOG_DEBUG << "alread_read_ is " << alread_read_;
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
//        LOG_ERROR << e.what() << std::endl;
        return -1;
    }
    if(reply.status() == fs::proto::packet::Status::STATUS_SUCCESS) {
//        LOG_DEBUG << "Status::STATUS_SUCCESS";
    }
    else {
//        LOG_DEBUG << "Status: ERROR";
    }
    return 0;
}


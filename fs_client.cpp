#include "fs_client.h"

extern boost::asio::io_service service;
std::string _token = "NO_SUCH_TOKEN";

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
    if(error){
        std::cout << "connect() failed : "
                  << error.message()
                  << std::endl;
        return false;
    }
    std::cout << "connection built : " << std::endl;
    return true;
}


bool fs_client::send_request(const fs::proto::Request request){
    using namespace fs::proto;
    std::cout << "start send request : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;
    int len = request.ByteSize();
    char* buf = new char[len+4];
    *(int*)buf = len;
    request.SerializeToArray(buf+4,len);
    boost::system::error_code err;
    _sock.write_some(boost::asio::buffer(buf,len+4), err);
    delete[] buf;
    if(err){
        std::cout << "send_request() failed : "
                  << err.message()
                  << std::endl;
        return false;
    }
    std::cout << "send_request() success : "
              << std::endl;
    return true;

}

bool fs_client::receive_response(fs::proto::Response& response){
    using namespace fs::proto;
    std::cout << "start receive response : "
              << Response::ResponseType_Name(response.resp_type())
              << std::endl;

    // read length of packet
    int len;
    int* lenptr = &len;
    boost::system::error_code err;
    _sock.read_some(boost::asio::buffer((char*)lenptr,4),err);
    if(err){
        std::cout << "receive_response() read len failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // read packet
    char* buf = new char[len];
    _sock.read_some(boost::asio::buffer(buf,len),err);
    if(err){
        delete[] buf;
        std::cout << "receive_response() read packet failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // deserialize packet
    if(response.ParseFromArray(buf,len) == false){
        delete[] buf;
        std::cout << "receive_response() deserialize packet failed : "
                  << std::endl;
        return false;
    }
    delete[] buf;
    std::cout << "receive_response() success : "
              << std::endl;
    return true;
}





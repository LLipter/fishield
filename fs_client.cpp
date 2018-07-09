#include "fs_client.h"
#include "fs_scheduler.h"

extern boost::asio::io_service service;
std::string _token = "NO_SUCH_TOKEN";


boost::asio::ip::address _server_addr = DEFAULT_SERV_ADDR;
short _server_port = DEFAULT_SERV_PORT;

fs_client::fs_client():
    _sock(service),
    _ep(_server_addr, _server_port){}

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


bool fs_client::send_request(const fs::proto::Request& request){
    using namespace fs::proto;
    std::cout << "start send request : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;
    int len = request.ByteSize();
    char* buf = new char[len+4];
    *(int*)buf = len;
    request.SerializeToArray(buf+4,len);
    boost::system::error_code err;
    boost::asio::write(_sock,boost::asio::buffer(buf,len+4), err);
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



    // read length of response
    int len;
    int* lenptr = &len;
    boost::system::error_code err;
    boost::asio::read(_sock,boost::asio::buffer((char*)lenptr,4),err);
    if(err){
        std::cout << "receive_response() read len failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // read response
    char* buf = new char[len];
    boost::asio::read(_sock,boost::asio::buffer(buf,len),err);
    if(err){
        delete[] buf;
        std::cout << "receive_response() read response failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // deserialize response
    if(response.ParseFromArray(buf,len) == false){
        delete[] buf;
        std::cout << "receive_response() deserialize response failed : "
                  << std::endl;
        return false;
    }
    delete[] buf;
    std::cout << "receive_response() success : "
              << std::endl;
    return true;
}


bool send_receive(const fs::proto::Request& request,fs::proto::Response& response){
    // connect to server
    fs_client client;
    if(client.connect() == false){
        std::cout << "connect() failed"
                  << std::endl;
        return false;
    }

    // send request
    if(client.send_request(request) == false){
        std::cout << "send_request() failed"
                  << std::endl;
        return false;
    }

    // receive response
    if(client.receive_response(response) == false){
        std::cout << "receive_response() failed"
                  << std::endl;
        return false;
    }

    return true;
}


extern std::string tasks_finished_path;
extern std::string tasks_current_path;
void fs_client::init(){

    // load task information from file
    get_task_from_file(tasks_finished_path,
                       fs_scheduler::instance()->task_map_finished);
    get_task_from_file(tasks_current_path,
                       fs_scheduler::instance()->task_map_current);


}





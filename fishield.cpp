#include "fishield.h"

extern std::map<std::string, fs_callback> callback_map;

int fs_register_event(const char* event, fs_func_ptr handler, void* user_data){
    fs_callback cb;
    cb.callback = handler;
    cb.user_data = user_data;
    callback_map[event] = cb;
    return 0;
}


extern std::string _server_addr;
extern int _server_port;
extern std::string _user_name;
extern std::string _token;

bool fs_start_up(std::string addr, int port,std::string user_name,std::string token){

    try{
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(addr,std::to_string(port));
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
        (void)iterator;
    }
    catch(std::exception& e){
        std::cerr << "client startup error: " << e.what() << std::endl;
        return false;
    }

    _server_addr = addr;
    _server_port = port;
    _user_name = user_name;
    _token = token;
    return true;

}

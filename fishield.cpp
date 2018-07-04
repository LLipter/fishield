#include "fishield.h"

extern std::map<std::string, fs_callback> callback_map;

int fs_register_event(const char* event, fs_func_ptr handler, void* user_data){
    fs_callback cb;
    cb.callback = handler;
    cb.user_data = user_data;
    callback_map[event] = cb;
    return 0;
}


bool fs_start_up(const char* addr, int port,const char* user_name,const char* token){
    if(!(addr&&user_name&&token)) {
        return false;
    }
    if(port <= 0) {
        return false;
    }
    _server_addr = addr;
    _server_port = port;
    _user_name = user_name;
    _token = token;
    //init log
    plog::init(plog::debug, property_map->at(SEC_PROP_LOGPATH).c_str());
    LOG_DEBUG << "secft client start up";
    //todo connect to server
    //https://www.zhihu.com/question/27908489
    //start thread pool
    return true;

}

#include "fishield.h"
#include <sys/mman.h>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

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

int check_upload_task(std::string path,std::map<std::string, std::string> params, fs_task_info& task_info){

    //generate task_info
    int file_descript = open(path.c_str(), O_RDONLY);
    if(file_descript < 0)
        err_quit("check_upload_task() open error %s", path.c_str());

    struct stat statbuf;
    if(fstat(file_descript, &statbuf) < 0)
        err_quit("check_upload_task() fstat error");

    task_info.local_path = path;
    if(S_ISDIR(statbuf.st_mode)) {
        err_quit("check_upload_task() %s is a directory", path.c_str());
        //todo upload or download dir
    }
    else {
        size_t file_size =  statbuf.st_size;
        char* file_buffer = (char*)mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
        unsigned char result[MD5_DIGEST_LENGTH];
        MD5((unsigned char*) file_buffer, file_size, result);
        munmap(file_buffer, file_size);
        task_info.size = file_size;
        memcpy(task_info.md5, result, MD5_DIGEST_LENGTH);
        task_info.offset = 0;
        task_info.task_status = (fs_task_status)std::stoi(params[FS_TASK_TYPE]);
    }
    close(file_descript);
    return 0;
}
int check_download_task(std::string path, std::map<std::string, std::string> params, fs_task_info& task_info){
    task_info.remote_path = path;
    task_info.offset = 0;
    task_info.size = -1;
    task_info.task_status = (fs_task_status)std::stoi(params[FS_TASK_TYPE]);
    //todo item.path
    task_info.local_path = "./";
    return 0;

}


int fs_start_task(std::string path, std::map<std::string, std::string> params)
{
    //check params
    auto it_find = params.find(FS_TASK_TYPE);
    if(it_find == params.end()) {
        std::cerr << "fs_start_task error : unknown task type" << std::endl;
        return -1;
    }
    fs_task_info task_info;
    switch (std::stoi(params[FS_TASK_TYPE])) {
    case START_UPLOAD:
        check_upload_task(path, params, task_info);
        break;
    case START_DOWNLOAD:
        check_download_task(path, params, task_info);
        break;
    default:
        err_quit("fs_start_task() unknow FS_TASK_TYPE");
    }
    std::map<std::string, std::string> config;
    task_info.config = config;
    return fs_scheduler::instance()->add_task(task_info);
}

extern int _port;
int fs_server_start_up(int port)
{
    _port = port;
    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(handle_clients_thread);
    threads.join_all();
    return 0;
}

extern std::map<std::string, std::string> username_token_map;
int fs_server_set_property(const char* prop, const char* value){
    //"SECFT_SERVER_ADD_AUTH"
    //"SECFT_SERVER_RM_AUTH"
    //"SECFT_SERVER_LOG"
    if(prop == nullptr || value == nullptr) {
        return -1;
    }
    std::vector<std::string> strs;
    boost::split(strs, value, boost::is_any_of(":"));
    if(strs.size() != 2)
        return -2;
    if(!strcmp(prop, FS_SERVER_ADD_AUTH))
        username_token_map[strs[0]] = strs[1];
    else if(!strcmp(prop, FS_SERVER_RM_AUTH))
        username_token_map.erase(prop);
    else
        return -3;

    return 0;
}



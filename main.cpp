#include "fishield.h"

using namespace std;

extern std::string fs_process_event_str[];

int user_system_callback(void* user_data, std::map<string,string> data){
    (void) user_data;
    string msg_str = data["system.callback.msg"];
    cout << msg_str << endl;
    return 0;
}

//  process data
/*
 *  type            the type of callback, see fs_process_event
 *  local_path
 *  task_id
 *  process         A percent that shows how much a file is already uploaded or downloaded
 */
int user_process_callback(void* user_data, map<string,string> data){
    (void) user_data;
    int type = stoi(data["type"]);
    int task_id = stoi(data["task_id"]);
    double process = stod(data["process"]);
    string local_path = data["local_path"];
    std::cout   <<  fs_process_event_str[type]          << std::endl
                 <<  "local_path : "     <<  local_path  << std::endl
                  <<  "task id : "        <<  task_id     << std::endl
                   <<  "process : "        <<  process     << std::endl;
    return 0;
}

int main()
{

    fs_register_event(FS_EVENT_SYSTEM, &user_system_callback, nullptr);
    fs_register_event(FS_EVENT_PROCESS, &user_process_callback, nullptr);

    string serv_addr = "127.0.0.1";
    string username = "LLipter";
    string token = "qwertyuiop";
    int port = 7614;

    if(!fs_start_up(serv_addr, port, username, token)){
        cerr << "client startup error" << endl;
        return 1;
    }



    return 0;
}

#include "config.h"
#include "fs_scheduler.h"
#include "fs_callback.h"

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

    return 0;
}

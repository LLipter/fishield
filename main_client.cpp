#include "fishield.h"

using namespace std;

extern std::string fs_process_event_str[];
extern std::string _user_name;
extern std::string _token;

void splitline(){
    cout << "------------------------------------" << endl;
}

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

    std::cout << fs_process_event_str[type] << std::endl;
    std::cout << "local_path : "            <<  local_path  << std::endl;
    std::cout << "task id : "               <<  task_id     << std::endl;
    std::cout << "process : "               <<  process     << std::endl;
    splitline();
    return 0;
}



int main()
{

    fs_register_event(FS_EVENT_SYSTEM, &user_system_callback, nullptr);
    fs_register_event(FS_EVENT_PROCESS, &user_process_callback, nullptr);

    string serv_addr = "127.0.0.1";
    string username = "token";
    string token = "token";
    int port = 7614;

    if(fs_client_start_up(serv_addr, port, username, token) != 0){
        cout << "client startup failed" << endl;
        return 1;
    }
    cout << "client startup successfully!" << endl;
    splitline();

    int id;
    map<int, string> param;
    param[FS_TASK_STATUS] = to_string(START_UPLOAD);

    //Test case 1: No such file
    id = fs_start_task("no_such_file", param);
    if(id < 0)
        cout << "Test case 1 pased" << endl;
    else{
        cout << "Test case 1 failed" << endl;
        return 1;
    }
    splitline();


    //Test cases: normal file
    id = fs_start_task("/home/irran/Desktop/music.mp3", param);
    if(id == -1) {
        std::cerr << "add task failed\n";
    }

    cout << id << endl;

    while(true){
        sleep(1);
    }


    return 0;
}

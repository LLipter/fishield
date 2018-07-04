#include "fishield.h"

using namespace std;

extern std::string fs_process_event_str[];
extern std::string _user_name;
extern std::string _token;

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
    string username = "token";
    string token = "token";
    int port = 7614;

    if(!fs_start_up(serv_addr, port, username, token))
        return 1;

    cout << _user_name << endl;
    cout << _token << endl;

    cout << "client startp successfully!" << endl;

    int id;
    //Test cases: Start upload without parameters
    map<string, string> test;
    test[FS_TASK_TYPE] = to_string(START_UPLOAD);
    //Test case one: No such file
//    id = fs_start_task("test", test);
//    if(id == -1) {
//        std::cerr << "add task failed\n";
//    }
    //Test cases: normal file
    id = fs_start_task("/home/irran/Desktop/music.mp3", test);
    if(id == -1) {
        std::cerr << "add task failed\n";
    }
//    usleep(10000);
//    secft_cancel_stream(id, test);

    //Test case: cancel upload without parameters

    //Test cases: download normal file download_test1 and download_test2
//    test[FS_TASK_TYPE] = START_DOWNLOAD;
//    id = secft_start_stream("download_test1", test);
//    if(id == -1) {
//        std::cerr << "add task failed\n";
//    }

//    id = secft_start_stream("download_test2", test);
//    if(id == -1) {
//        std::cerr << "add task failed\n";
//    }



    while(true){
        sleep(1);
    }


    return 0;
}

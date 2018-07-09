#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

class fs_task
{
public:

    fs_task();                  // default constructor
    fs_task(const fs_task& rhs);// copy constructor

    // member variables
    int task_id;
    std::string localbasepath;
    std::string remotebasepath;
    std::string filename;
    int total_packet_no;
    int received_packet_no;
    int sent_packet_no;
    long long last_packet_time;
    fs::proto::Task::TaskStatus status;

    // member functions
    void upload();
    void download();

private:
    void _upload();
    void _download();
};

#endif // FS_TASK_H

#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"
#include "fs_task_info.h"
#include "protobuf/file_transfer.pb.h"

#define FS_TASK_TYPE "task.type"

class fs_task
{
public:
    fs_task();
    fs_task(fs_task_info task_info);
    void stop();
    void upload_file();

    fs_task_info task_info;
private:
    void _upload();
    fs::proto::packet::Request get_packet();
};

#endif // FS_TASK_H

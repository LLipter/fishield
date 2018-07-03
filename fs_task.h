#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_task_info.h"

class fs_task
{
public:
    fs_task();
    fs_task(fs_task_info task_info);
    fs_task_info task_info;
    void stop();

};

#endif // FS_TASK_H

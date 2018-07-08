#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#include "fs_config.h"
#include "fs_task.h"


class fs_scheduler
{
private:

    fs_scheduler();                     // private constructor, Singleton Pattern
    static fs_scheduler scheduler_instance;
    void scheduler();                   // main sechedule method
    void _add_task(fs_task task);


    // member variables
    int max_task_num;
    int task_count;
    std::map<int, fs_task> task_map;

public:
    static fs_scheduler* instance();
    void set_task_max(int num);
    void add_task(fs_task task);
    void add_upload_task(fs_task task);
    void add_download_task(fs_task task);
};

#endif // FS_SCHEDULER_H

#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#include "fs_config.h"
#include "fs_task.h"


class fs_scheduler
{
private:

    fs_scheduler();                     // private constructor, Singleton Pattern
    static fs_scheduler scheduler_instance;
    void scheduler_thread();            // main sechedule method
    void save_thread();

    // member variables
    int max_task_num;
    int task_count;


public:
    static fs_scheduler* instance();
    void set_task_max(int num);
    void add_task(fs::proto::Task task);
    void add_upload_task(fs::proto::Task task);
    void add_download_task(fs::proto::Task task);

    std::map<int, fs::proto::Task> task_map_current;
    std::map<int, fs::proto::Task> task_map_finished;
};

#endif // FS_SCHEDULER_H

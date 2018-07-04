#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#define DEFAULE_MAX_TASKNO  5

#include "fs_task.h"


class fs_scheduler
{
private:

    fs_scheduler();                     // private constructor, Singleton Pattern
    static fs_scheduler scheduler_instance;
    void scheduler();                   // main sechedule method
    void increase_count();
    void decrease_count();

    // member variables
    int max_task_num;
    int task_count;
    std::map<int, fs_task> task_map;
    std::mutex task_count_mutex;

public:
    static fs_scheduler* instance();
    void set_task_max(int num);
    int add_task(fs_task_info task_info);
    void remove_task(int task_id);
    void stop_task(int task_id);
};

#endif // FS_SCHEDULER_H

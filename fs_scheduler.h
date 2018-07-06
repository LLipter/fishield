#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#define DEFAULE_MAX_TASKNO  5

#include "fishield.h"
#include <mutex>

class fs_scheduler
{
private:

    fs_scheduler();                     // private constructor, Singleton Pattern
    static fs_scheduler scheduler_instance;
    void scheduler();                   // main sechedule method


    // member variables
    int max_task_num;
    int task_count;
    std::mutex task_count_mutex;

public:
    static fs_scheduler* instance();
    void set_task_max(int num);
    void increase_count();
    void decrease_count();
};

#endif // FS_SCHEDULER_H

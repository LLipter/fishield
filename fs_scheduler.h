#ifndef FS_SCHEDULER_H
#define FS_SCHEDULER_H

#define DEFAULE_MAX_TASKNO  5


class fs_scheduler
{
private:

    fs_scheduler();                     // private constructor, Singleton Pattern
    static fs_scheduler* scheduler_ptr;
    void scheduler();                   // main sechedule method

    // member variables
    int max_task_num;
    int task_count;

public:
    static fs_scheduler* instance();
    void set_task_max(int num);
};

#endif // FS_SCHEDULER_H

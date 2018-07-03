#include "fs_scheduler.h"
#include <thread>
#include <unistd.h>
#include <iostream>

fs_scheduler* fs_scheduler::scheduler_ptr = nullptr;

fs_scheduler* fs_scheduler::instance(){
    if(scheduler_ptr != nullptr)
        return scheduler_ptr;
    return scheduler_ptr = new fs_scheduler();
}

fs_scheduler::fs_scheduler(){
    max_task_num = DEFAULE_MAX_TASKNO;
    task_count = 0;
    std::thread scheduler_thread(&fs_scheduler::scheduler, this);
    scheduler_thread.detach();
}

void fs_scheduler::scheduler(){
    while(true){
        // todo :  upload or download
        std::cout << "in scheduler..." << std::endl;
        sleep(1);
    }
}

void fs_scheduler::set_task_max(int num){
    max_task_num = num;
}

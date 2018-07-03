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

int task_id = 1;

int fs_scheduler::add_task(fs_task_info task_info){
    task_info.task_id = task_id;
    fs_task task(task_info);
    task_map[task_id] = task;
    return task_id++;
}

void fs_scheduler::stop_task(int task_id){
    if(task_map.find(task_id) != task_map.end())
        task_map[task_id].stop();
}

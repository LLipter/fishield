#include "fs_scheduler.h"

fs_scheduler fs_scheduler::scheduler_instance;

fs_scheduler* fs_scheduler::instance(){
    return &scheduler_instance;
}

fs_scheduler::fs_scheduler(){
    max_task_num = DEFAULE_MAX_TASKNO;
    task_count = 0;
    std::thread scheduler_thread(&fs_scheduler::scheduler, this);
    scheduler_thread.detach();
}

void fs_scheduler::scheduler(){
    while(true){
        // TODO:
        if(task_count < max_task_num) {
            for(;;) {

            }
        }
    }
}

void fs_scheduler::set_task_max(int num){
    max_task_num = num;
}

int task_id = 1;


void fs_scheduler::increase_count(){
    task_count_mutex.lock();
    task_count++;
    task_count_mutex.unlock();
}

void fs_scheduler::decrease_count(){
    task_count_mutex.lock();
    task_count--;
    task_count_mutex.unlock();
}


#include "fs_scheduler.h"
#include <thread>
#include <unistd.h>
#include <iostream>

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
        if(task_count < max_task_num) {
            for(auto iter = task_map.begin(); iter != task_map.end(); ++iter) {
                switch (iter->second.task_info.task_status) {
                case START_UPLOAD:
                    iter->second.task_info.task_status = UPLOADING;
                    iter->second.upload_file();
                    increase_count();
                    break;
//                case START_DOWNLOAD:
//                    iter->second.task_info.task_status = DOWNLOADING;
//                    iter->second.download_file();
//                    increase_count();
//                    break;
                default:
                    break;
                }
            }
        }
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
    if(task_map.find(task_id) != task_map.end()){
        task_map[task_id].stop();
        decrease_count();
    }
}

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

void fs_scheduler::remove_task(int task_id){
    task_map.erase(task_id);
}

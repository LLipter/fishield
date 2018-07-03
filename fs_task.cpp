#include "fs_task.h"
#include <iostream>

fs_task::fs_task(){

}


fs_task::fs_task(fs_task_info task_info){
    this->task_info = task_info;
}

void fs_task::stop(){
    this->task_info.task_status = UPLOAD_CANCELED;
    std::cout << this->task_info.local_path << ":canceled" << std::endl;
}

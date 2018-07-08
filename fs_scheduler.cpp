#include "fs_scheduler.h"
#include "fs_client.h"

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
                switch (iter->second.status) {
                case UPLOAD_INIT:
                    iter->second.status = UPLOADING;
                    iter->second.upload();
                    increase_count();
                    break;
                default:
                    // TODO : SET UNKNOWN STATUS RESPONSE
                    break;
                }
            }
        }
    }
}

void fs_scheduler::set_task_max(int num){
    max_task_num = num;
}

extern std::string _token;
void fs_scheduler::_add_task(fs_task task){
    using namespace fs::proto;

    Request upload_request;
    upload_request.set_req_type(Request::UPLOAD);
    upload_request.set_remote_path(task.remotebasepath);
    upload_request.set_filename(task.filename);
    upload_request.set_token(_token);
    upload_request.set_packet_no(task.total_packet_no);

    // send request and receive response
    Response response;
    if(send_receive(upload_request,response) == false){
        task.cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    switch (response.resp_type()) {
    case Response::SUCCESS:
        task.cb_start_upload(response.task_id());
        task.task_id = response.task_id();
        task_map[task.task_id] = task;
        break;
    case Response::ILLEGALTOKEN:
    case Response::ILLEGALPATH:
    case Response::ILLEGALREQUEST:
        task.cb_failed(response.resp_type());
        break;
    default:
        task.cb_failed(Response::UNKNOWN);
        break;
    }

}

void fs_scheduler::add_task(fs_task task){
    std::thread thd(&fs_scheduler::_add_task, this, task);
    thd.detach();
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



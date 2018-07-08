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
        for(auto iter=task_map.begin();iter!=task_map.end();iter++) {
            switch (iter->second.status) {
            case UPLOAD_INIT:
                if(task_count < max_task_num) {
                    iter->second.status = UPLOADING;
                    iter->second.upload();
                    increase_count();
                }
                break;
            case UPLOADED:
                decrease_count();
                // TODO : erase this task
            default:
                // TODO : SET UNKNOWN STATUS RESPONSE
                break;
            }
        }
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(100));
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
    default:
        task.cb_failed(response.resp_type());
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



#include "fs_scheduler.h"
#include "fs_client.h"
#include "fs_task.h"

extern std::string _token;
extern fs_fp_intdouble cb_progress;
extern fs_fp_int cb_success;
extern fs_fp_error cb_failed;

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
    using namespace fs::proto;
    while(true){
        std::vector<int> removed_task;
        for(auto iter=task_map.begin();iter!=task_map.end();iter++) {
            switch (iter->second.status) {
            case Task::UPLOAD_INIT:
            case Task::UPLOAD_RESUME:
                if(task_count < max_task_num) {
                    iter->second.status = Task::UPLOADING;
                    iter->second.upload();
                    task_count++;
                }
                break;
            case Task::UPLOAD_PAUSING:
                iter->second.status = Task::UPLOAD_PAUSED;
                task_count--;
                break;

            case Task::DOWNLOAD_INIT:
            case Task::DOWNLOAD_RESUME:
                if(task_count < max_task_num) {
                    iter->second.status = Task::DOWNLOADING;
                    iter->second.download();
                    task_count++;
                }
                break;
            case Task::DOWNLOAD_PAUSING:
                iter->second.status = Task::DOWNLOAD_PAUSED;
                task_count--;
                break;

            case Task::UPLOADED:
            case Task::DOWNLOADED:
            case Task::CANCELED_WORKING:
                task_count--;
                removed_task.push_back(iter->first);
                break;
            case Task::CANCELED_PAUSED:
            case Task::UPLOAD_PAUSED:
            case Task::DOWNLOAD_PAUSED:
                removed_task.push_back(iter->first);
                break;
            default:
                // TODO : SET UNKNOWN STATUS RESPONSE
                break;
            }
        }

        // TODO : save some information in disk
        // remove all completed task
        for(int id : removed_task)
            task_map.erase(id);

        // have a sleep
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::millisec(100));
    }
}

void fs_scheduler::set_task_max(int num){
    max_task_num = num;
}

void fs_scheduler::add_upload_task(fs_task task){
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
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        task.task_id = response.task_id();
        task_map[task.task_id] = task;
    }else
        cb_failed(response.resp_type());
}

void fs_scheduler::add_download_task(fs_task task){
    using namespace fs::proto;
    Request download_request;
    download_request.set_req_type(Request::DOWNLOAD);
    download_request.set_remote_path(task.remotebasepath);
    download_request.set_filename(task.filename);
    download_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(download_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        task.task_id = response.task_id();
        task.total_packet_no = response.packet_no();
        task_map[task.task_id] = task;
    }else
        cb_failed(response.resp_type());
}

void fs_scheduler::add_task(fs_task task){
    using namespace fs::proto;

    if(task.status == Task::UPLOAD_INIT){
        std::thread thd(&fs_scheduler::add_upload_task, this, task);
        thd.detach();
    }else if(task.status == Task::DOWNLOAD_INIT){
        std::thread thd(&fs_scheduler::add_download_task, this, task);
        thd.detach();
    }else
        std::cout << "_add_task() : ILLEGAL TASK STATUS" << std::endl;

}





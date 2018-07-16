#include "fs_scheduler.h"
#include "fs_client.h"
#include "fs_task.h"

extern std::string _token;
extern fs_fp_intdouble cb_progress;
extern fs_fp_int cb_success;
extern fs_fp_interror cb_failed;
std::string tasks_finished_path = std::string(".") + SEPARATOR + DEFAULT_TASKS_FINISHED_FILE;
std::string tasks_current_path = std::string(".") + SEPARATOR + DEFAULT_TASKS_CURRENT_FILE;
std::mutex client_task_mutex;

fs_scheduler fs_scheduler::scheduler_instance;

fs_scheduler* fs_scheduler::instance(){
    return &scheduler_instance;
}

fs_scheduler::fs_scheduler(){
    max_task_num = DEFAULE_MAX_TASKNO;
    task_count = 0;
    std::thread scheduler_thread(&fs_scheduler::scheduler_thread, this);
    scheduler_thread.detach();
    std::thread save_thread(&fs_scheduler::save_thread, this);
    save_thread.detach();
}

void fs_scheduler::scheduler_thread(){
    using namespace fs::proto;
    while(true){
        std::vector<int> removed_task;
        client_task_mutex.lock();
        for(auto iter=task_map_current.begin();iter!=task_map_current.end();iter++) {
            switch (iter->second.task_status()) {
            case Task::UPLOAD_INIT:
            case Task::UPLOAD_RESUME:
                if(task_count < max_task_num) {
                    iter->second.set_task_status(Task::UPLOADING);
                    upload(iter->second);
                    task_count++;
                }
                break;
            case Task::UPLOAD_PAUSING:
                iter->second.set_task_status(Task::UPLOAD_PAUSED);
                task_count--;
                break;
            case Task::DOWNLOAD_INIT:
            case Task::DOWNLOAD_RESUME:
                if(task_count < max_task_num) {
                    iter->second.set_task_status(Task::DOWNLOADING);
                    download(iter->second);
                    task_count++;
                }
                break;
            case Task::DOWNLOAD_PAUSING:
                iter->second.set_task_status(Task::DOWNLOAD_PAUSED);
                task_count--;
                break;
            case Task::UPLOADED:
            case Task::DOWNLOADED:
                task_count--;
                removed_task.push_back(iter->first);
                break;
            case Task::CANCELING:
                task_count--;
                iter->second.set_task_status(Task::CANCELED);
                break;
            case Task::CANCELED:
                removed_task.push_back(iter->first);
                break;
            case Task::UPLOADING:
            case Task::DOWNLOADING:
            case Task::UPLOAD_PAUSED:
            case Task::DOWNLOAD_PAUSED:
                // do nothing
                break;
            case Task::FAILING:
                iter->second.set_task_status(Task::FAILED);
                task_count--;
                break;
            case Task::FAILED:
                removed_task.push_back(iter->first);
                break;
            default:
                std::cout << "fs_scheduler::scheduler() : UNKNOWN TASK STATUS" << std::endl;
                break;
            }
        }

        for(int id : removed_task){
            task_map_finished[id] = task_map_current[id];
            task_map_current.erase(id);
        }
        client_task_mutex.unlock();

        // have a sleep
        boost::this_thread::sleep(DEFAULT_CLIENT_SCHEDULER_SLEEP);
    }
}

void fs_scheduler::save_thread(){
    boost::this_thread::sleep(DEFAULT_CLIENT_SAVE_SLEEP);
    while(true){
        boost::this_thread::sleep(DEFAULT_CLIENT_SAVE_SLEEP);
        client_task_mutex.lock();

        // save removed tasks in disk
        save_task_to_file(tasks_finished_path, task_map_finished);
        save_task_to_file(tasks_current_path, task_map_current);

        client_task_mutex.unlock();
    }
}

void fs_scheduler::set_task_max(int num){
    max_task_num = num;
}

void fs_scheduler::add_upload_task(fs::proto::Task task){
    using namespace fs::proto;
    Request upload_request;
    upload_request.set_req_type(Request::UPLOAD);
    upload_request.set_remote_path(task.remotebasepath());
    upload_request.set_filename(task.filename());
    upload_request.set_token(_token);
    upload_request.set_packet_no(task.total_packet_no());

    // send request and receive response
    Response response;
    if(send_receive(upload_request,response) == false){
        cb_failed(FS_E_UOLOADFAILED, Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        task.set_task_id(response.task_id());
        client_task_mutex.lock();
        task_map_current[task.task_id()] = task;
        client_task_mutex.unlock();
    }else
        cb_failed(FS_E_UOLOADFAILED, response.resp_type());
}

void fs_scheduler::add_download_task(fs::proto::Task task){
    using namespace fs::proto;
    Request download_request;
    download_request.set_req_type(Request::DOWNLOAD);
    download_request.set_remote_path(task.remotebasepath());
    download_request.set_filename(task.filename());
    download_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(download_request,response) == false){
        cb_failed(FS_E_DOWNLOADFAILED, Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() == Response::SUCCESS){
        task.set_task_id(response.task_id());
        task.set_total_packet_no(response.packet_no());
        client_task_mutex.lock();
        task_map_current[task.task_id()] = task;
        client_task_mutex.unlock();
    }else
        cb_failed(FS_E_DOWNLOADFAILED, response.resp_type());
}

void fs_scheduler::add_task(fs::proto::Task task){
    using namespace fs::proto;

    if(task.task_status() == Task::UPLOAD_INIT){
        std::thread thd(&fs_scheduler::add_upload_task, this, task);
        thd.detach();
    }else if(task.task_status() == Task::DOWNLOAD_INIT){
        std::thread thd(&fs_scheduler::add_download_task, this, task);
        thd.detach();
    }else
        std::cout << "_add_task() : ILLEGAL TASK STATUS" << std::endl;

}





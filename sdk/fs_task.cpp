#include "fs_task.h"
#include "fs_client.h"
#include "fs_scheduler.h"


extern std::string _token;
extern std::mutex client_task_mutex;

fs_fp_intdouble cb_progress = boost::bind(default_cb_progress, _1, _2);
fs_fp_int       cb_success  = boost::bind(default_cb_success, _1);
fs_fp_interror  cb_failed   = boost::bind(default_cb_failed, _1, _2);


void _upload(fs::proto::Task& task){
    using namespace fs::proto;

    while(task.sent_packet_no() < task.total_packet_no()){
        if(task.task_status() == Task::CANCELED ||
                task.task_status() == Task::CANCELING ||
                task.task_status() == Task::UPLOAD_PAUSED ||
                task.task_status() == Task::UPLOAD_PAUSING ||
                task.task_status() == Task::FAILED ||
                task.task_status() == Task::FAILING)
            return;

        if(task.task_status() != Task::UPLOADING){
            // fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::FAILING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), Response::ILLEGALTASKSTATUS);
            return;
        }

        Request packet_request;
        packet_request.set_req_type(Request::SEND_PACKET);
        packet_request.set_task_id(task.task_id());
        packet_request.set_token(_token);


        // generate packet
        Packet* packet = packet_request.mutable_packet();
        packet->set_packet_id(task.sent_packet_no());
        std::string filepath = task.localbasepath() + SEPARATOR + task.filename();
        std::ifstream file(filepath, std::ios::binary);
        if(!file){
            // fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::FAILING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), Response::ILLEGALPATH);
            return;
        }
        char* buf = new char[PACKET_SIZE];
        file.seekg(task.sent_packet_no() * PACKET_SIZE);
        file.read(buf, PACKET_SIZE);
        packet->set_data(buf, file.gcount());


        // send request and receive response
        Response response;
        if(send_receive(packet_request,response) == false){
            // non-fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::UPLOAD_PAUSING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), Response::NORESPONSE);
            file.close();
            delete[] buf;
            return;
        }

        file.close();
        delete[] buf;

        // check response type
        switch (response.resp_type()) {
        case Response::SUCCESS:
            task.set_sent_packet_no(task.sent_packet_no()+1);
            cb_progress(task.client_id(), (double)task.sent_packet_no() / task.total_packet_no());
            break;
        case Response::ILLEGALPACKETID:
            task.set_sent_packet_no(response.packet_id());
            break;
        default:
            // fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::FAILING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), response.resp_type());
            return;
        }
    }

    client_task_mutex.lock();
    task.set_task_status(Task::UPLOADED);
    client_task_mutex.unlock();
    cb_success(task.client_id());
}

void upload(fs::proto::Task& task){
    std::thread thd(_upload, std::ref(task));
    thd.detach();
}

void _download(fs::proto::Task& task){
    using namespace fs::proto;

    std::string filepath = task.localbasepath()
                            + SEPARATOR
                            + DEFAULT_HIDDEN_PREFIX
                            + task.filename();

    if(boost::filesystem::exists(filepath))
        boost::filesystem::remove(filepath);

    while(task.received_packet_no() < task.total_packet_no()){
        if(task.task_status() == Task::CANCELED ||
                task.task_status() == Task::CANCELING ||
                task.task_status() == Task::DOWNLOAD_PAUSED ||
                task.task_status() == Task::DOWNLOAD_PAUSING ||
                task.task_status() == Task::FAILED ||
                task.task_status() == Task::FAILING)
            return;

        if(task.task_status() != Task::DOWNLOADING){
            // fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::FAILING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), Response::ILLEGALTASKSTATUS);
            return;
        }

        Request packet_request;
        packet_request.set_req_type(Request::RECEIVE_PACKET);
        packet_request.set_task_id(task.task_id());
        packet_request.set_packet_id(task.received_packet_no());
        packet_request.set_token(_token);

        // send request and receive response
        Response response;
        if(send_receive(packet_request,response) == false){
            // non-fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::UPLOAD_PAUSING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), Response::NORESPONSE);
            return;
        }

        // check response type
        if(response.resp_type() == Response::SUCCESS){

            if(response.packet().packet_id() != task.received_packet_no())
                continue;

            task.set_received_packet_no(task.received_packet_no()+1);

            // write data in file
            std::ofstream file(filepath, std::ios_base::app | std::ios_base::binary);
            if(!file){
                // fetal error
                client_task_mutex.lock();
                task.set_task_status(Task::FAILING);
                client_task_mutex.unlock();
                cb_failed(task.client_id(), Response::ILLEGALPATH);
                return;
            }
            file << response.packet().data();
            file.close();

            double progress = (double)task.received_packet_no() / task.total_packet_no();
            cb_progress(task.client_id(), progress);
        }else{
            // fetal error
            client_task_mutex.lock();
            task.set_task_status(Task::FAILING);
            client_task_mutex.unlock();
            cb_failed(task.client_id(), response.resp_type());
            return;
        }
    }


    Request confirm_request;
    confirm_request.set_req_type(Request::DOWNLOAD_CONFIRM);
    confirm_request.set_task_id(task.task_id());
    confirm_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(confirm_request,response) == false){
        // non-fetal error
        client_task_mutex.lock();
        task.set_task_status(Task::UPLOAD_PAUSING);
        client_task_mutex.unlock();
        cb_failed(task.client_id(), Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() != Response::SUCCESS){
        // fetal error
        client_task_mutex.lock();
        task.set_task_status(Task::FAILING);
        client_task_mutex.unlock();
        cb_failed(task.client_id(), response.resp_type());
        return;
    }

    cb_success(task.client_id());
    task.set_task_status(Task::DOWNLOADED);

    boost::filesystem::rename(filepath,
                              task.localbasepath() + SEPARATOR + task.filename());
}

void download(fs::proto::Task& task){
    std::thread thd(_download, std::ref(task));
    thd.detach();
}

int get_taskid_by_clientid(int clientid){
    fs_scheduler* scheduler = fs_scheduler::instance();
    auto& task_map = scheduler->task_map_current;
    for(auto it=task_map.begin();it!=task_map.end();it++)
        if((int)it->second.client_id() == clientid)
            return it->first;
    return FS_E_NOSUCHID;
}

void get_task_from_file(std::string filepath, std::map<int, fs::proto::Task>& task_map){
    using namespace fs::proto;

    Task task;
    int size = BUFFER_SIZE;
    char* buf = new char[size];
    int len;

    std::ifstream file(filepath, std::ios::binary);
    while(file.read((char*)&len, 4)){
        if(len > size){             // buffer is not big enough
            delete[] buf;
            buf = new char[len];
            size = len;
        }
        file.read(buf, len);
        task.ParseFromArray(buf, len);
        task_map[task.task_id()] = task;
    }

    file.close();
    delete[] buf;
}

void save_task_to_file(std::string filepath, std::map<int, fs::proto::Task>& task_map){
    using namespace fs::proto;

    int size = BUFFER_SIZE;         // buffer size
    char* buf = new char[size];

    std::ofstream file(filepath, std::ios::binary | std::ios::trunc);
    for(auto it=task_map.begin();it!=task_map.end();it++){
        Task& task = it->second;
        int len = task.ByteSize();
        if(len > size){             // buffer is not big enough
            delete[] buf;
            buf = new char[len];
            size = len;
        }

        task.SerializeToArray(buf, len);
        file.write((char*)&len, 4);
        file.write(buf, len);
    }
    file.close();
    delete[] buf;
}

bool is_timeout(const fs::proto::Task& task){
    return std::time(0) - task.last_packet_time() > DEFAULT_TASK_TIMEOUT;
}

void default_cb_progress(int clientid, double progress){
    std::cout << "callback : upload/download progress "
              << "(clientid=" << clientid << ")"
              << "(progress=" << progress << ")" << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

void default_cb_success(int clientid){
    std::cout << "callback : upload/download successfully (clientid="
              << clientid << ")" << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

void default_cb_failed(int clientid, fs::proto::Response::ResponseType error){
    std::cout << "callback : upload/download failed (clientid="
              << clientid << ") "
              << fs::proto::Response::ResponseType_Name(error)
              << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

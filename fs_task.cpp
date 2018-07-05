#include "fishield.h"
#include <boost/asio.hpp>


fs_task::fs_task(){}

fs_task::fs_task(fs_task_info task_info){
    this->task_info = task_info;
}

void fs_task::stop(){
    this->task_info.task_status = UPLOAD_CANCELED;
    std::cout << this->task_info.local_path << ":canceled" << std::endl;
}


void fs_task::upload_file(){
    std::thread upload_thread(&fs_task::_upload,this);
    upload_thread.detach();
}


extern std::string _user_name;
extern std::string _token;

fs::proto::packet::Request fs_task::get_packet()
{
    fs::proto::packet::Request request;
    std::ifstream infile;
    request.mutable_packet()->set_user_name(_user_name);
    request.mutable_packet()->set_token(_token);

    infile.open(this->task_info.local_path, std::ios::in|std::ios::binary);
    if(!infile.is_open()){
        std::cerr << "open failed";
        return request;
    }

    char memblock[BUFFER_SIZE] = {0};
    //todo
    request.mutable_upload_request()->set_path(this->task_info.local_path);
    request.mutable_upload_request()->set_overwrite(true);
    infile.seekg(this->task_info.offset,std::ios_base::beg);
    infile.read (memblock, BUFFER_SIZE);
    ssize_t data_size = BUFFER_SIZE;
    if (!infile){
        data_size = infile.gcount();
    }
    this->task_info.offset += data_size;
    if(this->task_info.offset == this->task_info.size) {
        std::cout << "send over\n";
        request.mutable_packet()->set_flags(fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_LAST_PACKET);
    }
    else if(this->task_info.offset == data_size){
        request.mutable_packet()->set_flags(fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_FIRST_PACKET);
    }
    else if(this->task_info.offset < this->task_info.size) {
        request.mutable_packet()->set_flags(fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_PACKET);
    }
    request.mutable_packet()->set_file_size(this->task_info.size);
    request.mutable_packet()->set_data(memblock, data_size);

    request.mutable_download_request()->set_path("");
    request.mutable_packet_request()->set_dummy(1);
    infile.close();
    return request;
}



extern std::map<int, fs_callback> callback_map;

void callback(int event, std::map<std::string, std::string> callback_data){
    if(callback_map[event].callback){
        void* user_data = callback_map[event].user_data;
        fs_func_ptr func = callback_map[event].callback;
        func(user_data, callback_data);
    }
}

void fs_task::_upload()
{
    std::string msg = "start upload " + this->task_info.local_path;
    std::map<std::string, std::string> system_callback_data;
    system_callback_data["system.callback.msg"] = msg;
    callback(FS_EVENT_SYSTEM,system_callback_data);
    //read file and generate packet
    //new connect
    fs_client client;
    if(!client.connect()) {
        msg = "connect error";
        system_callback_data["system.callback.msg"] = msg;
        callback(FS_EVENT_SYSTEM,system_callback_data);
        return;
    }

    while (this->task_info.offset < this->task_info.size) {
        if(this->task_info.task_status == UPLOAD_CANCELED ||
                this->task_info.task_status == UPLOAD_PAUSED ||
                this->task_info.task_status == UPLOADED ||
                this->task_info.task_status == UPLOAD_FAILED){
            fs_scheduler* scduler = fs_scheduler::instance();
            scduler->decrease_count();

            break;
        }
        // task_status is UPLOADING
        fs::proto::packet::Request request = get_packet();
        std::map<std::string, std::string> process_callback_data;
        process_callback_data["local_path"] = this->task_info.local_path;
        process_callback_data["task_id"] = std::to_string(this->task_info.task_id);
        process_callback_data["process"] = std::to_string((double)this->task_info.offset / this->task_info.size);

        if(!client.send_request(request)) {
            this->task_info.task_status = UPLOAD_FAILED;
            process_callback_data["type"] = std::to_string(fs_process_failed);
            callback(FS_EVENT_PROCESS,process_callback_data);
        }

        fs::proto::packet::Reply reply;
        if(client.read_reply(reply) == 0) {
            int reply_status = reply.status();
            if(reply_status == fs::proto::packet::Status::STATUS_SUCCESS){
                process_callback_data["type"] = fs_process_upload;
                callback(FS_EVENT_PROCESS,process_callback_data);
            }
        }
        else {
            std::cerr << "send failed\n";
            this->task_info.task_status = UPLOAD_FAILED;
            process_callback_data["type"] = std::to_string(fs_process_failed);
            callback(FS_EVENT_PROCESS,process_callback_data);

        }

        if(this->task_info.offset == this->task_info.size){
            this->task_info.task_status = UPLOADED;
            std::map<std::string, std::string> process_callback_data;
            process_callback_data["type"] = std::to_string(fs_process_uploaded);
            callback(FS_EVENT_PROCESS,process_callback_data);
        }
    }
}

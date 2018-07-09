#include "fs_task.h"
#include "fs_client.h"

fs_fp_intdouble cb_progress;
fs_fp_int cb_success;
fs_fp_error cb_failed;

extern std::string _token;
void _upload(fs::proto::Task& task){
    using namespace fs::proto;

    while(sent_packet_no < total_packet_no){
        if(status == Task::CANCELED_PAUSED ||
                status == Task::CANCELED_WORKING ||
                status == Task::UPLOAD_PAUSED)
            return;

        if(status != Task::UPLOADING){
            cb_failed(Response::ILLEGALTASKSTATUS);
            return;
        }

        Request packet_request;
        packet_request.set_req_type(Request::SEND_PACKET);
        packet_request.set_task_id(this->task_id);
        packet_request.set_token(_token);


        // generate packet
        Packet* packet = packet_request.mutable_packet();
        packet->set_packet_id(sent_packet_no);
        std::string filepath = localbasepath + SEPARATOR + filename;
        std::ifstream file(filepath, std::ios_base::binary);
        if(!file){
            cb_failed(Response::ILLEGALPATH);
            return;
        }
        char* buf = new char[PACKET_SIZE];
        file.seekg(sent_packet_no * PACKET_SIZE);
        file.read(buf, PACKET_SIZE);
        packet->set_data(buf, file.gcount());


        // send request and receive response
        Response response;
        if(send_receive(packet_request,response) == false){
            cb_failed(Response::NORESPONSE);
            file.close();
            delete[] buf;
            return;
        }

        file.close();
        delete[] buf;

        // check response type
        switch (response.resp_type()) {
        case Response::SUCCESS:
            sent_packet_no++;
            cb_progress(this->task_id, (double)sent_packet_no / total_packet_no);
            break;
        case Response::ILLEGALPACKETID:
            sent_packet_no = response.packet_id();
            break;
        default:
            cb_failed(response.resp_type());
            return;
        }
    }

    cb_success(task_id);
    status = Task::UPLOADED;
}

void upload(fs::proto::Task& task){
    std::thread thd(&fs_task::_upload, task);
    thd.detach();
}

void _download(fs::proto::Task& task){
    using namespace fs::proto;

    std::string filepath = this->localbasepath
                            + SEPARATOR
                            + DEFAULT_HIDDEN_PREFIX
                            + this->filename;

    while(received_packet_no < total_packet_no){
        if(status == Task::CANCELED_PAUSED ||
                status == Task::CANCELED_WORKING ||
                status == Task::DOWNLOAD_PAUSED)
            return;

        if(status != Task::DOWNLOADING){
            cb_failed(Response::ILLEGALTASKSTATUS);
            return;
        }

        Request packet_request;
        packet_request.set_req_type(Request::RECEIVE_PACKET);
        packet_request.set_task_id(this->task_id);
        packet_request.set_packet_id(received_packet_no);
        packet_request.set_token(_token);

        // send request and receive response
        Response response;
        if(send_receive(packet_request,response) == false){
            cb_failed(Response::NORESPONSE);
            return;
        }

        // check response type
        if(response.resp_type() == Response::SUCCESS){

            if((int)response.packet().packet_id() != received_packet_no)
                continue;

            received_packet_no++;

            // write data in file
            std::ofstream file(filepath, std::ios_base::app | std::ios_base::binary);
            if(!file){
                cb_failed(Response::ILLEGALPATH);
                return;
            }
            file << response.packet().data();
            file.close();

            double progress = (double)received_packet_no / total_packet_no;
            cb_progress(this->task_id, progress);
        }else{
            cb_failed(response.resp_type());
            return;
        }
    }


    Request confirm_request;
    confirm_request.set_req_type(Request::DOWNLOAD_CONFIRM);
    confirm_request.set_task_id(this->task_id);
    confirm_request.set_token(_token);

    // send request and receive response
    Response response;
    if(send_receive(confirm_request,response) == false){
        cb_failed(Response::NORESPONSE);
        return;
    }

    // check response type
    if(response.resp_type() != Response::SUCCESS){
        cb_failed(response.resp_type());
        return;
    }

    cb_success(task_id);
    status = Task::DOWNLOADED;

    boost::filesystem::rename(filepath,
                              this->localbasepath + SEPARATOR + this->filename);
}


void download(fs::proto::Task& task){
    std::thread thd(&fs_task::_download, task);
    thd.detach();
}



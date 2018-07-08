#include "fs_task.h"
#include "fs_client.h"

extern std::string _token;
void fs_task::_upload(){
    using namespace fs::proto;

    while(sent_packet_no < total_packet_no){
        if(status == CANCELED)
            return;
        if(status == UPLOAD_PAUSED){
            // TODO : save data in disks
            return;
        }
        if(status != UPLOADING){
            cb_failed(Response::ILLEGALTASKSTATUS);
            return;
        }

        Request packet_request;
        packet_request.set_req_type(Request::PACKET);
        packet_request.set_task_id(this->task_id);


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
            cb_progress((double)sent_packet_no / total_packet_no);
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
    status = UPLOADED;
}

void fs_task::upload(){
    std::thread thd(&fs_task::_upload, this);
    thd.detach();
}




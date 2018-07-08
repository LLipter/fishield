#include "fs_task.h"
#include "fs_client.h"

void fs_task::_upload(){
    using namespace fs::proto;

    for(int i=0;i<total_packet_no;i++){
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
        packet_request.set_task_id(task_id);

        // generate packet
        Packet* packet = packet_request.mutable_packet();
        packet->set_packet_id(i);
        std::string filepath = localbasepath + SEPARATOR + filename;
        std::ifstream file(filepath, std::ios_base::binary);
        if(!file){
            cb_failed(Response::ILLEGALPATH);
            return;
        }
        char* buf = new char[PACKET_SIZE];
        file.seekg(i * PACKET_SIZE);
        file.read(buf, PACKET_SIZE);
        packet->set_data(buf, PACKET_SIZE);


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
            i = response.packet_id() - 1;
            break;
        case Response::ILLEGALTOKEN:
        case Response::ILLEGALPATH:
        case Response::ILLEGALTASKID:
        case Response::ILLEGALREQUEST:
            cb_failed(response.resp_type());
            return;
        default:
            cb_failed(Response::UNKNOWN);
            break;
        }
    }

    cb_success(task_id);
}

void fs_task::upload(){
    std::thread thd(&fs_task::_upload, this);
    thd.detach();
}




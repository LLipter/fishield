#include "fs_server.h"
#include "fs_error.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

typedef boost::shared_ptr<fs_server> server_ptr;

extern boost::asio::io_service service;
std::map<std::string, std::string> username_token_map;
int _port = SERV_PORT;
std::vector<server_ptr> clients;

fs_server::fs_server() : _sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    is_login = false;
    already_read = 0;
}

boost::asio::ip::tcp::socket& fs_server::sock(){
    return _sock;
}

void fs_server::login(){
    is_login = true;
}

std::string fs_server::username(){
    return _username;
}

void fs_server::stop(){
    boost::system::error_code err;
    _sock.close(err);
}

void fs_server::read_request() {
    char buff[4] = {0};
    already_read = 0;
    if ( _sock.available()){
        (void)_sock.receive(boost::asio::buffer(buff, 4), boost::asio::ip::tcp::socket::message_peek );
        google::protobuf::uint32 size;
        google::protobuf::io::ArrayInputStream ais_hdr(buff,4);
        google::protobuf::io::CodedInputStream coded_input_hdr(&ais_hdr);
        coded_input_hdr.ReadVarint32(&size);//Decode the HDR and get the size

        already_read = _sock.read_some(boost::asio::buffer(data_buffer, 4+size));
        if(already_read <= 0)
            err_quit("fs_talk_to_client::read_request() - read_some() return non-positive value");
        google::protobuf::io::ArrayInputStream ais(data_buffer,size+4);
        google::protobuf::io::CodedInputStream coded_input(&ais);
        //Read an unsigned integer with Varint encoding, truncating to 32 bits.
        coded_input.ReadVarint32(&size);
        //After the message's length is read, PushLimit() is used to prevent the CodedInputStream
        //from reading beyond that length.Limits are used when parsing length-delimited
        //embedded messages
        google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(size);
        //De-Serialize

        fs::proto::packet::Request request;
        request.ParseFromCodedStream(&coded_input);
        //Once the embedded message has been parsed, PopLimit() is called to undo the limit
        coded_input.PopLimit(msgLimit);
        std::cout << request.packet().user_name();
        std::cout << request.packet().token();
        client_req = request;
        //Print the message
    }else
        err_quit("fs_talk_to_client::read_request() error - _sock is not available");
}


void fs_server::process_request() {
    if(already_read <= 0)
        err_quit("fs_talk_to_client::process_request() error - already_read is not positive");

    last_packet = boost::posix_time::microsec_clock::local_time();
    //Step 1. handle auth
    std::string username = client_req.packet().user_name();
    std::string token = client_req.packet().token();
    if(auth_username_token(username, token)) {
        _username = username;
        std::cout << "Auth ok: user_name is " << username
                  << " token is " << token << std::endl;
    }
    else {
        std::cout << "Auth failed: user_name is " << username
                  << " token is " << token << std::endl;
        stop();
        return;
    }
    //Step 2. handle packet
    std::string download_path = client_req.download_request().path();
    std::string upload_path = client_req.upload_request().path();
    if(download_path.size() != 0)
        do_download();
    else if (upload_path.size() != 0) {
        do_upload();
    }
    else{
        std::cout << "Invaliad packet: no upload request and download request" << std::endl;
        stop();
    }
}



void fs_server::do_download(){
    int packet_flag = client_req.packet().flags();
    if(packet_flag == fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_FIRST_PACKET){
        check_download();
    }
    else if(packet_flag == fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_PACKET){
        send_download_packet();
    }
}

void fs_server::check_download()
{
    std::string path = client_req.download_request().path();
    std::string root_path = "./";
    std::string file_path = root_path + path;
    fs::proto::packet::Reply reply;


    struct stat statbuf;
    if(stat(file_path.c_str(), &statbuf) == 0){ // file exists
        if(S_ISDIR(statbuf.st_mode)){
            // todo check if path refers to a directory
        }else{
            reply.set_status(fs::proto::packet::STATUS_PATH_ALREADY_EXISTS);
            fs::proto::packet::FileList *file_list = new fs::proto::packet::FileList();
            fs::proto::packet::FileList::Item *item = file_list->add_item();
            item->set_name(path);
            item->set_is_directory(false);
            off_t file_size = statbuf.st_size;
            item->set_size(file_size);
            long mtime = statbuf.st_mtim.tv_sec;
            item->set_modification_time(mtime);
            reply.set_allocated_file_list(file_list);
            send_reply(reply);
        }
    }else if(errno == ENOENT){                  // file doesn't exist
        reply.set_status(fs::proto::packet::STATUS_PATH_NOT_FOUND);
        send_reply(reply);
        stop();
    }else
        err_quit("fs_server::check_download() stat error");
}


void fs_server::send_reply(fs::proto::packet::Reply reply){
    std::cout << "reply status: " << reply.status() << std::endl;
    int siz = reply.ByteSize()+4;
    char *pkt = new char [siz];
    google::protobuf::io::ArrayOutputStream aos(pkt,siz);
    google::protobuf::io::CodedOutputStream *coded_output = new google::protobuf::io::CodedOutputStream(&aos);
    coded_output->WriteVarint32(reply.ByteSize());
    reply.SerializeToCodedStream(coded_output);
    boost::system::error_code err;
    _sock.write_some(boost::asio::buffer(pkt, siz), err);
    if(err) {
        delete[] pkt;
        err_quit("fs_server::send_reply _sock write_some error");
    }
    //todo: read response
    delete[] pkt;
}

void fs_server::send_download_packet(){
    std::string path = client_req.download_request().path();
    std::string root_path = "./";
    std::string file_path = root_path + path;
    fs::proto::packet::Reply reply;
    fs::proto::packet::Packet packet;
    ssize_t offset = 0;

    struct stat statbuf;
    if(stat(file_path.c_str(), &statbuf) == -1)
        err_quit("fs_server::send_download_packet() stat error");
    off_t file_size = statbuf.st_size;

    std::ifstream infile;
    infile.open(file_path, std::ios::in|std::ios::binary);
    if(!infile.is_open()){
        reply.set_status(fs::proto::packet::STATUS_FILE_OPEN_ERROR);
        send_reply(reply);
    }
    else {
        while(offset < file_size){
            reply.set_status(fs::proto::packet::STATUS_SUCCESS);
            ssize_t acc_read = get_packet(file_path, packet);
            if(acc_read > 0) {
                offset += acc_read;
                reply.set_allocated_packet(&packet);
                send_reply(reply);
            }
            else {
                reply.set_status(fs::proto::packet::STATUS_FILE_READ_ERROR);
                send_reply(reply);
                break;
            }
        }
    }
    stop();
}

ssize_t fs_server::get_packet(std::string file_path, fs::proto::packet::Packet &packet){
    // todo aaaa finish it!
    (void) file_path;
    (void) packet;
    return 0;
}

void fs_server::do_upload() {
    std::string path = client_req.upload_request().path();
    bool overwrite = client_req.upload_request().overwrite();
    std::cout << "upload path is " << path
              << " and overwrite is " << overwrite << std::endl;
    fs::proto::packet::Reply reply;
    //save file
    std::string root_path = "./";
    std::string file_path = root_path + path;
    std::ofstream outfile;
    int packet_flag = client_req.packet().flags();
    switch (packet_flag) {
    case fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_FIRST_PACKET:
        std::cout << "Start recving packets";
        if(overwrite){
            outfile.open(file_path, std::ios::out|std::ios::binary);
        }
        else{
            struct stat statbuf;
            if(stat(file_path.c_str(), &statbuf) != -1){    // file already exists
                reply.set_status(fs::proto::packet::Status::STATUS_PATH_ALREADY_EXISTS);
                send_reply(reply);
                stop();
                return;
            }
        }
        break;
    case fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_PACKET:
        std::cout << "Recving packets" << std::endl;
        outfile.open(file_path, std::ios::out|std::ios::binary|std::ios::app);
        break;
    case fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_LAST_PACKET:
        std::cout << "Last packet" << std::endl;
        outfile.open(file_path, std::ios::out|std::ios::binary|std::ios::app);
        break;
    default:
        break;
    }

    if(!outfile.is_open()){
        std::cout << file_path <<"open failed" << std::endl;
        return;
    }
    outfile.write(client_req.packet().data().c_str(), client_req.packet().data().size());
    outfile.close();
    reply.set_status(fs::proto::packet::Status::STATUS_SUCCESS);
    send_reply(reply);
    if(packet_flag == fs::proto::packet::Packet::Flags::Packet_Flags_FLAG_LAST_PACKET) {
        stop();
    }
    //uint64_t file_size = client_req.packet().file_size();
}

void fs_server::answer_to_client() {
    try {
        read_request();
        process_request();
    } catch ( boost::system::system_error&) {
        stop();
    }
    if (timed_out()) {
        stop();
        std::cout << "stopping " << _username << " - no ping in time" << std::endl;
    }
}


bool fs_server::timed_out() const {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    long long ms = (now - last_packet).total_milliseconds();
    return ms > 5000 ;
}

bool auth_username_token(std::string username, std::string token){
    std::map<std::string, std::string>::iterator find_iter = username_token_map.find(username);
    if(find_iter == username_token_map.end())
        return false;
    return find_iter->second == token;
}

void accept_thread() {
    boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port));
    while ( true) {
        server_ptr new_server( new fs_server);
        acceptor.accept(new_server->sock());
        clients.push_back(new_server);
    }
}


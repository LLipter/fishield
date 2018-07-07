#include "fs_server.h"

boost::asio::io_service service;
short _port = DEFAULT_SERV_PORT;
std::string rootdir = DEFAULT_ROOT_DIR;
std::string hidden_prefix = DEFAULT_HIDDEN_PREFIX;
std::vector<server_ptr> clients;

fs_server::fs_server():_sock(service){
    memset(data_buffer,0,sizeof(data_buffer));
    _is_stop = false;
}


boost::asio::ip::tcp::socket& fs_server::sock(){
    return _sock;
}

bool fs_server::is_stop(){
    return _is_stop;
}

void fs_server::set_stop(bool status){
    _is_stop = status;
}

void fs_server::init(){
    if(!boost::filesystem::exists(rootdir))
        boost::filesystem::create_directories(rootdir);
}


bool fs_server::receive_request(fs::proto::Request& request){
    using namespace fs::proto;

    // read length of request
    int len;
    int* lenptr = &len;
    boost::system::error_code err;
    boost::asio::read(_sock,boost::asio::buffer((char*)lenptr,4),err);

    if(err == boost::asio::error::eof)  // connection closed by other side
        return false;
    if(err){
        std::cout << "receive_request() read len failed : "
                  << err.message()
                  << std::endl;
        return false;
    }


    std::cout << "start receive request : "
              << Request::RequestType_Name(request.req_type())
              << std::endl;

    // read request
    char* buf = new char[len];
    boost::asio::read(_sock,boost::asio::buffer(buf,len),err);
    if(err){
        delete[] buf;
        std::cout << "receive_request() read request failed : "
                  << err.message()
                  << std::endl;
        return false;
    }

    // deserialize request
    if(request.ParseFromArray(buf,len) == false){
        delete[] buf;
        std::cout << "receive_request() deserialize request failed : "
                  << std::endl;
        return false;
    }
    delete[] buf;
    std::cout << "receive_request() success : "
              << std::endl;
    return true;
}

bool fs_server::send_response(const fs::proto::Response& response){
    using namespace fs::proto;
    std::cout << "start send response : "
              << Response::ResponseType_Name(response.resp_type())
              << std::endl;
    int len = response.ByteSize();
    char* buf = new char[len+4];
    *(int*)buf = len;
    response.SerializeToArray(buf+4,len);
    boost::system::error_code err;
    boost::asio::write(_sock,boost::asio::buffer(buf,len+4), err);
    delete[] buf;
    if(err){
        std::cout << "send_response() failed : "
                  << err.message()
                  << std::endl;
        return false;
    }
    std::cout << "send_response() success : "
              << std::endl;
    return true;
}




void accept_thread() {
    typedef boost::asio::ip::tcp::acceptor acceptor;
    typedef boost::asio::ip::tcp::endpoint endpoint;
    acceptor acptor(service, endpoint(boost::asio::ip::tcp::v4(), _port));
    while (true) {
        server_ptr serptr(new fs_server);
        acptor.accept(serptr->sock());
        clients.push_back(serptr);
        std::thread thd(communicate_thread,serptr);
        thd.detach();
    }
}


void remove_clients_thread() {
    while (true) {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        // erase clients that are stoped
        clients.erase(std::remove_if(clients.begin(),
                                     clients.end(),
                                     boost::bind(&fs_server::is_stop,_1)),
                      clients.end());
        // TODO : REMOVE ALL TIMEOUT CLIENTS
    }
}


void getFilelist(const std::string& dirpath, fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string realpath_str = rootdir + dirpath;
    path realpath(realpath_str);
    if(!exists(realpath) || !is_directory(realpath)){
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    response.set_resp_type(Response::SUCCESS);
    FileList* filelist = new FileList;
    filelist->set_path(dirpath);
    directory_iterator end_iter;
    for(directory_iterator iter(realpath);iter!=end_iter;iter++){
        if(boost::algorithm::starts_with(iter->path().filename().string(), hidden_prefix))
            // ignore hidden files
            continue;

        File* file = filelist->add_file();
        // set file name
        file->set_filename(iter->path().filename().string());

        // set file type
        if(is_directory(iter->path()))
            file->set_file_type(File::DIRECTORY);
        else if(is_regular_file(iter->path()))
            file->set_file_type(File::REGULAR);
        else if(is_symlink(iter->path()))
            file->set_file_type(File::SYMLINK);
        else
            file->set_file_type(File::OTHER);

        // set file size
        if(is_regular_file(iter->path()))
            file->set_size(file_size(iter->path()));

        // set last modified time
        file->set_mtime(last_write_time(iter->path()));

    }

    response.set_allocated_file_list(filelist);

}

void mkdir(const std::string& basepath, const std::string& dirname, fs::proto::Response& response){
    using namespace fs::proto;
    using namespace boost::filesystem;

    std::string base_str = rootdir + basepath;
    path base(base_str);
    if(!exists(base)){
        // basepath doesn't exist
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    std::string newdir_str = base_str + SEPARATOR + dirname;
    path newdir(newdir_str);
    if(exists(newdir)){
        // newdir already exists
        response.set_resp_type(Response::ILLEGALPATH);
        return;
    }

    create_directory(newdir);
    response.set_resp_type(Response::SUCCESS);

}

// TODO verify token
bool verify_token(const std::string& token){
    return true;
}

void communicate_thread(server_ptr serptr){
    using namespace fs::proto;
    int ret_req;
    int ret_resp;

    std::cout << "connection built with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;

    while(true){
        Request request;
        ret_req = serptr->receive_request(request);
        if(ret_req == false)
            break;
        Response response;
        switch (request.req_type()) {
        case Request::LOGIN:
            // TODO connect to database to verify password&username
            // TODO generate a random token and stored it
            response.set_resp_type(Response::SUCCESS);
            response.set_token("ttttooookkkkeeeennnn");
            break;
        case Request::FILELIST:
            if(verify_token(request.token()))
                getFilelist(request.remote_path(), response);
            else
                response.set_resp_type(Response::ILLEGALTOKEN);
            break;
        case Request::MKDIR:
            if(verify_token(request.token()))
                mkdir(request.remote_path(), request.filename(), response);
            else
                response.set_resp_type(Response::ILLEGALTOKEN);
            break;
        default:
            response.set_resp_type(Response::ILLEGALREQUEST);
            break;
        }
        ret_resp = serptr->send_response(response);
        if(ret_resp == false)
            break;
    }

    serptr->set_stop(true);
    std::cout << "connection lost with "
              << serptr->sock().remote_endpoint().address().to_string()
              << std::endl;
}


#ifndef FS_SERVER_H
#define FS_SERVER_H

#include "fs_config.h"
#include "protobuf/file_transfer.pb.h"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#define FS_SERVER_ADD_AUTH "secft.server.add_auth"
#define FS_SERVER_RM_AUTH  "secft.server.rm_auth"


bool auth_username_token(std::string username, std::string token);
void accept_thread();
void handle_clients_thread();

class fs_server :
        public boost::enable_shared_from_this<fs_server>,
        public boost::noncopyable
{
public:
    fs_server();
    boost::asio::ip::tcp::socket& sock();
    void login();
    std::string username();
    void stop();
    void answer_to_client();
    bool timed_out() const;
private:
    // member varibales
    boost::asio::ip::tcp::socket _sock;
    char data_buffer[BUFFER_SIZE];
    bool is_login;
    std::string _username;
    int already_read;
    fs::proto::packet::Request client_req;
    boost::posix_time::ptime last_packet;
    // member functions
    void read_request();
    void process_request();
    void do_download();
    void do_upload();
    void check_download();
    void send_reply(fs::proto::packet::Reply reply);
    void send_download_packet();
    ssize_t get_packet(std::string file_path, fs::proto::packet::Packet& packet);
};

#endif // FS_SERVER_H

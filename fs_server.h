#ifndef FS_SERVER_H
#define FS_SERVER_H

#include "fs_config.h"
#include "protobuf/file_transfer.pb.h"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

bool auth_username_token(std::string username, std::string token);


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
    void check_download();
    void send_reply(fs::proto::packet::Reply reply);
};

#endif // FS_SERVER_H

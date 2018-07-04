#ifndef FS_TALK_TO_CLIENT_H
#define FS_TALK_TO_CLIENT_H

#include "fs_config.h"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

class fs_talk_to_client :
        public boost::enable_shared_from_this<fs_talk_to_client>,
        public boost::noncopyable
{
public:
    fs_talk_to_client();
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
    // member functions
    void read_request();
};

#endif // FS_TALK_TO_CLIENT_H

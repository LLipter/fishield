#ifndef FS_TALK_TO_CLIENT_H
#define FS_TALK_TO_CLIENT_H

#include "fs_config.h"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

class fs_talk_to_client :
        public boost::enable_shared_from_this<fs_talk_to_client>,
        public boost::noncopyable
{
public:
    fs_talk_to_client();
private:
    boost::asio::ip::tcp::socket _sock;
    char data_buffer[BUFFER_SIZE];
};

#endif // FS_TALK_TO_CLIENT_H

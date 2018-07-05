#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "fs_config.h"
#include "file_transfer.pb.h"
#include <boost/asio.hpp>


class fs_client
{
public:
    fs_client();
    bool connect();
    void close();
    bool send_request(fs::proto::packet::Request request);
    int read_reply(fs::proto::packet::Reply& reply);
private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _ep;
    int _already_read;
    char _buffer[BUFFER_SIZE];
};




#endif // FS_CLIENT_H

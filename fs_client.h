#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "protobuf/file_transfer.pb.h"
#include <boost/asio.hpp>

class fs_client
{
public:
    fs_client(boost::asio::ip::tcp::endpoint ep);
    bool connect();
    void close();
private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _ep;
};

#endif // FS_CLIENT_H

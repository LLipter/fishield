#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "file_transfer.pb.h"
#include "fishield.h"


class fs_client
{
public:
    fs_client();
    bool connect();
private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _ep;
    char data_buffer[BUFFER_SIZE];
};




#endif // FS_CLIENT_H

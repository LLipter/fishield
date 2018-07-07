#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "fishield.h"
#include "file_transfer.pb.h"

bool send_receive(const fs::proto::Request& request,fs::proto::Response& response);


class fs_client
{
public:
    fs_client();
    bool connect();
    bool send_request(const fs::proto::Request& request);
    bool receive_response(fs::proto::Response& response);
private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _ep;
    char data_buffer[BUFFER_SIZE];
};




#endif // FS_CLIENT_H

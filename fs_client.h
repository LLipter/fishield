#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "fs_config.h"

bool send_receive(const fs::proto::Request& request,fs::proto::Response& response);


class fs_client
{
public:
    fs_client();
    // TODO : load task data from disk
    static void init();
    bool connect();
    bool send_request(const fs::proto::Request& request);
    bool receive_response(fs::proto::Response& response);
private:
    boost::asio::ip::tcp::socket _sock;
    boost::asio::ip::tcp::endpoint _ep;
};




#endif // FS_CLIENT_H

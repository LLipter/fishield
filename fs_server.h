#ifndef FS_SERVER_H
#define FS_SERVER_H

#include "fishield.h"


class fs_server
{
public:
    fs_server();
    boost::asio::ip::tcp::socket& sock();
    int receive_request();
    int send_response();

private:
    //member variables
    char data_buffer[BUFFER_SIZE];
    boost::asio::ip::tcp::socket _sock;
};

typedef boost::shared_ptr<fs_server> server_ptr;

void accept_thread();
void communicate_thread(server_ptr serptr);

#endif // FS_SERVER_H

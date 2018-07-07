#ifndef FS_SERVER_H
#define FS_SERVER_H

#include "fishield.h"
#include "file_transfer.pb.h"


class fs_server
{
public:
    fs_server();
    boost::asio::ip::tcp::socket& sock();
    bool receive_request(fs::proto::Request& request);
    bool send_response(const fs::proto::Response& response);
    bool is_stop();
    void set_stop(bool status);
    static void init();

private:
    //member variables
    char data_buffer[BUFFER_SIZE];
    boost::asio::ip::tcp::socket _sock;
    bool _is_stop;
};

typedef boost::shared_ptr<fs_server> server_ptr;

void accept_thread();
void remove_clients_thread();
void communicate_thread(server_ptr serptr);

#endif // FS_SERVER_H

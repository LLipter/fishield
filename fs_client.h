#ifndef FS_CLIENT_H
#define FS_CLIENT_H

#include "protobuf/file_transfer.pb.h"
#include <boost/asio.hpp>
#include <google/protobuf/io/zero_copy_stream_impl.h>   // google::protobuf::io::ArrayOutputStream

#define MAX_MSG 4096*2
#define SERV_PORT 7614

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
    char _buffer[MAX_MSG];
};




#endif // FS_CLIENT_H

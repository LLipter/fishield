#include "fs_client.h"

fs_client::fs_client(boost::asio::ip::tcp::endpoint ep){
    _ep = ep;
}

bool fs_client::connect(){
    boost::system::error_code err;
    _sock.connect(_ep,err);
    if(err)
        return false;
    return true;
}

void fs_client::close(){
    _sock.close();
}

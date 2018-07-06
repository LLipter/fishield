#include "fishield.h"

extern std::string _server_addr;
extern int _server_port;
extern boost::asio::io_service service;
int fs_client_startup(std::string addr, short port){
    try{
        boost::asio::ip::tcp::resolver resolver(service);
        boost::asio::ip::tcp::resolver::query qry(addr, std::to_string(port));
        boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(qry);
        boost::asio::ip::tcp::resolver::iterator end;
        if(it != end){
            std::cout << addr << " : " << (*it).endpoint().address().to_string() << std::endl;
            _server_addr = (*it).endpoint().address().to_string();
            _server_port = port;
        }else
            return FS_E_UNKNOWN;
    }catch(std::exception& e){
        return FS_E_ILLEGAL_VALUE;
    }


    return 0;
}

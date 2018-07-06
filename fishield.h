#ifndef FISHIELD_H
#define FISHIELD_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#define BUFFER_SIZE         4096
#define DEFAULT_SERV_ADDR   boost::asio::ip::address::from_string("127.0.0.1")
#define DEFAULT_SERV_PORT   7614


#include "fs_client.h"
#include "fs_server.h"
#include "fs_error.h"


int fs_client_startup(std::string addr, short port);


int fs_server_startup(short port);


#endif

#ifndef FISHIELD_H
#define FISHIELD_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#define BUFFER_SIZE         4096
#define DEFAULT_SERV_ADDR   boost::asio::ip::address::from_string("127.0.0.1")
#define DEFAULT_SERV_PORT   7614


#include "fs_client.h"
#include "fs_server.h"
#include "fs_error.h"

typedef boost::function<void()> fs_funcptr;

int fs_client_startup(const std::string addr, const short port);
void fs_login(const std::string username,const std::string password, fs_funcptr cb_success, fs_funcptr cb_failed);

void fs_server_startup(const short port);


#endif

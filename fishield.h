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
#include <boost/bind.hpp>

#define BUFFER_SIZE         4096
#define DEFAULT_SERV_ADDR   boost::asio::ip::address::from_string("127.0.0.1")
#define DEFAULT_SERV_PORT   7614


#include "fs_client.h"
#include "fs_server.h"
#include "fs_error.h"

// fs stands for "fishield" or "file security"
// fp stands for "function pointer"
// cb stands for "callback"
typedef boost::function<void()> fs_fp_void;
typedef boost::function<void(fs::proto::Response::ResponseType)> fs_fp_error;
typedef boost::function<void(fs::proto::FileList)> fs_fp_filelist;

int fs_client_startup(const std::string& addr, const short port);
void fs_login(const std::string& username, const std::string& password, fs_fp_void cb_success, fs_fp_error cb_failed);
void fs_get_file_list(const std::string& dirpath, fs_fp_filelist cb_success, fs_fp_error cb_failed);

void fs_server_startup(const short port);


#endif

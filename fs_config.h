#ifndef FS_CONFIG_H
#define FS_CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "file_transfer.pb.h"


#define PACKET_SIZE             4096
#define SEPARATOR               boost::filesystem::path::preferred_separator
#define DEFAULT_SERV_ADDR       boost::asio::ip::address::from_string("127.0.0.1")
#define DEFAULT_SERV_PORT       7614
#define DEFAULT_ROOT_DIR        std::string(".")+SEPARATOR+"fs_root"
#define DEFAULT_HIDDEN_PREFIX   ".fs_"
#define DEFAULT_TASKID_FILE     ".fs_taskid"
#define DEFAULE_MAX_TASKNO      5



// fs stands for "fishield" or "file security"
// fp stands for "function pointer"
// cb stands for "callback"
typedef boost::function<void()>                                     fs_fp_void;
typedef boost::function<void(fs::proto::Response::ResponseType)>    fs_fp_error;
typedef boost::function<void(fs::proto::FileList)>                  fs_fp_filelist;
typedef boost::function<void(int)>                                  fs_fp_int;
typedef boost::function<void(double)>                               fs_fp_double;



#endif // FS_CONFIG_H

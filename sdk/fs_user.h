#ifndef FS_USER_H
#define FS_USER_H

#include "fs_config.h"

struct fs_user{
    std::string username;
    std::string password;
    int privilege;
    long long last_packet_time;
};

#endif // FS_USER_H

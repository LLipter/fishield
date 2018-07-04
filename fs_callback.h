#ifndef FS_CALLBACK_H
#define FS_CALLBACK_H

#include "config.h"

#define FS_EVENT_SYSTEM     "event.system"
#define FS_EVENT_PROCESS    "event.process"

typedef int (*fs_func_ptr)(void* user_data, std::map<std::string,std::string> msg);

struct fs_callback{
    void* user_data;
    fs_func_ptr callback;
};


#endif // FS_CALLBACK_H

#ifndef FS_CALLBACK_H
#define FS_CALLBACK_H

#include "config.h"

#define FS_EVENT_SYSTEM     "event.system"
#define FS_EVENT_PROCESS    "event.process"

typedef enum fs_process_event {
    fs_process_upload = 1,
    fs_process_download,
    fs_process_paused,
    fs_process_canceled,
    fs_process_resume,
    fs_process_failed,
    fs_process_uploaded,
    fs_process_downloaded,
} fs_process_event;

typedef int (*fs_func_ptr)(void* user_data, std::map<std::string,std::string> msg);

struct fs_callback{
    void* user_data;
    fs_func_ptr callback;
};


#endif // FS_CALLBACK_H

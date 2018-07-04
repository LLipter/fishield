#ifndef FS_CALLBACK_H
#define FS_CALLBACK_H

#include "config.h"

#define FS_EVENT_SYSTEM     "event.system"
#define FS_EVENT_PROCESS    "event.process"

typedef enum fs_process_event {
    sec_process_upload = 1,
    sec_process_download,
    sec_process_paused,
    sec_process_canceled,
    sec_process_resume,
    sec_process_failed,
    sec_process_uploaded,
    sec_process_downloaded,
} fs_process_event;

typedef int (*fs_func_ptr)(void* user_data, std::map<std::string,std::string> msg);

struct fs_callback{
    void* user_data;
    fs_func_ptr callback;
};


#endif // FS_CALLBACK_H

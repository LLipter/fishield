#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_callback.h"
#include "fs_client.h"
#include "fs_config.h"
#include "fs_scheduler.h"
#include "fs_task.h"

int fs_register_event(const char* event, fs_func_ptr handler, void* user_data);




#endif

#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_callback.h"
#include "fs_client.h"
#include "fs_config.h"
#include "fs_scheduler.h"
#include "fs_task.h"
#include "fs_error.h"
#include "fs_server.h"

#define FS_EVENT_SYSTEM     1
#define FS_EVENT_PROCESS    2

// interface for client program
int fs_register_event(int event, fs_func_ptr handler, void* user_data);
/***
 * DESC:    Resolver will try to resolve the ip address.
 *          Both domain like "www.irran.top" and digit address like "127.0.0.1" will be ok.
 *
 * RETURN:  0 upon successful completion, FS_E_ILLADDR if addr is a illegal address like "xxx"
 *
 * NOTE:    even if 0 is returned, it just means addr is a possible legal address.
 *          It's still possible that addr is unreachable
 */
int fs_client_start_up(std::string addr, int port,std::string user_name, std::string token);
int fs_start_task(std::string path, std::map<std::string, std::string> params);

// interface for server program
int fs_server_start_up(int port);
int fs_server_set_property(const char* prop, const char* value);

#endif

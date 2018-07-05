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

#define FS_TASK_STATUS      3


#define FS_SERVER_ADD_AUTH  4
#define FS_SERVER_RM_AUTH   5

// interface for client program

/***
 * DESC:    register callback function
 *
 * RETURN:  0 upon successful completion
 */
int fs_register_event(int event, fs_func_ptr handler, void* user_data);

/***
 * DESC:    Resolver will try to resolve the ip address.
 *          Both domain like "www.irran.top" and digit address like "127.0.0.1" will be ok.
 *
 * RETURN:  0 upon successful completion, FS_E_ILLEGAL_VALUE if addr is a illegal address like "xxx"
 *
 * NOTE:    even if 0 is returned, it just means addr is a possible legal address.
 *          It's still possible that addr is unreachable
 */
int fs_client_start_up(std::string addr, int port,std::string user_name, std::string token);

/***
 * DESC:    start a new task
 *
 * RETURN:  task_id upon successful completion
 *          FS_E_NOT_SPECIFIED if FS_TASK_STATUS is not specified
 *          FS_E_ILLEGAL_VALUE if undefined FS_TASK_STATUS is used
 */
int fs_start_task(std::string path, std::map<int, std::string> params);

// interface for server program

/***
 * DESC:    startup the server
 *
 * RETURN:  never return
 */
int fs_server_start_up(int port);


/***
 * DESC:    set property in server
 *
 * RETURN:  0 upon successful completion
 *          FS_E_ILLEGAL_VALUE if FS_TASK_STATUS or value is illegal
 */
int fs_server_set_property(int prop, std::string value);

#endif

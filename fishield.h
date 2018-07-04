#ifndef FISHIELD_H
#define FISHIELD_H

#include "fs_callback.h"
#include "fs_client.h"
#include "fs_config.h"
#include "fs_scheduler.h"
#include "fs_task.h"
#include "fs_error.h"
#include "fs_server.h"

// interface for client program
int fs_register_event(const char* event, fs_func_ptr handler, void* user_data);
bool fs_start_up(std::string addr, int port,std::string user_name,std::string token);
int fs_start_task(std::string path, std::map<std::string, std::string> params);

// interface for server program
int fs_server_start_up(int port);

#endif

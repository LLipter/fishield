#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

// TODO : ADD A LOCK IN TASK!!!!! PROTECT IT IN MULTI-THREADING ENVRIONMENT

void upload(fs::proto::Task& task);
void download(fs::proto::Task& task);
void _upload(fs::proto::Task& task);
void _download(fs::proto::Task& task);

int get_taskid_by_clientid(int clientid);
void get_task_from_file(std::string filepath, std::map<int, fs::proto::Task>& task_map);
void save_task_to_file(std::string filepath, std::map<int, fs::proto::Task>& task_map);
bool is_timeout(const fs::proto::Task& task);

#endif // FS_TASK_H

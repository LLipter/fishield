#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

// TODO : ADD A LOCK IN TASK!!!!! PROTECT IT IN MULTI-THREADING ENVRIONMENT

void upload(fs::proto::Task& task);
void download(fs::proto::Task& task);
void _upload(fs::proto::Task& task);
void _download(fs::proto::Task& task);

int get_taskid_by_clientid(int clientid);

#endif // FS_TASK_H

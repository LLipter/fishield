#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

void upload(fs::proto::Task& task);
void download(fs::proto::Task& task);
void _upload(fs::proto::Task& task);
void _download(fs::proto::Task& task);



#endif // FS_TASK_H

#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

void upload(fs::proto::Task& task);
void download(fs::proto::Task& task);
void _upload(fs::proto::Task& task);
void _download(fs::proto::Task& task);

void get_task_from_file(std::string filepath, std::map<int, fs::proto::Task>& task_map);
void save_task_to_file(std::string filepath, std::map<int, fs::proto::Task>& task_map);
bool is_timeout(const fs::proto::Task& task);

void default_cb_progress(int clientid, double progress);
void default_cb_success(int clientid);
void default_cb_failed(int clientid, fs::proto::Response::ResponseType error);
void default_cb_report(std::vector<fs::proto::Task> tasks);
#endif // FS_TASK_H

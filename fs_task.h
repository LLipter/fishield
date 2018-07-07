#ifndef FS_TASK_H
#define FS_TASK_H

#include "fishield.h"

typedef enum TaskStatus{
    UPLOADING,
    UPLOADED,
    DOWNLOADING,
    DOWNLOADED,
    PAUSED,
    CANCELED
}TaskStatus;

struct fs_task
{
    int task_id;
    std::string path;
    int total_packet_no;
    int received_packet_no;
    long long last_packet_time;
    TaskStatus status;
};

#endif // FS_TASK_H

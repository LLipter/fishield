#ifndef FS_TASK_H
#define FS_TASK_H

#include "fs_config.h"

typedef enum TaskStatus{
    UPLOAD_INIT,
    UPLOADING,
    UPLOADED,
    UPLOAD_PAUSED,
    DOWNLOAD_INIT,
    DOWNLOADING,
    DOWNLOADED,
    DOWNLOAD_PAUSED,
    CANCELED
}TaskStatus;

class fs_task
{
public:
    // member variables
    int task_id;
    std::string localbasepath;
    std::string remotebasepath;
    std::string filename;
    int total_packet_no;
    int received_packet_no;
    int sent_packet_no;
    long long last_packet_time;
    TaskStatus status;
    // callback functions
    fs_fp_int cb_start_upload;
    fs_fp_double cb_progress;
    fs_fp_int cb_success;
    fs_fp_error cb_failed;


    // member functions
    void upload();

private:
    void _upload();
};

#endif // FS_TASK_H

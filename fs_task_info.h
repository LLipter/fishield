#ifndef FS_TASK_INTO_H
#define FS_TASK_INTO_H

#include <string>
#include <vector>
#include <map>
#include <openssl/md5.h>

typedef enum fs_task_status{
    START_UPLOAD = 0,
    START_DOWNLOAD,
    UPLOADED,
    DOWNLOADED,
    UPLOAD_CANCELED,
    DOWNLOAD_CANCELED,
    UPLOAD_PAUSED,
    DOWNLOAD_PAUSED,
    UPLOADING,
    DOWNLOADING,
    UPLOAD_FAILED,
    DOWNLOAD_FAILED,
}fs_task_status;


struct fs_task_info
{
    int task_id;
    std::string local_path;
    std::string remote_path;
    unsigned char md5[MD5_DIGEST_LENGTH];
    ssize_t offset;
    ssize_t size;
    fs_task_status task_status;
    std::vector<fs_task_info> children;
    std::map<std::string,std::string> config;

};

#endif // FS_TASK_INTO_H

#ifndef FS_STREAM_ITEM_H
#define FS_STREAM_ITEM_H

#include <string>
#include <vector>
#include <map>
#include <openssl/md5.h>

typedef enum fs_stream_status{
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
}fs_stream_status;


struct fs_stream_item
{
    int stream_id;
    std::string local_path;
    std::string remote_path;
    unsigned char md5[MD5_DIGEST_LENGTH];
    ssize_t offset;
    ssize_t size;
    fs_stream_status stream_status;
    std::vector<fs_stream_item> children;
    std::map<std::string,std::string> config;

};

#endif // FS_STREAM_ITEM_H

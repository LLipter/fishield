#include "fs_callback.h"

std::map<int, fs_callback> callback_map;
std::string fs_process_event_str[] = {
    "UPLOADING",
    "DOWNLOADING",
    "CANCELED",
    "PAUSED",
    "RESUEM",
    "FAILED",
    "UPLOADED",
    "DOWNLOADED"
};

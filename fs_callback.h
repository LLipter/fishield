#ifndef FS_CALLBACK_H
#define FS_CALLBACK_H

#include <map>

typedef int (*secft_func_ptr)(void* user_data, std::map<std::string,std::string> msg);

struct fs_callback{
    void* user_data;
    secft_func_ptr callback;
};


#endif // FS_CALLBACK_H

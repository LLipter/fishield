#ifndef FS_ERROR_H
#define FS_ERROR_H   

#define FS_E_TSNOTFOUND     -1      // task status not found
#define FS_E_TSILLEGAL      -2      // illegal task status
#define FS_E_ILLADDR        -3      // illegal ip address
#define FS_E_NOSUCHFILE     -4      // file not exists
#define FS_E_UNKNOWN        -5      // unknown error

void err_quit(const char* msg,...);

void err_print(const char* msg,...);


#endif // FS_ERROR_H

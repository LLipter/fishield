#ifndef FS_ERROR_H
#define FS_ERROR_H   

#define FS_E_TASKSTATUS     -1      // unknown task status
#define FS_E_ILLADDR        -2      // illegal ip address

void err_quit(const char* msg,...);


#endif // FS_ERROR_H

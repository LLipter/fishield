#ifndef FS_ERROR_H
#define FS_ERROR_H   

#define FS_E_NOT_SPECIFIED  -1      // parameter not specified
#define FS_E_ILLEGAL_VALUE  -2      // illegal values
#define FS_E_NOSUCHFILE     -3      // file not exists
#define FS_E_UNKNOWN        -4      // unknown error
#define FS_E_TODO           -5      // something hasn't been implemented
#define FS_E_CBNULL         -6      // callback function ptr is not registered

void err_quit(const char* msg,...);
void err_print(const char* msg,...);


#endif // FS_ERROR_H

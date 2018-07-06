#ifndef FS_ERROR_H
#define FS_ERROR_H   

#include "fishield.h"

#define FS_E_UNKNOWN        -1     // unknown error
#define FS_E_TODO           -2      // something hasn't been implemented

void err_print(const char* msg,...);


#endif // FS_ERROR_H

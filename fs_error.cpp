#include "fs_error.h"
#include "fs_config.h"
#include <cstdarg>
#include <cstdlib>

void err_quit(const char *msg,...){
    char msgstr[BUFFER_SIZE];
    std::va_list arg;
    va_start(arg,msg);
    std::vsnprintf(msgstr,BUFFER_SIZE,msg,arg);
    va_end(arg);
    perror(msgstr);
    std::exit(1);
}

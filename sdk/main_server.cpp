#include "fishield.h"
#include <sys/resource.h>

using namespace std;

void daemonize(){
    int             fd0, fd1, fd2;
    pid_t           pid;
    struct rlimit   rl;

    umask(0);

    if((pid=fork()) != 0)
        exit(0);
    setsid();

    if((pid=fork()) != 0)
        exit(0);

    if(chdir("/") < 0)
        exit(0);

    if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
        exit(0);
    if(rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for(int i=0;i<rl.rlim_max;i++)
        close(i);

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
}

int main(){

#ifndef DEBUG
    daemonize();
#endif

    fs_server_startup(7614);

    cout << "never print this" << endl;
}

#include "fishield.h"
#include <cstdlib>

using namespace std;

int ret;
void quit_if_error(){
    if(ret != 0)
        exit(1);
}

void splitline(){
    quit_if_error();
    cout << "------------------------------------" << endl;
}


int main()
{
    ret = fs_client_startup("localhost", 7614);
    if(ret == 0)
        cout << "client startup ok" << endl;
    else if (ret == FS_E_ILLEGAL_VALUE)
        cout << "illegal address" << endl;
    else
        cout << "unknown error in fs_client_startup()" << endl;
    splitline();

    fs_client client;
    if(client.connect())
        cout << "connection built" << endl;
    else
        cout << "connection failed" << endl;

    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

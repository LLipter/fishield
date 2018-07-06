#include "fishield.h"
#include <cstdlib>

using namespace std;
using namespace boost;

int ret;
void quit_if_error(){
    if(ret != 0)
        exit(1);
}

void splitline(){
    quit_if_error();
    cout << "------------------------------------" << endl;
}

void cb_login_success(){
    cout << "callback : login successfully" << endl;
}

void cb_login_fail(){
    cout << "callback : login failed" << endl;
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

    const std::string username = "LLipter";
    const std::string passwd = "123456";
    fs_login(username,passwd,boost::bind(cb_login_success),boost::bind(cb_login_fail));

    splitline();


    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

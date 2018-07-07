#include "fishield.h"
#include <cstdlib>

using namespace std;
using namespace boost;
using namespace fs::proto;

extern std::string _token;
int ret;
const std::string username = "LLipter";
const std::string password = "123456";

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
    std::cout << username << "-"
              << password << " login success : token="
              << _token
              << std::endl;
    splitline();
}

void cb_login_fail(fs::proto::Response::ResponseType error){
    cout << "callback "
         << username << "-"
         << password << " login failed : ";
    switch (error) {
    case Response::ILLEGALPASSWD:
        cout << "illegal password";
        break;
    case Response::NOSUCHUSER:
        cout << "no such user";
        break;
    case Response::NORESPONSE:
        cout << "cannot get response from server";
        break;
    default:
        cout << "unknow error";
        break;
    }
    cout << endl;
    splitline();
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

    fs_login(username,password,boost::bind(cb_login_success),boost::bind(cb_login_fail,_1));




    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

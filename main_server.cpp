#include "fishield.h"

using namespace std;

void splitline(){
    cout << "------------------------------------" << endl;
}


int main(){



    fs_server_startup(7614);
    cout << "server startup ok" << endl;
    splitline();


    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


}

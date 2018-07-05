#include "fishield.h"

using namespace std;

void splitline(){
    cout << "------------------------------------" << endl;
}


int main(){


    if(fs_server_set_property(FS_SERVER_ADD_AUTH, "LLipter:token") != 0){
        cout << "set property failed" << std::endl;
        return 1;
    }
    cout << "set property successful!" << endl;
    splitline();

    fs_server_start_up(7614);

    std::cout << "never print this" << std::endl;

}

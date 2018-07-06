#include "fishield.h"

using namespace std;

void splitline(){
    cout << "------------------------------------" << endl;
}


int main(){

    fs_server_startup(7614);

    cout << "never print this" << endl;
}

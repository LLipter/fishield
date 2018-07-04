#include "fishield.h"

int main(){


    fs_server_set_property(FS_SERVER_ADD_AUTH, "token:token");
    fs_server_start_up(7614);

    std::cout << "never print this" << std::endl;

}

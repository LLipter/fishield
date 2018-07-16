#include "fishield.h"
#include <unistd.h>
#include <sstream>

using namespace std;
using namespace fs::proto;


bool getYN(string promp){
    while(true){
        cout << promp << endl;
        string result;
        cin >> result;
        if(result == "Y" || result == "y")
            return true;
        if(result == "N" || result == "n")
            return false;
    }
}

void usage(){
    cout << "usage: " << endl
         << "       listuser                : list all user information" << endl
         << "       adduser                 : add a new user" << endl
         << "       removeuser <username>   : remove a user" << endl
         << "       listip                  : list all allowed ip addresses" << endl
         << "       addip      <address>    : add a new ip address" << endl
         << "       removeip   <address>    : add a new ip address" << endl;
}

void printUserList(const UserList& userlist){
    for(int i=0;i<userlist.user_size();i++){
        const User& user = userlist.user(i);
        cout << user.username() << " "
             << user.password() << " "
             << "( "
             << (((user.privilege() >> ROOT_BIT) % 2) ? "root " : "")
             << (((user.privilege() >> UPLOAD_BIT) % 2) ? "upload " : "")
             << (((user.privilege() >> DOWNLOAD_BIT) % 2) ? "download " : "")
             << ")" << endl;
    }
}

int main(){
    char* root_pass = getpass("Please enter root password:");
    fs_client_startup("www.irran.top", 7614);
    Response response = fs_root_login(root_pass);
    if(response.resp_type() != Response::SUCCESS){
        cout << Response::ResponseType_Name(response.resp_type()) << endl;
        return 1;
    }
    cout << "Welcome, administrator!" << endl;


    cout << "%";
    cout.flush();
    string line;
    while(getline(cin, line)){
        stringstream ss(line);
        vector<string> args;
        string arg;
        while(ss >> arg)
            args.push_back(arg);
        string& cmd = args[0];
        if(cmd == "listuser"){
            response = fs_userlist();
            if(response.resp_type() != Response::SUCCESS)
                cout << Response::ResponseType_Name(response.resp_type()) << endl;
            else
                printUserList(response.userlist());
        }else if(cmd == "adduser"){
            cout << "Please enter username : ";
            string username;
            cin >> username;
            char* password = getpass("Please enter password:");
            bool can_upload = getYN("Can this user upload file?(y/n)");
            bool can_download = getYN("Can this user download file?(y/n)");
            User* user = new User;
            user->set_username(username);
            user->set_password(password);
            int privilege = 0;
            if(can_upload)
                privilege += (1 << UPLOAD_BIT);
            if(can_download)
                privilege += (1 << DOWNLOAD_BIT);
            user->set_privilege(privilege);
            // TODO : fs_adduser(user);
        }else if(cmd == "removeuser"){
            string username;
            cin >> username;
            // TODO : fs_remove_user(username);
        }else if(cmd == "listip"){

        }else if(cmd == "addip"){

        }else if(cmd == "removeip"){

        }else
            usage();
        cout << "%";
        cout.flush();
    }


}

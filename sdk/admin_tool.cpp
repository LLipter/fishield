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
         << "       removeip   <address>    : remove a ip address" << endl;
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

void printIPList(const IPList& iplist){
    for(int i=0;i<iplist.ip_size();i++){
        const IP& ip = iplist.ip(i);
        cout << ip.address() << endl;
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
        if(args.size() == 0)
            continue;
        string& cmd = args[0];
        if(cmd == "listuser"){
            response = fs_userlist();
            if(response.resp_type() != Response::SUCCESS)
                cout << Response::ResponseType_Name(response.resp_type()) << endl;
            else
                printUserList(response.userlist());
        }else if(cmd == "adduser"){
            cout << "Please enter username:";
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
            response = fs_adduser(user);
            if(response.resp_type() != Response::SUCCESS)
                cout << Response::ResponseType_Name(response.resp_type()) << endl;
            else
                cout << "OK" << endl;
        }else if(cmd == "removeuser"){
            if(args.size() == 1)
                usage();
            else{
                string& username = args[1];
                response = fs_remove_user(username);
                if(response.resp_type() != Response::SUCCESS)
                    cout << Response::ResponseType_Name(response.resp_type()) << endl;
                else
                    cout << "OK" << endl;
            }
        }else if(cmd == "listip"){
            response = fs_iplist();
            if(response.resp_type() != Response::SUCCESS)
                cout << Response::ResponseType_Name(response.resp_type()) << endl;
            else
                printIPList(response.iplist());
        }else if(cmd == "addip"){
            if(args.size() == 1)
                usage();
            else{
                string& addr = args[1];
                boost::system::error_code err;
                boost::asio::ip::address::from_string(addr, err);
                if(err)
                    cout << err.message( ) << endl;
                else{
                    response = fs_add_ipaddr(addr);
                    if(response.resp_type() != Response::SUCCESS)
                        cout << Response::ResponseType_Name(response.resp_type()) << endl;
                    else
                        cout << "OK" << endl;
                }
            }
        }else if(cmd == "removeip"){
            if(args.size() == 1)
                usage();
            else{
                string& addr = args[1];
                response = fs_remove_ipaddr(addr);
                if(response.resp_type() != Response::SUCCESS)
                    cout << Response::ResponseType_Name(response.resp_type()) << endl;
                else
                    cout << "OK" << endl;
            }
        }else
            usage();
        cout << "%";
        cout.flush();
    }

    cout << endl;
}

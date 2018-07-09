#include "fishield.h"
#include <cstdlib>

using namespace std;
using namespace boost;
using namespace fs::proto;

extern std::string _token;
const std::string username = "LLipter";
const std::string password = "123456";

void splitline(){
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

void cb_filelist_success(fs::proto::FileList filelist){
    cout << "callback : filelist successfully" << endl;
    for(int i=0;i<filelist.file_size();i++){
        File file = filelist.file(i);
        cout << "filename-" << file.filename() << " "
             << "filetype-" << File::FileType_Name(file.file_type()) << " "
             << "mtime-" << file.mtime() << " "
             << "size-" << file.size() << endl;
    }
    splitline();
}


void cb_success(string msg){
    cout << "callback : " << msg << " successfully" << endl;
    splitline();
}


void cb_fail(fs::proto::Response::ResponseType error, string type){
    cout << "callback : " << type << " failed --- ";
    cout << Response::ResponseType_Name(error) << endl;
    splitline();
}


int main()
{
    int ret = fs_client_startup("localhost", 7614);
    if(ret == 0)
        cout << "client startup ok" << endl;
    else if (ret == FS_E_ILLEGAL_VALUE)
        cout << "illegal address" << endl;
    else
        cout << "unknown error in fs_client_startup()" << endl;
    if(ret != 0)
        return 1;
    splitline();

//    fs_login(username,
//             password,
//             boost::bind(cb_login_success),
//             boost::bind(cb_fail,_1,"login"));

//    fs_filelist("/",
//                boost::bind(cb_filelist_success, _1),
//                boost::bind(cb_fail,_1,"filelist"));

//    fs_mkdir("/newdir/LLipter",
//             boost::bind(cb_success, "mkdir"),
//             boost::bind(cb_fail,_1,"mkdir"));

//    // should failed
//    fs_mkdir("/LLipter/LLipter",
//             boost::bind(cb_success, "mkdir"),
//             boost::bind(cb_fail,_1,"mkdir"));

//    fs_remove("/newdir/LLipter",
//              boost::bind(cb_success, "remove"),
//              boost::bind(cb_fail,_1, "remove"));

//    fs_rename("/1.jpg",
//              "/2.jpg",
//              boost::bind(cb_success, "rename"),
//              boost::bind(cb_fail,_1, "rename"));

//    if(boost::filesystem::exists("/home/irran/Desktop/fs_root/music.mp3"))
//        boost::filesystem::remove("/home/irran/Desktop/fs_root/music.mp3");

//    fs_upload(1,
//              "/home/irran/Desktop",
//              "",                   // upload to root directory
//              "music.mp3");

//    fs_download(2
//                ,"/home/irran/Desktop",
//                "",                   // download from root directory
//                "2.jpg");




    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

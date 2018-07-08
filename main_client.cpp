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


void print_err(fs::proto::Response::ResponseType error){
    using namespace fs::proto;
    cout << Response::ResponseType_Name(error) << endl;
}

void cb_login_success(){
    cout << "callback : login successfully" << endl;
    std::cout << username << "-"
              << password << " login success : token="
              << _token
              << std::endl;
    splitline();
}

void cb_mkdir_success(){
    cout << "callback : mkdir successfully" << endl;
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

void cb_upload_progress(int taskid, double progress){
    cout << "callback : upload progress "
         << "(taskid=" << taskid << ")"
         << progress << endl;
    splitline();
}

void cb_upload_success(int taskid){
    cout << "callback : upload successfully (taskid="
         << taskid << ")" << endl;
    splitline();
}




void cb_fail(fs::proto::Response::ResponseType error, string type){
    cout << "callback : " << type << " failed --- ";
    print_err(error);
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

    fs_login(username,
             password,
             boost::bind(cb_login_success),
             boost::bind(cb_fail,_1,"login"));

    fs_mkdir("/","newdir",
             boost::bind(cb_mkdir_success),
             boost::bind(cb_fail,_1,"mkdir"));

    fs_get_filelist("/",
                    boost::bind(cb_filelist_success, _1),
                    boost::bind(cb_fail,_1,"filelist"));


//    if(boost::filesystem::exists("/home/irran/Desktop/fs_root/music.mp3"))
//        boost::filesystem::remove("/home/irran/Desktop/fs_root/music.mp3");

    fs_upload("/home/irran/Desktop",
              "/",
              "music.mp3",
              boost::bind(cb_upload_progress, _1, _2),
              boost::bind(cb_upload_success, _1),
              boost::bind(cb_fail,_1,"upload"));


    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

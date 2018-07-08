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
    case Response::ILLEGALREQUEST:
        cout << "illegal request";
        break;
    case Response::ILLEGALTOKEN:
        cout << "illegal token";
        break;
    case Response::ILLEGALPATH:
        cout << "illegal path";
        break;
    default:
        cout << "unknow error";
        break;
    }
    cout << endl;
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
    print_err(error);
    splitline();
}

void cb_mkdir_success(){
    cout << "callback : mkdir successfully" << endl;
    splitline();
}


void cb_mkdir_fail(fs::proto::Response::ResponseType error){
    cout << "callback : mkdir failed --- " << endl;
    print_err(error);
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

void cb_filelist_fail(fs::proto::Response::ResponseType error){
    cout << "callback : filelist failed --- " << endl;
    print_err(error);
    splitline();
}

void cb_upload_start(int taskid){
    cout << "callback : start uploading, task_id is "
         << taskid << endl;
    splitline();
}

void cb_upload_progress(double progress){
    cout << "callback : upload progress " << progress << endl;
    splitline();
}

void cb_upload_success(int taskid){
    cout << "callback : upload successfully (taskid="
         << taskid << ")" << endl;
    splitline();
}

void cb_upload_fail(fs::proto::Response::ResponseType error){
    cout << "callback : upload failed --- " << endl;
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
             boost::bind(cb_login_fail,_1));

    fs_mkdir("/","newdir",
             boost::bind(cb_mkdir_success),
             boost::bind(cb_mkdir_fail,_1));

    fs_get_filelist("/",
                    boost::bind(cb_filelist_success, _1),
                    boost::bind(cb_filelist_fail, _1));

    fs_upload("/home/irran/Desktop/project/fishield",
              "/",
              "file_transfer.proto",
              boost::bind(cb_upload_start,_1),
              boost::bind(cb_upload_progress, _1),
              boost::bind(cb_upload_success, _1),
              boost::bind(cb_upload_fail,_1));


    while(true){
        boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
    }


    return 0;
}

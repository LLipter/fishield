#include "login_backend.h"
#include <boost/bind.hpp>

login_backend::login_backend(QObject *parent) : QObject(parent){
}


void login_backend::login(std::string username, std::string password){
    is_timeout = false;
    if(username == "LLipter" && password == "123456")
        emit logined();

//    fs_client_startup("127.0.0.1", 7614);
//    fs_login(username,
//             password,
//             boost::bind(&login_backend::handle_login_success, this),
//             boost::bind(&login_backend::handle_login_failed, this, _1));


}

void login_backend::timeout(){
    is_timeout = true;
}

//void login_backend::handle_login_success(){
//    if(is_timeout)
//        return;
//    emit logined();
//}

//void login_backend::handle_login_failed(fs::proto::Response::ResponseType error){
//    if(is_timeout)
//        return;
//    // TODO : parse json object here

//    emit logined();
//}

#include "login_backend.h"
#include <boost/bind.hpp>

login_backend::login_backend(QObject *parent) : QObject(parent){
}


void login_backend::login(QString username, QString password){
    is_timeout = false;
//    if(username == "LLipter" && password == "123456")
//        emit logined();

    // TODO : send request
//    requestManager.send();

}

void login_backend::timeout(){
    is_timeout = true;
}

void login_backend::handle_login_response()
{
    if(is_timeout)
        return;
    // TODO : parse json object here

    emit logined();
}

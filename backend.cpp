#include "backend.h"
#include <boost/bind.hpp>

backend::backend(QObject *parent) : QObject(parent){
}


void backend::login(QString username, QString password){
    is_timeout = false;
    fs_client_startup("localhost", 7614);
    fs_login(username.toStdString(),
             password.toStdString(),
             boost::bind(&backend::handle_login_success, this),
             boost::bind(&backend::handle_login_failed, this, _1));

}

void backend::timeout(){
    is_timeout = true;
}

void backend::handle_login_success()
{
    if(is_timeout)
        return;
    emit logined();
}
void backend::handle_login_failed(fs::proto::Response::ResponseType error){
    // TODO : check error
}

#include "login_backend.h"

login_backend::login_backend(QObject *parent) : QObject(parent){}


void login_backend::login(QString username, QString password){
    is_timeout = false;
    if(username == "LLipter" && password == "123456")
        emit loginStateChanged(true);
    else
        emit loginStateChanged(false);

}

void login_backend::timeout(){
    is_timeout = true;
}

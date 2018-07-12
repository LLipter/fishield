#include "login_backend.h"

login_backend::login_backend(QObject *parent) : QObject(parent){
    is_timeout = false;
}


void login_backend::login(QString username, QString password){
    if(username == "LLipter" && password == "123456")
        emit loginStateChanged(true);
    else
        emit loginStateChanged(false);

}

#ifndef LOGIN_BACKEND_H
#define LOGIN_BACKEND_H

#include <QObject>
#include "request_manager.h"

class login_backend : public QObject
{
    Q_OBJECT
public:
    explicit login_backend(QObject *parent = nullptr);
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void timeout();

private:
    static QString _token;
    bool is_timeout;
    request_manager requestManager;

signals:
    void logined();

public slots:
    void handle_login_response(QJsonObject res_json);
};

#endif // LOGIN_BACKEND_H

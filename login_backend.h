#ifndef LOGIN_BACKEND_H
#define LOGIN_BACKEND_H

#include <QObject>

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

signals:
    void logined();

public slots:
    void handle_login_response();
};

#endif // LOGIN_BACKEND_H

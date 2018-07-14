#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "fishield.h"

class backend : public QObject
{
    Q_OBJECT
public:
    explicit backend(QObject *parent = nullptr);
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void timeout();

private:
    static QString _token;
    bool is_timeout;

signals:
    void logined();

public slots:
    void handle_login_success();
    void handle_login_failed(fs::proto::Response::ResponseType error);
};

#endif // BACKEND_H

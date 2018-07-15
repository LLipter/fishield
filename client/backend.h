#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include "fishield.h"
 #include <QVariant>

class backend : public QObject
{
    Q_OBJECT
public:
    explicit backend(QObject *parent = nullptr);
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void timeout();
    Q_INVOKABLE void getFileList(QString path);

private:
    static QString _token;
    bool is_timeout;

signals:
    void logined();
    void no_such_user();
    void illegal_password();
    void fileLoaded(QVariantList _file_names,
                    QVariantList _is_dir,
                    QVariantList _file_sizes,
                    QVariantList _m_times);

public slots:
    void handle_login_success();
    void handle_login_failed(fs::proto::Response::ResponseType error);
    void handle_filelist_success(fs::proto::FileList filelist);
    void handle_filelist_failed(fs::proto::Response::ResponseType error);

};

#endif // BACKEND_H

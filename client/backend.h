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
    Q_INVOKABLE void newdir(QString path);
    Q_INVOKABLE void upload(QString localpath, QString remotepath);
    Q_INVOKABLE void download(QString localbasepath,
                              QString remotebasepath,
                              QString filename);
    Q_INVOKABLE void file_history();

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
    void newdir_created();
    void process_report(QVariantList _file_names,
                        QVariantList _file_states,
                        QVariantList _file_processes);
    void history_loaded(QVariantList _file_names,
                        QVariantList _file_states);

public slots:
    void handle_login_success();
    void handle_login_failed(fs::proto::Response::ResponseType error);
    void handle_filelist_success(fs::proto::FileList filelist);
    void handle_filelist_failed(fs::proto::Response::ResponseType error);
    void handle_newdir_success();
    void handle_newdir_failed(fs::proto::Response::ResponseType error);
    void handle_transfer_success(int taskid);
    void handle_transfer_failed(int taskid, fs::proto::Response::ResponseType error);
    void handle_transfer_report(std::vector<fs::proto::Task> tasks);
    void handle_file_history(std::vector<fs::proto::Task> tasks);
};

#endif // BACKEND_H

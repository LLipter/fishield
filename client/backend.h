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
    Q_INVOKABLE void pause_task(QString taskid);
    Q_INVOKABLE void resume_task(QString taskid);
    Q_INVOKABLE void cancel_task(QString taskid);
    Q_INVOKABLE void remove_file(QString path);
    Q_INVOKABLE void disk_space();

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
                        QVariantList _file_processes,
                        QVariantList _task_ids);
    void history_loaded(QVariantList _file_names,
                        QVariantList _file_states);
    void file_removed();
    void relogin();
    void disk_space_loaded(QString available, QString total);

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
    void handle_pause_success(int taskid);
    void handle_pause_failed(int taskid, fs::proto::Response::ResponseType error);
    void handle_resume_success(int taskid);
    void handle_resume_failed(int taskid, fs::proto::Response::ResponseType error);
    void handle_cancel_success(int taskid);
    void handle_cancel_failed(int taskid, fs::proto::Response::ResponseType error);
    void cleaning_up();
    void handle_remove_success();
    void handle_remove_failed(fs::proto::Response::ResponseType error);
    void handle_diskspace_success(long long available, long long total);
    void handle_diskspace_failed(fs::proto::Response::ResponseType error);

};

#endif // BACKEND_H

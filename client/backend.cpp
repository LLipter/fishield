#include "backend.h"
#include <boost/bind.hpp>
#include <QVector>
#include <QVariant>

backend::backend(QObject *parent) : QObject(parent){
}


void backend::login(QString username, QString password){
    is_timeout = false;
    fs_client_startup("www.irran.top", 7614);
    fs_login(username.toStdString(),
             password.toStdString(),
             boost::bind(&backend::handle_login_success, this),
             boost::bind(&backend::handle_login_failed, this, _1));
    fs_register_task_callback(boost::bind(&backend::handle_transfer_report, this, _1),
                              boost::bind(&backend::handle_transfer_success, this, _1),
                              boost::bind(&backend::handle_transfer_failed, this, _1, _2));
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
    if(error == fs::proto::Response::NOSUCHUSER)
        emit no_such_user();
    else if(error == fs::proto::Response::ILLEGALPASSWD)
        emit illegal_password();
}

void backend::getFileList(QString path){
    is_timeout = false;
    fs_filelist(path.toStdString(),
                boost::bind(&backend::handle_filelist_success, this, _1),
                boost::bind(&backend::handle_filelist_failed, this, _1));
}

void backend::handle_filelist_success(fs::proto::FileList filelist){
    QVariantList names;
    QVariantList is_dir;
    QVariantList filesizes;
    QVariantList mtimes;

    for(int i=0;i<filelist.file_size();i++){
        const fs::proto::File& file = filelist.file(i);
        names << QString::fromStdString(file.filename());
        if(file.file_type() == fs::proto::File::DIRECTORY){
            is_dir << true;
            filesizes << " ";
        }else{
            is_dir << false;
            int fsize = file.size();
            if(fsize < 1024)
                filesizes << (QString::fromStdString(std::to_string(fsize)) + "B");
            else if(fsize < 1024*1024)
                filesizes << (QString::fromStdString(std::to_string(fsize/1024)) + "K");
            else if(fsize < 1024*1024*1024)
                filesizes << (QString::fromStdString(std::to_string(fsize/1024/1024)) + "M");
            else
                filesizes << (QString::fromStdString(std::to_string(fsize/1024/1024/1024)) + "G");
        }
        int modify_diff = std::time(0) - file.mtime();
        if(modify_diff < 60)
            mtimes << (QString::fromStdString(std::to_string(modify_diff)) + " seconds ago");
        else if(modify_diff < 60*60)
            mtimes << (QString::fromStdString(std::to_string(modify_diff/60)) + " mintues ago");
        else if(modify_diff < 24*60*60)
            mtimes << (QString::fromStdString(std::to_string(modify_diff/60/60)) + " hours ago");
        else if(modify_diff < 30*24*60*60)
            mtimes << (QString::fromStdString(std::to_string(modify_diff/60/60/24)) + " days ago");
        else if(modify_diff < 12*30*24*60*60)
            mtimes << (QString::fromStdString(std::to_string(modify_diff/60/60/24/30)) + " months ago");
        else
            mtimes << (QString::fromStdString(std::to_string(modify_diff/60/60/24/30/12)) + " years ago");

    }

    emit fileLoaded(names, is_dir, filesizes, mtimes);

}

void backend::handle_filelist_failed(fs::proto::Response::ResponseType error){
    // TODO : CHECK ERROR TYPE (ILLEGAL TOKEN)
}

void backend::upload(QString localpath, QString remotepath){
    is_timeout = false;

    std::string localbasepath = localpath.toStdString();
    int idx = localbasepath.find_last_of(SEPARATOR);
    std::string filename = localbasepath.substr(idx+1, localbasepath.length()-idx-1);
    localbasepath = localbasepath.substr(0, idx);
    localbasepath = localbasepath.substr(7, localbasepath.length()-7);
    std::string remotebasepath = remotepath.toStdString();
    remotebasepath = remotebasepath.substr(0, remotebasepath.length()-1);

    std::cout << localbasepath << std::endl
              << filename << std::endl
              << remotebasepath << std::endl;

    bool ret = fs_upload(localbasepath,
                         remotebasepath,
                         filename);
    if(ret == false){
        // TODO : SEND A ERROR MESSAGE
        std::cout << "fuck ..." << std::endl;
    }

}

void backend::handle_transfer_success(int taskid){
    // TODO : SEND MESSAGE TO SHOW A MESSAGEBOX
}

void backend::handle_transfer_failed(int taskid, fs::proto::Response::ResponseType error){
    // TODO : CHECK ERROR TYPE (ILLEGAL TOKEN)
}

void backend::handle_transfer_report(std::vector<fs::proto::Task> tasks){
    using namespace fs::proto;

    QVariantList names;
    QVariantList states;
    QVariantList processes;

    for(Task& task : tasks){
        names << QString::fromStdString(task.filename());
        states << QString::fromStdString(Task::TaskStatus_Name(task.task_status()));
        double proces;
        switch (task.task_status()) {
        case Task::UPLOADING:
        case Task::UPLOAD_PAUSED:
        case Task::UPLOAD_PAUSING:
        case Task::UPLOAD_RESUME:
            proces = (double)task.sent_packet_no() / task.total_packet_no();
            break;
        case Task::DOWNLOADING:
        case Task::DOWNLOAD_PAUSED:
        case Task::DOWNLOAD_PAUSING:
        case Task::DOWNLOAD_RESUME:
            proces = (double)task.received_packet_no() / task.total_packet_no();
            break;
        default:
            break;
        }
        proces = std::round(proces*1000) / 10;
        std::string proces_str = std::to_string(proces).substr(0,4) + "%";
        processes << QString::fromStdString(proces_str);
    }

    emit process_report(names,
                        states,
                        processes);
}


void backend::newdir(QString path){
    is_timeout = false;
    fs_mkdir(path.toStdString(),
             boost::bind(&backend::handle_newdir_success, this),
             boost::bind(&backend::handle_newdir_failed, this, _1));
}

void backend::handle_newdir_success(){
    if(is_timeout)
        return;
    emit newdir_created();
}

void backend::handle_newdir_failed(fs::proto::Response::ResponseType error){
    // TODO : CHECK ERROR TYPE (ILLEGAL TOKEN)
}

void backend::download(QString localbasepath,
                       QString remotebasepath,
                       QString filename){
    is_timeout = false;
    std::string localpath = localbasepath.toStdString();
    localpath = localpath.substr(7, localpath.length()-7);
    std::string remotepath = remotebasepath.toStdString();
    remotepath = remotepath.substr(0, remotepath.length()-1);

    fs_download(localpath,
                remotepath,
                filename.toStdString());
}


void backend::file_history(){
    fs_history_tasks(boost::bind(&backend::handle_file_history, this, _1));

}

void backend::handle_file_history(std::vector<fs::proto::Task> tasks){
    using namespace fs::proto;
    QVariantList names;
    QVariantList states;

    for(Task& task : tasks){
        names << QString::fromStdString(task.filename());
        states << QString::fromStdString(Task::TaskStatus_Name(task.task_status()));
    }

    emit history_loaded(names, states);
}

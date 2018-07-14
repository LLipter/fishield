#include "fs_dbmanager.h"



sql::Driver* fs_DBManager::driver = get_driver_instance();

fs_DBManager::fs_DBManager(){
    conn = driver->connect(DB_HOST, DB_USERNAME, DB_PASSWORD);
    conn->setSchema(DB_DBNAME);
}

fs_DBManager::~fs_DBManager(){
    if(conn)
        delete conn;
}

int fs_DBManager::login(fs_user* user){
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM user WHERE username=?");
    pstmt->setString(1,user->username);
    sql::ResultSet* res = pstmt->executeQuery();

    // no such user
    if(!res->next()){
        delete pstmt;
        delete res;
        return FS_E_NOSUCHUSER;
    }

    // illegal password
    if((std::string)res->getString("username") != user->username){
        delete pstmt;
        delete res;
        return FS_E_ILLEGALPASSWD;
    }

    delete pstmt;
    delete res;
    return 0;

}

void fs_DBManager::insert_task(const fs::proto::Task& task){
    sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO task VALUES(?,?,?,?,?,?,?,?,?)");
    pstmt->setInt(1,task.task_id());
    pstmt->setString(2,task.localbasepath());
    pstmt->setString(3,task.remotebasepath());
    pstmt->setString(4,task.filename());
    pstmt->setInt(5,task.total_packet_no());
    pstmt->setInt(6,(int)task.task_status());
    pstmt->setInt(7,task.received_packet_no());
    pstmt->setInt(8,task.sent_packet_no());
    pstmt->setInt64(9,task.last_packet_time());
    pstmt->executeUpdate();
    delete pstmt;
}

void fs_DBManager::update_task(const fs::proto::Task& task){
    sql::PreparedStatement *pstmt = conn->prepareStatement("UPDATE task SET "
                                                           "localbasepath=?,"
                                                           "remotebasepath=?,"
                                                           "filename=?,"
                                                           "total_packet_no=?,"
                                                           "task_status=?,"
                                                           "received_packet_no=?,"
                                                           "sent_packet_no=?,"
                                                           "last_packet_time=? "
                                                           "WHERE task_id=?");

    pstmt->setString(1,task.localbasepath());
    pstmt->setString(2,task.remotebasepath());
    pstmt->setString(3,task.filename());
    pstmt->setInt(4,task.total_packet_no());
    pstmt->setInt(5,(int)task.task_status());
    pstmt->setInt(6,task.received_packet_no());
    pstmt->setInt(7,task.sent_packet_no());
    pstmt->setInt64(8,task.last_packet_time());
    pstmt->setInt(9,task.task_id());
    pstmt->executeUpdate();
    delete pstmt;
}

int fs_DBManager::get_task(int task_id, fs::proto::Task& task){
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM task WHERE task_id=?");
    pstmt->setInt(1, task_id);
    sql::ResultSet* res = pstmt->executeQuery();

    // no such task
    if(!res->next()){
        delete pstmt;
        delete res;
        return FS_E_NOSUCHTASK;
    }

    task.set_task_id(res->getInt("task_id"));
    task.set_localbasepath(res->getString("localbasepath"));
    task.set_remotebasepath(res->getString("remotebasepath"));
    task.set_filename(res->getString("filename"));
    task.set_total_packet_no(res->getInt("total_packet_no"));
    task.set_task_status((fs::proto::Task::TaskStatus)res->getInt("task_status"));
    task.set_received_packet_no(res->getInt("received_packet_no"));
    task.set_sent_packet_no(res->getInt("sent_packet_no"));
    task.set_last_packet_time(res->getInt64("last_packet_time"));

    delete pstmt;
    delete res;
    return 0;
}

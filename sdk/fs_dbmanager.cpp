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

int fs_DBManager::login(fs::proto::User& user){
    fs::proto::User real_user;
    real_user.CopyFrom(user);
    if(this->getUser(real_user) == FS_E_NOSUCHUSER)
        return FS_E_NOSUCHUSER;

    // illegal password
    if(user.password() != real_user.password())
        return FS_E_ILLEGALPASSWD;

    user.set_privilege(real_user.privilege());
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


void fs_DBManager::getUserList(fs::proto::UserList* userlist){
    using namespace fs::proto;
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM user");
    sql::ResultSet* res = pstmt->executeQuery();

    while(res->next()){
        User* user = userlist->add_user();
        user->set_username(res->getString("username"));
        user->set_password(res->getString("password"));
        user->set_privilege(res->getInt("privilege"));
    }

    delete pstmt;
    delete res;
}

int fs_DBManager::getUser(fs::proto::User& user){
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM user WHERE username=?");
    pstmt->setString(1, user.username());
    sql::ResultSet* res = pstmt->executeQuery();

    // no such user
    if(!res->next()){
        delete pstmt;
        delete res;
        return FS_E_NOSUCHUSER;
    }

    user.set_password(res->getString("password"));
    user.set_privilege(res->getInt("privilege"));

    delete pstmt;
    delete res;
    return 0;
}


int fs_DBManager::addUser(const fs::proto::User& user){
    using namespace fs::proto;

    fs::proto::User real_user;
    real_user.CopyFrom(user);
    if(this->getUser(real_user) != FS_E_NOSUCHUSER)
        return FS_E_DUPLICATE_USER;

    sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO user(username, password, privilege) VALUES(?,?,?)");
    pstmt->setString(1, user.username());
    pstmt->setString(2, user.password());
    pstmt->setInt(3, user.privilege());

    pstmt->executeUpdate();

    delete pstmt;
    return 0;
}

void fs_DBManager::removeUser(const std::string& username){
    sql::PreparedStatement *pstmt = conn->prepareStatement("DELETE FROM user WHERE username=?");
    pstmt->setString(1, username);
    pstmt->executeUpdate();

    delete pstmt;
}

void fs_DBManager::getIPList(fs::proto::IPList* iplist){
    using namespace fs::proto;
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM ip");
    sql::ResultSet* res = pstmt->executeQuery();

    while(res->next()){
        IP* ipaddress = iplist->add_ip();
        ipaddress->set_address(res->getString("address"));
    }

    delete pstmt;
    delete res;
}

int fs_DBManager::addIPAddr(const std::string& addr){
    if(this->getIPAddr(addr))
        return FS_E_DUPLICATE_IPADDR;

    sql::PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO ip VALUES(?)");
    pstmt->setString(1, addr);
    pstmt->executeUpdate();
    delete pstmt;
    return 0;

}

void fs_DBManager::removeIPAddr(const std::string& ipaddr){
    sql::PreparedStatement *pstmt = conn->prepareStatement("DELETE FROM ip WHERE address=?");
    pstmt->setString(1, ipaddr);
    pstmt->executeUpdate();

    delete pstmt;
}

bool fs_DBManager::getIPAddr(const std::string& ipaddr){
    sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM ip WHERE address=?");
    pstmt->setString(1, ipaddr);
    sql::ResultSet* res = pstmt->executeQuery();
    if(res->next()){
        delete pstmt;
        delete res;
        return true;
    }

    delete pstmt;
    delete res;
    return false;
}

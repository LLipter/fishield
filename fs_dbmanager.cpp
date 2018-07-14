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
    sql::PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM user WHERE username=? AND password=?");
    pstmt->setString(1,user->username);
    pstmt->setString(2,user->password);
    sql::ResultSet* res = pstmt->executeQuery();

    // no such user
    if(!res->next()){
        delete stmt;
        delete res;
        return FS_E_NOSUCHUSER;
    }

    // illegal password
    if(res->getString("username") != user->username){
        delete stmt;
        delete res;
        return FS_E_ILLEGALPASSWD;
    }

    delete stmt;
    delete res;
    return 0;

}

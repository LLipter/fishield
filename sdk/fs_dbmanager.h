#ifndef FS_DBMANAGER_H
#define FS_DBMANAGER_H

#include "fs_client.h"
#include "fs_config.h"
#include "fs_user.h"
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#define DB_HOST         std::string("tcp://")+_server_addr.to_string()+":3306"
#define DB_USERNAME     "fishield"
#define DB_PASSWORD     "fishield"
#define DB_DBNAME       "fishield"

extern boost::asio::ip::address _server_addr;

class fs_DBManager
{
public:
    fs_DBManager();
    ~fs_DBManager();
    int login(fs_user* user);
    void insert_task(const fs::proto::Task& task);
    void update_task(const fs::proto::Task& task);
    int get_task(int task_id, fs::proto::Task& task);
private:
    static sql::Driver* driver;
    sql::Connection* conn;
};

#endif // FS_DBMANAGER_H

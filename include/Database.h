#ifndef _DATABASE_
#define _DATABASE_
#include "logging.h"
#include <iostream>
#include <queue>
#include <string>
#include <sys/types.h>
#include <fstream>
#include <sys/epoll.h>
#include <openssl/md5.h>
#include <cstring>
// #include <md5.h>
#include <map>
#include <mysql/mysql.h>
#include <mysql/mysql_time.h>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <read_config.h>
#include <chrono>

using std::chrono::seconds;
using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::queue;
using std::string;
using std::pair;
using std::to_string;
using std::vector;
using std::map;
using std::tuple;
using std::make_tuple;
using std::unique_ptr;
    // stu_no char(7) not null,
    // stu_name char(8) not null,
    // stu_password char(32) not null,
    // stu_common_password char(32) not null,
    // stu_enable char(1) not null,
using student_t = std::tuple<string, string, string, string, char>;
string MD5(const char *data);
string generate_string(string src);
string generate_timestamp();
extern string DBConfig;
extern ReadConfig rc;
extern string dbname;
extern string dbuser;
extern string dbpw;
extern string dbhost;


enum class GameOver_t{
    GameFinished,
    GameOver,
    GameTimeout
};

// 接收学号和密码，查询是否可以登录
// 接收学号，MAPID，行，列，分数，步数，合成值，时长，结果，写入数据库

class Database
{
    DISABLE_COPY(Database)
private:
    MYSQL* mysql_;
    MYSQL_RES* res_;
    MYSQL_ROW row_;

    // mysql configs
    string db_;
    string pw_;
    string user_;
    string host_;
    // tcp configs
    string ipaddr_;
    uint16_t port_;


public:
    Database(const string& dbname, const string& hostname, 
        const string& username, const string& passwd,
        const uint16_t& port
        );

    void ConnectDB();
    void DisconnectDB();
    ~Database();

public:
    // 由学号获取学生的所有信息
    vector<student_t> getStudent(string stu_no);
    // 由学号获取学生的密码
    vector<string> getPW(string stu_no);
    // 由学号密码判断学生是否存在，0：不存在，>1存在，competition模式下为应该开始的局数
    int check_login(const string& stu_no, const string& pw, const string& mode);
    // 插入游戏数据
    void insert_base_data(const string &no, const int mapid, const int row, const int col,
                          const int score, const int step, const int maxvalue,
                          const int64_t createtime, const int64_t completetime,
                          const GameOver_t& gameovertype);
    void insert_competition_data(const string &no, const int mapid, const int row, const int col,
                                const int score, const int step, const int maxvalue,
                                const int64_t createtime, const int64_t completetime,
                                const GameOver_t& gameovertype);     
    // const char* str = "555601560a0306354e01505703515701065f5b56525501050351000000540a020d550d0507555d50";
};


#endif
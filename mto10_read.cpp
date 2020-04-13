#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>
#include <thread>
#include "logging.h"
#include <read_config.h>
#include <mysql/mysql.h>
#include <argument.h>
#include <tuple>
#include <ctime>
#define CRITICAL(x) Critical() << "[" << __FILE__ << " line:" << __LINE__ << "]" #x " failed!"
using namespace std;
// createtime, no, name, mapid, row, col, score, step, maxvalue, gametime, result, value
using gameinfo_stest_t =
    tuple<string, string, string, int, int, int, int, int, int, int, string, double>;
using gameinfo_gtest_t =
    tuple<string, string, string, int, int, int, int, int, int, int, string, double>;
using gameinfo_gtest_sumscore_t = 
    tuple<string, int, string, string, double>;


void show_gameinfo_stest(vector<gameinfo_stest_t> const&v){
    string createtime;
    string stuno;
    string name;
    int mapid;
    int row;
    int col;
    int score;
    int step;
    int maxvalue;
    int gametime;
    string result;
    double value;
    cout<<"�� "<<v.size()<<" ����¼"<<endl;
    cout << setw(20) << "��Ϸʱ��"
         << setw(10) << "ѧ��"
         << setw(10) << "����"
         << setw(12) << "��ͼID"
         << setw(5) << "��"
         << setw(5) << "��"
         << setw(8) << "����"
         << setw(6) << "����"
         << setw(9) << "�ϳ�ֵ"
         << setw(12) << "ʱ��(ms)"
         << setw(12) << "���"
         << setw(10) << "�÷�" << endl;

    for(const auto&t:v){
        tie(createtime, stuno, name, mapid, row, col, score,
            step, maxvalue, gametime, result, value)
             = t;
        cout << setw(20) << createtime
             << setw(10) << stuno
             << setw(10) << name
             << setw(12) << mapid
             << setw(5) << row
             << setw(5) << col
             << setw(8) << score
             << setw(6) << step
             << setw(9) << maxvalue
             << setw(12) << gametime
             << setw(12) << result
             << setw(10) << value << endl;
    }
}

void show_gameinfo_gtest(vector<gameinfo_stest_t> const&v){
    string grouptime;
    string stuno;
    string name;
    int mapid;
    int row;
    int col;
    int score;
    int step;
    int maxvalue;
    int gametime;
    string result;
    double value;
    cout<<"�� "<<v.size()<<" ����¼"<<endl;
    cout << setw(20) << "��ʱ��"
         << setw(10) << "ѧ��"
         << setw(10) << "����"
         << setw(12) << "��ͼID"
         << setw(5) << "��"
         << setw(5) << "��"
         << setw(8) << "����"
         << setw(6) << "����"
         << setw(9) << "�ϳ�ֵ"
         << setw(12) << "ʱ��(ms)"
         << setw(12) << "���"
         << setw(10) << "�÷�" << endl;

    for(const auto&t:v){
        tie(grouptime, stuno, name, mapid, row, col, score,
            step, maxvalue, gametime, result, value)
             = t;
        cout << setw(20) << grouptime
             << setw(10) << stuno
             << setw(10) << name
             << setw(12) << mapid
             << setw(5) << row
             << setw(5) << col
             << setw(8) << score
             << setw(6) << step
             << setw(9) << maxvalue
             << setw(12) << gametime
             << setw(12) << result
             << setw(10) << value << endl;
    }
}

void show_gameinfo_gtest_sum_score(vector<gameinfo_gtest_sumscore_t> const&v){
    string grouptime;
    int cnt;
    string stuno;
    string name;
    double score;
    cout<<"�� "<<v.size()<<" ����¼"<<endl;
    cout << setw(20) << "��ʱ��"
         << setw(6) << "����"
         << setw(10) << "ѧ��"
         << setw(10) << "����"
         << setw(10) << "�÷�" << endl;

    for(const auto&t:v){
        tie(grouptime, cnt, stuno, name, score)
             = t;
        cout << setw(20) << grouptime
             << setw(6) << cnt
             << setw(10) << stuno
             << setw(10) << name
             << setw(8) << score << endl;
    }

}
const string results[3]{"��Ϸ���", "��Ϸ����", "��Ϸ��ʱ"};

class Database
{
    DISABLE_COPY(Database)
private:
    MYSQL *mysql_;
    MYSQL_RES *res_;
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
    Database(const string &dbname, const string &hostname,
             const string &username, const string &passwd,
             const uint16_t &port)
        : db_(dbname), host_(hostname), user_(username),
          pw_(passwd), port_(port)
    {
        ConnectDB();
    }

    void ConnectDB()
    {
        Info() << "Use " << user_ << "@" << host_ << ", password:" << pw_ << ", database:" << db_ << "to login database";
        if (nullptr == (mysql_ = mysql_init(nullptr)))
        {
            CRITICAL(mysql_init);
            exit(-1);
        }
        if (nullptr == mysql_real_connect(mysql_, host_.data(), user_.data(), "123456", db_.data(), 0, nullptr, 0))
        {
            CRITICAL(mysql_real_connect);
            Critical() << mysql_error(mysql_);
            exit(-1);
        }
        Info() << "mysql connect successfully!";
        mysql_set_character_set(mysql_, "gbk");
    }

    void DisconnectDB()
    {
        mysql_close(mysql_);
        mysql_ = nullptr;
    }

    ~Database()
    {
        if (mysql_)
            mysql_close(mysql_);
    }

public:
    vector<gameinfo_stest_t> mto10read_stest(int limit)
    {
        stringstream ss;
        ss << "call get_gameinfo_stest(null, null, null," << limit << ");";
        return mto10read_stest_help(ss.str());
    }

    vector<gameinfo_stest_t> mto10read_stest(int mapid, int limit)
    {
        stringstream ss;
        ss << "call get_gameinfo_stest(" << mapid << ", null, null, " << limit << ");";
        return mto10read_stest_help(ss.str());
    }  

    vector<gameinfo_stest_t> mto10read_stest(int mapid, int row, int col, int limit)
    {
        stringstream ss;
        ss<<"call get_gameinfo_stest("<<mapid<<','<<row<<','<<col<<','<<limit<<");";
        return mto10read_stest_help(ss.str());
    }

    vector<gameinfo_stest_t> mto10read_stest_help(string const& q){
        if(mysql_query(mysql_, q.data())){
            CRITICAL(mysql_query);
            Critical()<<"sqlcmd: "<<q;
            return {};
        }
        vector<gameinfo_stest_t> res;
        string createtime;
        string stuno;
        string name;
        int mapid;
        int row;
        int col;
        int score;
        int step;
        int maxvalue;
        int gametime;
        string result;
        double value;
        // 1 createtime,
        // 2 no,
        // 3 stu_name as name,
        // 4 mapid,
        // 5 row,
        // 6 col, 
        // 7 score,
        // 8 step,
        // 9 maxvalue,
        // 10 gametime,
        // 11 result
        do
        {
            if(res_ = mysql_store_result(mysql_)){
                // int rows = mysql_num_fields(res_);
                // cout << "rows: " << rows << endl;
                while(row_ = mysql_fetch_row(res_)){
                    createtime = row_[0];
                    stuno = row_[1];
                    name = row_[2];
                    mapid = atoi(row_[3]);
                    row = atoi(row_[4]);
                    col = atoi(row_[5]);
                    score = atoi(row_[6]);
                    step = atoi(row_[7]);
                    maxvalue = atoi(row_[8]);
                    gametime = atoi(row_[9]);
                    result = results[atoi(row_[10])];
                    value = double(score)/row/col;
                    res.push_back(make_tuple(
                        createtime, stuno, name, mapid, row, col, score, 
                        step, maxvalue, gametime, result, value
                    ));
                }
                if(mysql_errno(mysql_)){
                    Warning()<<mysql_error(mysql_);
                }
            }
        } while (!mysql_next_result(mysql_));
        return res;
    }

    vector<gameinfo_gtest_t> mto10read_gtest(const string& grouptime, const string& no){
        stringstream ss;
        ss << "call get_gameinfo_gtest('" << grouptime << "'," << no << ");";
        return mto10read_gtest_help(ss.str());
    }

    vector<gameinfo_gtest_t> mto10read_gtest_help(string const& q){
        Info()<<"sqlcmd: "<<q;
        if(mysql_query(mysql_, q.data())){
            CRITICAL(mysql_query);
            return {};
        }
        vector<gameinfo_stest_t> res;
        string grouptime;
        string stuno;
        string name;
        int mapid;
        int row;
        int col;
        int score;
        int step;
        int maxvalue;
        int gametime;
        string result;
        double value;
        // 1 grouptime,
        // 2 no,
        // 3 stu_name as name,
        // 4 mapid,
        // 5 row,
        // 6 col, 
        // 7 score,
        // 8 step,
        // 9 maxvalue,
        // 10 gametime,
        // 11 result
        do
        {
            if(res_ = mysql_store_result(mysql_)){
                // int rows = mysql_num_fields(res_);
                // cout << "rows: " << rows << endl;
                while(row_ = mysql_fetch_row(res_)){
                    grouptime = row_[0];
                    stuno = row_[1];
                    name = row_[2];
                    mapid = atoi(row_[3]);
                    row = atoi(row_[4]);
                    col = atoi(row_[5]);
                    score = atoi(row_[6]);
                    step = atoi(row_[7]);
                    maxvalue = atoi(row_[8]);
                    gametime = atoi(row_[9]);
                    result = results[atoi(row_[10])];
                    value = double(score)/row/col;
                    res.push_back(make_tuple(
                        grouptime, stuno, name, mapid, row, col, score, 
                        step, maxvalue, gametime, result, value
                    ));
                }
                if(mysql_errno(mysql_)){
                    Warning()<<mysql_error(mysql_);
                }
            }
        } while (!mysql_next_result(mysql_));
        return res;

    }

    vector<gameinfo_gtest_sumscore_t> mto10read_gtest_sum_score(const string& grouptime, const string& no){
        stringstream ss;
        ss << "call get_sum_score('" << grouptime << "'," << no << ");";
        return mto10read_gtest_sum_score_help(ss.str());
    }

    vector<gameinfo_gtest_sumscore_t> mto10read_gtest_sum_score(nullptr_t, const string& no){
        stringstream ss;
        ss << "call get_sum_score(null," << no << ");";
        return mto10read_gtest_sum_score_help(ss.str());
    }

    vector<gameinfo_gtest_sumscore_t> mto10read_gtest_sum_score(const string& grouptime, nullptr_t){
        stringstream ss;
        ss << "call get_sum_score('" << grouptime << "',null);";
        return mto10read_gtest_sum_score_help(ss.str());
    }

    vector<gameinfo_gtest_sumscore_t> mto10read_gtest_sum_score(nullptr_t, nullptr_t){
        stringstream ss;
        ss << "call get_sum_score(null, null);";
        return mto10read_gtest_sum_score_help(ss.str());
    }

    vector<gameinfo_gtest_sumscore_t> mto10read_gtest_sum_score_help(string const& q){
        Info() << "sqlcmd: " << q;
        if(mysql_query(mysql_, q.data())){
            CRITICAL(mysql_query);
            return {};
        }
        vector<gameinfo_gtest_sumscore_t> res;
        string grouptime;
        int cnt;
        string no;
        string name;
        double score;
        do
        {
            if(res_ = mysql_store_result(mysql_)){
                // int rows = mysql_num_fields(res_);
                // cout << "rows: " << rows << endl;
                while(row_ = mysql_fetch_row(res_)){
                    grouptime = row_[0];
                    cnt = atoi(row_[1]);
                    no = row_[2];
                    name = row_[3];
                    score = atof(row_[4]);
                    res.push_back(make_tuple(
                        grouptime, cnt, no, name, score
                    ));
                }
                if(mysql_errno(mysql_)){
                    Warning()<<mysql_error(mysql_);
                }
            }
        } while (!mysql_next_result(mysql_));
        return res;
    }
};

void Usage()
{
    cout << "Usage: " << endl
         << "       mto10_read --stest [--mapid xxx --row xx --col xx --limit xx]" << endl
         << "       mto10_read --final" << endl
         << "e.g. :" << endl
         << "       mto10_read --gtest --time \"2020-03-15 22:00:00\" --user all    : ��ʾ�����û���ĳʱ�β��Ե�ͳ�ƽ��   " << endl
         << "       mto10_read --gtest --time \"2020-03-15 22:00:00\" --user 1752877: ��ʾ�û�1752877ĳʱ�β��Ե�ͳ�ƽ��   " << endl
         << "       mto10_read --gtest --time all --user 1752877                    : ��ʾ�û�1752877����ʱ�β��Ե�ͳ�ƽ��   " << endl
         << "       mto10_read --gtest --time all --user all                        : ��ʾ�����û�����ʱ�β��Ե�ͳ�ƽ��   " << endl
         << "e.g. :" << endl
         << "       mto10_read --stest                                              : ��ʾ���µ�10����Ϸ�Ľ�����Ǿ�����" << endl
         << "       mto10_read --stest --limit 64                                   : ��ʾ���µ�64����Ϸ�Ľ�����Ǿ�����" << endl
         << "       mto10_read --stest --mapid 12345                                : ��ʾ���µ�10��[mapid=12345/����ֵ����]��Ϸ�Ľ�����Ǿ�����" << endl
         << "       mto10_read --stest --mapid 12345 --limit 16                     : ��ʾ���µ�32��[mapid=12345/����ֵ����]��Ϸ�Ľ�����Ǿ�����" << endl
         << "       mto10_read --stest --mapid 12345 --row 8 --col 10               : ��ʾ���µ�10��[mapid=12345/��=8/��=10]��Ϸ�Ľ�����Ǿ�����" << endl
         << "       mto10_read --stest --mapid 12345 --row 8 --col 10 --limit 16    : ��ʾ���µ�16��[mapid=12345/��=8/��=10]��Ϸ�Ľ�����Ǿ�����" << endl
         << "��־������log/mto10_read.txt��\n\n";
}

void ShowParamError(string const&param){
    cerr << "--" << param << " ��Ĳ������� " << endl;
    Usage();
    exit(-1);
}

int main(const int _Argc, char const **const _Argv)
{
    if(_Argc == 1){
        Usage();
        exit(0);
    }
    MessageLogger::setLogFile("./log/mto10_read.txt");
    Argument args(_Argc, _Argv);
    string DBConfig{"./config/db.config"};
    ReadConfig rc(DBConfig);
    string dbname = rc.getValue("dbname");
    string dbuser = rc.getValue("user");
    string dbpw = rc.getValue("pw");
    string dbhost = rc.getValue("host");
    Database db(dbname, dbhost, dbuser, dbpw, 4000);
    db.ConnectDB();
    vector<gameinfo_stest_t> res;

    int limit = 10;
    if(args.hasSet("stest")){
        if(args.hasSet("limit")){
            auto p = args.getArg("limit");
            if(p.first){
                try
                {
                    limit = stoi(p.second);
                }
                catch(const std::exception& e)
                {
                    ShowParamError("limit");
                }
            }
        }
        if(args.hasSet("mapid")){
            int mapid;
            auto p = args.getArg("mapid");
            if(p.first){
                try
                {
                    mapid = stoi(p.second);
                }
                catch(const std::exception& e)
                {
                    ShowParamError("mapid");
                }
            }
            else{
                ShowParamError("mapid");
            }
            if(args.hasSet("row")&&args.hasSet("col")){
                int row;
                int col;
                auto pp = args.getArg("row");
                if(pp.first){
                    try
                    {
                        row = stoi(pp.second);
                    }
                    catch(const std::exception& e)
                    {
                        ShowParamError("row");
                    }
                }
                else{
                    ShowParamError("row");
                }
                pp = args.getArg("col");
                if(pp.first){
                    try
                    {
                        col = stoi(pp.second);
                    }
                    catch(const std::exception& e)
                    {
                        ShowParamError("row");
                    }
                }else{
                    ShowParamError("row");
                }
                res = db.mto10read_stest(mapid, row, col, limit);
            }
            else if ((args.hasSet("row") && !args.hasSet("col")) ||
                    (args.hasSet("col") && !args.hasSet("row"))){
                cerr<<"�뽫row, colͬʱָ��"<<endl;
                Usage();
                exit(-1);
            }
            else
            {
                res = db.mto10read_stest(mapid, limit);
            }
        }
        else{
            res = db.mto10read_stest(limit);
        }
        show_gameinfo_stest(res);
    }
    else if(args.hasSet("gtest")){
        string grouptime;
        string user;
        if(args.hasSet("time")&&args.hasSet("user")){
            auto p = args.getArg("time");
            if(p.first){
                if(p.second == "all"){

                }else{
                    grouptime = p.second;
                }
            }else{
                ShowParamError("time");
            }
            p = args.getArg("user");
            if(p.first){
                if(p.second == "all"){

                }else{
                    user = p.second;
                }
            }
            else{
                ShowParamError("user");
            }
            if(args.hasSet("detailed")){
                auto v = db.mto10read_gtest(grouptime, user);
                show_gameinfo_gtest(v);
            }
            else{
                vector<gameinfo_gtest_sumscore_t> res;
                if(user.empty()&&grouptime.empty()){
                    res = db.mto10read_gtest_sum_score(nullptr, nullptr);
                }else if(user.empty()){
                    res = db.mto10read_gtest_sum_score(grouptime, nullptr);
                }else if(grouptime.empty()){
                    res = db.mto10read_gtest_sum_score(nullptr, user);
                }else{
                    res = db.mto10read_gtest_sum_score(grouptime, user);
                }
                show_gameinfo_gtest_sum_score(res);
            }
        }
        else{
            Usage();
            exit(-1);
        }
    }
    else {
        Usage();
        exit(-1);
    }
}

#include <iostream>
#include <mysql/mysql.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <md5.h>
#include <fstream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

string hostname = "localhost";
string username = "root";
string password = "123456";
string dbname = "hw-mto10-u1752877";

string randPasswd(int length=16) {
    string res;
    for(int i=0;i<length;++i){
        res.push_back(rand()%('~'-'!'+1)+'!');
    }
    return res;
}

string quoted(string const&str){
    return '\"' + str + '\"';
}

int insert(
    MYSQL* mysql,
    string stu_no, 
    string stu_name, 
    string stu_password, 
    string stu_common_password,
    string stu_enable
    ){
    std::stringstream ss;
    ss << "call insert_student("
       << quoted(stu_no) << ','
       << quoted(stu_name) << ','
       << quoted(stu_password) << ','
       << quoted(stu_common_password) << ','
       << quoted(stu_enable) << ");";
    if (mysql_query(mysql, ss.str().data())){
        cerr << "mysql_query failed: "<<ss.str().data()<<endl;
        return -1;
    }
    return 0;
}

int main()
{
    MYSQL* mysql;
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::fstream fout;
    fout.open("../config/user.tsv");
    if(!fout.is_open()){
        cerr << "file user.tsv cannot be opened" << endl;
        return -1;
    }

    if(nullptr == (mysql = mysql_init(nullptr))){
        cerr<<("mysql_init failed")<<endl;
        exit(-1);
    }
    if(nullptr == (mysql_real_connect(
        mysql,
        hostname.data(),
        username.data(),
        password.data(),
        dbname.data(),
        0, nullptr, 0 ))){
        
        cerr<<("mysql_real_connect")<<endl;
        exit(-1);
    }
    cout<<"db connect successfully"<<endl;
    mysql_set_character_set(mysql, "gbk");
    std::fstream f("namelist.csv");
    string name;
    int stu_no = 2000000;
    while(f>>name){
        string pw = randPasswd();
        MD5 md5(pw);
        int ret = insert(mysql,std::to_string(stu_no),name,md5.toStr(), "12345678","1");
        if(ret==-1)
            break;
        fout<<stu_no<<'\t'<<pw<<endl;
        ++stu_no;
    }
    MD5 md5("xZx3aUL2%kt#-9+&");
    insert(mysql, "1752877", "ºúÐù", md5.toStr(), "12345678", "1");
}

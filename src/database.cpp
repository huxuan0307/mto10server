#include <Database.h>

string DBConfig{"./config/db.config"};
ReadConfig rc(DBConfig);
string dbname = rc.getValue("dbname");
string dbuser = rc.getValue("user");
string dbpw = rc.getValue("pw");
string dbhost = rc.getValue("host");

string MD5(const char * data)
{
	MD5_CTX ctx;
    string res;
	unsigned char md[16];
	char tmp[3]={'\0'};
	int i;
	MD5_Init(&ctx);
	MD5_Update(&ctx,data,strlen(data));
	MD5_Final(md,&ctx);

	for( i=0; i<16; i++ ){
		sprintf(tmp,"%02x",md[i]);
		//strcat(buf,tmp);
        res += string(tmp);
	}
	return res;
}


Database::Database(const string& dbname, const string& hostname, 
    const string& username, const string& passwd,
    const uint16_t& port
    )
    :db_(dbname), host_(hostname), user_(username),
    pw_(passwd), port_(port)
{
    ConnectDB();
}

void Database::ConnectDB(){
    Info() << "Use " << user_ << "@" << host_ << ", password:" << pw_ << ", database:" << db_ << "to login database";
    if (nullptr == (mysql_ = mysql_init(nullptr))){
        CRITICAL(mysql_init);
        exit(-1);
    }
    auto t = milliseconds(50);
    while (nullptr == mysql_real_connect(mysql_, host_.data(), user_.data(), "123456", db_.data(), 0, nullptr, 0)){
        WARNING(mysql_real_connect);
        Warning()<<mysql_error(mysql_);
        this_thread::sleep_for(t*=2);   // 指数增加
    }
    Info()<<"mysql connect successfully!";
    mysql_set_character_set(mysql_, "gbk");
}

void Database::DisconnectDB()
{
    mysql_close(mysql_);
    mysql_ = nullptr;
}

Database::~Database(){
    if(mysql_)
        mysql_close(mysql_);
}

vector<student_t> Database::getStudent(string stu_no){
    string q = "call get_student("+stu_no+");";
    Info() << "execute mysql cmd: " << q;
    vector<student_t> res;
    if(mysql_query(mysql_, q.data())){
        CRITICAL(mysql_query);
        return {};
    }
    do
    {
        if(res_=mysql_store_result(mysql_)){
            int j=mysql_num_fields(res_);
            while(row_ = mysql_fetch_row(res_)){
                res.push_back(make_tuple(row_[0], row_[1], row_[2], row_[3], *row_[4]));
            }
            if(mysql_errno(mysql_)){
                Warning()<<mysql_error(mysql_);
            }
        }
        mysql_free_result(res_);
    } while (!mysql_next_result(mysql_));
    
    return res;
}

vector<string> Database::getPW(string stu_no){
    vector<string> res;
    string q = "call get_pw("+stu_no+");";
    Info() << "execute mysql cmd: " << q;
    
    if(mysql_query(mysql_, q.data())){
        CRITICAL(mysql_query);
        return {};
    }
    do
    {
        if(res_=mysql_store_result(mysql_)){
            int j=mysql_num_fields(res_);
            while(row_ = mysql_fetch_row(res_)){
                res.push_back(row_[0]);
            }
            if(mysql_errno(mysql_)){
                Warning()<<mysql_error(mysql_);
            }
        }
        mysql_free_result(res_);
    } while (!mysql_next_result(mysql_));
    return res;
}

int Database::check_login(const string& stu_no, const string& pw, const string& mode)
{
    
    int res;
    string q = "call check_login(" + stu_no + "," + quoted(pw) + "," + quoted(mode) + ");";
    Info() << q;

    if(mysql_query(mysql_, q.data())){
        CRITICAL(mysql_query);
        Critical() << mysql_error(mysql_);
        return false;
    }
    do
    {
        if(res_=mysql_store_result(mysql_)){
            while(row_ = mysql_fetch_row(res_)){
                res = atoi(row_[0]);
            }
            if(mysql_errno(mysql_)){
                Warning()<<mysql_error(mysql_);
            }
        }
        mysql_free_result(res_);
    } while (!mysql_next_result(mysql_));

    return res;
}

void Database::insert_base_data(const string &no, const int mapid, const int row, const int col,
                const int score, const int step, const int maxvalue,
                const int64_t createtime, const int64_t completetime,
                const GameOver_t &gameovertype)
{
    stringstream ss;
    ss << "call insert_gameinfo_stest("
       << no << ','
       << mapid << ','
       << row << ','
       << col << ','
       << score << ','
       << step << ','
       << maxvalue << ','
       << createtime << ','
       << completetime << ','
       << static_cast<const int>(gameovertype) << ");";
    string q = ss.str();
    Info() << "execute mysql cmd: " << q;

    if(mysql_query(mysql_, q.data())){
        Warning()<<"mysql_query failed";
        WARNING(insert_base_data);
    }
}

void Database::insert_competition_data(const string &no, const int mapid, const int row, const int col,
                const int score, const int step, const int maxvalue,
                const int64_t createtime, const int64_t completetime,
                const GameOver_t &gameovertype)
{
    stringstream ss;
    ss << "call insert_gameinfo_gtest("
       << no << ','
       << mapid << ','
       << row << ','
       << col << ','
       << score << ','
       << step << ','
       << maxvalue << ','
       << createtime << ','
       << completetime << ','
       << static_cast<const int>(gameovertype) << ");";
    string q = ss.str();
    Info() << "execute mysql cmd: " << q;

    if(mysql_query(mysql_, q.data())){
        Warning()<<"mysql_query failed";
        WARNING(insert_competition_data);
    }
}
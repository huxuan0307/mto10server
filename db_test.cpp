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
#include <md5.h>
#include <Database.h>
#include <mto10server.h>
#include <sys/time.h>
using namespace std;

int main (const int _Argc, char const** const _Argv)
{
	MessageLogger::setLogFile ("./log/databaselog.txt");
	string DBConfig{"./config/db.config"};
	ReadConfig rc(DBConfig);
	string dbname = rc.getValue("dbname");
	string user = rc.getValue("user");
	string pw = rc.getValue("pw");
	string host = rc.getValue("host");
	Database db(dbname, host, user, pw, 4000);
	auto v = db.getStudent("1752877");
	for(auto&t:v){
		cout << get<0>(t) << endl
			<< get<1>(t) << endl
			<< get<2>(t) << endl
			<< get<3>(t) << endl
			<< get<4>(t) << endl;
	}
	string pwmd5 = MD5("xZx3aUL2%kt#-9+&");
	cout<<pwmd5<<endl;
	bool res = db.check_login("1752877", pwmd5, "s");
	cout<<res<<endl;
	timeval tt;
	struct timezone tz;
	// timezone tz;
	gettimeofday(&tt, &tz);

	db.insert_base_data("1752877", rand(), 6, 8, 1000,
    100, 8, tt.tv_sec*1000, tt.tv_sec*1000 + tt.tv_usec/1000, GameOver_t::GameFinished);
	
}



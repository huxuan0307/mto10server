#include <regex>
#include <iostream>
#include <string>
#include <map>

using namespace std;

int main ()
{
	regex re ("(.*) = (.*)\n");
	string str(
		"Type = ParameterAuthenticate\r\n"
		"MD5 = 0707020d5753524c5402070757020450030f5509075153520c520453580050055603060204040654\r\n"
		"Row = 5\r\n"
		"Col = 5\r\n"
		"GameID = 0\r\n"
		"Delay = 5000\r\n"
		"Length = 176\r\n");
    // cout << str << endl;
	stringstream ss(str);
	string key, val;
	char eq;
	map<string, string> mmm;
	while(ss>>key){
		ss>>eq>>val;
		mmm.emplace(move(key), move(val));
	}
	for(auto&p: mmm){
		cout<<p.first<<'='<<p.second<<endl;
	}
}
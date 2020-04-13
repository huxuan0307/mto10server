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
#include <argument.h>
using namespace std;

void Usage(){
	puts(
"Usage:\n"
"        mto10server --port xxxxx			: 指定绑定xxxxx端口\n"
	);
}

#if _HAS_CXX17
using var = variant<int, uint16_t, string>;
int main (const int _Argc, char const** const _Argv)
{
	MessageLogger::setLogFile ("./log/runtimelog.txt"s);
	//string ipaddr = "10.60.102.252";
	//uint16_t port = 21345;
	//NetworkForMto10 network (move (ipaddr), port);

	//while (network.getBufLen () < 88)
	//	;
	//auto res = network.getRandStr ();
	//for (char c : res) {
	//	putchar (c);
	//}


	var v = "10.60.102.252"s;
	unordered_map<string, var> m;
	m["ipaddr"] = "10.60.102.252"s;
	m["port"] = static_cast<uint16_t>(21345u);
	m["mapid"] = 0;
	m["stuno"] = "1234567";
	m["passwd"] = "0HhJ)j8JGx+3uq.#";
	m["row"] = 5;
	m["col"] = 5;
	m["delay"] = 60;
	MTO10 mto10 (m);

}

#else
int main (const int _Argc, char const** const _Argv)
{
	if(_Argc == 1){
		Usage();
		exit(0);
	}
	MessageLogger::setLogFile ("./log/runtimelog.txt");
	Argument args(_Argc, _Argv);
	uint16_t port;
	if(args.hasSet("port")){
		auto p = args.getArg("port");
		if(p.first){
			try
			{
				port = stoul(p.second);
			}
			catch(const std::exception& e)
			{
				Usage();
				exit(-1);
			}
			Mto10Server server(5000u);
			server.Start();
		}
		else{
			Usage();
			exit(-1);
		}
	}

}
#endif

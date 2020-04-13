#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iomanip>
#include <thread>
#include <future>
#include <chrono>

#include "global.h"
#include "logging.h"
#include "RingBuffer.h"
using namespace std;
using namespace std::chrono;
constexpr int BUFFER_SIZE{ 1024 };



class TCPClient
{
public:
	DISABLE_COPY (TCPClient);

	TCPClient (string&& ipaddr_, const uint16_t port_);
	TCPClient (const string& ipaddr_, const uint16_t port_);
	TCPClient ();
	void init (const string& ipaddr_, const uint16_t port_);
	void connect ();
	int readFromNet ();
	inline int getBufLen ()
	{
		return rbuf.getSize ();
	}
	void getBytes (char* dataptr, int len);
	string getline ();
	void disconnect ();
	void run ();
	int writeToNet (string&& data);
private:
	void init ();

	string ipaddr;
	uint16_t port;
	int sockclnt;
	int sockfd;
	sockaddr_in server_addr;
	RingBuffer<2048> rbuf;
	RingBuffer<2048> wbuf;
	char buf[1024];
	bool is_init;
protected:
	future<void> th_for_recv;
	bool ready;
	bool shouldClose;
};

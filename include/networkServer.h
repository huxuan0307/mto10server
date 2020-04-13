
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
#include <unordered_map>
#include <queue>
#include <sys/epoll.h>
#include "global.h"
#include "logging.h"
#include "RingBuffer.h"
using namespace std;
using namespace std::chrono;
constexpr int BUFFER_SIZE{ 1024 };
constexpr int LISTEN_NUM{20000};

using session_t = int;
class NetworkServer
{
public:
	DISABLE_COPY (NetworkServer);

	NetworkServer (string&& ipaddr_, const uint16_t port_);
	NetworkServer (const string& ipaddr_, const uint16_t port_);

	int readFromNet ();
	inline int getBufLen ()
	{
		return rbuf.getSize ();
	}
	void getBytes (char* dataptr, int len);
	string getline ();
	void disconnect ();
	void run ();
	int writeToNet (string&& data)
	{
		int ret = ::send (socklisten, data.data (), data.size (), 0);
		if (ret == -1) {
			if (errno == EWOULDBLOCK) {
				this_thread::sleep_for (milliseconds(20));
				Warning () << "SOCKET_ERROR:EWOULDBLOCK";
			}
			return -1;
		}
		else if (ret == 0) {
			shouldClose = true;
			Info () << "Connection closed by server";
			return -1;
		}
		Info () << "writeToNet" << "put" << ret << "Bytes to Net";
		return ret;
	}
private:
	void init ();
	
	string ipaddr;
	uint16_t port;
	int socklisten;
	int sockfd;
	sockaddr_in server_addr;
	RingBuffer<2048> rbuf;
	RingBuffer<2048> wbuf;
	char buf[1024];
protected:
	future<void> th_for_recv;
	bool ready;
	bool shouldClose;
	int epfd;
	epoll_event ep_events[LISTEN_NUM];

	session_t session_cnt;
	queue<session_t> loginQue;

	unordered_map<session_t, unique_ptr<RingBuffer<1024>>> bufs;
	unordered_map<session_t, future<void>> threads;
	session_t getNewSession();
	void EpollCreate();
	void EpollAdd(int fd, uint32_t events);
	void EpollDel(int fd);
	int EpollWait();
	void handleNewConnection(int sockclnt);
	void threadForRecv(session_t session);
};

class NetworkForMto10 :public NetworkServer
{
public:
	NetworkForMto10 (string&& ipaddr_, const uint16_t port_) :NetworkServer{ move (ipaddr_), port_ }
	{
		th_for_recv = async (launch::async, &NetworkServer::run, this);
		
	}
	NetworkForMto10 (string const& ipaddr_, const uint16_t port_) :NetworkServer{ ipaddr_, port_ }
	{
		th_for_recv = async (launch::async, &NetworkServer::run, this);
		
	}
	~NetworkForMto10 ()
	{
		shouldClose = true;
		// Network::disconnect ();
		th_for_recv.get();
	}
private:
	char buffer[1024];
};
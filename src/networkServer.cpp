#define _CRT_SECURE_NO_WARNINGS
#include <networkServer.h>

NetworkServer::NetworkServer (string&& ipaddr_, const uint16_t port_)
	:ipaddr{ move (ipaddr_) }, port{ port_ }, sockfd{ -1 }, 
	shouldClose{ false }, ready{ false }, session_cnt{0}
{
	server_addr.sin_addr.s_addr = inet_addr (ipaddr.data ());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (port);
	init ();
	EpollCreate();
	// add listen socket
	EpollAdd(socklisten, EPOLLIN);
	// run server
	run();

}

NetworkServer::NetworkServer (const string& ipaddr_, const uint16_t port_)
	:ipaddr{ ipaddr_ }, port{ port_ }, sockfd{ -1 }, 
	shouldClose{ false }, ready{ false }, session_cnt{0}
{
	server_addr.sin_addr.s_addr = inet_addr (ipaddr.data ());
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (port);
	init ();
}

int NetworkServer::readFromNet ()
{
	int ret = ::recv (socklisten, buf, BUFFER_SIZE, 0);
	if (ret == -1) {
		if (errno == EWOULDBLOCK) {
			this_thread::sleep_for (milliseconds(20));
			Warning () << "SOCKET_ERROR:WSAEWOULDBLOCK";
		}
		else if (errno == ETIMEDOUT || errno == ENETDOWN
			|| errno == ECONNRESET) {
			Warning () << "connention interupted!";
		}
		//Critical () << "recv error! errcode=" << err;
		return -1;
	}
	else if (ret == 0) {
		shouldClose = true;
		Info () << "Connection closed by server";

		return -1;
	}
	Info () << "readFromNet" << "get" << ret << "Bytes from Net";
	rbuf.write (buf, ret);
	return ret;
}

void NetworkServer::getBytes (char* dataptr, int len)
{
#undef min
	if (len > rbuf.getSize ())
		Warning () << "buffer length less than required";
	rbuf.read (dataptr, min (len, rbuf.getSize ()));
}

void NetworkServer::init ()
{
	int ret;
	socklisten = socket (AF_INET, SOCK_STREAM, 0);
	if(socklisten == -1){
		Critical()<<"Failed to create listen socket!!";
		abort();
	}

	// 设置地址复用
	int opt{1};
	ret = setsockopt(socklisten, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));
	if(ret==-1){
		Critical()<<"Failed to setsockopt";
		abort();
	}

	// 设置地址
	if(-1==inet_pton(AF_INET, this->ipaddr.data(), &server_addr.sin_addr)){
		Critical()<<"invalid address";
		abort();
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(this->port);

	ret = bind(socklisten, (sockaddr*)&server_addr, sizeof(server_addr));
	if(-1==ret){
		Critical()<<"bind error";
		abort();
	}

	ret = listen(socklisten, LISTEN_NUM);
	if(-1==ret){
		Critical()<<"listen error";
		abort();
	}

	Info()<<"Start Server at "<<ipaddr<<":"<<port;

}

string NetworkServer::getline ()
{
	return rbuf.readline ();
}

void NetworkServer::disconnect ()
{
	close (sockfd);
	Info () << "Disconnected from" << ipaddr << ':' << port;
	cout << "Disconnected from" << ipaddr << ':' << port << endl;
}

void NetworkServer::run ()
{
	int epcnt;
	while(true){
		epcnt = EpollWait();
		for(int i=0;i<epcnt;++i){
			auto& ep_ev = this->ep_events[i];
			if(ep_ev.data.fd == socklisten){

				if(ep_ev.events & EPOLLERR){
                    Critical()<<"EPOLLERR socket listensocket";
                    EpollDel(ep_ev.data.fd);
                    exit(-1);
                }
				// new connection
				else if (ep_ev.events & EPOLLIN){
					sockaddr_in addrclnt;
					socklen_t addrlen = sizeof(addrclnt);
					int sockclnt = accept(socklisten, (sockaddr*)&addrclnt, &addrlen);
					if(sockclnt==-1){
						Critical()<<"accept error";
					}
					Info()<<"new connection from "<< inet_ntoa(addrclnt.sin_addr)
						<<":"<<ntohs(addrclnt.sin_port);
                    handleNewConnection(sockclnt);
				}
			}
		}
	}
}

void NetworkServer::EpollAdd(int fd, uint32_t events)
{	
	Info()<<"EpollAdd begin";
	static epoll_event ep_ev;
	ep_ev.data.fd = fd;
	ep_ev.events = events;
	if( -1 == epoll_ctl(epfd, EPOLL_CTL_ADD, ep_ev.data.fd, &ep_ev)){
		Warning()<<"EpollAdd failed";
	}
}

void NetworkServer::EpollDel(int fd)
{
	Info()<<"EpollDel begin";
	if( -1 == epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr)){
		Warning()<<"EpollDel failed";
	}
}

int NetworkServer::EpollWait(){
	Info()<<"EpollWait begin";
	int cnt;
	cnt = epoll_wait(epfd, this->ep_events, LISTEN_NUM, -1);
	Info()<<"EpollWait end, cnt = "<<cnt;
	return cnt;
}

void NetworkServer::EpollCreate(){
	Info()<<"EpollCreate begin";
	epfd = epoll_create1(0);
	if(-1==epfd){
		Critical()<<"EpollCreate Error";
		abort();
	}
}

void NetworkServer::handleNewConnection(int sockclnt)
{
	int session = getNewSession();
	// create read buffer
	bufs[session].reset(new RingBuffer<1024>);
	// create read thread to recv message 
	threads[session] = async(launch::async, threadForRecv, session);
}

void NetworkServer::threadForRecv(session_t session)
{
	
}

session_t NetworkServer::getNewSession(){
	// maybe need lock!!!
	return ++session_cnt;
}
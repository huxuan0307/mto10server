#ifndef _MTO10SERVER_
#define _MTO10SERVER_

#include "../TCP/TcpServer.h"
#include <memory>
#include <array>
#include <functional>
#include "mto10session.h"
#include "Database.h"
#include <future>
using namespace std;

class Mto10Server
{
private:
    shared_ptr<TcpServer> tcpserver_;
    shared_ptr<EventLoop> loop_;
    uint16_t port_;
    Database db_;
    map<TcpConnection*, shared_ptr<Mto10Session>> mto10sessionlist_;
    void HandleNewConnection(TcpConnection* ptcpconn);
    void HandleMessage(TcpConnection* ptcpconn, string& s);
    void HandleSendComplete(TcpConnection *ptcpconn);
    void HandleClose(TcpConnection* ptcpconn);
    void HandleError(TcpConnection* ptcpconn);
    
public:
    Mto10Server(uint16_t port);
    void Start();
    friend class Mto10Session;
};

#endif
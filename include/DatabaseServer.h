#ifndef _DATABASESERVER_
#define _DATABASESERVER_

#include "../TCP/TcpServer.h"
#include <memory>
#include <array>
#include <functional>
// #include "DatabaseSession.h"
using namespace std;

class DatabaseServer
{
private:
    shared_ptr<TcpServer> tcpserver_;
    shared_ptr<EventLoop> loop_;
    uint16_t port_;
    // map<TcpConnection*, shared_ptr<Mto10Session>> mto10sessionlist_;
    void HandleNewConnection(TcpConnection* ptcpconn);
    void HandleMessage(TcpConnection* ptcpconn, string& s);
    void HandleSendComplete(TcpConnection *ptcpconn);
    void HandleClose(TcpConnection* ptcpconn);
    void HandleError(TcpConnection* ptcpconn);
    
public:
    DatabaseServer(uint16_t port);
    void Start();
};

#endif
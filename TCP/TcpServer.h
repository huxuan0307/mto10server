

#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <functional>
#include <string>
#include <map>
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"

#define MAXCONNECTION 60000

class TcpServer
{
public:
    //typedef std::function<void(TcpConnection*)> NewConnectionCallback;
    typedef std::function<void(TcpConnection*, std::string&)> MessageCallback;
    typedef std::function<void(TcpConnection*)> Callback;
    TcpServer(EventLoop* loop, int port);
    ~TcpServer();

    void Start();

    //ҵ����ע��
    void SetNewConnCallback(Callback cb)
    {
        newconnectioncallback_ = cb; 
    }
    void SetMessageCallback(MessageCallback cb)
    {
        messagecallback_ = cb;
    }
    void SetSendCompleteCallback(Callback cb)
    { 
        sendcompletecallback_ = cb; 
    }
    void SetCloseCallback(Callback cb)
    {
        closecallback_ = cb;
    }    
    void SetErrorCallback(Callback cb)
    { 
        errorcallback_ = cb;
    }


private:
    /* data */
    Socket serversocket_;
    EventLoop *loop_;
    Channel serverchannel_;
    int conncount_;
    std::map<int, TcpConnection*> tcpconnlist_;

    //ҵ��ӿں���
    Callback newconnectioncallback_;
    MessageCallback messagecallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;

    //�����������Ӵ���ĺ�����ҵ���޹�
    void OnNewConnection();
    void RemoveConnection(TcpConnection *ptcpconnection);
    void OnConnectionError();

};


#endif
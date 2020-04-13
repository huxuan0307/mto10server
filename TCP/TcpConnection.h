
//TcpConnection�࣬�ͻ�������

#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include <functional>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Channel.h"
#include "EventLoop.h"
#include "logging.h"
//#include "Socket.h"

class TcpConnection
{
public:
    using Callback =  std::function<void(TcpConnection*)>;
    using MessageCallback =  std::function<void(TcpConnection*, std::string&)>;
    using TaskCallback =  std::function<void()>;
    TcpConnection(EventLoop *loop, int fd, struct sockaddr_in clientaddr);
    ~TcpConnection();

    int fd()
    { return fd_; }
    //void FillBuffout(std::string &s);
    void Send(std::string &s);
    void HandleRead();
    void HandleWrite();
    void HandleError();
    void HandleClose();

    // �����¼��ص�
    void SetMessaeCallback(MessageCallback cb)
    {
        messagecallback_ = cb;
    }
    // ���÷�����ɻص�
    void SetSendCompleteCallback(Callback cb)
    {
        sendcompletecallback_ = cb;
    }
    // ���ùر����ӻص�
    void SetCloseCallback(Callback cb)
    {
        closecallback_ = cb;
    }
    // ���ó���ص�
    void SetErrorCallback(Callback cb)
    {
        errorcallback_ = cb;
    }
    // ��������������
    void SetConnectionCleanUp(TaskCallback cb)
    {
        connectioncleanup_ = cb;
    }
private:
    /* data */
    EventLoop *loop_;
    Channel *pchannel_;
    int fd_;
    struct sockaddr_in clientaddr_;
    bool halfclose_; //��رձ�־λ
    bool disconnected_; //�ѹرձ�־λ

    //��д����
    std::string bufferin_;
    std::string bufferout_;


    MessageCallback messagecallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;

    TaskCallback connectioncleanup_;
};


#endif
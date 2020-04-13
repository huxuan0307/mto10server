
//TcpConnection类，客户端连接

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

    // 设置事件回调
    void SetMessaeCallback(MessageCallback cb)
    {
        messagecallback_ = cb;
    }
    // 设置发送完成回调
    void SetSendCompleteCallback(Callback cb)
    {
        sendcompletecallback_ = cb;
    }
    // 设置关闭连接回调
    void SetCloseCallback(Callback cb)
    {
        closecallback_ = cb;
    }
    // 设置出错回调
    void SetErrorCallback(Callback cb)
    {
        errorcallback_ = cb;
    }
    // 设置连接清理函数
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
    bool halfclose_; //半关闭标志位
    bool disconnected_; //已关闭标志位

    //读写缓冲
    std::string bufferin_;
    std::string bufferout_;


    MessageCallback messagecallback_;
    Callback sendcompletecallback_;
    Callback closecallback_;
    Callback errorcallback_;

    TaskCallback connectioncleanup_;
};


#endif
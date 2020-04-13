
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TcpServer.h"
#include "logging.h"
void Setnonblocking(int fd);

TcpServer::TcpServer(EventLoop* loop, int port)
    : serversocket_(),
    loop_(loop),
    serverchannel_(),
    conncount_(0)
{
    //serversocket_.SetSocketOption(); 
    serversocket_.SetReuseAddr();   
    serversocket_.BindAddress(port);
    serversocket_.Listen();
    serversocket_.Setnonblocking();

    serverchannel_.SetFd(serversocket_.fd());
    serverchannel_.SetReadHandle(std::bind(&TcpServer::OnNewConnection, this));
    serverchannel_.SetErrorHandle(std::bind(&TcpServer::OnConnectionError, this));
    std::cout<<"TcpServer has started\n";
}

TcpServer::~TcpServer()
{
}

void TcpServer::Start()
{
    serverchannel_.SetEvents(EPOLLIN | EPOLLET);
    loop_->AddChannelToPoller(&serverchannel_);
}

//��TCP���Ӵ���,���Ĺ��ܣ�ҵ����ע�ᣬ����ַ�
void TcpServer::OnNewConnection()
{
    //ѭ������accept����ȡ���еĽ��������ӵĿͻ���fd
    struct sockaddr_in clientaddr;
    int clientfd;
    while( (clientfd = serversocket_.Accept(clientaddr)) > 0) 
    {
        //std::cout << "New client from IP:" << inet_ntoa(clientaddr.sin_addr) 
        //    << ":" << ntohs(clientaddr.sin_port) << std::endl;
        
        if(++conncount_ >= MAXCONNECTION)
        {
            close(clientfd);
            continue;
        }
        Setnonblocking(clientfd);

        //�������ӣ�ע��ҵ����
        TcpConnection *ptcpconnection = new TcpConnection(loop_, clientfd, clientaddr);
        ptcpconnection->SetMessaeCallback(messagecallback_);
        ptcpconnection->SetSendCompleteCallback(sendcompletecallback_);
        ptcpconnection->SetCloseCallback(closecallback_);
        ptcpconnection->SetErrorCallback(errorcallback_);
        ptcpconnection->SetConnectionCleanUp(std::bind(&TcpServer::RemoveConnection, this, ptcpconnection));
        tcpconnlist_[clientfd] = ptcpconnection;

        newconnectioncallback_(ptcpconnection);
    }
}

//��������
void TcpServer::RemoveConnection(TcpConnection *ptcpconnection)
{
     --conncount_;
    //std::cout << "clean up connection, conncount is" << conncount_ << std::endl;   
    tcpconnlist_.erase(ptcpconnection->fd());
    delete ptcpconnection;
}

void TcpServer::OnConnectionError()
{    
    std::cout << "UNKNOWN EVENT" << std::endl;
    serversocket_.Close();
}

void Setnonblocking(int fd)
{
    int opts = fcntl(fd, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(fd,GETFL)");
        exit(1);
    }
    if (fcntl(fd, F_SETFL, opts | O_NONBLOCK) < 0)
    {
        perror("fcntl(fd,SETFL,opts)");
        exit(1);
    }
}

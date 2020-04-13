#include <DatabaseServer.h>

void DatabaseServer::HandleNewConnection(TcpConnection* ptcpconn)
{
    
}
void DatabaseServer::HandleMessage(TcpConnection* ptcpconn, string& s)
{

}
void DatabaseServer::HandleSendComplete(TcpConnection *ptcpconn)
{

}
void DatabaseServer::HandleClose(TcpConnection* ptcpconn)
{

}
void DatabaseServer::HandleError(TcpConnection* ptcpconn)
{

}

DatabaseServer::DatabaseServer(uint16_t port){
    loop_.reset(new EventLoop{});
    tcpserver_.reset(new TcpServer(loop_.get(), port_));
    tcpserver_->SetNewConnCallback(bind(
        &DatabaseServer::HandleNewConnection, this, placeholders::_1));

    tcpserver_->SetMessageCallback(bind(
        &DatabaseServer::HandleMessage, this, placeholders::_1, placeholders::_2
    ));

    tcpserver_->SetCloseCallback(bind(
        &DatabaseServer::HandleClose, this, placeholders::_1
    ));

    tcpserver_->SetErrorCallback(bind(
        &DatabaseServer::HandleError, this, placeholders::_1
    ));

    tcpserver_->SetSendCompleteCallback(bind(
        &DatabaseServer::HandleSendComplete, this, placeholders::_1
    ));


}
void DatabaseServer::Start(){
    tcpserver_->Start();
    loop_->loop();
}
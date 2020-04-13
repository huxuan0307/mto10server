#include <mto10server.h>

static string CreateSecurityString(){
    static default_random_engine dre(time(nullptr));
    static const char alpha[17]{"0123456789abcdef"};
    string res(40, '\0');
    for(int i=0;i<40;++i){
        res[i] = alpha[dre()&0xf];
    }
    return res;
    // return "22c8b1118c20bc358a2896286a50e0e6a58b10fe";
}

Mto10Server::Mto10Server(uint16_t port):
    port_{port}, db_(dbname, dbhost, dbuser, dbpw, 4000)
{
    // Debug()<<"Mto10Server begin";
    db_.ConnectDB();
    loop_.reset(new EventLoop{});
    tcpserver_.reset(new TcpServer(loop_.get(), port_));
    tcpserver_->SetNewConnCallback(bind(
        &Mto10Server::HandleNewConnection, this, placeholders::_1));

    tcpserver_->SetMessageCallback(bind(
        &Mto10Server::HandleMessage, this, placeholders::_1, placeholders::_2
    ));

    tcpserver_->SetCloseCallback(bind(
        &Mto10Server::HandleClose, this, placeholders::_1
    ));

    tcpserver_->SetErrorCallback(bind(
        &Mto10Server::HandleError, this, placeholders::_1
    ));

    tcpserver_->SetSendCompleteCallback(bind(
        &Mto10Server::HandleSendComplete, this, placeholders::_1
    ));
    // Debug()<<"Mto10Server end";
}

void Mto10Server::Start()
{
    tcpserver_->Start();
    loop_->loop();
}

void Mto10Server::HandleNewConnection(TcpConnection* ptcpconn)
{
    shared_ptr<Mto10Session> mto10session{new Mto10Session(this)};
    this->mto10sessionlist_[ptcpconn] = mto10session;
    mto10session->securityString_ = CreateSecurityString();
    Debug() << "SecurityString: " << mto10session->securityString_;
    string msg = mto10MessageToString(*mto10session, "SecurityString", mto10session->securityString_);
    ptcpconn->Send(msg);
}

void Mto10Server::HandleMessage(TcpConnection* ptcpconn, string& s)
{

    // if(time(nullptr))
    // Info()<<"HandleMessage begin";
    shared_ptr<Mto10Session> mto10session = mto10sessionlist_[ptcpconn];
    // 接收请求
    mto10session->HandleRequest(s);
    if(mto10session->login_suc_){
        // 处理请求，得到回复
        mto10session->Mto10Process();
        // 将回复加入发送缓冲区
        string msg;
        mto10session->AddToBuf(msg);
        // 发送回复
        ptcpconn->Send(msg);
        // 如果判定应该关闭连接，那么就直接关闭
        if(mto10session->shouldclose_){
            if(mto10session->mode_[0] == 's'){
                db_.insert_base_data(mto10session->stuno_, mto10session->mapid_, mto10session->row_,
                                    mto10session->col_, mto10session->score_, mto10session->step_,
                                    mto10session->maxvalue_, mto10session->getCreatetime(), mto10session->getCompletetime(),
                                    mto10session->gameovertype_);
            }
            else if (mto10session->mode_[0] == 'g'){
                db_.insert_competition_data(mto10session->stuno_, mto10session->mapid_, mto10session->row_,
                                    mto10session->col_, mto10session->score_, mto10session->step_,
                                    mto10session->maxvalue_, mto10session->getCreatetime(), mto10session->getCompletetime(),
                                    mto10session->gameovertype_);
            }
            ptcpconn->HandleClose();
        }
    }
    else{
        ptcpconn->HandleClose();
    }
}

void Mto10Server::HandleSendComplete(TcpConnection *ptcpconn)
{
    gettimeofday(&mto10sessionlist_[ptcpconn]->lastresponsetime_, nullptr);
}

void Mto10Server::HandleClose(TcpConnection* ptcpconn)
{
    // 使用了shared_ptr，不再需要手动释放Mto10Session对象
    // auto mto10session = mto10sessionlist_[ptcpconn];
    mto10sessionlist_.erase(ptcpconn);
}
void Mto10Server::HandleError(TcpConnection* ptcpconn)
{
    mto10sessionlist_.erase(ptcpconn);
}
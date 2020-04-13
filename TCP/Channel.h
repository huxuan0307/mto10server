//Channel�࣬fd���¼��ķ�װ

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <functional>

class Channel
{

public:
    typedef std::function<void()> Callback;

    Channel();
    ~Channel();

    void SetFd(int fd) 
    {
        fd_ = fd; 
    }
    int GetFd() 
    { 
        return fd_; 
    }
    
    void SetEvents(uint32_t events)
    { 
        events_ = events; 
    }
    uint32_t GetEvents()
    { 
        return events_; 
    }

    void HandleEvent();
    void SetReadHandle(Callback cb)
    {
        readhandler_ = cb;
    }
    void SetWriteHandle(Callback cb)
    {
        writehandler_ = cb; 
    }    
    void SetErrorHandle(Callback cb)
    { 
        errorhandler_ = cb;
    }
    void SetCloseHandle(Callback cb)
    {
        closehandler_ = cb;
    }

private:
    /* data */
    int fd_;
    uint32_t events_;//epoll events 

    //�¼�����ʱִ�еĺ�������tcpconn��ע��
    Callback readhandler_;
    Callback writehandler_;
    Callback errorhandler_;
    Callback closehandler_;
};



#endif
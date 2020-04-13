
//    typedef union epoll_data {
//        void    *ptr;
//        int      fd;
//        uint32_t u32;
//        uint64_t u64;
//    } epoll_data_t;

//    struct epoll_event {
//        uint32_t     events;    /* Epoll events */
//        epoll_data_t data;      /* User data variable */
//    };

#include <iostream>
#include <stdio.h> //perror
#include <stdlib.h> //exit
#include <unistd.h> //close
#include <errno.h>
#include "Poller.h"

#define EVENTNUM 4096 //��󴥷��¼�����
#define TIMEOUT 1000 //epoll_wait ��ʱʱ������

Poller::Poller(/* args */)
    : pollfd_(-1),
    eventlist_(EVENTNUM),
    channelmap_()
{
    pollfd_ = epoll_create1(0);
    if(pollfd_ == -1)
    {
        perror("epoll_create1");
        exit(1);
    }
    std::cout << "epoll_create" << pollfd_ << std::endl;
}

Poller::~Poller()
{
    close(pollfd_);
}

//�ȴ�I/O�¼�
void Poller::poll(ChannelList &activechannellist)
{
    int timeout = TIMEOUT;
    // std::cout << "epoll_wait..." << std::endl;
    int nfds = epoll_wait(pollfd_, &*eventlist_.begin(), (int)eventlist_.capacity(), timeout);
    //int nfds = epoll_wait(pollfd_, &*eventlist_.begin(), (int)channelmap_.size()*0.7+1, timeout);
    if(nfds == -1)
    {
        printf("error code is:%d", errno);
        perror("epoll wait error");
        //exit(1);
    }
    //printf("event num:%d\n", nfds);
    //std::cout << "event num:" << nfds << "\n";// << std::endl;
    for(int i = 0; i < nfds; ++i)
    {
        int events = eventlist_[i].events;
        //int fd = eventlist_[i].data.fd;
        Channel *pchannel = (Channel*)eventlist_[i].data.ptr;
        int fd = pchannel->GetFd();
        if(channelmap_.find(fd) != channelmap_.end())
        {
            pchannel->SetEvents(events);
            activechannellist.push_back(pchannel);
        }
        else
        {
            std::cout << "not find channel!" << std::endl;
        }
    }
    if(nfds == (int)eventlist_.capacity())
    {
        std::cout << "resize:" << nfds << std::endl;
        eventlist_.resize(nfds * 2);
    }
    //eventlist_.clear();

}

//����¼�
void Poller::AddChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    struct epoll_event ev;
    ev.events = pchannel->GetEvents();
    //data��������
    //ev.data.fd = fd;
    ev.data.ptr = pchannel;
    channelmap_[fd] = pchannel;

    if(epoll_ctl(pollfd_, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        perror("epoll add error");
        exit(1);
    }
    //std::cout << "addchannel!" << std::endl;
}

//ɾ���¼�
void Poller::RemoveChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    struct epoll_event ev;
    ev.events = pchannel->GetEvents();
    ///ev.data.fd = fd;
    ev.data.ptr = pchannel;
    channelmap_.erase(fd);

    if(epoll_ctl(pollfd_, EPOLL_CTL_DEL, fd, &ev) == -1)
    {
        perror("epoll del error");
        exit(1);
    }
    //std::cout << "removechannel!" << std::endl;
}

//�����¼�
void Poller::UpdateChannel(Channel *pchannel)
{
    int fd = pchannel->GetFd();
    struct epoll_event ev;
    ev.events = pchannel->GetEvents();
    //ev.data.fd = fd;
    ev.data.ptr = pchannel;

    if(epoll_ctl(pollfd_, EPOLL_CTL_MOD, fd, &ev) == -1)
    {
        perror("epoll update error");
        exit(1);
    }
    //std::cout << "updatechannel!" << std::endl;
}


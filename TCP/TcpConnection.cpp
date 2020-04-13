
//TcpConnection�࣬��ʾ�ͻ�������

#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include "TcpConnection.h"
#define BUFSIZE 4096

int recvn(int fd, std::string &bufferin);
int sendn(int fd, std::string &bufferout);

TcpConnection::TcpConnection(EventLoop *loop, int fd, struct sockaddr_in clientaddr)
    : loop_(loop),
	fd_(fd),
    clientaddr_(clientaddr),
	halfclose_(false),
	disconnected_(false)
{
	//�����¼��࣬��ע���¼�ִ�к���
    pchannel_ = new Channel();
    pchannel_->SetFd(fd_);
    pchannel_->SetEvents(EPOLLIN | EPOLLET);
    pchannel_->SetReadHandle(std::bind(&TcpConnection::HandleRead, this));
    pchannel_->SetWriteHandle(std::bind(&TcpConnection::HandleWrite, this));
    pchannel_->SetCloseHandle(std::bind(&TcpConnection::HandleClose, this));
    pchannel_->SetErrorHandle(std::bind(&TcpConnection::HandleError, this));    

	//���߳��£�����loop���������
    loop_->AddChannelToPoller(pchannel_);
}

TcpConnection::~TcpConnection()
{
	//���߳��£�����loop��������У����ã���Ϊ�Ѿ��ڵ�ǰloop�߳�
	//�Ƴ��¼�
	loop_->RemoveChannelToPoller(pchannel_);
	delete pchannel_;
	close(fd_);
}

void TcpConnection::Send(std::string &s)
{
    bufferout_ += s;//copyһ��
    int result = sendn(fd_, bufferout_);
    if(result > 0)
    {
		uint32_t events = pchannel_->GetEvents();
        if(bufferout_.size() > 0)
        {
			//���������ˣ�����û���꣬������EPOLLOUT�¼�����			
			pchannel_->SetEvents(events | EPOLLOUT);
			loop_->UpdateChannelToPoller(pchannel_);
        }
		else
		{
			//�����ѷ���
			Debug().nospace() << "Send\n" 
					<< bitstreamToString(s);
			pchannel_->SetEvents(events & (~EPOLLOUT));
			sendcompletecallback_(this);
			//20190217
			if(halfclose_)
				HandleClose();
		}
    }
    else if(result < 0)
    {        
		HandleError();
    }
	else
	{
		HandleClose();
	}     
}

void TcpConnection::HandleRead()
{
    //�������ݣ�д�뻺����
    int result = recvn(fd_, bufferin_);
	Debug().nospace() << "Recv\n"
					  << bitstreamToString(bufferin_);
	//ҵ��ص�,�������ù����̳߳ش���Ͷ������
    if(result > 0)
    {
        messagecallback_(this, bufferin_);
    }
    else if(result == 0)
    {
        HandleClose();
    }
    else
    {
        HandleError();
    }
}

void TcpConnection::HandleWrite()
{
    int result = sendn(fd_, bufferout_);
    if(result > 0)
    {
		uint32_t events = pchannel_->GetEvents();
        if(bufferout_.size() > 0)
        {
			//���������ˣ�����û���꣬������EPOLLOUT�¼�����			
			pchannel_->SetEvents(events | EPOLLOUT);
			loop_->UpdateChannelToPoller(pchannel_);
        }
		else
		{
			//�����ѷ���
			pchannel_->SetEvents(events & (~EPOLLOUT));
			sendcompletecallback_(this);
			//������ϣ�����ǰ�ر�״̬�������close��
			if(halfclose_)
				HandleClose();
		}
    }
    else if(result < 0)
    {        
		HandleError();
    }
	else
	{
		HandleClose();
	}     
}

void TcpConnection::HandleError()
{
	if(disconnected_) {return;}
	errorcallback_(this);
	//loop_->RemoveChannelToPoller(pchannel_);
	//���ӱ��Ϊ����
	//task���
	loop_->AddTask(connectioncleanup_);
	disconnected_ = true;
}

void TcpConnection::HandleClose()
{
	//�Ƴ��¼�
	//loop_->RemoveChannelToPoller(pchannel_);
	//���ӱ��Ϊ����
	//task���
	loop_->AddTask(connectioncleanup_);
	closecallback_(this);

	// if(disconnected_) {return;}
	// if(bufferout_.size() > 0 || bufferin_.size() > 0)
	// {
	// 	//����������ݴ����ͣ����ȷ���,���ð�رձ�־λ
	// 	halfclose_ = true;
	// 	if(bufferin_.size() > 0)
	// 	{
	// 		messagecallback_(this, bufferin_);
	// 	}
	// }
	// else
	// {
	// 	loop_->AddTask(connectioncleanup_);
	// 	closecallback_(this);
	// 	disconnected_ = true;
	// }

}

int recvn(int fd, std::string &bufferin)
{
    int nbyte = 0;
    int readsum = 0;
    char buffer[BUFSIZE];
    for(;;)
    {
        //nbyte = recv(fd, buffer, BUFSIZE, 0);
		nbyte = read(fd, buffer, BUFSIZE);
		
    	if (nbyte > 0)
		{
            bufferin.append(buffer, nbyte);//Ч�ʽϵͣ�2�ο���
            readsum += nbyte;
			if(nbyte < BUFSIZE)
				return readsum;//���Ż�����Сһ�ζ����ã���Ϊһ�ε��ú�ʱ10+us
			else
				continue;
		}
		else if (nbyte < 0)//�쳣
		{
			if (errno == EAGAIN)//ϵͳ������δ�����ݣ�����������
			{
				//std::cout << "EAGAIN,ϵͳ������δ�����ݣ�����������" << std::endl;
				return readsum;
			}
			else if (errno == EINTR)
			{
				std::cout << "errno == EINTR" << std::endl;
				continue;
			}
			else
			{
				//������RST
				perror("recv error");
				std::cout << "recv error" << std::endl;
				return -1;
			}
		}
		else//����0���ͻ��˹ر�socket��FIN
		{
			std::cout << "client close the Socket" << std::endl;
			return 0;
		}
    }
}

int sendn(int fd, std::string &bufferout)
{
	ssize_t nbyte = 0;
    int sendsum = 0;
	char buffer[BUFSIZE+1];
	size_t length = 0;
	//length = bufferout.copy(buffer, BUFSIZE, 0);
	//buffer[length] = '\0';	
	// if(bufferout.size() >= BUFSIZE)
	// {
	// 	length =  BUFSIZE;
	// }
	// else
	// {
	// 	length =  bufferout.size();
	// }
	//�޿����Ż�
	length = bufferout.size();
	if(length >= BUFSIZE)
	{
		length = BUFSIZE;
	}
	for (;;)
	{
		//nbyte = send(fd, buffer, length, 0);
		//nbyte = send(fd, bufferout.c_str(), length, 0);
		nbyte = write(fd, bufferout.c_str(), length);
		if (nbyte > 0)
		{
            sendsum += nbyte;
			bufferout.erase(0, nbyte);
			//length = bufferout.copy(buffer, BUFSIZE, 0);
			//buffer[length] = '\0';
			length = bufferout.size();
			if(length >= BUFSIZE)
			{
				length = BUFSIZE;
			}
			if (length == 0 )
			{
				return sendsum;
			}
		}
		else if (nbyte < 0)//�쳣
		{
			if (errno == EAGAIN)//ϵͳ��������������������
			{
				std::cout << "write errno == EAGAIN,not finish!" << std::endl;
				return sendsum;
			}
			else if (errno == EINTR)
			{
				std::cout << "write errno == EINTR" << std::endl;
				continue;
			}
			else if (errno == EPIPE)
			{
				//�ͻ����Ѿ�close��������RST������wirte�ᱨEPIPE������0����ʾclose
				perror("write error");
				std::cout << "write errno == client send RST" << std::endl;
				return -1;
			}
			else
			{
				perror("write error");//Connection reset by peer
				std::cout << "write error, unknow error" << std::endl;
				return -1;
			}
		}
		else//����0
		{
			//Ӧ�ò��᷵��0
			//std::cout << "client close the Socket!" << std::endl;
			return 0;
		}
	}
}
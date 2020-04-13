
//Channel�࣬��ʾÿһ���ͻ������ӵ�ͨ��
//
// EPOLLIN ����ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ���
// EPOLLOUT����ʾ��Ӧ���ļ�����������д��
// EPOLLPRI����ʾ��Ӧ���ļ��������н��������ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�������
// EPOLLERR����ʾ��Ӧ���ļ���������������
// EPOLLHUP����ʾ��Ӧ���ļ����������Ҷϣ�
// EPOLLET�� ��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵�ġ�
// EPOLLONESHOT��ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL������

#include <iostream>
#include <sys/epoll.h>
#include "Channel.h"
#include "logging.h"
Channel::Channel ()
{ }

Channel::~Channel ()
{

}

void Channel::HandleEvent ()
{
	if (events_ & EPOLLRDHUP)//�Է��쳣�ر��¼�
	{
		std::cout << "Event EPOLLRDHUP" << std::endl;
		closehandler_ ();
	}
	else if (events_ & (EPOLLIN | EPOLLPRI))//���¼����Զ������ݻ��������ر�
	{
		// std::cout << "Event EPOLLIN" << std::endl;
		readhandler_ ();
	}
	else if (events_ & EPOLLOUT)//д�¼�
	{
		std::cout << "Event EPOLLOUT" << std::endl;
		writehandler_ ();
	}
	else {
		std::cout << "Event error" << std::endl;
		errorhandler_ ();//���Ӵ���
	}
}

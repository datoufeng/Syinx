#include "SyZinx.h"
#include "IChannel.h"
#include "IRole.h"
#include "IProtocol.h"
//��ʼ���Լ���ָ��
SyZinxKernel* SyZinxKernel::mSyZinx = NULL;

struct event_base* SyZinxKernel::SyZinxBase = nullptr;

struct evconnlistener* SyZinxKernel::SyZinxListen = nullptr;

MemRecycled Recycler;

SyZinxKernel::SyZinxKernel()
{
	
}

SyZinxKernel::~SyZinxKernel()
{
}



/*
evconnlistener_cb cb : �ص��������ڷ���һ���ͻ������ӵ��ļ�������
void(*evconnlistener_cb)(struct evconnlistener* listener,
	evutil_socket_t fd,
	struct sockaddr* sock, int socklen, void* arg);
�����ص�����
A callback that we invoke when a listener has a new connection.

   @param listener The evconnlistener
   @param fd The new file descriptor
   @param addr The source address of the connection
   @param socklen The length of addr
   @param user_arg the pointer passed to evconnlistener_new()
�����µĿͻ������ӵ�ʱ��ͻ���ô˻ص�����
fdΪ�ͻ��˵��ļ�������
sockΪ�ͻ��˵�ԭʼ�ṹ��
socklenΪ����

*/

//event recv send�ص��������ݲ���
struct  SyZinxDeliver;
struct SyZinxBuffer_Event_Deliver
{
	SyZinxDeliver* iSyZinxDeliver;
	int iClient_Fd;
};

//listen�ص���������Ĳ���
struct  SyZinxDeliver
{

	//base���
	struct event_base* iSyZinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* iSyZinxListen;

	//���Ŀ�ܵ�ַ
	SyZinxKernel* iSyZinx;
};

//���ص�
void SyZinxBuffer_RecvData_Cb(struct bufferevent* buffer, void* arg)
{
	//���пɶ��¼�����ʱ������ô˺������뻷��������һϵ������
	SyZinxBuffer_Event_Deliver* tmpEvent_Deliver = (SyZinxBuffer_Event_Deliver*)(arg);
	int mClient_fd = tmpEvent_Deliver->iClient_Fd;
#if 1
	char buf[BUFFSIZE];
	memset(buf, 0, sizeof(buf));
	int iRet = bufferevent_read(buffer, buf, BUFFSIZE);
	if (-1 == iRet)
	{
		return;
	}
	std::string RecvString(buf, iRet);
	//���ݵ�ǰ�Ĵ����Ŀͻ����׽��ֲ�ͬ�����ò�ͬ�Ŀͻ���������
	auto client_buffer = SyZinxKernel::mSyZinx->Ichannel_Map[mClient_fd];
	client_buffer->Ichannel_RecvBuffer(RecvString);
#endif
}

//д�ص�
void SyZinxBuffer_SendData_Cb(struct bufferevent* buffer, void* arg)
{
	//���п�д�¼�����ʱ������ô˺��������������ص����л��ַ��������ú���
	std::cout << "write event is trigger" << std::endl;

	SyZinxBuffer_Event_Deliver* tmpEvent_Deliver = (SyZinxBuffer_Event_Deliver*)(arg);
	int mClient_fd = tmpEvent_Deliver->iClient_Fd;

}




//�¼��ص�
void SyZinxBuffer_Event_Cb(struct bufferevent* buffer, short events, void* arg)
{
	SyZinxBuffer_Event_Deliver* tmpEvent_Deliver = (SyZinxBuffer_Event_Deliver*)(arg);

	//��ܵ�ַ
	SyZinxKernel* mSyZinx = tmpEvent_Deliver->iSyZinxDeliver->iSyZinx;
	int iKey;
	if (events & BEV_EVENT_EOF) // Client�˹ر����� 
	{
		std::cout << "Cliented : "<< tmpEvent_Deliver->iClient_Fd<<" is exit"<< std::endl;
		delete tmpEvent_Deliver;
		iKey = tmpEvent_Deliver->iClient_Fd;
		SyZinxKernel::SyZinxKernel_Client_Close(iKey);
	}
	else if (events & BEV_EVENT_ERROR) // ���ӳ��� 
	{
		std::cout << "Got an error on the connection:" << std::endl;
		
			
	}
	else if (events & BEV_EVENT_WRITING)//д��ʱ����������
	{
		std::cout << "send to client is err" << std::endl;
	}
	// �������������eventû�д����Ǿ͹ر����bufferevent 

#if 0 //switch�����events�ж��������������
	switch (events)
	{
	case BEV_EVENT_READING://��ȡʱ��������
		std::cout << "�� �� ʱ �� �� �� ��" << std::endl;
		delete tmpEvent_Deliver;
		iKey = tmpEvent_Deliver->iClient_Fd;
		SyZinxKernel::SyZinxKernel_Client_Close(iKey);
		break;
	case BEV_EVENT_WRITING://д�����ʱ����ĳ�¼��������������¼��뿴������־	
	{
		//�����ǿͻ����Ѿ��Ͽ�����
		delete tmpEvent_Deliver;
		iKey = tmpEvent_Deliver->iClient_Fd;
		SyZinxKernel::SyZinxKernel_Client_Close(iKey);
		std::cout << "client is exit" << std::endl;
		break;
	}
	case BEV_EVENT_ERROR: //�� �� ʱ �� �� �� �� �� �� �� �� �� �� �� �� �� Ϣ �� �� �� ��
		std::cout << "�� �� ʱ �� �� �� ��" << std::endl;
		delete tmpEvent_Deliver;
		iKey = tmpEvent_Deliver->iClient_Fd;
		SyZinxKernel::SyZinxKernel_Client_Close(iKey);
		std::cout << "client is exit" << std::endl;
		break;
	case BEV_EVENT_TIMEOUT://������ʱ��
		break;
	case BEV_EVENT_EOF://�ͻ����˴�
		std::cout << "client is exit" << std::endl;
		break;
	case BEV_EVENT_CONNECTED://��������ӹ����Ѿ���ɡ�
		break;
	default:
		break;
	}
#endif
	return;
}


void SyZinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg)
{
	SyZinxDeliver* poSyZinxDeliver = (SyZinxDeliver*)arg;
	if (NULL == poSyZinxDeliver)
	{
		return;
	}

	struct event_base* poSyZinxBase = poSyZinxDeliver->iSyZinxBase;
	
	//��ܵ�ַ
	SyZinxKernel* mSyZinx = poSyZinxDeliver->iSyZinx;

	//test
	std::cout << "new client is connect  " <<"Client:"<<fd<< std::endl;

	//Ϊ�ͻ��˵�һ��bufferevent�ṹ��
	struct bufferevent* Client_buffer = NULL;

	Client_buffer = bufferevent_socket_new(poSyZinxBase, fd, BEV_OPT_CLOSE_ON_FREE);
	//����һϵ�й�����
	SyZinxTcpFactory* NewClientFac = new SyZinxTcpFactory(fd, Client_buffer);

	//���������ݲ��������Լ��׽��ִ��ݵ��ص�����
	SyZinxBuffer_Event_Deliver* poEvent_Deliver = new SyZinxBuffer_Event_Deliver;
	poEvent_Deliver->iSyZinxDeliver = poSyZinxDeliver;
	poEvent_Deliver->iClient_Fd = fd;

	//���ÿͻ��˵Ļص�����
	//���ݿͻ��˶�ȡ����д��ʱ����
	bufferevent_setcb(Client_buffer, SyZinxBuffer_RecvData_Cb, SyZinxBuffer_SendData_Cb, SyZinxBuffer_Event_Cb, (void*)poEvent_Deliver);

	//��bufferevent�Ͽ�ܵ�map����
	mSyZinx->iBufferevent_Map.insert(std::make_pair(fd, Client_buffer));

	//���ÿͻ���eventsΪ���¼��Լ�ѭ������¼�
	bufferevent_enable(Client_buffer, EV_READ | EV_PERSIST);

	
#if 0
	auto client_buffer = SyZinxKernel::mSyZinx->IRole_Map[fd];
	client_buffer->Init_Player();
#endif
}
int SyZinxKernel::SyZinxKernel_Init(const short _inPort)
{
	if (_inPort <= 0 || _inPort > 65250)
	{
		return -1;
	}
	mSyZinx = new SyZinxKernel();

	int iRet = -1;

	//��ʼ���ṹ��
	struct sockaddr_in _Serveraddr;
	memset(&_Serveraddr, 0, sizeof _Serveraddr);
	_Serveraddr.sin_family = AF_INET;
	_Serveraddr.sin_port = htons(_inPort);

	//�������
	SyZinxBase = event_base_new();
	if (NULL == SyZinxBase)
	{
		return -1;
	}

	//���ô��ݲ���
	SyZinxDeliver* poSyZinxDeliver = new SyZinxDeliver;
	poSyZinxDeliver->iSyZinxBase = SyZinxBase;
	poSyZinxDeliver->iSyZinxListen = SyZinxListen;
	poSyZinxDeliver->iSyZinx = mSyZinx;

	//���ü���
	SyZinxListen = evconnlistener_new_bind(SyZinxBase, SyZinxKernel_Listen_CB, (void*)poSyZinxDeliver,
	LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE, 10, (const sockaddr*)& _Serveraddr, sizeof(_Serveraddr));
	if (SyZinxListen == NULL)
	{
		return -1;
	}
	return 1;
}

void SyZinxKernel::SyZinxKernel_Run()
{
	
	return SyZinxKernel::mSyZinx->Run();
}

void SyZinxKernel::Run()
{
	//ѭ�������¼�
	event_base_dispatch(SyZinxKernel::SyZinxBase);
}

void SyZinxKernel::SyZinxKernel_Close()
{
	//�ر�
	evconnlistener_free(SyZinxKernel::SyZinxListen);
	event_base_free(SyZinxKernel::SyZinxBase);

	

}
bufferevent* SyZinxKernel::SyZinxKernel_GetBuffer(int _iclientfd)
{
	return mSyZinx->iBufferevent_Map[_iclientfd];
}

void SyZinxKernel::SyZinxKernel_Client_Close(const int mClient_fd)
{
	struct bufferevent* tmpbuffer = SyZinxKernel::mSyZinx->iBufferevent_Map[mClient_fd];
	Ichannel* tmpIchannel = SyZinxKernel::mSyZinx->Ichannel_Map[mClient_fd];
	Iprotocol* tmpIprotocol = SyZinxKernel::mSyZinx->IcProtocol_Map[mClient_fd];
	Irole* tmpIrole = SyZinxKernel::mSyZinx->IRole_Map[mClient_fd];

	SyZinxKernel::mSyZinx->iBufferevent_Map.erase(mClient_fd);
	SyZinxKernel::mSyZinx->Ichannel_Map.erase(mClient_fd);
	SyZinxKernel::mSyZinx->IcProtocol_Map.erase(mClient_fd);
	SyZinxKernel::mSyZinx->IRole_Map.erase(mClient_fd);

	//�ͷ�buffer
	bufferevent_free(tmpbuffer);
	delete tmpIprotocol->mClient_ByteString;
	delete tmpIchannel;
	delete tmpIprotocol;
	delete tmpIrole;
	std::cout << "free success" << std::endl;

}
void SyZinxKernel::SyZinxKernel_SaveDatabase(SyZinxDatabase* _iDatabase)
{
	if (_iDatabase == NULL)
	{
		return;
	}
	mSyZinx->Database = _iDatabase;
}
/*no*/

MemRecycled::~MemRecycled()
{
	if (SyZinxKernel::mSyZinx != nullptr)
	{
		delete SyZinxKernel::mSyZinx;
		SyZinxKernel::mSyZinx = nullptr;
	}
	std::cout << "~MemRecycled()" << std::endl;
}


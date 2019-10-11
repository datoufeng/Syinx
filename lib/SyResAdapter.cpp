#include "Syinx.h"
#include "SyAdapter.h"
#include "SyResAdapter.h"
#include "SyPthreadAdapter.h"
#include "SyTaskAdapter.h"
#include <vector>
#include <memory>
#include <random>
#include <iterator>
//����һ���������(���ȷֲ�)
static std::default_random_engine gRandomEngine(time(nullptr));

//���¼��ص�
void SyinxBuffer_RecvData_Cb(struct bufferevent* buffer, void* arg);
//д�¼��ص�
void SyinxBuffer_SendData_Cb(struct bufferevent* buffer, void* arg);
//�����¼��ص�
void SyinxBuffer_Event_Cb(struct bufferevent* buffer, short events, void* arg);
SyinxAdapterResource::SyinxAdapterResource()
{

}

SyinxAdapterResource::~SyinxAdapterResource()
{
}
//���¼��ص�
void SyinxBuffer_RecvData_Cb(struct bufferevent* buffer, void* arg)
{
	auto mIC = (IChannel*)arg;
	if (buffer != mIC->mICMsg->buffer)
	{
		mIC->mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, -1, "buffer != mIC->mICMsg->buffer is failed");
		std::cout << "buffer != mIC->mICMsg->buffer is failed";
		return;
	}
	char buf[BUFFSIZE] = { 0 };
	int iRet = bufferevent_read(buffer, buf, BUFFSIZE);
	if (iRet <= 0)
	{
		mIC->mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, -1, "bufferevent_read is err");
		std::cout << iRet;
		return;
	}
	std::string tmp(buf, iRet - 1);
	mIC->StrByte->_InStr = tmp;
	mIC->StrByte->_InSize = iRet - 1;
	
	mIC->IChannelTaskProcessing();

}
//д�¼��ص�
void SyinxBuffer_SendData_Cb(struct bufferevent* buffer, void* arg)
{

}
//�¼��ص�
void SyinxBuffer_Event_Cb(struct bufferevent* buffer, short events, void* arg)
{
	auto mIC = (IChannel*)arg;
	if (events & BEV_EVENT_EOF) // Client�˹ر����� 
	{
		mIC->mICnSaveRes->deleteClient(mIC->mICMsg->Socket, mIC->mICMsg->Where);
	}
	else if (events & BEV_EVENT_ERROR) // ���ӳ��� 
	{

	}
	else if (events & BEV_EVENT_WRITING)//д��ʱ����������
	{

	}

}
//ʱ�䴦��ص�
void SyinxTimer_Event_Cb(struct bufferevent* buffer, void* arg)
{
	
}

int SyinxAdapterResource::SyinxAdapterResource_Init(int _PthNum)
{
	
	if (_PthNum == 0)
	{
	
	}
	this->mSyChannelVec.reserve(_PthNum);
	this->mSyBaseVec.reserve(_PthNum);

	//
	//��ʼ����Դ,��ʼ��map
	for (size_t i = 0; i < _PthNum; i++)
	{
		this->mSyChannelVec.push_back(new std::multimap<SOCKETS, IChannel*>);
		this->mSyBaseVec.push_back(event_base_new());

	}
	this->PthNum = _PthNum;
	//����ļ�������
	this->SyinxAdapterResource_Addtimefd();

	
	return 0;
}
//�������ӵĿͻ��˽��з���
int SyinxAdapterResource::SyinxAdapterResource_AllotClient(SOCKETS _FD)
{
	int Client_fd = _FD;
	if (Client_fd <= 0)
	{
		
		return -1;
	}
	//�������
	if (this->RandAllotFlags == true)
	{
		
	}
	else if (this->RandAllotFlags == false)
	{
	//�̶�����
		this->SocketFd_Add(_FD, 0, 1);
	}
	return 0;
}

int SyinxAdapterResource::SocketFd_Add(SOCKETS _FD, int where, int events)
{
	if (_FD <= 0 | where > PthNum | where < 0 )
	{
		
		return -1;
	}
	struct bufferevent* buffer = bufferevent_socket_new(this->mSyBaseVec[where], _FD, BEV_OPT_CLOSE_ON_FREE);

	IChannelMsg* icmsg = new IChannelMsg;;  //need free
	icmsg->buffer = buffer;
	icmsg->Socket = _FD;
	icmsg->Where = where;

	//��ʼ����������ͨ����
	IChannel* newClieICh = new IChannel;
	newClieICh->ICannel_Init(icmsg);
	//����Դ�������ĵ�ַ
	newClieICh->mICnSaveRes = this;
	//��map����
	this->mSyChannelVec[where]->insert(std::make_pair(_FD, newClieICh));
	
	newClieICh->IChannelTaskInit();

	//���¹����ڴ�
	this->SyinxAdapterResource_UpdateShm();

	//����buffer�ص�����
	bufferevent_setcb(buffer, SyinxBuffer_RecvData_Cb, SyinxBuffer_SendData_Cb, SyinxBuffer_Event_Cb, (void*)newClieICh);

	//����buffer�¼�
	bufferevent_enable(buffer, SET_SOCKETS_EVENT_RD);
	
	return 0;
}

int SyinxAdapterResource::SocketFd_Del(SOCKETS _FD, int where)
{
	
	auto _Map = this->mSyChannelVec[where];
	
	if (_Map->count(_FD)> 0)
	{
		auto _It = _Map->find(_FD);

		IChannel* _DelICh = _It->second;
		_Map->erase(_FD);
		
		//�ͷ�IChannel �Լ�buffer
		bufferevent_free(_DelICh->mICMsg->buffer);
		_DelICh->IChannel_free();

		//���¹����ڴ�
		this->SyinxAdapterResource_UpdateShm();
		
		delete _DelICh;
		
		return 1;

	}
	else
	{
		return -1;
	}
}

int SyinxAdapterResource::SyinxAdapterResource_Addtimefd()
{
	struct itimerspec setitimerspec;
	//set ����
	setitimerspec.it_interval.tv_sec = 1;
	setitimerspec.it_interval.tv_nsec = 0;

	//set ��һ��
	setitimerspec.it_value.tv_sec = 1;
	setitimerspec.it_value.tv_nsec = 0;
	int tmfd;

	for (auto it : this->mSyBaseVec)
	{
		tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
		if (tmfd <= 0)
		{
			std::cout << "timerfd_create if failed" << std::endl;
			mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, tmfd, "timefd_create is failed");
		}
		int iRet = timerfd_settime(tmfd, 0, &setitimerspec, NULL);
		if (iRet <= 0)
		{
			std::cout << "timerfd_settime if failed" << std::endl;
			mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, iRet, "timefd_create is failed");
		}

		auto buffer = bufferevent_socket_new(it, tmfd, BEV_OPT_CLOSE_ON_FREE);

		bufferevent_setcb(buffer, SyinxTimer_Event_Cb,NULL ,NULL , NULL);

		//����buffer�¼�
		bufferevent_enable(buffer, EV_READ);

	}


	return 0;
}

int SyinxAdapterResource::SyinxAdapterResource_Free()
{
	//�ͷ�map
	if (&this->mSyChannelVec != NULL)
	{
		for (auto _Vec : this->mSyChannelVec)
		{
			delete _Vec;
		}
	}
	return 0;
}

int SyinxAdapterResource::SyinxAdapterResource_UpdateShm()
{
	auto ShmData = this->mSyinx->ShmData;
	auto mShmData = (SyinxKernelShmMsg*)ShmData;
	int _Num = 0;
	for (int i = 0; i < this->PthNum; ++i)
	{
		mShmData->CurrentClientNum[i] = this->mSyChannelVec[i]->size();
		_Num += mShmData->CurrentClientNum[i];
	}
	mShmData->AllClientNum = _Num;

}

int SyinxAdapterResource::getClientNum(int where)
{
	if (where <0 | where >PthNum)
		return 0;
	return this->mSyChannelVec[where]->size();
}

int SyinxAdapterResource::getallClientNum()
{
	int _Num = 0;
	for (auto _Vec : this->mSyChannelVec)
	{
		_Num += _Vec->size();
	}
	return _Num;
}

int SyinxAdapterResource::deleteClient(SOCKETS _FD, int where)
{
	
	return this->SocketFd_Del(_FD, where);
	
}


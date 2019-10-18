#include "Syinx.h"
#include "SyAdapter.h"
#include "SyResAdapter.h"
#include "SyPthreadAdapter.h"
#include "SyTaskAdapter.h"
#include "SyConfig.h"
#include <vector>
#include <memory>
#include <random>
#include <errno.h>
#include <iterator>
//����һ���������(���ȷֲ�)
static std::default_random_engine gRandomEngine(time(nullptr));

SyinxAdapterResource::SyinxAdapterResource()
{

}

SyinxAdapterResource::~SyinxAdapterResource()
{
}



//�������ӵĿͻ��˽��з���
int SyinxAdapterResource::SyinxAdapterResource_AllotClient(bufferevent* buffer, SOCKETS _FD)
{
	return this->SocketFd_Add(buffer, _FD);
}

int SyinxAdapterResource::SocketFd_Add(bufferevent* buffer, SOCKETS _FD)
{
	if (buffer == NULL || _FD < 0)
	{
		return -1;
	}
	

	IChannelMsg* icmsg = new IChannelMsg;;  //need free
	icmsg->buffer = buffer;
	icmsg->Socket = _FD;


	//��ʼ����������ͨ����
	IChannel* newClieICh = new IChannel;
	newClieICh->ICannel_Init(icmsg);

	//����Դ�������ĵ�ַ
	newClieICh->mICnSaveRes = this;
	//��map����
	this->mIChannelMap.insert(std::make_pair(buffer, newClieICh));

	//���¹����ڴ�
	this->SyinxAdapterResource_UpdateShm();


	//����buffer�ص��Լ��¼��ص�
	bufferevent_setcb(buffer, SyinxKernel_Recv_Cb, SyinxKernel_Send_Cb, SyinxKernel_Event_Cb, (void*)newClieICh);

	//����buffer�¼�
	bufferevent_enable(buffer, SET_SOCKETS_EVENT_RD);
	

	return 0;
}

int SyinxAdapterResource::SocketFd_Del(bufferevent* buffer, SOCKETS _FD)
{
	auto _it = this->mIChannelMap.find(buffer);
	_it->second->IChannel_free();
	bufferevent_free(_it->first);
	this->mIChannelMap.erase(_it);

	//���¹����ڴ�
	this->SyinxAdapterResource_UpdateShm();
	char WriteLog[BUFFSIZE] = { 0 };
	sprintf(WriteLog, "Client is Exit, Current number of connections[%ld]", this->mIChannelMap.size());
	SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::EVENT, 0, WriteLog);
}

int SyinxAdapterResource::SyinxAdapterResource_Free()
{
	for (auto _it : this->mIChannelMap)
	{
		this->SocketFd_Del(_it.first, 0);
	}
}

int SyinxAdapterResource::SyinxAdapterResource_UpdateShm()
{
	auto ShmData = this->mSyinx->ShmData;
	auto mShmData = (SyinxKernelShmMsg*)ShmData;

	mShmData->AllClientNum = this->mIChannelMap.size();

}

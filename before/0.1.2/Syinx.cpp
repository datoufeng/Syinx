#include "Syinx.h"
#include "SyPthreadAdapter.h"
#include "SyTaskAdapter.h"
#include "SyResAdapter.h"
#include <iostream>
#include <fstream>
#include <istream>
using namespace std;

SyinxKernel* SyinxKernel::mSyinx = nullptr;


SyinxRecycle recycle;


//listen�ص���������Ĳ���
struct  SyinxDeliver
{

	//base���
	struct event_base* iSyinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* iSyinxListen;

	//���Ŀ�ܵ�ַ
	SyinxKernel* iSyinx;
};

SyinxKernel::SyinxKernel()
{
	
}
SyinxKernel::~SyinxKernel()
{
}

//����пͻ�������
void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg)
{
	SyinxDeliver* poSyinxDeliver = (SyinxDeliver*)arg;
	if (NULL == poSyinxDeliver)
	{
		return;
	}

	struct event_base* poSyinxBase = poSyinxDeliver->iSyinxBase;

	//��ܵ�ַ
	SyinxKernel* mSyinx = poSyinxDeliver->iSyinx;

	//test
	std::cout << "new client is connect  " << "Client:" << fd << std::endl;

	//���ͻ����ϵ�map��Ⱥ
	mSyinx->mSyResource->SyinxAdapterResource_AllotClient(fd);
	
	

}
int SyinxKernel::SyinxKernel_Init(const short _inPort)
{
	if (_inPort <= 0 || _inPort >= 65535)
	{
		return -1;
	}
	SyinxKernel::mSyinx = new SyinxKernel;

	//��ʼ���ṹ��
	struct sockaddr_in _Serveraddr;
	memset(&_Serveraddr, 0, sizeof _Serveraddr);
	_Serveraddr.sin_family = AF_INET;
	_Serveraddr.sin_port = htons(_inPort);
	

	
	//�������
	mSyinx->SyinxBase = event_base_new();
	if (NULL == mSyinx->SyinxBase)
	{
		return -1;
	}
	//���ô��ݲ���
	SyinxDeliver* poSyinxDeliver = new SyinxDeliver;
	poSyinxDeliver->iSyinxBase = mSyinx->SyinxBase;
	poSyinxDeliver->iSyinxListen = mSyinx->SyinxListen;
	poSyinxDeliver->iSyinx = mSyinx;

	
	mSyinx->Server_Sockaddr.family = _Serveraddr.sin_family;
	mSyinx->Server_Sockaddr.Prot = to_string(ntohs(_Serveraddr.sin_port));
	mSyinx->Server_Sockaddr.sin_addr = inet_ntoa(_Serveraddr.sin_addr);

	//��ʼ���߳�������
	SyinxAdapterPth* pth = new SyinxAdapterPth(mSyinx->PthNum);
	pth->SyinxAdapterPth_Init();
	mSyinx->mSyPth = pth;

	//��ʼ������������
	SyinxAdapterMission* pMiss = new IChannel;
	mSyinx->mSyMission = pMiss;

	//��ʼ����Դ������
	SyinxAdapterResource* pRes = new SyinxAdapterResource;
	pRes->SyinxAdapterResource_Init(mSyinx->PthNum);
	mSyinx->mSyResource = pRes;

	//pth->SyinxAdapter_Pth_Add();
	//�߳�����������Դ������
	pth->mPthRes = pRes;
	pRes->mResPth = pth;

	pth->mSyinx = mSyinx;

	//���ü���
	mSyinx->SyinxListen = evconnlistener_new_bind(mSyinx->SyinxBase, SyinxKernel_Listen_CB, (void*)poSyinxDeliver,
		SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING, 10, (const sockaddr*)& _Serveraddr, sizeof(_Serveraddr));
	if (mSyinx->SyinxListen == NULL)
	{
		return -1;
	}
	return 1;
}

void SyinxKernel::SyinxKernel_Run()
{
	try
	{

		int iRet = event_base_dispatch(mSyinx->SyinxBase);
		throw iRet;
	}
	catch (int err)
	{
		mSyinx->mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, err, "event_base_dispatch");
	}
}

void SyinxKernel::SyinxKernel_Close()
{
	//�ر�
	evconnlistener_free(mSyinx->SyinxListen);
	event_base_free(mSyinx->SyinxBase);
}

void SyinxKernel::SyinxKernel_Client_Close(const int mClient_fd)
{
}


SyinxRecycle::SyinxRecycle()
{
}
SyinxRecycle::~SyinxRecycle()
{
	mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, 0, "SyinxRecycle delete all Res!");
	std::cout << "��������������" << std::endl;
	delete SyinxKernel::mSyinx;
	delete  SyinxKernel::mSyinx->mSyPth;
	delete SyinxKernel::mSyinx->mSyMission;
	delete SyinxKernel::mSyinx->mSyResource;
}

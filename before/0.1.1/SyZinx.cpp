#include "SyZinx.h"

#include <iostream>
#include <fstream>
#include <istream>
using namespace std;

SyZinxKernel* SyZinxKernel::mSyZinx = nullptr;


SyZinxRecycle recycle;


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

SyZinxKernel::SyZinxKernel()
{
	
}
SyZinxKernel::~SyZinxKernel()
{
}

//����пͻ�������
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
	std::cout << "new client is connect  " << "Client:" << fd << std::endl;

	//���ͻ����ϵ�map��Ⱥ
	SyZinxKernelMsg * newMsg = new SyZinxKernelMsg;
	
	

}
int SyZinxKernel::SyZinxKernel_Init(const short _inPort)
{
	if (_inPort <= 0 || _inPort >= 65535)
	{
		return -1;
	}
	SyZinxKernel::mSyZinx = new SyZinxKernel;

	//��ʼ���ṹ��
	struct sockaddr_in _Serveraddr;
	memset(&_Serveraddr, 0, sizeof _Serveraddr);
	_Serveraddr.sin_family = AF_INET;
	_Serveraddr.sin_port = htons(_inPort);
	

	
	//�������
	mSyZinx->SyZinxBase = event_base_new();
	if (NULL == mSyZinx->SyZinxBase)
	{
		return -1;
	}
	//���ô��ݲ���
	SyZinxDeliver* poSyZinxDeliver = new SyZinxDeliver;
	poSyZinxDeliver->iSyZinxBase = mSyZinx->SyZinxBase;
	poSyZinxDeliver->iSyZinxListen = mSyZinx->SyZinxListen;
	poSyZinxDeliver->iSyZinx = mSyZinx;

	//���ü���
	mSyZinx->SyZinxListen = evconnlistener_new_bind(mSyZinx->SyZinxBase, SyZinxKernel_Listen_CB, (void*)poSyZinxDeliver,
		LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE, 10, (const sockaddr*)& _Serveraddr, sizeof(_Serveraddr));
	if (mSyZinx->SyZinxListen == NULL)
	{
		return -1;
	}
	mSyZinx->Server_Sockaddr.family = _Serveraddr.sin_family;
	mSyZinx->Server_Sockaddr.Prot = to_string(ntohs(_Serveraddr.sin_port));
	mSyZinx->Server_Sockaddr.sin_addr = inet_ntoa(_Serveraddr.sin_addr);

	//��ʼ���߳�������
	SyZinxAdapterPth* pth = new SyZinxAdapterPth(mSyZinx->PthNum);
	pth->SyZinxAdapterPth_Init();
	mSyZinx->mSyPth = pth;

	//��ʼ������������
	SyZinxAdapterMission* pMiss = new IChannel;
	mSyZinx->mSyMission = pMiss;

	//��ʼ����Դ������
	SyZinxAdapterResource* pRes = new SyZinxAdapterResource;
	pRes->SyZinxAdapterResource_Init(mSyZinx->PthNum);
	mSyZinx->mSyResource = pRes;

	//pth->SyZinxAdapter_Pth_Add();
	//�߳�����������Դ������
	pth->mPthRes = pRes;
	pRes->mResPth = pth;

	pth->mSyZinx = mSyZinx;
	return 1;
}

void SyZinxKernel::SyZinxKernel_Run()
{
	try
	{

		int iRet = event_base_dispatch(mSyZinx->SyZinxBase);
		throw iRet;
	}
	catch (int err)
	{
		mSyZinx->mLog.Log(__FILE__, __LINE__, SyZinxLog::ERROR, err, "event_base_dispatch");
	}
}

void SyZinxKernel::SyZinxKernel_Close()
{
	//�ر�
	evconnlistener_free(mSyZinx->SyZinxListen);
	event_base_free(mSyZinx->SyZinxBase);
}

void SyZinxKernel::SyZinxKernel_Client_Close(const int mClient_fd)
{
}


SyZinxRecycle::SyZinxRecycle()
{
}
SyZinxRecycle::~SyZinxRecycle()
{
	mLog.Log(__FILE__, __LINE__, SyZinxLog::ERROR, 0, "SyZinxRecycle delete all Res!");
	std::cout << "��������������" << std::endl;
	delete SyZinxKernel::mSyZinx;
	delete  SyZinxKernel::mSyZinx->mSyPth;
	delete SyZinxKernel::mSyZinx->mSyMission;
	delete SyZinxKernel::mSyZinx->mSyResource;
}

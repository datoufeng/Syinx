
#ifndef _Syinx_H_
#define _Syinx_H_
#include "SyInc.h"
#include "SyLog.h"

//�����̰߳�ȫ�Լ��رյײ��׽���  ��ϸ�뿴libevent�ĵ�
#define SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING           (LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE)
//�����̰߳�ȫ�Լ��رյײ��׽����Լ�����close��־λ  ��ϸ�뿴libevent�ĵ�
#define SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING_OR_FREE   (LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE |  LEV_OPT_CLOSE_ON_FREE)
//���ö�дѭ��
#define SET_SOCKETS_EVENT_RDWR                          (EV_READ | EV_WRITE | EV_PERSIST)
//���ö�ѭ��
#define SET_SOCKETS_EVENT_RD                            (EV_READ | EV_PERSIST)

#define SYINXMOD_ADD_MYSQL                               
#define SYINXMOD_ADD_ORACLE                              
class SyinxLog;
class IChannel;
class SyinxAdapterMission;
class SyinxAdapterPth;
class SyinxAdapterResource;

struct SyinxKernelMsg
{
	int mClient_fd;
	struct buffereveant* Client_buffer;
};
class SyinxKernel;
class SyinxKernelObject
{
public:
	SyinxKernelObject() {}
	~SyinxKernelObject() {}
private:

};
//���ڳ�ʼ���ĺ��Ŀ��
class SyinxKernel 
{
	friend class SyinxAdapterPth;
	friend class SyinxRecycle;
	friend class SyinxAdapterResource;
public:
	
	//��ʼ�����
	static int SyinxKernel_Init(const short _inPort);


	//���п��(wait)
	static void SyinxKernel_Run();

	//�رտ��
	static void SyinxKernel_Close();

	//�ͻ����˳�ʱ�����ͷſͻ���������,�ͻ����ļ�������Ϊkeyֵ       �ѷ���
	static void SyinxKernel_Client_Close(const int mClient_fd);

	SyinxLog mLog;
private:
	//��ܳ�ʼ��������
	int SyinxKernel_InitAdapter();

	//���̳�ʼ��
	int SyinxKernel_Procedureinit();

private:
	//���ڱ���һ����Ҫ��base���
	struct event_base* SyinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* SyinxListen;

	//�ͻ���port
	short _Port;
public:
	//��һ��һ��SyinxAdapterPt�߳�������
	SyinxAdapterPth* mSyPth;
	//��һ������������
	SyinxAdapterMission* mSyMission;
	//��һ����Դ������
	SyinxAdapterResource* mSyResource;
	
public:
	int PthNum = 1;
	//�����������sockaddr_in
	struct Server_Sockaddr {
		std::string Prot;
		std::string sin_addr;
		std::string family;
	}Server_Sockaddr;

public:
	//�Լ���ָ��
	static SyinxKernel* mSyinx;


private: //˽�й��캯��
	SyinxKernel();
	SyinxKernel(SyinxKernel&) = default;
	~SyinxKernel();
public:

	//���ݿ����ģ������
	static int SyDatabaseMod;
private:
	//������������
	int SyinxAddDatabase();

};
//�йܺ��ĳ�ʼ��������
class SyinxKernelWork
{
public:
	SyinxKernelWork(){}
	SyinxKernelWork(int Prot = 8080);
	~SyinxKernelWork(){}

	int SyinxClose();
};
//����
class SyinxRecycle
{
public:
	SyinxRecycle();
	~SyinxRecycle();
	SyinxLog mLog;
	/*
	1.static std::multimap<int, SyinxKernelMsg*> mZinxMap; �е�SyinxKernelMsg��Ҫ�ͷ�
	2.
	*/

};

#endif 


//json
/*
{

	�����û�:char
	�������˿�:int

	��������Ⱥ����:int
	#����������Group��server������һ��
	Group:(char)��������Ⱥ����{
			server#1:(int)id
			server#2:
	}
}


*/
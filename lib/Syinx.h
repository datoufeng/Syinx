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
//�������
#define COMMMEND_PARAM                                   argv[1]
#define SET_SHM_KEY                                      0x8855

#define SYINXMOD_ADD_MYSQL                               
#define SYINXMOD_ADD_ORACLE    

//���ȫ����Ϣ���ڽ���ͨ��
struct SyinxKernelShmMsg
{
	bool SyinxKernewWork;     /*���浱ǰ������״̬*/

	int PthNum;               /*���浱ǰ����������������*/

	int AllClientNum;         /*���пͻ�����������*/

	int CurrentClientNum[4];

	pthread_t threads[4];        /*����ÿһ��tid*/
	int       mPthStatus[4];     /*����ÿһ���̵߳Ĺ���״̬*/

	char IP[16];                 /*����ip*/

	char Port[8];                /*����˿ں�*/

	bool ExitSignal;             /*syinx�˳��ź�*/

}; 
enum  PthStatus
{
	PthRun = 0,
	PthWait,
	PthExit,
};
class SyinxLog;
class IChannel;
class SyinxAdapterMission;
class SyinxAdapterPth;
class SyinxAdapterResource;
class SyinxPthreadPool;

class SyinxKernel;
struct SyinxKernelShmMsg;

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
	friend class SyinxKernelWork;
	friend class SyinxAdapterPth;
	friend class SyinxRecycle;
	friend class SyinxAdapterResource;
	friend void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
public:
	
	//��ʼ�����
	static int SyinxKernel_Init(const short _inPort);


	//���п��(wait)
	static void SyinxKernel_Run();

	//�رտ��
	static void SyinxKernel_Close();

	//�ͻ����˳�ʱ�����ͷſͻ���������,�ͻ����ļ�������Ϊkeyֵ       �ѷ���
	static void SyinxKernel_Client_Close(const int mClient_fd);

	//ͨ�������ڴ湲�������鱨
	//���������ڴ�
	static void SyinxSyinxKernel_MakeShm();
	
	//�ͷŹ����ڴ�
	static void SyinxSyinxKernel_FreeShm();

	//������ʼ�������׽���
	static void SyinxSyinxKernel_LocalSock();

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

	//����һ�������ڴ��shmid
	int ShmId;

	//���湲���ڴ������ָ��
	void* ShmData;
	SyinxKernelShmMsg* mShmData;
	
	//���汾�׽���
	int LocalSocket;
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
//�йܺ��ĳ�ʼ��������,�Լ��ж��������
class SyinxKernelWork
{
public:
	SyinxKernelWork(){}
	SyinxKernelWork(int Prot, int argc, char* argv[]);
	~SyinxKernelWork();
public:
	
	//��ӡ������״̬  -s
	void PrintfServerStatus();

	
	void Makedaemon(int Prot);

	void SyinxExit();

	//�����ܵĵ�ַ
	SyinxKernel* mSyinx;

	int Port;

	//�������id
	int Pid_t;

	bool SyinxKernelStatus = false;
	
	int SyinxKernelExit;
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
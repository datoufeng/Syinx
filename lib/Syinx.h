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
enum  PthStatus
{
	PthRun = 0,
	PthWait,
	PthExit,
};
/*
	������Ҫд��log���¼�����������ַ���
	��Ϊ
	EVENT    �¼�����   1
	WARNING  ����       2
	ERROR    ����       3
*/


enum SyinxKernelErrno
{
	PortErr = 3,
	CreateBaseErr = 3,
	ClientConErr = 3,
};



class SyinxKernel;

//���ȫ����Ϣ���ڽ���ͨ��
struct SyinxKernelShmMsg
{
	bool SyinxKernewWork;     /*���浱ǰ������״̬*/

	int PthNum;               /*���浱ǰ����������������*/

	int AllClientNum;         /*���пͻ�����������*/

	char IP[16];                 /*����ip*/

	char Port[8];                /*����˿ں�*/

	bool ExitSignal;             /*syinx�˳��ź�*/

	SyinxKernel* mSyinx;         /*�����ܵĵ�ַ*/

}; 
struct SyinxKernelTimer_task_t
{
	void* (*taskfunc)(void*);
	void* arg;
};

class SyinxLog;
class IChannel;
class SyinxAdapterMission;
class SyinxAdapterPth;
class SyinxAdapterResource;
class SyinxPthreadPool;
class SyinxConfig;

class SyinxKernel;
struct SyinxKernelShmMsg;
struct SyinxConfMsg;


//�����ص�
//ʱ�䴦��ص�
void SyinxKernel_TimerEvent_Cb(struct bufferevent* buffer, void* arg);
//�����ص�
void SyinxKernel_Recv_Cb(struct bufferevent* bev, void* ctx);
//д�¼��ص�
void SyinxKernel_Send_Cb(struct bufferevent* bev, void* ctx);
//�¼��ص�
void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx);

//����һ��������
void* IChannelTaskProcessing(void* arg);
//���ڳ�ʼ���ĺ��Ŀ��
class SyinxKernel 
{
	friend class SyinxKernelWork;
	friend class SyinxAdapterPth;
	friend class SyinxRecycle;
	friend class SyinxAdapterResource;
	friend void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
	friend void SyinxKernel_TimerEvent_Cb(struct bufferevent* buffer, void* arg);
public:
	
	//��ʼ�����
	static int SyinxKernel_Init();


	//���п��(wait)
	static void SyinxKernel_Run();

	//�رտ��
	static void SyinxKernel_Close();


	//ͨ�������ڴ湲�������鱨
	//���������ڴ�
	static void SyinxKernel_MakeShm();
	
	//�ͷŹ����ڴ�
	static void SyinxKernel_FreeShm();

	//������ʼ�������׽���
	static void SyinxKernel_LocalSock();

	//���ع����ڴ��ַ
	SyinxKernelShmMsg* GetSyinxKernelShmMsg()const;

private:
	//Ϊ�Լ����ʱ���ʱ��
	void SyinxKernel_Addtimefd();

	//��������¼����������¼�����

	SyinxKernelTimer_task_t** TimeEvent_Task;
	//��ܳ�ʼ��������
	int SyinxKernel_InitAdapter();

private:
	//���ڱ���һ����Ҫ��base���ֻ����������
	struct event_base* SyinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* SyinxListen;

	//�ͻ���port
	short _Port;

	//����һ�������ڴ��shmid
	int ShmId;

	//���湲���ڴ������ָ��
	void* ShmData;
	SyinxKernelShmMsg* mShmData = nullptr;
	
	//���汾�׽���
	int LocalSocket;

	//�����ȡ�����ļ�����Ϣ
	SyinxConfMsg* SyConfMsg;         //free
public:
	//��һ��һ��SyinxAdapterPt�߳�������
	SyinxAdapterPth* mSyPth;
	//��һ������������
	SyinxAdapterMission* mSyMission;
	//��һ����Դ������
	SyinxAdapterResource* mSyResource;

public:

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
	SyinxKernelWork(int argc, char* argv[]);
	~SyinxKernelWork();
public:
	
	//��ӡ������״̬  -s
	void PrintfServerStatus();

	
	void Makedaemon();

	void SyinxExit();

	//�����ܵĵ�ַ
	SyinxKernel* mSyinx;


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
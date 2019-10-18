#include "Syinx.h"
#include "SyAdapter.h"
#include "SyPthreadAdapter.h"
#include "SyTaskAdapter.h"
#include "SyPthreadPool.h"
#include "SyResAdapter.h"
#include "SyConfig.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <stdio.h>

#ifdef SYINXMOD_ADD_MYSQL
#include <mysql/mysql.h>
#else
#endif 


using namespace std;

SyinxKernel* SyinxKernel::mSyinx = nullptr;

//����Ĭ��Ϊ��
int SyinxKernel::SyDatabaseMod = 0;

const char* LogEvent_Event = "EVENT !";
const char* LogEvent_Warning = "WARNING !";
const char* LogEvent_Error = "ERROR !";

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

//�����ص�
void SyinxKernel_Recv_Cb(struct bufferevent* bev, void* ctx)
{
	auto mIC = (IChannel*)ctx;

	char _buff[BUFFSIZE] = { 0 };
	int iRet = bufferevent_read(bev, _buff, BUFFSIZE);
	if (iRet < 0)
	{
		return;
	}
	std::string _tmp(_buff, iRet - 1);
	mIC->StrByte->_InStr = _tmp;
	mIC->StrByte->_InSize = iRet - 1;

	try
	{
		iRet = SyinxAdapterPth::SyinxAdapter_Pth_Add(IChannelTaskProcessing, (void*)mIC);
		throw(iRet);
	}
	catch (int RetErr)
	{
		switch (RetErr)
		{
			case Success:
			{
				break;
			}
			case MutexInitErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, MutexInitErr, "SyinxAdapter_Pth_Add MutexInitErr");
				break;
			}
			case CondInitErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, CondInitErr, "SyinxAdapter_Pth_Add CondInitErr");
				break;
			}

			case LockErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, LockErr, "SyinxAdapter_Pth_Add LockErr");
				break;
			}
			case UnLockErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, UnLockErr, "SyinxAdapter_Pth_Add UnLockErr");
				break;
			}

			case CondWaitErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, CondWaitErr, "SyinxAdapter_Pth_Add CondWaitErr");
				break;
			}

			case CondSignalErr:
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, CondSignalErr, "SyinxAdapter_Pth_Add CondSignalErr");
				break;
			}
			case VarIsNULL:         /*���ݱ���Ϊ��*/
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, VarIsNULL, "SyinxAdapter_Pth_Add VarIsNULL");
				break;
			}
			case QueueIsMax:        /*�����������*/
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, QueueIsMax, "SyinxAdapter_Pth_Add VarIsNULL");
				break;
			}
			case Shutdown:          /*�ر�*/
			{
				SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, Shutdown, "Pthread_Pool Shutdown");
				break;
			}
			default:
				break;
			}
	}
}
//д�¼��ص�
void SyinxKernel_Send_Cb(struct bufferevent* bev, void* ctx)
{
	auto mIC = (IChannel*)ctx;

}

//�¼��ص�
void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx)
{
	auto mIC = (IChannel*)ctx;
	if (what & BEV_EVENT_EOF) // Client�˹ر����� 
	{
		SyinxKernel::mSyinx->mSyResource->SocketFd_Del(bev, 0);
	}
	else if (what & BEV_EVENT_ERROR) // ���ӳ��� 
	{

	}
	else if (what & BEV_EVENT_WRITING)//д��ʱ����������
	{
		SyinxKernel::mSyinx->mSyResource->SocketFd_Del(bev, 0);
	}
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

	struct bufferevent* buffer = NULL;
	buffer = bufferevent_socket_new(poSyinxBase, fd, BEV_OPT_CLOSE_ON_FREE);
	if (buffer == NULL)
	{
		cout << "bufferevent_socket_new is failed" << endl;
		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, ClientConErr, "bufferevent_socket_new is failed");
	}
	char buf[BUFSIZ] = { 0 };

	sprintf(buf, "New Client is connect : [%d]", fd);
	SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::EVENT, SyinxLog::EVENT, buf);

	//���������Ŀͻ���ί����Դ������������
	int iRet = mSyinx->mSyResource->SyinxAdapterResource_AllotClient(buffer,fd);

	return;
}
int SyinxKernel::SyinxKernel_Init()
{
	SyinxKernel::mSyinx = new SyinxKernel;
	//��ȡ�����ļ�
	SyinxConfig conf;
	mSyinx->SyConfMsg = conf.Read_Msgconfig();

	uint16_t _inPort = (uint16_t)stoi(mSyinx->SyConfMsg->Port);

	if (_inPort<=0 || _inPort >=65535 )
	{
		char WriteLog[BUFFSIZE] = { 0 };
		sprintf(WriteLog, "Port is %s : not within the scope of 0 - 65535", LogEvent_Error);
		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR , _inPort, WriteLog);
	}
	

	//��ʼ���ṹ��
	struct sockaddr_in _Serveraddr;
	memset(&_Serveraddr, 0, sizeof _Serveraddr);
	_Serveraddr.sin_family = AF_INET;
	_Serveraddr.sin_port = htons(_inPort);
	

	
	//�������
	mSyinx->SyinxBase = event_base_new();
	if (NULL == mSyinx->SyinxBase)
	{
		cout << "event_base_new" << endl;
		char WriteLog[BUFFSIZE] = { 0 };
		sprintf(WriteLog, "Create Base %s : event_base_new is failed", LogEvent_Error);
		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, SyinxLog::ERROR, WriteLog);

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


	//���ü���
	mSyinx->SyinxListen = evconnlistener_new_bind(mSyinx->SyinxBase, SyinxKernel_Listen_CB, (void*)poSyinxDeliver,
		SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING, 10, (const sockaddr*)& _Serveraddr, sizeof(_Serveraddr));
	if (mSyinx->SyinxListen == NULL)
	{
		char WriteLog[BUFFSIZE] = { 0 };
		sprintf(WriteLog, "Create Base %s : evconnlistener_new_bind is failed", LogEvent_Error);
		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, _inPort, WriteLog);
		return -1;
	}
	//��ʼ����ʱ��
	mSyinx->SyinxKernel_Addtimefd();

	//��ʼ��������
	mSyinx->SyinxKernel_InitAdapter();
	return 1;
	
}
//ʱ�䴦��ص�
void SyinxKernel_TimerEvent_Cb(struct bufferevent* buffer, void* arg)
{
	auto mSyinx = (SyinxKernel*)arg;
	if (mSyinx->mShmData->ExitSignal == true)
	{
		mSyinx->SyinxKernel_Close();
	}
		
}
void SyinxKernel::SyinxKernel_Addtimefd()
{
	//�ȳ�ʼ���¼�����


	struct itimerspec setitimerspec;
	//set ����
	setitimerspec.it_interval.tv_sec = this->SyConfMsg->Timerinterval;
	setitimerspec.it_interval.tv_nsec = 0;

	//set ��һ��
	setitimerspec.it_value.tv_sec = this->SyConfMsg->Timervalue;
	setitimerspec.it_value.tv_nsec = 0;


	
	
	
	int tmfd;

	tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (tmfd < 0)
	{

		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, tmfd, "timefd_create is failed");
		return;
	}
	int iRet = timerfd_settime(tmfd, 0, &setitimerspec, NULL);
	if (iRet < 0)
	{

		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, iRet, "timerfd_settime is failed");
		return;
	}

	auto buffer = bufferevent_socket_new(this->SyinxBase, tmfd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(buffer, SyinxKernel_TimerEvent_Cb, NULL, NULL, (void*)mSyinx);

	//����buffer�¼�
	bufferevent_enable(buffer, EV_READ);

}

int SyinxKernel::SyinxKernel_InitAdapter()
{
	//��ʼ���̹߳�����
	SyinxAdapterPth* nSyPth = new SyinxAdapterPth(SyConfMsg->PthNum,SyConfMsg->TaskNum);

	//��ʼ����Դ������
	SyinxAdapterResource* nSyRes = new SyinxAdapterResource();

	/*�����*/
	//pth
	nSyPth->mPthRes = nSyRes;
	nSyPth->mSyinx = mSyinx;

	//res
	nSyRes->mResPth = nSyPth;
	nSyRes->mSyinx = mSyinx;
	//msyinx
	mSyinx->mSyPth = nSyPth;
	mSyResource = nSyRes;
	int iRet = 0;

	//��ʼ���̳߳�
	try 
	{
		int iRet = mSyinx->mSyPth->SyinxAdapterPth_Init();
		throw iRet;
	}
	catch (int RetErr)
	{
		if (RetErr == -1)
		{
			SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, iRet, "SyinxAdapterPth_Init is failed");
			return -1;
		}

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
		SyinxLog::mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, err, "event_base_dispatch");
	}
}

void SyinxKernel::SyinxKernel_Close()
{

	//free
	if (SyinxKernel::mSyinx->mSyPth != NULL)
	{
		SyinxKernel::mSyinx->mSyPth->SyinxAdapter_Pth_destroy();
		delete  SyinxKernel::mSyinx->mSyPth;
	}


	if (SyinxKernel::mSyinx->mSyResource != NULL)
	{
		SyinxKernel::mSyinx->mSyResource->SyinxAdapterResource_Free();
		delete SyinxKernel::mSyinx->mSyResource;
	}

	//�رռ���
	evconnlistener_free(mSyinx->SyinxListen);
	event_base_free(mSyinx->SyinxBase);


	if (SyinxKernel::mSyinx != NULL)
		delete SyinxKernel::mSyinx;

	SyinxKernel::SyinxKernel_FreeShm();

	cout << "Syinx is close";
}

void SyinxKernel::SyinxKernel_MakeShm()
{
	int shmid = shmget(SET_SHM_KEY, sizeof(SyinxKernelShmMsg), IPC_CREAT | IPC_EXCL | 0664);
	if (-1 == shmid)
	{
		perror("shmget is failed");
		exit(0);
	}
	//
	SyinxKernel::mSyinx->ShmId = shmid;
	void* ShmData = shmat(shmid, NULL, 0);
	mSyinx->ShmData = ShmData;
	memset(ShmData, 0, sizeof(SyinxKernelShmMsg));
	auto mShmData = (SyinxKernelShmMsg*)ShmData;

	auto confMsg = mSyinx->SyConfMsg;


	mShmData->PthNum = confMsg->PthNum;

	mShmData->SyinxKernewWork = true;
	mShmData->ExitSignal = false;

	strcpy(mShmData->IP, mSyinx->Server_Sockaddr.sin_addr.c_str());
	strcpy(mShmData->Port, mSyinx->Server_Sockaddr.Prot.c_str());

	//�����ڴ汣���ܵ�ַ
	mShmData->mSyinx = SyinxKernel::mSyinx;

	//���ñ��湲���ڴ�
	mSyinx->mShmData = mShmData;

	
}


void SyinxKernel::SyinxKernel_FreeShm()
{
	if (shmctl(mSyinx->ShmId, IPC_RMID, NULL) == -1)
	{
		perror("shmctl delete is failed");
		return;
	}
}

void SyinxKernel::SyinxKernel_LocalSock()
{


}

SyinxKernelShmMsg* SyinxKernel::GetSyinxKernelShmMsg()const
{
	return mShmData;
}


void SyinxKernelWork::PrintfServerStatus()
{

	int shmid = shmget(SET_SHM_KEY, 0, 0);
	if (-1 == shmid)
	{
		perror("Syinx is not run");
		exit(0);
	}
	void* ShmData = shmat(shmid, NULL, SHM_RDONLY);
	auto mShmData = (SyinxKernelShmMsg*)ShmData;
	if (mShmData->SyinxKernewWork)
	{
		printf("\n");
		cout << "Syinx-status:" << "\033[32;1m  run  \033[0m" << endl;
	}
	else
	{
		
		cout << "Syinx-status:" << "\033[31;1m  close  \033[0m" << endl;
		exit(0);
	}
	printf("IP:[%s] Prot:[%s]\n",mShmData->IP, mShmData->Port);
	printf("Total server connections : [%d]\n", mShmData->AllClientNum);

	printf("\n");
	if (shmdt(ShmData) == -1)
	{
		perror("shmdt is failed");
		return;
	}
}
void SyinxKernelWork::Makedaemon()
{
	int pid_t;
	pid_t = fork();
	if (-1 == pid_t)
	{
		perror("ERR : fork()");
		exit(0);
	}

	else if (pid_t > 0)
		exit(0);

	setsid();
	chdir("./");
	umask(0);

	SyinxKernel::SyinxKernel_Init();
	this->mSyinx = SyinxKernel::mSyinx;
	this->SyinxKernelStatus = true;
	SyinxKernel::SyinxKernel_MakeShm();
	SyinxKernel::SyinxKernel_Run();


}
void SyinxKernelWork::SyinxExit()
{
	int shmid = shmget(SET_SHM_KEY, 0, 0);
	if (-1 == shmid)
	{
		perror("Syinx is not run");
		exit(0);
	}
	void* ShmData = shmat(shmid, NULL, 0);
	auto mShmData = (SyinxKernelShmMsg*)ShmData;
	
	mShmData->ExitSignal = true;


	if (shmdt(ShmData) == -1)
	{
		perror("shmdt is failed");
		return;
	}
	return;
}
SyinxKernelWork::SyinxKernelWork(int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << "ERR : Syinx Uncarried parameter" << endl;
		exit(0);
	}
	if (!strcmp(COMMMEND_PARAM, "run"))
	{
		this->Makedaemon();
	}
	else if (!strcmp(COMMMEND_PARAM, "-s"))
	{
		//��ӡ״̬
		this->PrintfServerStatus();
		exit(0);
	}
	else if (!strcmp(COMMMEND_PARAM, "-v") | !strcmp(COMMMEND_PARAM, "version"))
	{
		cout << "Syinx Version:0.1.8" << endl;
		exit(0);
	}
	else if (!strcmp(COMMMEND_PARAM, "-c") | !strcmp(COMMMEND_PARAM, "close"))
	{
		this->SyinxExit();
		exit(0);
	}
	else
	{
		cout << "ERR : Syinx Error carrying parameter" << endl;
		exit(0);
	}

}

SyinxKernelWork::~SyinxKernelWork()
{
	
}


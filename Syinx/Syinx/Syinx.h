//Syinx Kernel ,DO NOT EDIT!
/*

				######
			   #########
			  ############
			  #############
			 ##  ###########
			###  ###### #####
			### #######   ####
		   ###  ########## ####
		  ####  ########### ####
		 ####   ###########  #####
		#####   ### ########   #####
	   #####   ###   ########   ######
	  ######   ###  ###########   ######
	 ######   #### ##############  ######
	#######  #####################  ######
	#######  ######################  ######
   #######  ###### #################  ######
   #######  ###### ###### #########   ######
   #######    ##  ######   ######     ######
   #######        ######    #####     #####
	######        #####     #####     ####
	 #####        ####      #####     ###
	  #####       ###        ###      #
		###       ###        ###
		 ##       ###        ###
__________#_______####_______####______________
				���ǵ�δ��û��BUG

*/

#ifndef _Syinx_H_
#define _Syinx_H_

struct event_base;
struct evconnlistener;
struct bufferevent;

//�������������
class SyinxLog;
class IChannel;
class SyinxAdapterPth;
class SyinxAdapterResource;
class SyinxPthreadPool;
class SyinxKernel;
class DBServerNet;

//�����̰߳�ȫ�Լ��رյײ��׽���  
//set thread and close socket
#define SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING				(LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE)

//�����̰߳�ȫ�Լ��رյײ��׽����Լ�����close��־λ  
#define SETOPT_THREADSAFE_OR_SOCKETS_BLOCKING_OR_FREE		(LEV_OPT_LEAVE_SOCKETS_BLOCKING | LEV_OPT_THREADSAFE |  LEV_OPT_CLOSE_ON_FREE)

//���ö�дѭ��
#define SET_SOCKETS_EVENT_RDWR								(EV_READ | EV_WRITE | EV_PERSIST)

//���ö�ѭ��
//set read and persist
#define SET_SOCKETS_EVENT_RD								(EV_READ | EV_PERSIST)


/*
* �ɱ༭��
* ���ݲ�ͬ�����ñ༭�������ö���ֵ
*/
//��������ͬ����
#define LISTEN_OPT											10

//����buffer���ô�С
#define READBUFFER											4086

//���ö���ˮλ
#define LIBEVENT_READ_WATERMARK								0

//����֡ͬ�������֡��һ��15֡
#define GAME_SERVER_FRAME_NUMS								15

//�����ļ�·��
#define GAME_CONFIG_PATH									"./Config/Syinx-Server.config.json"


#define HASJSONKEYS(keys)				if (!doc.HasMember(keys)){										\
																LOG(ERROR)<<"NOT FIND Keys : "<<keys;	\
																return false;							\
															}

#define GETICHANNEL						 (IChannel*)ctx


struct SyinxKernelTimer_task_t
{
	void* (*taskfunc)(void*);
	void* arg;
};



//Call Back Function

void SyinxKernel_Recv_Cb(struct bufferevent* bev, void* ctx);

void SyinxKernel_Send_Cb(struct bufferevent* bev, void* ctx);

void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx);

void SIG_HANDLE(int Sig);


class SyinxKernel 
{
	enum SYINX_LINK_STATUS
	{
		SYINX_LINK_CLOSE ,
		SYINX_LINK_WORK ,
		SYINX_LINK_WAIT,
		SYINX_LINK_MAX,
	};
private:
	void					(SyinxKernel::* m_SyinxStatusFunc[SYINX_LINK_MAX])();
	//status function
	void		OnStatusDoAction();
	void		OnStatusDoClose();
	short					m_Port;				
	int						m_ClientContentNum;								//���ͻ���;������
	int						m_PthPoolNum;
	int						m_TaskNum;
	int						m_TimerSec;										// timer sec
	bool					mUsePthreadPool;								//�Ƿ�ʹ���̳߳����߲���

	//DBServer
	int						m_DBPort;
	DBServerNet*			m_DBServer;									//dbserver


	//libedvent
	event_base*				mSyinxBase;								
	evconnlistener*			mSyinxListen;								

	SyinxAdapterPth*		mSyPth;																 //bing AdapterPth
	SyinxAdapterResource*	mSyResource;														 //bind AdapterResource

public:
	SyinxKernel();
	~SyinxKernel();
	static SyinxKernel& MakeSingleton();
	bool Initialize();
	void SyinxKernel_Run();
	void SyinxKernel_Close();





	inline SyinxAdapterPth*				 GetPth();
	inline SyinxAdapterResource*		 GetResource();
	inline event_base*					 GetEventBase();
	inline evconnlistener*				 GetListener();
	inline int							 GetPthreadPoolNum()const;
	inline int							 GetPthreadTaskNum()const;

	/*
	*�鿴���Դ�С�������С���򷵻�1������Ǵ���򷵻�0
	*/
	int	 GetEndian();

	
	int m_nWorkStatus;		// ״̬����������
	int m_endian;			// ��С��ģʽ
private:
	int			JudgeSystem(void);					

	/*
	* ��ʼ�������ļ�
	*/
    bool		SyinxKernelReadconfig();

	/*
	* ��ʼ��������
	*/
	bool		SyinxKernelInitAdapter();

	/*
	* ע���ź�
	*/
	bool		RegisterSignal();

	
};

extern SyinxKernel&		g_pSyinx;
extern uint64_t			g_nGameServerSecond;



uint64_t			GetMselTime();
void				Skipping(const int mesltime);
#endif	


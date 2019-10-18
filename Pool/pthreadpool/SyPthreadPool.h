#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

/*��Ҫ�̺߳�*/
//���߳�
#define PRINCIPAL_PTHREAD                                public
//��Ҫ�߳�
#define MINOR_PTHREAD                                    public

/*�����̻߳ص���������*/
//set flags call_back func is equall
#define SET_PTHPOOLCB_EQUALL                                 0x01
//set flags call_back func is unlike
#define SET_PTHPOLLCB_UNLIKE                                 0x02

/*�̹߳���״̬�ĺ�*/
class SyinxKernel;
class SyinxLog;
struct threadpool_t;
struct Principal_Pth;


enum SyPthreadPoolErr
{
	Success = 1,
	//��ʧ��
	MutexInitErr,
	CondInitErr,
	LockErr,
	UnLockErr,
	CondWaitErr,
	CondSignalErr,

	DeleteErr,
	DistoryErr,             /*�ͷ�ʧ��*/
	VarIsNULL,              /*���ݱ���Ϊ��*/
	QueueIsMax,        /*�����������*/

	Shutdown = 404,         /*�ر�*/

};

//������лص������������
typedef struct {
	void* (*startfunc)(void*);
	void* arg;
} SyPthreadPool_task_t;

struct threadpool_t {
	pthread_mutex_t                     Pthlock;    /*�����ڲ������Ļ�����*/
	pthread_cond_t                      Pthcond;    /*�̼߳�֪ͨ����������*/
	pthread_t* threads;                             /*�߳����飬������ָ������ʾ*/

	SyPthreadPool_task_t* Taskqueue;                /*�洢��������飬���������*/
	int queue_size;                                 /*��������������*/
	int count;                                      /*��ǰ��������*/
	int HeadIndex;                                  /*�������ͷ*/
	int TailIndex;                                  /*�������β*/


	int thread_count;                               /*�߳�����*/

	bool shutdown;                                   /*��ʶ�̳߳��Ƿ�ر�*/
	int  started;                                    /*�������߳���*/
};


class SyinxPthreadPool
{
	enum PrincipalPthWorkStatus
	{
		PriPthWork=1,
		PriPthWait,
		PRiPthClose,
		PriPthOther,
	};
public:
	SyinxPthreadPool();
	~SyinxPthreadPool();

	//�̳߳غ���
	/*
	@function threadpool_create
	 ����һ��threadpool_t����
	 @param thread_countΪ�̳߳ص�����,�߳�����
	 @param queue_size�����������
	 @param flagsΪռλ����,������ʹ�á�
	 @����һ���´������̳߳ػ�NULL
	ʧ�ܷ���NULL
	�ɹ������̳߳ؽṹ��
	*/
	static threadpool_t* threadpool_create(int thread_count, int queue_size, int flags);

	/*
	@function threadpool_add
	 �������Լ�������ӵ��������
	 @param poolΪ���̳߳ص�ַ
	 @param void* (*callback)(void*)Ϊ�ص�����
	 @param arg�������
	 @param flagsΪռλ����,������ʹ�á�
	ʧ�ܷ���SyPthreadPoolErrö��
	�ɹ�����Success
	*/
	static	int threadpool_add(threadpool_t* pool, void* (*callback)(void*), void* arg, int flags);

	/*
	@function threadpool_destroy
	 ����pool�̳߳�
	ʧ�ܷ���SyPthreadPoolErrö��
	�ɹ�����Success
	*/
	static  int threadpool_destroy(threadpool_t* pool, int flags);


	static threadpool_t* PthPool;
};


#endif

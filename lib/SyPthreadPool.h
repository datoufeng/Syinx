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

class SyinxLog;
struct threadpool_t;
struct Principal_Pth;


/*���̻߳ص��ṹ��*/
typedef struct
{
	void* (*function)(void*);          /* ����ָ�룬�ص����� */
	void* arg;                          /* ���溯���Ĳ��� */
} PrincipalPth_task_t;

//Ϊ���߳�����һ���������߳�
typedef struct
{
	void* (*function)(void*);          /* ����ָ�룬�ص����� */
	void* arg;                          /* ���溯���Ĳ��� */
} PrincipalPth_adjust_t;

//���߳̽ṹ��
struct Principal_Pth
{
	//pthread_mutex_t lock;               /* ������ס���ṹ�� */
	//pthread_mutex_t thread_counter;     /* ��¼æ״̬�̸߳���de�� -- busy_thr_num */

	pthread_t* threads;                 /* ����̳߳���ÿ���̵߳�tid������             free*/
	pthread_t adjust_tid;               /* ������߳�tid */

	PrincipalPth_task_t* task_queue;      /* �������(�����׵�ַ)                       free*/

	int* PthStatus;                     /*����ÿһ���̵߳Ĺ���״̬                     free*/

	//uint64_t* PthRecvDataByte;              /*ÿһ���߳̽���������,��λ:Byte(����)*/
	//uint64_t* PthSendDataByte;              /*ÿһ���̷߳���������,��λ:Byte(����)*/

	int thr_num;                        /* ��ǰ���߳����� */
	int busy_thr_num;                   /* æ״̬�̸߳��� */
	int wait_exit_thr_num;              /* Ҫ���ٵ��̸߳��� */

	int queue_front;                    /* task_queue��ͷ�±� */
	int queue_rear;                     /* task_queue��β�±� */
	int queue_size;                     /* task_queue����ʵ�������� */
	int queue_max_size;                 /* task_queue���п��������������� */

	bool shutdown;                       /* ��־λ���̳߳�ʹ��״̬��true��false */
};

/* ���߳̽ṹ�� */
typedef struct
{
	void* (*function)(void*);          /* ����ָ�룬�ص����� */
	void* arg;                          /* ���溯���Ĳ��� */
} threadpool_task_t;                    /* �����߳�����ṹ�� */

/* ��Ҫ�߳̽ṹ�� */
struct threadpool_t
{
	pthread_mutex_t lock;               /* ������ס���ṹ�� */
	pthread_mutex_t thread_counter;     /* ��¼æ״̬�̸߳���de�� -- busy_thr_num */

	pthread_cond_t queue_not_full;      /* �����������ʱ�����������߳��������ȴ����������� */
	pthread_cond_t queue_not_empty;     /* ��������ﲻΪ��ʱ��֪ͨ�ȴ�������߳� */

	pthread_t* threads;                 /* ����̳߳���ÿ���̵߳�tid������     free*/  
	pthread_t adjust_tid;               /* ������߳�tid */
	threadpool_task_t* task_queue;      /* �������(�����׵�ַ)                free */

	int min_thr_num;                    /* �̳߳���С�߳��� */
	int max_thr_num;                    /* �̳߳�����߳��� */
	int live_thr_num;                   /* ��ǰ����̸߳��� */
	int busy_thr_num;                   /* æ״̬�̸߳��� */
	int wait_exit_thr_num;              /* Ҫ���ٵ��̸߳��� */

	int queue_front;                    /* task_queue��ͷ�±� */
	int queue_rear;                     /* task_queue��β�±� */
	int queue_size;                     /* task_queue����ʵ�������� */
	int queue_max_size;                 /* task_queue���п��������������� */

	int shutdown;                       /* ��־λ���̳߳�ʹ��״̬��true��false */
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
	//��Ҫ�߳�
PRINCIPAL_PTHREAD:
	/*
	 @function PrincipalPth_create
	 ����һ��Principal_Pth��Ҫ�̶߳���
	 @param max_thr_num��Ҫ�߳�������Ҫ����������
	 @����һ���´������̳߳ػ�NULL
	 ע��:��Ҫ�߳�������Ҫ������ǰ���Ժ�����
	 */
	static Principal_Pth* PrincipalPth_create(int max_thr_num);

	/**
	* @function PrincipalPth_funcbind
	* @desc���ڰ����̵߳Ļص������Լ�����ָ��
	* @param�̳߳أ���������ӵ����С�
	* @param����ָ��ִ������ĺ�����
	* @param�������ݸ������Ĳ�����
	* @���һ��˳��������1������-1����0
	����������ûص������Ƿ�һ��,flags������0,location ������������
	����location���������ǰ�������߳���
	 */
	static int PrincipalPth_funcbind(Principal_Pth* PrPth, void* (*function)(void* arg), std::vector<event_base*>* BaseVec,int flags, int location);

	/**
	* @function PrincipalPth_add
	* @descz�����߳���󶨺�Ļص��������д���,���߳̽���ִ�����еĻص�����
	* @PrPth ��Ҫִ����������̳߳ؽṹ��
	* @���һ��˳��������1������-1����0
	flags ������
	 */
	static int PrincipalPth_add(Principal_Pth* PrPth,int flags );

	/**
	* @function PrincipalPth_Destory
	* @descֹͣ�������̳߳ء�
	* @param��Ҫ�����̳߳ء�
	* @���destory�ɹ�����0���򷵻�-1����0
	 */
	static int PrincipalPth_Destory(Principal_Pth* PrPth);

//��Ҫ�߳�
MINOR_PTHREAD:


	/**
	 @function threadpool_create
	 ����һ��threadpool_t����
	 @param min_thr_num��С�߳�����
	 @param max_thr_num����߳�
	 @param queue_max_size���еĴ�С��
	 @����һ���´������̳߳ػ�NULL
	 */
	static threadpool_t* threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);

	/**
	* @function threadpool_add
	* @desc���̳߳ض��������һ��������
	* @param�̳߳أ���������ӵ����С�
	* @param����ָ��ִ������ĺ�����
	* @param�������ݸ������Ĳ�����
	* @���һ��˳��������0������-1
	 */
	static int threadpool_add(threadpool_t* pool, void* (*function)(void* arg), void* arg);

	/**
	* @function threadpool_destroy
	* @descֹͣ�������̳߳ء�
	* @param��Ҫ�����̳߳ء�
	* @���destory�ɹ�����0���򷵻�-1	
	 */
	static int threadpool_destroy(threadpool_t* pool);

	/**
	* @desc��ȡ�̺߳�
	* @pool�̳߳�
	* @�����̵߳�#
	 */
	static int threadpool_all_threadnum(threadpool_t* pool);

	/**
	��ȡ��æ�̺߳�
	* @param��threadpool
	*���ط�æ�̵߳�#
	 */
	static int threadpool_busy_threadnum(threadpool_t* pool);

};


#endif

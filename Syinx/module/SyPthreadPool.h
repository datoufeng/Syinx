#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_


class SyinxKernel;
class SyinxLog;
class SyinxAdapterPth;
struct threadpool_t;
struct Principal_Pth;


typedef struct {
	void* (*startfunc)(void*);
	void* arg;
} SyPthreadPool_task_t;

struct threadpool_t {

	pthread_mutex_t					Pthlock;									//	�����ڲ������Ļ�����
	pthread_cond_t					Pthcond;									//	�̼߳�֪ͨ����������
	pthread_t*						threads;									//	�߳����飬������ָ������ʾ
																				
	SyPthreadPool_task_t*			Taskqueue;									//	�洢��������飬���������
	int								queue_size;									//	��������������
	int								count;										//	��ǰ��������
	int								HeadIndex;									//	�������ͷ
	int								TailIndex;									//	�������β
																				
																				
	int								thread_count;								//	�߳�����
																				
	bool							PthpoolClose;								//	��ʶ�̳߳��Ƿ�ر�
	int								started;									//	�������߳���
	int								WorkStatus;									//	����״̬
};

class SyinxPthreadPool
{
public:
	enum SyPthreadPoolErr
	{
		PoolErr = 0,
		Success = 1,
		MutexInitErr = 2,							//��ʼ��������ʧ��
		CondInitErr = 3,							//����������ʼ��ʧ��
		LockErr = 4,
		UnLockErr = 5,
		CondWaitErr = 6,
		CondSignalErr = 7,
		DeleteErr = 8,
		DistoryErr = 9,								//�ͷ�ʧ��
		VarIsNULL = 10,								//���ݱ���Ϊ��
		QueueIsMax = 11,							//�����������
		PthShutdown = 12,							//�ر�
	};
	enum PrincipalPthWorkStatus
	{
		PriPthWork = 1,
		PriPthWait = 2,
		PRiPthClose = 3,
		PriPthOther = 4,
	};
	SyinxPthreadPool();
	~SyinxPthreadPool();

	static SyinxPthreadPool& MakeSingleton();

	inline threadpool_t* GetPthreadPool()
	{
		if (PthPool != nullptr)
			return PthPool;
		else
			return nullptr;
	}
	threadpool_t* threadpool_create(uint32_t thread_count, uint32_t queue_size, uint32_t flags = 0);

	uint32_t threadpool_add(threadpool_t* pool, void* (*callback)(void*), void* arg, uint32_t flags= 0);

	uint32_t  threadpool_destroy(threadpool_t* pool, uint32_t flags = 0);

private:
	threadpool_t* PthPool;
};

extern SyinxPthreadPool& g_SyinxPthPool;

#endif


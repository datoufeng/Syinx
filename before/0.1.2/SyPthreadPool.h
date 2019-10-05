#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_
#include "SyDef.h"

typedef struct threadpool_t threadpool_t;
typedef struct Principal_Pth Principal_Pth;
class SyinxPthreadPool
{
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
	 */
	static Principal_Pth* PrincipalPth_create(int max_thr_num);

	/**
	* @function PrincipalPth_Add
	* @desc���̳߳ض��������һ��������
	* @param�̳߳أ���������ӵ����С�
	* @param����ָ��ִ������ĺ�����
	* @param�������ݸ������Ĳ�����
	* @���һ��˳��������1������-1����0
	 */
	static int PrincipalPth_Add(Principal_Pth* PrPth, void* (*function)(void* arg), void* arg);

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

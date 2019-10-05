#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_
typedef struct threadpool_t threadpool_t;
class PthreadPool
{
public:
	PthreadPool();
	~PthreadPool();
public:
	

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

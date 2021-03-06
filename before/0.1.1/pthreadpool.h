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
	 创建一个threadpool_t对象。
	 @param min_thr_num最小线程数量
	 @param max_thr_num最大线程
	 @param queue_max_size队列的大小。
	 @返回一个新创建的线程池或NULL
	 */
	static threadpool_t* threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);

	/**
	* @function threadpool_add
	* @desc在线程池队列中添加一个新任务
	* @param线程池，将任务添加到其中。
	* @param函数指向执行任务的函数。
	* @param参数传递给函数的参数。
	* @如果一切顺利，返回0，否则-1
	 */
	static int threadpool_add(threadpool_t* pool, void* (*function)(void* arg), void* arg);

	/**
	* @function threadpool_destroy
	* @desc停止并销毁线程池。
	* @param池要销毁线程池。
	* @如果destory成功返回0，则返回-1	
	 */
	static int threadpool_destroy(threadpool_t* pool);

	/**
	* @desc获取线程号
	* @pool线程池
	* @返回线程的#
	 */
	static int threadpool_all_threadnum(threadpool_t* pool);

	/**
	获取繁忙线程号
	* @param池threadpool
	*返回繁忙线程的#
	 */
	static int threadpool_busy_threadnum(threadpool_t* pool);


};


#endif

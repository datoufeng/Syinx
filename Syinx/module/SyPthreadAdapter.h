#pragma once
#include <stdint.h>
#define SOCKETS	                                         int
class SyinxAdapter;
class SyinxPthreadPool;

struct threadpool_t;
struct Principal_Pth;

enum SyinxAdapterPthErrno
{
	InitFailed,
	AddFuncFailed,
	DesPoolFailed,
};
//�߳�������
class SyinxAdapterPth 
{
	friend class SyinxKernel;
	friend class SyinxAdapterResource;
private:
	int										 m_PthPoolNum;					//�����̳߳�����
	int										 m_TaskMaxNum;					//���������������
	threadpool_t*							 m_sSyinxPthPool;				//�����Ҫ�̳߳����Խṹ��

public:
	SyinxAdapterPth(uint32_t PthPoolNum, uint32_t TaskMaxNum);
	~SyinxAdapterPth();

	bool SyinxAdapterPth_Init();
	static uint32_t SyinxAdapterPth_Add(void* (*taskfunc)(void*), void* arg);
	uint32_t SyinxAdapterPth_destroy();
};

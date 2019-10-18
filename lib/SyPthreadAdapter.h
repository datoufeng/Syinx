#pragma once
#include "SyPthreadPool.h"

#define SOCKETS	                                         int
class SyinxAdapter;
class SyinxPthreadPool;

struct threadpool_t;
struct Principal_Pth;

//�߳�������
class SyinxAdapterPth :public SyinxAdapter
{
	friend class SyinxKernel;
	friend class SyinxAdapterResource;
public:
	SyinxAdapterPth();
	SyinxAdapterPth(int PthPoolNum, int TaskMaxNum) : PthPoolNum(PthPoolNum), TaskMaxNum(TaskMaxNum){}
	~SyinxAdapterPth();


public://��ʼ����Ҫ�̳߳�
	int SyinxAdapterPth_Init();

   //���̳߳������һ������,����ᱻ���ص��������
	static int SyinxAdapter_Pth_Add(void* (*taskfunc)(void*), void* arg);

	//�����̳߳�
	int SyinxAdapter_Pth_destroy();

	//�����̳߳�����
	int PthPoolNum;
	//���������������
	int TaskMaxNum;
private:

	//����Դ������
	SyinxAdapterResource* mPthRes;

	//�󶨿�ܵ�ַ
	SyinxKernel* mSyinx;


	//�����Ҫ�̳߳����Խṹ��
	threadpool_t* mSyinxPthPool;

};
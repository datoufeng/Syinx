#pragma once
#include "SyPthreadPool.h"

#define SOCKETS	                                         int
class SyinxAdapter;
class SyinxPthreadPool;

struct threadpool_t;
struct Principal_Pth;

//���̹߳����ص�����
void* PrincipalPth_Cb(void* dst);

//�߳�������
class SyinxAdapterPth :public SyinxAdapter
{
	friend class SyinxKernel;
	friend class SyinxAdapterResource;
public:
	SyinxAdapterPth();
	SyinxAdapterPth(int PthNum) : PthNum(PthNum) {}
	~SyinxAdapterPth();
	//��ʼ���������̳߳�
	int SyinxAdapterPriPth_Init();


	//��ʼ���ص����������ṹ��
	int SyinxAdapterPriPth_InitMsg();

	/*
	 @function SyinxAdapterPriPth_Setcb
	 ���õ�ǰ�����̻߳ص���������
	 @param flags ���ûص���������
	 @����һ���´������̳߳ػ�NULL
	
	 */
	int SyinxAdapterPriPth_Setcb(std::vector<event_base*>* BaseVec,int flags, int locaiont);

	//create���߳�ʹ�乤��
	int SyinxAdapterPriPth_Run();

private:
	
public://��ʼ����Ҫ�̳߳�
	int SyinxAdapterPth_Init();

   //���̳߳����һ��ͨ����
	int SyinxAdapter_Pth_Add();

	//�����̳߳�
	int SyinxAdapter_Pth_destroy();

	//��ȡ��ǰ�����߳�����
	int getPthNum() const;
public:

	//������������߳�����
	int PthNum;//����Ϊ5

private:
	//�ص�������������
	void* arg;

	//����Դ������
	SyinxAdapterResource* mPthRes;

	//�󶨿�ܵ�ַ
	SyinxKernel* mSyinx;

	//������Ҫ�̳߳ؽṹ��
	Principal_Pth* mSyinxPriPthPool;

	//�����Ҫ�̳߳����Խṹ��
	threadpool_t* mSyinxPthPool;

};


#include "SyPthreadPool.h"
class SyinxAdapter;
//�̴߳��ݲ���
struct PthTransmitMsg
{
	//������Դ������map��ַ
	std::multimap<SOCKETS, bufferevent*>* sChannelMap;
	struct event_base* sBase;
};

//�߳�������
class SyinxAdapterPth :public SyinxAdapter
{
	friend class SyinxKernel;
	friend class SyinxAdapterResource;
public:
	SyinxAdapterPth();
	SyinxAdapterPth(int PthNum) : PthNum(PthNum) {}
	~SyinxAdapterPth();

	//��ʼ���̳߳�
	int SyinxAdapterPth_Init();
	//�������߳�����̷ֿ߳�

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
	//����Դ������
	SyinxAdapterResource* mPthRes;

	//�󶨿�ܵ�ַ
	SyinxKernel* mSyinx;

	//�����̳߳����Խṹ��
	threadpool_t* mSyinxPthPool;

};
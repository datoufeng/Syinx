#include "pthreadpool.h"
#include "Log.h"

#include <pthread.h>
#include <iostream>
#include <vector>
#include <map>
#include <event2/listener.h>  
#include <event2/bufferevent.h>  
/*
class SyZinxAdapter -> class SyZinxAdapterPth              �߳�������
class SyZinxAdapter -> class SyZinxAdapterMission		   ����������
class SyZinxAdapterMission -> class IChannel            ͨ�������ڽ��տͻ��˷���������
class IChannelFactory /                                 ���ڳ�ʼ����һϵ������

*/

#ifndef _SYPTH_H_
#define _SYPTH_H_

class SyZinxKernel;
class SyZinxAdapter
{
public:
	SyZinxAdapter() {}
	virtual ~SyZinxAdapter() {}
	SyZinxLog mLog;
};

class SyZinxAdapterResource;
//�߳�������
class SyZinxAdapterPth :public SyZinxAdapter
{
	friend class SyZinxKernel;
public:
	SyZinxAdapterPth();
	SyZinxAdapterPth(int PthNum) : PthNum(PthNum) {}
	~SyZinxAdapterPth();

	//��ʼ���̳߳�
	 int SyZinxAdapterPth_Init();

	//���̳߳����һ��ͨ����
	int SyZinxAdapter_Pth_Add();

	//�����̳߳�
	static int SyZinxAdapter_Pth_destroy();

	//��ȡ��ǰ�����߳�����
	int getPthNum() const;
public:

	//������������߳�����
	int PthNum;//����Ϊ5
	
private:
	//����Դ������
	SyZinxAdapterResource* mPthRes;

	//�󶨿�ܵ�ַ
	SyZinxKernel* mSyZinx;

	//�����̳߳����Խṹ��
	threadpool_t* mSyZinxPthPool;
	
};

class SyZinxAdapterMission;
class IChannel;

struct ClientMsg {
	IChannel* pChannel;
	bufferevent* pClient_buffer;
};

//��Դ������
class SyZinxAdapterResource : public SyZinxAdapter
{
	friend class SyZinxKernel;
	friend class SyZinxAdapterPth;
	friend class SyZinxAdapterMission;
public:
	SyZinxAdapterResource();
	~SyZinxAdapterResource();

	//��ȡ��Ⱥ�����л�Ծ�������
	int getallClientNum();

	//����Ⱥ��ĳһ��������
	int deleteClient();
private:
	//��ʼ����Ⱥ��Դ
	int SyZinxAdapterResource_Init(int PthNum);

private:
	SyZinxAdapterPth* mResPth;
	SyZinxAdapterResource(SyZinxAdapterResource& _intmp) = delete;

	//һ���̶߳�Ӧһ���¼���,ÿ���¼������û�����multimap��
	std::vector<std::multimap<int, ClientMsg*>> mSyChannelVec;
	std::vector<event_base*> mSyBaseVec;
};

//��������
class SyZinxAdapterMission : public SyZinxAdapter
{
public:
	SyZinxAdapterMission() {}
	virtual ~SyZinxAdapterMission() {}
public:
	//���ý������ݵĿ�
	virtual void RecvData() = 0;
	virtual void SendData() = 0;

};

//����ͨ���㹤��������һϵ�г�ʼ��
class IChannelFactory
{
public:
	IChannelFactory() {}
	~IChannelFactory() {}
};

class IChannel : public SyZinxAdapterMission
{
public:
	IChannel() {}
	~IChannel() {}
public:
	virtual void RecvData() override;
	virtual void SendData() override;
};

#endif 



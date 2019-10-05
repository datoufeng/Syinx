
#ifndef _SYRESADAPTER_H_
#define _SYRESADAPTER_H_
class SyinxAdapter;
class SyinxAdapterPth;
class SyinxKernel;
#include <vector>
#define SOCKETS	                                         int
#define BUFFSIZE                                         256
class mResourceMsg 
{
	int PthNum;                /*���浱ǰ��������������(�߳���)*/
};
//��Դ������
class SyinxAdapterResource : public SyinxAdapter
{
	
	friend void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
	friend class SyinxKernel;
	friend class SyinxAdapterPth;
	friend class SyinxAdapterMission;
	friend class Ichannel;
public:
	SyinxAdapterResource();
	SyinxAdapterResource(int PthNum) :PthNum(PthNum) {}
	~SyinxAdapterResource();

	//��ȡ��Ⱥ�����л�Ծ�������
	int getallClientNum();

	//����Ⱥ��ĳһ��������
	int deleteClient(SOCKETS _FD, int where);

	//���ͻ������ö�Ϊ�������,���ǲ��������������ķ�Χ
	int SetClientRandAllot(bool flags);

	//���ÿͻ��˹̶�����
	int SetClientFixationAllot(int Num);

private:
	//��ʼ����Ⱥ��Դ
	int SyinxAdapterResource_Init(int PthNum);

	//�������ӵĿͻ��˽��з���
	int SyinxAdapterResource_AllotClient(SOCKETS _FD);

	//���µ��ļ�����������������
	int SocketFd_Add(SOCKETS _FD, int where, int events = 0);

	//���ļ�����������
	int SocketFd_Del(SOCKETS _FD, int where);

	//Ϊbase��ʱ��
	int SyinxAdapterResource_Addtimefd();

private:/*��(mRes+Task ��Դ�����������������)*/
	//��������
	SyinxAdapterMission* mResTask;
	//���߳���
	SyinxAdapterPth* mResPth;
	//�󶨺��Ŀ����
	SyinxKernel* mSyinx;
private:

	//�߳���(IO��)
	int PthNum;
	//�ͻ����������Ĭ��Ϊtrue
	bool RandAllotFlags = false;

	SyinxAdapterResource(SyinxAdapterResource& _intmp) = delete;

	//һ���̶߳�Ӧһ���¼���,ÿ���¼������û�����multimap��
	std::vector<std::multimap<SOCKETS, IChannel*>*> mSyChannelVec;
	std::vector<event_base*> mSyBaseVec;
	
};
#endif


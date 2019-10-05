#pragma once
class SyinxAdapter;
class SyinxAdapterPth;


//��Դ������
class SyinxAdapterResource : public SyinxAdapter
{
	friend void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
	friend class SyinxKernel;
	friend class SyinxAdapterPth;
	friend class SyinxAdapterMission;
public:
	SyinxAdapterResource();
	~SyinxAdapterResource();

	//��ȡ��Ⱥ�����л�Ծ�������
	int getallClientNum();

	//����Ⱥ��ĳһ��������
	int deleteClient();

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
	//���ļ���������
	int SocketFd_Del(SOCKETS _FD, int where);
private:
	//�߳���(IO��)
	int PthNum;
	//�ͻ����������Ĭ��Ϊtrue
	bool RandAllotFlags = true;

	SyinxAdapterPth* mResPth;
	SyinxAdapterResource(SyinxAdapterResource& _intmp) = delete;

	//һ���̶߳�Ӧһ���¼���,ÿ���¼������û�����multimap��
	std::vector<std::multimap<SOCKETS, bufferevent*>> mSyChannelVec;
	std::vector<event_base*> mSyBaseVec;
};
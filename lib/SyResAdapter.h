#ifndef _SYRESADAPTER_H_
#define _SYRESADAPTER_H_
class SyinxAdapter;
class SyinxAdapterPth;
class SyinxKernel;
#include <vector>
#define SOCKETS	                                         int
#define BUFFSIZE                                         256
struct SyinxConfMsg;
class SyinxConfig;
//��Դ������
class SyinxAdapterResource : public SyinxAdapter
{
	
	friend void SyinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
	friend class SyinxKernel;
	friend class SyinxAdapterPth;
	friend class SyinxAdapterMission;
	friend class IChannel;
	friend void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx);
public:
	SyinxAdapterResource();
	SyinxAdapterResource(int PthNum) :PthNum(PthNum) {}
	~SyinxAdapterResource();

private:

	//�������ӵĿͻ��˽��з���
	int SyinxAdapterResource_AllotClient(bufferevent *buffer,SOCKETS _FD);

	//���µ��ļ�����������������
	int SocketFd_Add(bufferevent* buffer, SOCKETS _FD);

	//���ļ�����������
	int SocketFd_Del(bufferevent* buffer, SOCKETS _FD);

	//�ͷ���Դ���
	int SyinxAdapterResource_Free();

	//���¹����ڴ�
	int SyinxAdapterResource_UpdateShm();
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

	SyinxAdapterResource(SyinxAdapterResource& _intmp) = delete;

	//���û���ͨ�����������
	std::multimap<bufferevent*, IChannel*> mIChannelMap;
	
};
#endif


#ifndef _SYRESADAPTER_H_
#define _SYRESADAPTER_H_
#include <unordered_map>
#include <queue>

class SyinxKernel;
class SyinxAdapter;
class SyinxAdapterPth;
class SyinxLog;
class IChannel;
class CPlayer;

#define SOCKETS	                                         int
#define BUFFSIZE										 256

class SyinxAdapterResource
{
	friend void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx);
private:
	//���û���ͨ�����������
	std::list<IChannel*>				 m_listConnectClient;
	std::queue<IChannel*>				 m_queueILoop;
	size_t								m_IChannelNum;
public:
	SyinxAdapterResource(int IChNum);
	~SyinxAdapterResource();
	bool Initialize();
	bool Close();

	//�������ӵĿͻ��˽��з���
	bool AllocationIChannel(bufferevent* buffer, SOCKETS _FD);	//����һ��IChannel

	bool SocketFdAdd(IChannel* ICh);							//���µ��ļ�����������������
	bool SocketFdDel(IChannel* ICh);							//���ļ�����������

	void GameServerDoAction();								//��������֡

};
#endif
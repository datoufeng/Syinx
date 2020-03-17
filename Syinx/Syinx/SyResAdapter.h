#ifndef _SYRESADAPTER_H_
#define _SYRESADAPTER_H_
#include "GameObject.h"
#include <unordered_map>
#include <queue>

class CFrame;
class SyinxKernel;
class SyinxAdapter;
class SyinxAdapterPth;
class SyinxLog;
class IChannel;
class CPlayer;

#define SOCKETS	                                         int
#define BUFFSIZE										 4096

class SyinxAdapterResource : public SObject
{
	friend void SyinxKernel_Event_Cb(struct bufferevent* bev, short what, void* ctx);
	friend class SyinxKernel;
private:
	//���û���ͨ�����������
	std::list<IChannel*>				 m_listConnectClient;
	std::queue<IChannel*>				 m_queueILoop;
	size_t								 m_IChannelNum;
public:
	SyinxAdapterResource(int IChNum);
	~SyinxAdapterResource();
	bool Initialize();
	void Close();

	//�������ӵĿͻ��˽��з���
	bool AllocationIChannel(bufferevent* buffer, intptr_t _FD);	//����һ��IChannel

	bool SocketFdAdd(IChannel* ICh);							//���µ��ļ�����������������
	bool SocketFdDel(IChannel* ICh);							//���ļ�����������

	void GameServerDoAction();								//��������֡
private:

};
#endif
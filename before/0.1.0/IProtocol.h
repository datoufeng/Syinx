#include "IChannel.h"
#include "ProtocolMsg.h"

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//class UserData;

class Iprotocol : public SyZinxHandle
{

public:
	Iprotocol() {}
	~Iprotocol() {}

	//Э���ú������ͨ���㷢������Ϣ
	void Iprotocol_RecvString(UserData& _iData);

	//---�ɵ���---�ú�����Ҫ��������ĺ��������ղ��ҽ���һ�������л�,Ȼ�����������͵�ҵ���
	void Iprotocol_RecvDecode_ByteString(std::string &_iputs);

	//---�ɵ���---�ú�����Ҫ���л��ṹ����͸��ͻ���
	void Iprotocol_SendEncode_ByteString(GameMsg* _getmsg);

	//Э������������֮����Ҫ�������Ľṹ�巢�͵�ҵ�����д���
	virtual IZinxSys* InternalHandle(UserData& _Input) { return nullptr; }

	/*��ȡ��һ�������ں�������������д�ú���������ָ����ǰ�����ڽ�������һ��������ʲô��ͨ��������Ϣ���󣬿�����Բ�ͬ������зֱ���*/
	virtual SyZinxHandle* GetNextHandler()override;
public:
	//�����ֽ������
	ByteString* mClient_ByteString;

	//��������ͻ��˷�����
	std::string mLastBuffer;

public:
	Ichannel* mLinkChannel;
	Irole* mLinkRole;

	//���浱ǰ�ͻ���bufferevent
	struct bufferevent* mClient_buffer;

	//�ͻ����׽���
	int mClient_Fd;
};

#endif


#ifndef _ROLE_H_
#define _ROLE_H_
#include "IChannel.h"
#include "IProtocol.h"
#include "PlayerState.h"

#define _ROLE_BEHAVIOR_    public:
#define _ROLE_BATTLE_	   public:
#define _ROLE_SOCIAL_      public:

class Irole : public SyZinxHandle
{

public:
	Irole() {}
	~Irole() {}
	//��ʼ�������Ϣ
	void Init_Player();

	//ҵ����յ�һ��msg�������������
	void Irole_RecvMsgFromProtocol(GameMsg &_iMSg);

	//ҵ��㽫msg�����͵�Э�����з�װ  -��Э���ӵ�к�������ֱ�ӵ��ý�������-
	void Irole_SendMsgToProtocol(GameMsg& _iMSg);

	/*��Ϣ����������������д�ú���ʵ����Ϣ�Ĵ���������Ҫһ�����ڼ�������ʱ��Ӧ�ô����µ���Ϣ���󣨶Ѷ��󣩲�����ָ��*/
	inline virtual IZinxSys* InternalHandle(UserData& _Input) { return nullptr; }

	/*��ȡ��һ�������ں�������������д�ú���������ָ����ǰ�����ڽ�������һ��������ʲô��ͨ��������Ϣ���󣬿�����Բ�ͬ������зֱ���*/
	inline virtual SyZinxHandle* GetNextHandler() { return nullptr; }

public: /*------------ҵ������-----------------*/
_ROLE_BEHAVIOR_ /*��Ϊ*/
	//����������λ�ñ������͸������
	void MakePlayerNewPosToAny();

	/*��������ʱ��id��������Ϣ*/
	GameMsg* MakeLogonSyncPid();

	/*�����㲥������Ϣ*/
	GameMsg* MakeTalkBroadcast(std::string _talkContent);

	/*�����㲥����λ����Ϣ*/
	GameMsg* MakeInitPosBroadcast();

	/*�����㲥�ƶ�����λ����Ϣ*/
	GameMsg* MakeNewPosBroadcast();

	/*��������ʱ��id��������Ϣ*/
	GameMsg* MakeLogoffSyncPid();

	/*������Χ���λ����Ϣ*/
	GameMsg* MakeSurPlays();

public://������Ժ���
	//����������������
	void ProcRandPos();
	//����ƶ���ʵʱ������ҵ�xyzv
	void ProcNewPos(float _x, float _y, float _z, float _v);
	//������Ϣ  �������������Ϣ�ĺ���
	void ProcTalkContent(std::string szContent);
public://���������Ϣ
	//�������Ữ�����״̬Ϊ����һ����
	float x;
	float y;
	float z;
	float v;
	int id;
	std::string name;

public:
	Ichannel* mLinkChannel;
	Iprotocol* mLinkProtocol;

	//���浱ǰ�ͻ���bufferevent
	struct bufferevent* mClient_buffer;

	//�ͻ����׽���
	int mClient_Fd;
};

#endif

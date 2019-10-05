#include "SyZinx.h"
#include "msg.pb.h"
#include <list>
#ifndef _PROTOCLOLMSG_H_
#define _PROTOCLOLMSG_H_

//�����Ϣ��һ����� protocol ����Э��֮��ͽ������Ϣ�ŵ�����
class GameSingleTLV;

class GameMsg
{
public:
	GameMsg();
	virtual ~GameMsg();
	//�ֶ��ͷ������ں���9/18������
	void GameMsg_FreePackage();
	void GameMsg_FreeOutPackage();
	std::list<GameSingleTLV*> mGameMsgList;

};
//����TLV��Ϣ
class GameSingleTLV : public GameMsg
{
public:
	//������ϢID������
	enum GameMsgType {
		GAME_MSG_LOGON_SYNCPID = 1,	//ͬ�����id������
		GAME_MSG_TALK_CONTENT = 2,	//������Ϣ
		GAME_MSG_NEW_POSTION = 3,	//ͬ�����λ��
		GAME_MSG_BROADCAST = 200,   //�㲥��Ϣ
		GAME_MSG_LOGOFF_SYNCPID = 201,//���������Ϣ
		GAME_MSG_SUR_PLAYER = 202,  //ͬ���ܱ������Ϣ
	};
	GameMsgType m_MsgType;
	//����һ��protobuf�Ķ���ָ��,protocolЭ��������Э��֮����������Ķ���
	google::protobuf::Message* m_pbMsg;

	//�������л�Ϊ�ַ����ĺ���
	std::string Serialize();

	//������ϢID �Լ� content ,���캯���Զ���string �����л�Ϊ protobuf����
	GameSingleTLV(GameMsgType type, std::string content);

	//����ṹ�彫�ṹ��������л�
	GameSingleTLV(GameMsgType type, ::google::protobuf::Message* poObject);
	~GameSingleTLV();

};






#endif
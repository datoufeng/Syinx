#include "ProtocolMsg.h"

GameMsg::GameMsg()
{
}

GameMsg::~GameMsg()
{
	for (auto& it : this->mGameMsgList)
	{
		auto _freeTlv = it;
		delete _freeTlv;
	}

}

//ҵ������������İ���Ҫ���������
void GameMsg::GameMsg_FreePackage()
{

}

void GameMsg::GameMsg_FreeOutPackage()
{

}


GameSingleTLV::GameSingleTLV(GameMsgType type, std::string content)
{
	this->m_MsgType = type;
	switch (type)
	{
	case GameSingleTLV::GAME_MSG_LOGON_SYNCPID:  //ͬ���������id
		this->m_pbMsg = new pb::SyncPid;
		break;

	case GameSingleTLV::GAME_MSG_TALK_CONTENT:   //���������Ϣ
		this->m_pbMsg = new pb::Talk;
		break;
	case GameSingleTLV::GAME_MSG_NEW_POSTION:   //ͬ�����λ��
		this->m_pbMsg = new pb::Position;
		break;
	case GameSingleTLV::GAME_MSG_BROADCAST:     //�㲥��Ϣ
		this->m_pbMsg = new pb::BroadCast;
		break;
	case GameSingleTLV::GAME_MSG_LOGOFF_SYNCPID: //���������Ϣ
		this->m_pbMsg = new pb::SyncPid;
		break;
	case GameSingleTLV::GAME_MSG_SUR_PLAYER:    //ͬ���ܱ������Ϣ
		this->m_pbMsg = new pb::SyncPlayers;
		break;
	default:
		;
	}

	//��content�����л�
	if (m_pbMsg != NULL)
	{
		this->m_pbMsg->ParseFromString(content);
	}
}

GameSingleTLV::GameSingleTLV(GameMsgType type, ::google::protobuf::Message* poObject)
{
	this->m_MsgType = type;
	this->m_pbMsg = poObject;
}

GameSingleTLV::~GameSingleTLV()
{
	if (m_pbMsg != nullptr)
	{
		delete m_pbMsg;
		m_pbMsg = nullptr;
	}
}

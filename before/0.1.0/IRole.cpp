#include "IRole.h"
#include "IProtocol.h"
#include "ProtocolMsg.h"

#include<iostream>
#include <memory>
#include <random>
using namespace std;

static int namepid = 0;
//����һ���������(���ȷֲ�)
static default_random_engine gRandomEngine(time(nullptr));	//���������һ���������
void Irole::Init_Player()
{
	//�������λ��
	ProcRandPos();
	/*��������ʱ��id��������Ϣ*/
	auto gMsg = this->MakeLogonSyncPid();
	this->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);
	usleep(100);
	/*�����㲥����λ����Ϣ*/
	gMsg = this->MakeInitPosBroadcast();
	this->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);
	
	MakePlayerNewPosToAny();
}

/*
GAME_MSG_LOGON_SYNCPID = 1,	//ͬ�����id������  s->c
GAME_MSG_TALK_CONTENT = 2,	//������Ϣ
GAME_MSG_NEW_POSTION = 3,	//ͬ�����λ��
GAME_MSG_BROADCAST = 200,   //�㲥��Ϣ
GAME_MSG_LOGOFF_SYNCPID = 201,//���������Ϣ   
GAME_MSG_SUR_PLAYER = 202,  //ͬ���ܱ������Ϣ  s->c
*/
void Irole::Irole_RecvMsgFromProtocol(GameMsg& _iMsg)
{
	GameMsg& tmpMsg = _iMsg;
	//����ÿһ��tlv��
	for (auto& it : tmpMsg.mGameMsgList)
	{
		switch (it->m_MsgType)
		{
		case GameSingleTLV::GAME_MSG_TALK_CONTENT:
		{
			auto iPlayTalk = dynamic_cast<pb::Talk*>(it->m_pbMsg);
			cout << iPlayTalk->content() << endl;
			this->ProcTalkContent(iPlayTalk->content());
			break;
		}
		case GameSingleTLV::GAME_MSG_NEW_POSTION:
		{
			auto iPlayPos = dynamic_cast<pb::Position*>(it->m_pbMsg);
			cout << "x:" << iPlayPos->x() << " y:" << iPlayPos->y() << " z:" << iPlayPos->z() << " v:" << iPlayPos->v() << endl;
			this->ProcNewPos(iPlayPos->x(), iPlayPos->y(), iPlayPos->z(), iPlayPos->v());
			break;
		}
		default:
			;
		}
	}
	//�������������а�
	_iMsg.GameMsg_FreePackage();

}

//����������λ�ñ������͸������
void Irole::MakePlayerNewPosToAny()
{
	auto poSyzinx = SyZinxKernel::mSyZinx;
	//�㲥�������
	for (auto& it : poSyzinx->IRole_Map)
	{
		if (it.second == this)
		{
			continue;
		}
		auto poRole = it.second;
		/*�����㲥����λ����Ϣ*/
		auto gMsg = MakeInitPosBroadcast();
		poRole->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);
	}
#if 0
	
	for (auto& it : poSyzinx->IRole_Map)
	{
		if (it.second == this)
		{
			continue;
		}
		usleep(500000);
		auto poRole = it.second;
		/*�����㲥����λ����Ϣ*/
		GameMsg* tmpMsg = new GameMsg;
		auto gMsg = poRole->MakeInitPosBroadcast();
		this->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);
	}
	
#else

	//����Ҫ������ҷ��͸������
	//

	if (poSyzinx->IRole_Map.size() > 0)
	{
		pb::SyncPlayers* players = new pb::SyncPlayers;
		for (auto& it : poSyzinx->IRole_Map)
		{
			if (it.second == this)
			{
				continue;
			}
			std::cout << "old to new" << std::endl;
			auto poRole = it.second;
			//��protobuf ����������ӳ�Ա,ʹ��add_xxx  �ᴴ��һ����Ա�����ҷ��ظö����ָ��
			auto player = players->add_ps();
			player->set_pid(poRole->id);
			player->set_username(poRole->name);
			//����һ��position����,���ظö����ָ��
			auto pos = player->mutable_p();
			pos->set_x(poRole->x);
			pos->set_y(poRole->y);
			pos->set_z(poRole->z);
			pos->set_v(poRole->v);
			std::cout << poRole->x << std::endl;
			std::cout << poRole->y << std::endl;
			std::cout << poRole->z << std::endl;
			std::cout << poRole->v << std::endl;
		}
		GameMsg* _igMsg = new GameMsg;
		GameSingleTLV* singTlv = new GameSingleTLV(GameSingleTLV::GAME_MSG_SUR_PLAYER, players);
		_igMsg->mGameMsgList.push_back(singTlv);

		/*�����㲥����λ����Ϣ*/
		this->mLinkProtocol->Iprotocol_SendEncode_ByteString(_igMsg);
	}

#endif
}

/*��������ʱ��id��������Ϣ*/
GameMsg* Irole::MakeLogonSyncPid()
{
	
	pb::SyncPid* sync = new pb::SyncPid;
	sync->set_pid(this->id);
	sync->set_username(this->name);
	auto singTlv = new GameSingleTLV(GameSingleTLV::GAME_MSG_LOGON_SYNCPID, sync);
	auto gMsg = new GameMsg;
	gMsg->mGameMsgList.push_back(singTlv);
	return gMsg;
}
/*�����㲥������Ϣ*/
GameMsg* Irole::MakeTalkBroadcast(std::string _talkContent)
{
	pb::BroadCast* PlayerrTalk = new pb::BroadCast;
	PlayerrTalk->set_pid(this->id);
	PlayerrTalk->set_username(this->name);
	PlayerrTalk->set_tp(1);
	PlayerrTalk->set_content(_talkContent);
	GameSingleTLV* singTlv = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST, PlayerrTalk);
	auto gMsg = new GameMsg;
	gMsg->mGameMsgList.push_back(singTlv);
	return gMsg;

}
/*�����㲥����λ����Ϣ*/
GameMsg* Irole::MakeInitPosBroadcast()
{
	
	pb::BroadCast* PlayerPos = new pb::BroadCast;
	PlayerPos->set_pid(this->id);
	PlayerPos->set_username(this->name);
	PlayerPos->set_tp(2);
	auto pos = PlayerPos->mutable_p();
	pos->set_x(this->x);
	pos->set_y(this->y);
	pos->set_z(this->z);
	pos->set_v(this->v);
	GameSingleTLV* singTlv = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST, PlayerPos);
	auto gMsg = new GameMsg;
	gMsg->mGameMsgList.push_back(singTlv);
	return gMsg;
}

/*�����㲥�ƶ�����λ����Ϣ*/
GameMsg* Irole::MakeNewPosBroadcast()
{
	//������ƶ�ʱ�����ƶ���Ϣ
	pb::BroadCast* bc = new pb::BroadCast;
	bc->set_pid(this->id);
	bc->set_username(this->name);
	bc->set_tp(4);
	//����һ��pos
	auto pos = bc->mutable_p();
	pos->set_x(this->x);
	pos->set_y(this->y);
	pos->set_z(this->z);
	pos->set_v(this->v);
	GameSingleTLV* singTlv = new GameSingleTLV(GameSingleTLV::GAME_MSG_BROADCAST, bc);
	auto gMsg = new GameMsg;
	gMsg->mGameMsgList.push_back(singTlv);
	return gMsg;

}
/*��������ʱ��id��������Ϣ*/
GameMsg* Irole::MakeLogoffSyncPid()
{

}
/*������Χ���λ����Ϣ*/
GameMsg* Irole::MakeSurPlays()
{

}

void Irole::ProcRandPos()
{
	this->x = gRandomEngine() % 20 + 100;
	this->y = 0;
	this->z = gRandomEngine() % 20 + 100;
	this->v = 0;
	this->id = namepid += 1;
	this->name = "player"+to_string(this->id);
}

void Irole::ProcNewPos(float _x, float _y, float _z, float _v)
{
	auto poSyzinx = SyZinxKernel::mSyZinx;
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->v = _v;

	//��λ�÷��͸������Լ�����������
	if (poSyzinx->IRole_Map.size() > 0)
	{
		for (auto& it : poSyzinx->IRole_Map)
		{
			if (it.second == this)
				continue;

			auto poRole = it.second;
			/*��������ƶ���Ϣ*/
			auto gMsg = this->MakeNewPosBroadcast();
			poRole->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);

		}
	}
}
//������Ϣ  �������������Ϣ�ĺ���
void Irole::ProcTalkContent(std::string szContent)
{
	auto poSyzinx = SyZinxKernel::mSyZinx;
	//����
	for (auto& it : poSyzinx->IRole_Map)
	{
		auto poRole = it.second;
		auto gMsg = this->MakeTalkBroadcast(szContent);
		poRole->mLinkProtocol->Iprotocol_SendEncode_ByteString(gMsg);
	}
}

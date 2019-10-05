#pragma once
#include "SyAdapter.h"

//��������
class SyinxAdapterMission : public SyinxAdapter
{
public:
	SyinxAdapterMission() {}
	virtual ~SyinxAdapterMission() {}
public:
	//���ý������ݵĿ�
	virtual void RecvData() = 0;
	virtual void SendData() = 0;

};

//����ͨ���㹤��������һϵ�г�ʼ��
class IChannelFactory
{
public:
	IChannelFactory() {}
	~IChannelFactory() {}
};

class IChannel : public SyinxAdapterMission
{
public:
	IChannel() {}
	~IChannel() {}
public:
	virtual void RecvData() override;
	virtual void SendData() override;
};
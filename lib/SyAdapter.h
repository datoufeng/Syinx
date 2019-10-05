#ifndef _SYPTH_H_
#define _SYPTH_H_
#include "SyLog.h"

/*
class SyinxAdapter -> class SyinxAdapterPth              �߳�������
class SyinxAdapter -> class SyinxAdapterMission		   ����������
class SyinxAdapterMission -> class IChannel            ͨ�������ڽ��տͻ��˷���������
class IChannelFactory /                                 ���ڳ�ʼ����һϵ������

*/
class SyinxLog;
class SyinxAdapter
{
public:
	SyinxAdapter();
	virtual ~SyinxAdapter();
	SyinxLog mLog;
};

#define   on     true;
#define   off    false;
struct Switch
{
	void* arg;
	bool state;  //on or off
};
#endif
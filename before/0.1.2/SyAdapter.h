#ifndef _SYPTH_H_
#define _SYPTH_H_
#include "Log.h"
#include "SyDef.h"
/*
class SyinxAdapter -> class SyinxAdapterPth              �߳�������
class SyinxAdapter -> class SyinxAdapterMission		   ����������
class SyinxAdapterMission -> class IChannel            ͨ�������ڽ��տͻ��˷���������
class IChannelFactory /                                 ���ڳ�ʼ����һϵ������

*/


class SyinxKernel;
class SyinxAdapter
{
public:
	SyinxAdapter() {}
	virtual ~SyinxAdapter() {}
	SyinxLog mLog;
};
#endif
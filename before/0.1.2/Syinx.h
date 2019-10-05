#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include<time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <map>
#include <jsoncpp/json/json.h>
#include "Log.h"
#include "SyAdapter.h"
#include "SyDef.h"

/*
һ���̶߳�Ӧһ���¼���
	�ͻ���ѡ����߱���ʼ����Ӧ��ͬ���¼�������io





*/


//����
class IChannel;
class SyinxAdapterMission;
class SyinxAdapterPth;
class SyinxAdapterResource;

#ifndef _Syinx_H_
#define _Syinx_H_

#ifdef USE_JSON_CONFIG
class SyinxConfig {
public:
	SyinxConfig();
	~SyinxConfig();

public:
	//����json�ļ�
	void Make_Msgconfig();
	//��ȡjson�ļ�
	void Reader_Msgconfig();
private:
	//д��json�ļ�
	void Writer_MSgconfig();
public:
	SyinxLog mLog;
};
#endif 

struct SyinxKernelMsg
{
	int mClient_fd;
	struct buffereveant* Client_buffer;
};

//���ڳ�ʼ���ĺ��Ŀ��
class SyinxKernel 
{
	friend class SyinxAdapterPth;
	friend class SyinxRecycle;
	friend class SyinxAdapterResource;
public:
	
	//��ʼ��
	static int SyinxKernel_Init(const short _inPort);


	//���п��(wait)
	static void SyinxKernel_Run();

	//�رտ��
	static void SyinxKernel_Close();

	//�ͻ����˳�ʱ�����ͷſͻ���������,�ͻ����ļ�������Ϊkeyֵ
	static void SyinxKernel_Client_Close(const int mClient_fd);



private:
	//���ڱ���һ��base���
	struct event_base* SyinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* SyinxListen;

	//�ͻ���port
	short _Port;
public:
	//��һ��һ��SyinxAdapterPt�߳�������
	SyinxAdapterPth* mSyPth;
	//��һ������������
	SyinxAdapterMission* mSyMission;
	//��һ����Դ������
	SyinxAdapterResource* mSyResource;
	
public:
	int PthNum = 5;
	//�����������sockaddr_in
	struct Server_Sockaddr {
		std::string Prot;
		std::string sin_addr;
		std::string family;
	}Server_Sockaddr;

public:
	//�Լ���ָ��
	static SyinxKernel* mSyinx;


private: //˽�й��캯��
	SyinxKernel();
	SyinxKernel(SyinxKernel&) = default;
	~SyinxKernel();
public:
	SyinxLog mLog;


};

class SyinxRecycle
{
public:
	SyinxRecycle();
	~SyinxRecycle();
	SyinxLog mLog;
	/*
	1.static std::multimap<int, SyinxKernelMsg*> mZinxMap; �е�SyinxKernelMsg��Ҫ�ͷ�
	2.
	*/

};

#endif 


//json
/*
{

	�����û�:char
	�������˿�:int

	��������Ⱥ����:int
	#����������Group��server������һ��
	Group:(char)��������Ⱥ����{
			server#1:(int)id
			server#2:
	}
}


*/
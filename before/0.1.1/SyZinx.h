#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include<time.h>
#include<event2/event.h>
#include <event2/listener.h>  
#include <event2/bufferevent.h>  
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
#include "Adapter.h"
/*
һ���̶߳�Ӧһ���¼���
	�ͻ���ѡ����߱���ʼ����Ӧ��ͬ���¼�������io





*/
#define USE_JSON_CONFIG
#define CMD_PRINTF_ERR
#define Client_fd	     int

//����
class IChannel;

#ifndef _SYZINX_H_
#define _SYZINX_H_

#ifdef USE_JSON_CONFIG
class SyZinxConfig {
public:
	SyZinxConfig();
	~SyZinxConfig();

public:
	//����json�ļ�
	void Make_Msgconfig();
	//��ȡjson�ļ�
	void Reader_Msgconfig();
private:
	//д��json�ļ�
	void Writer_MSgconfig();
public:
	SyZinxLog mLog;
};
#endif 

struct SyZinxKernelMsg
{
	int mClient_fd;
	struct buffereveant* Client_buffer;
};

//���ڳ�ʼ���ĺ��Ŀ��
class SyZinxKernel 
{
	friend class SyZinxAdapterPth;
	friend class SyZinxRecycle;
	friend class SyZinxAdapterResource;
public:
	
	//��ʼ��
	static int SyZinxKernel_Init(const short _inPort);


	//���п��(wait)
	static void SyZinxKernel_Run();

	//�رտ��
	static void SyZinxKernel_Close();

	//�ͻ����˳�ʱ�����ͷſͻ���������,�ͻ����ļ�������Ϊkeyֵ
	static void SyZinxKernel_Client_Close(const int mClient_fd);



private:
	//���ڱ���һ��base���
	struct event_base* SyZinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	struct evconnlistener* SyZinxListen;

	//�ͻ���port
	short _Port;
public:
	//��һ��һ��SyZinxAdapterPt�߳�������
	SyZinxAdapterPth* mSyPth;
	//��һ������������
	SyZinxAdapterMission* mSyMission;
	//��һ����Դ������
	SyZinxAdapterResource* mSyResource;
	
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
	static SyZinxKernel* mSyZinx;


private: //˽�й��캯��
	SyZinxKernel();
	SyZinxKernel(SyZinxKernel&) = default;
	~SyZinxKernel();
public:
	SyZinxLog mLog;


};

class SyZinxRecycle
{
public:
	SyZinxRecycle();
	~SyZinxRecycle();
	SyZinxLog mLog;
	/*
	1.static std::multimap<int, SyZinxKernelMsg*> mZinxMap; �е�SyZinxKernelMsg��Ҫ�ͷ�
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
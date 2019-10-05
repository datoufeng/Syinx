

#ifndef _SYZINX_H_
#define _SYZINX_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include<time.h>
#include<event.h>
#include <event2/listener.h>  
#include <event2/bufferevent.h>  
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <map>

#include <mysql/mysql.h>
class Irole;
class Iprotocol;
class Ichannel;
class SyZinxKernel;
class SyZinxDatabase;
/*define*/
#define SERVER_EPOLL_EVENT 1024
#define Client_fd          int

//�ݲ�ʹ�����ݿ�
//#define _USE_DATABASE_

//���ڳ�ʼ���ĺ��Ŀ��
class SyZinxKernel {
	friend class MemRecycled;
	friend void SyZinxKernel_Listen_CB(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sock, int socklen, void* arg);
	friend void SyZinxBuffer_Event_Cb(struct bufferevent* buffer, short events, void* arg);
	friend void SyZinxBuffer_RecvData_Cb(struct bufferevent* buffer, void* arg);
	friend void SyZinxBuffer_SendData_Cb(struct bufferevent* buffer, void* arg);
	friend class SyZinxTcpFactory; 
	friend class Irole;
public:
	//��ʼ��
	static int SyZinxKernel_Init(const short _inPort);


	//���п��(wait)
	static void SyZinxKernel_Run();

	//�رտ��
	static void SyZinxKernel_Close();

	//��ȡ��ǰ�ͻ����׽��ֵ�bufferevent
	static struct bufferevent* SyZinxKernel_GetBuffer(int _iclientfd);

	//�ͻ����˳�ʱ�����ͷſͻ���������,�ͻ����ļ�������Ϊkeyֵ
	static void SyZinxKernel_Client_Close(const int mClient_fd);

	//���浱ǰ�������������ӵĵ�һ��datebase����
	static void SyZinxKernel_SaveDatabase(SyZinxDatabase *_iDatabase);

private:
	//���ڱ���һ��base���
	static struct event_base* SyZinxBase;

	//�������ڼ����׽��ֵ�evconnlistener
	static struct evconnlistener* SyZinxListen;

	//�ͻ���port
	short _Port;

	//�������ݿ���
	SyZinxDatabase* Database;

private: //˽�й��캯��
	SyZinxKernel();
	~SyZinxKernel();

private:/*����*/
	void Run();

public:
	//�Լ���ָ��
	static SyZinxKernel* mSyZinx;
private:

	//����ͨ��Э��ҵ��������
	std::map<Client_fd, struct bufferevent*> iBufferevent_Map;
	std::map<Client_fd, Ichannel*> Ichannel_Map;
	std::map<Client_fd, Iprotocol*> IcProtocol_Map;
	std::map<Client_fd, Irole*> IRole_Map;

private:



private:
	bool SyZinxKernelExit = true;
};

#ifdef _USE_DATABASE_
//mysql���ݿ������,��ʵ��(�ݶ��ǵ���ģʽ)
class SyZinxDatabase 
{
#define _HOST_      "192.168.12.217"
#define _USER_      "root"
#define _PASSWD_    "123456"
#define _DATABASE_  "mytest"

public:

	SyZinxDatabase();
	~SyZinxDatabase();

	//��ʼ��һ�����ݿ�
	int SyZinxKernel_MysqlInit();

	//���ӵ����ݿⲢ�趨����
	int SyZinxKernel_ConnetMysql(char* _SetiCharacter);

	//���ӵ��Զ������ݿ�
	int SyZinxKernel_ConnetMysql(char *Host, char* User, char* Passwd , char *Database, char* _SetiCharacter);

	//�ر����ݿ�
	int SyZinxKernel_CloseMysql();

private:
	//��ֵ���ݿ����
	int SyZinxKernel_MysqlSetCharacter(char* _iCharacter);


	MYSQL* mSyZinxSql;
	char  Host[16];
	char User[12];
	char Passwd[12];
	char Database[12];
	
};
#endif

//����վ���ܽ��ں������   ----��ʵ��----
class MemRecycled : public SyZinxKernel
{
public:
	MemRecycled() {}
	~MemRecycled();

private:
	MemRecycled(MemRecycled* _recycler) = delete;
	MemRecycled& operator=(MemRecycled* _recycler) = delete;
};

#endif

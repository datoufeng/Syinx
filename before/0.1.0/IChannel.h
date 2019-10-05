#include "SyZinx.h"
#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#define BUFFSIZE           1024    
class UserData
{
public:
	UserData() {}
	virtual~UserData() {}
	virtual void InString(std::string& _istring) = 0;
	virtual void OutString(std::string& _ostring) = 0;
};

//�ӿͻ��˽��յ�����
class ByteString : public UserData
{
public:
	ByteString(int client_fd) : mClient_Fd(client_fd){}
	~ByteString() {}

	//ͨ���㻺����ƺ���,��ĳЩ����ԭ���¶�дʧ��,�����ݱ����뻺��  ----��ʵ��----
	virtual void InString(std::string &_istring) override;
	virtual void OutString(std::string &_ostring) override;

	//��stringת���޷���char
	char* StringToUnsignedChar(std::string& _outstring);
	//��ӡ��ǰ�ַ���
	void PutInString();
	void PutOutString(std::string &_outString);
	int mClient_Fd;
public:
	std::string _instring;
	std::string _outstring;
};


//zinx����,���ڶ�̬
class IZinxSys
{

public:
	IZinxSys() {}
	virtual~IZinxSys() {}
};

class SysIODirection : public IZinxSys
{

public:
	enum IO_DIC {
		IN = 1,//����
		OUT  //����
	}mio_dic; //IO_DIC mio_dic;
	SysIODirection(IO_DIC _dic) :mio_dic(_dic) {}

};

class SyZinxHandle :public IZinxSys
{

public:
	SyZinxHandle() {}
	virtual ~SyZinxHandle() {}

public:
	/*��Ϣ����������������д�ú���ʵ����Ϣ�Ĵ���������Ҫһ�����ڼ�������ʱ��Ӧ�ô����µ���Ϣ���󣨶Ѷ��󣩲�����ָ��*/
	virtual IZinxSys* InternalHandle(UserData& _Input) = 0;

	/*��ȡ��һ�������ں�������������д�ú���������ָ����ǰ�����ڽ�������һ��������ʲô��ͨ��������Ϣ���󣬿�����Բ�ͬ������зֱ���*/
	virtual SyZinxHandle* GetNextHandler() = 0;
};


class Ichannel : public SyZinxHandle
{
	friend class SyZinxTcpFactory;
	friend class Iprotocol;
public:
	Ichannel(const int _iClient_fd, bufferevent* _ibufferevent);
	~Ichannel();

public:

	//����Ҫ����ҽ��г�ʼ�������ǵ��øú���
	void Ichannel_DataInit();

	//���пɶ��¼�������Ҫ���ô˺����������տɶ����ַ���
	void Ichannel_RecvBuffer(std::string& _istring);

	//�����������ݷ��͵��ͻ���ʱ����
	void Ichannel_SendBuffer(std::string& _istring);

	

	//��Ϣ������������������ʱ�����ݷ��͵�Э������
	virtual IZinxSys* InternalHandle(UserData& _Input)override;

	/*��ȡ��һ�������ں�������������д�ú���������ָ����ǰ�����ڽ�������һ��������ʲô��ͨ��������Ϣ���󣬿�����Բ�ͬ������зֱ���*/
	virtual SyZinxHandle* GetNextHandler()override;
private:

	//���浱ǰ�ͻ���bufferevent
	struct bufferevent* mClient_buffer;

	//�ͻ����׽���
	int mClient_Fd;

	//�������ݵ���
	ByteString* mClient_ByteString;
	//����Э���
	Iprotocol* mLinkProtocol;
	//����ҵ���
	Irole* mLinkRole;
};


class SyZinxTcpFactory
{
public:
	SyZinxTcpFactory(const int _iClient_fd, struct bufferevent* _ibufferevent);
	~SyZinxTcpFactory();

private:
	//���ڴ���һϵ�е�ͨ�� Э�� ҵ���
	 int SyZinxTcpFactory_Init(const int _iClient_fd, struct bufferevent* _ibufferevent);


};

#endif 
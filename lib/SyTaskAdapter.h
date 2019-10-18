#include "SyAdapter.h"

#ifndef _SYTASKADAPTER_H_
#define _SYTASKADAPTER_H_
class SyinxAdapterResource;
struct IChannelMsg
{
	
	int             Socket;                  /*�����׽���*/
	bufferevent*    buffer;                  /*����buffer*/

	int             ClientID;                /*�ͻ���Ψһid*/
    
};
//��������
class SyinxAdapterMission : public SyinxAdapter
{
public:
	SyinxAdapterMission() {}
	virtual ~SyinxAdapterMission() {}
public:
	
	//���ý������ݵĿ�
	virtual void RecvData() { return; }
	virtual void SendData() { return; }

};

//����ͨ���㹤��������һϵ�г�ʼ��
class IChannelFactory
{
public:
	IChannelFactory() {}
	~IChannelFactory() {}
};

//ͨ������������Ӧ����
struct StringByte 
{
	std::string _InStr;
	int _InSize;

};
class IChannel : public SyinxAdapterMission
{
	friend class SyinxAdapterResource;
public:
	IChannel();
	virtual ~IChannel();
	//��ȡһ�����ݷ��õ�string---�ѷ���
	virtual void RecvData() override;
	//����һ������---------------�ѷ���
	virtual void SendData() override;
public:
	
	/*
	@   -��ȡ��ǰȫ���������ݵ�string
	@arg:�����κ�Э���ʽ��ȫ�����ݶ�ȡstring�����ռ����
	@�ɹ����ض�ȡ�������ݳ���,ʧ�ܷ���-1 
	*/
	int RecvAllDataToString(std::string &arg);

	/*
	@   -��Э��ĸ�ʽ�������ݵ�string   len  type  values
	@arg:���κ�Э���ʽ��values�����ݶ�ȡstring�����ռ����
	@OutLen:�ɹ���ȡ�������ݵĳ���
	@Type:�ɹ���ȡ������������
	@str:�ɹ���ȡ��������
	@�ɹ�������1�Ҵ�ʱ˵����Ȼ�����ݿɶ�
	@����0ʱ˵����ʣ�����ݿɶ�
	@ʧ�ܷ���-1
	*/
	int RecvValuesToString(unsigned int* _OutLen, unsigned int* _OutType, std::string& _OutStr);


	/*
	@   -ֱ�ӷ��͵�ǰstring���ÿͻ���
	@instr:ֱ�ӽ���ǰ�ַ������͵���ǰ�ͻ���
	@�ɹ����ط��͵����ݳ���,ʧ�ܷ���-1
	*/
	int SendAllDataToString(std::string &_InStr);

	/*
	@   -��Э���ʽת����Asn.1��string���ݰ������� ( len  type  values)
	@Inlen:�������ݰ��ĳ���
	@InType:�������ݱ�������(����ָ�����п���)
	@InStr:��Ҫ���͵����ݱ�
	@�ɹ����ط������ݰ��ĳ���,ʧ�ܷ���-1
	*/
	int SendValuesToString(unsigned int _InLen, unsigned int _InType, std::string& _InStr);


	//������һ�������ڵ��ָ��
	void* _NextNode;

	/*mysql������*/
#ifdef SYINXMOD_ADD_MYSQL

#endif 

	//���ݻ���ṹ��
	StringByte* StrByte;

	//����ͨ���������
	IChannelMsg* mICMsg;
private:
	//��ʼ��ͨ����
	int ICannel_Init(IChannelMsg* Info);
	//����ͨ����
	void IChannel_free();

	//����ȡ�������ݷŵ�������
	int DatatoInStrByte(char* buf);

	
private://����Դ������
	SyinxAdapterResource* mICnSaveRes;
};

#endif
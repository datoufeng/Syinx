#include "Syinx.h"
#include "SyTaskAdapter.h"

#include <iostream>
using namespace std;

//�û�����ָ���ù��캯����Ҫ��ʼ����������������Щ����
IChannel::IChannel()
{
}
//�û�����ָ�����������캯����Ҫ�ͷ���Щ����
IChannel::~IChannel()
{
}
/*
@  -�û���Ҫ�ڸó�ʼ������,��Ϊ�ͻ��˵ĳ�ʼ������
@ÿ���ͻ������ӷ�������ʱ����Ĭ�ϵ��øú���
*/
int IChannel::IChannelTaskInit()
{
	std::string _send = "hello world";
	this->SendAllDataToString(_send);
}
/*
@  -�û���Ҫ�ڸ��������̴�������д�����������滮����������
@ÿ���ͻ��˷�������ʱ������øú���
*/
int IChannel::IChannelTaskProcessing()
{
	string Test;
	this->RecvAllDataToString(Test);
	cout << Test << endl;
	
}
int main()
{
	SyinxKernelWork a(8855);
	
	return a.SyinxClose();
}

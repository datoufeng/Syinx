#ifndef _SYCONFIG_H_
#define _SYCONFIG_H_ 
class SyinxLog;
struct SyinxConfMsg {

	std::string Host;                //ip
	std::string Port;                //�˿�
	int    PthNum;             //����epoll������
	int    TaskNum;             //����epoll������


	int    Timerinterval;       //���ü�ʱ������ʱ��
	int    Timervalue;          //��ʱ����һ���ӳ��¼�

};
class SyinxConfig {
public:
	SyinxConfig();
	~SyinxConfig();

public:
	//��ȡ�����ļ�
	SyinxConfMsg* Read_Msgconfig();

public:
	SyinxLog mLog;
};

#endif

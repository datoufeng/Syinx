#ifndef _SYCONFIG_H_
#define _SYCONFIG_H_ 
class SyinxLog;
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

#pragma once
#include "SyDef.h"
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

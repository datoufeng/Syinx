#include "SyZinx.h"
#include <fstream>
#include <istream>
#include <iostream>
using namespace std;

#ifdef CMD_PRINTF_ERR
int use_cmd_printf_log(const char* _errtext);
#endif
int use_cmd_printf_log(const char* _errtext)
{
	cout << _errtext << endl;
}


SyZinxConfig::SyZinxConfig()
{
}
SyZinxConfig::~SyZinxConfig()
{
}
void SyZinxConfig::Make_Msgconfig()
{
	auto Dir = opendir("./conf");
	if (Dir == NULL)
	{
		int DirVal = S_IRUSR | S_IWUSR | S_IXUSR ;
		mkdir("./conf", DirVal);
	}

	do
	{
		int iRet = access("./conf/SyZinx.json", F_OK);
		if (iRet <= 0)
		{
			mLog.Log(__FILE__, __LINE__, SyZinxLog::ERROR, -1, "SyZinx.json not find");
			use_cmd_printf_log("json is not find");
			break;
		}
		else
		{
			//��ȡconf
			this->Reader_Msgconfig();
			cout << "json find" << endl;
			return;
		}
	} while (0);
	//û�оͳ�ʼ��һ�������ļ�
	int File_Fd = open("./conf/SyZinx.json", O_WRONLY | O_CREAT);
	if (File_Fd <= 0)
	{
		//log
		return;
	}
	close(File_Fd);
	/*
	{

	�����û�:char
	�������˿�:int//Ĭ�ϰ�8800�˿�

	��������Ⱥ����:int
	#����������Group��server������һ��
	Group:(char)��������Ⱥ����{
			server#1:(int)id
			server#2:
	}
}*/
	Json::Value root;
	root["User"] = Json::Value("����");
	root["Port"] = Json::Value(8080);
	root["Cluster number"] = Json::Value(1);
	
	
	root["Group Name"] = Json::Value("admin");
	//��Ⱥ�ӽڵ�
	Json::Value Group;
	Group["server#1"] = Json::Value(1001);
	root["Group"] = Json::Value(Group);


	cout << "FastWriter:" << endl;
	Json::FastWriter fw;
	cout << fw.write(root) << endl << endl;

	//д��
	Json::StyledWriter sw;
	ofstream WriteJson;
	WriteJson.open("./conf/SyZinx.json");
	WriteJson << sw.write(root);
	WriteJson.close();
	closedir(Dir);
	

}
void SyZinxConfig::Reader_Msgconfig()
{
}

void SyZinxConfig::Writer_MSgconfig()
{
}

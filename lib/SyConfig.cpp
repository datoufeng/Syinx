
#include <fstream>
#include <istream>
#include <iostream>
#include<time.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include "SyLog.h"
#include "SyConfig.h"
using namespace std;




SyinxConfig::SyinxConfig()
{
}
SyinxConfig::~SyinxConfig()
{
}
void SyinxConfig::Make_Msgconfig()
{
	auto Dir = opendir("./conf");
	if (Dir == NULL)
	{
		int DirVal = S_IRUSR | S_IWUSR | S_IXUSR ;
		mkdir("./conf", DirVal);
	}

	do
	{
		int iRet = access("./conf/Syinx.json", F_OK);
		if (iRet <= 0)
		{
			mLog.Log(__FILE__, __LINE__, SyinxLog::ERROR, -1, "Syinx.json not find");
			
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
	int File_Fd = open("./conf/Syinx.json", O_WRONLY | O_CREAT);
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
	WriteJson.open("./conf/Syinx.json");
	WriteJson << sw.write(root);
	WriteJson.close();
	closedir(Dir);
	

}
void SyinxConfig::Reader_Msgconfig()
{
}

void SyinxConfig::Writer_MSgconfig()
{
}

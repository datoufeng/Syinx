#ifndef _SYTASKADAPTER_H_
#define _SYTASKADAPTER_H_
#include <vector>
class SyinxLog;
class SyinxAdapterResource;
class IChannel;
class CPlayer;
struct bufferevent;

class IChannel 
{
	friend void SyinxKernel_Recv_Cb(struct bufferevent* bev, void* ctx);
	friend class SyinxAdapterResource;
public:
	enum ICNANNEL_LINK_STATUS
	{
		CLIENT_LOGOUT = 1,
		CLIENT_LOGIN = 2,
	};
private:
	/*
	* ��ǰICh�������һ��Socket�׽���
	*/
	int					 m_Socket;

	/*
	* �������һ���ͻ���ID
	*/
	int					 m_ClientID;

	/*
	* ���ͻ��˷����һ��libevent  bufferevent ����
	*/
	bufferevent*		 m_buffer;

	/*
	* ��ҵ�ָ��
	*/
	CPlayer* m_pPlayer;

	/*
	* ��ҵ�Frame����
	*/
	std::vector<string>	 m_ClientBuffer;

	typedef bool (IChannel::*RecvBufferParse)(std::string& _Instr, uint32_t& _OutLen, uint32_t& _OutType, std::string& _OutStr);
	RecvBufferParse ParseFunc;

	typedef bool (IChannel::*Callalbe)(uint32_t& _InLen, uint32_t& _InType, std::string& _InStr);
	Callalbe		CallFunc;

	bool DoNotBindParse;
public:
	IChannel();
	~IChannel();
	/*
	*��ʼ��һ��ͨ����
	*/
	bool Initialize();

	/*
	*���ͻ�������ʱ����һ��buffer
	*/
	int  OnClientConnect(int _fd, bufferevent* buffer, int m_ClientID = 0);

	/*
	* ��ȡ���߼�����
	*/
	void OnStatusDoAction();

	/*
	*	������߿��,�󶨽��յ����߼�֡����εĽ��� 
	*
	*/
	void SetupFrameInputFunc();

	/*
	* ��ȡ�ͻ��˷�����Frame���뵽������
	*/
	void GetClientFrameOnBuffer(std::string& _instr);

	/*
	* �鿴�Ƿ��û����˽�������
	*/
	inline bool GetWhetherBindParseFunc();



	//Э�����
	bool RecvValuesFromString(string& _InStr, uint32_t& _OutLen, uint32_t& _OutType, std::string& _OutStr);
	bool RecvClientPack(uint32_t& _InLen, uint32_t& _InType, std::string& _InStr);

	bool SendValuesToString(uint32_t _InLen, uint32_t _InType, std::string& _InStr);

	
	/*
	* ���ͻ����˳�ʱ���øú�����������ǰ�Ļ���
	*/
	bool Clear();

	/*
	
	*/
	int					GetUniqueID()const;
	int					GetSocket()const;
	bufferevent*		GetBuffer();
	CPlayer*			GetCPlayer();
	int					IChStatus;
private:
	/*
	*�󶨴���Ļ��߽����ĺ�����Ҫ�û��Լ��������ȥ����
	bool RecvBufferParse(std::string& _Instr, uint32_t& _OutLen, uint32_t& _OutType, std::string& _OutStr);
	_InStr��һ�������ԭʼ�ַ���
	_OutLen�ǽ�����󴫳��ĳ���
	_OutType�ǽ�����󴫳�������
	_OutStr�ǽ�����󴫳���IO��

	����ֵ����:IO��������п��ܻᷢ��ճ��,���һ��InStr�а��������߼�֡,��ô�ڽ��������һ��֡�������߿�ܷ���false,������ճ������
	����϶��ᷢ����ѭ��

	bool Callalbe(uint32_t& _InLen, uint32_t& _InType, std::string& _InStr)
	//��������IO�󽫻����Callalbe����֮ǰ�ĵ���˳����д���

	*/
	void BindParse(bool (IChannel::*RecvBufferParse)(std::string& _Instr, uint32_t& _OutLen, uint32_t& _OutType, std::string& _OutStr), bool (IChannel::* Callalbe)(uint32_t& _InLen, uint32_t& _InType, std::string& _InStr));
	
};

#endif

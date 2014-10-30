#include "define.h"
#include "ClientMessage.h"



//���������ʵ�ּ�ظ���ָ����PSS���ݵ�״̬��
//�����յ������ݣ����з��������ϣ����ƻ���һ��

enum EM_EVENT
{
	EM_EVENT_COMMAND_MONITORINFO = 0,
	EM_EVENT_COMMAND_FINISH,
};

class CClientMessage : public IClientMessage
{
public:
	CClientMessage() { m_nServerID = 0; };
	virtual ~CClientMessage() {};

	void ReConnect(int nServerID)
	{
		OUR_DEBUG((LM_INFO, "[ReConnect]nServerID=%d.\n", nServerID));
		m_nServerID = nServerID;
	}

	bool RecvData(ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_INFO, "[RecvData]nServerID=%d, size=%d.\n", m_nServerID, mbRecv->length()));

		return true;
	}

	bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_INFO, "[ConnectError]nServerID=%d, errno=%d.\n", m_nServerID, nError));
		return true;
	}

	int GetServerID()
	{
		return m_nServerID;
	}

private:
	ACE_Message_Block* m_mbRecv;
	int                m_nServerID;
};

class MonitorFSM
{
public:
	MonitorFSM();
	~MonitorFSM();

	IClientMessage* GetClientMessage(); 

	int GetServerID();
	char* GetKey();

	void Init(const char* pIP, uint32 u4Port, const char* pKey);

private:
	CClientMessage* m_pClientMessage;
	char            m_szIP[MAX_BUFF_50];
	uint32          m_u4Port;
	char            m_szKey[MAX_BUFF_50];
};
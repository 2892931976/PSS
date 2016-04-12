#ifndef _SERVERMESSAGETASK_H
#define _SERVERMESSAGETASK_H

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "IClientManager.h"
#include "MessageBlockManager.h"

//�����������������ݰ����̴���
//������������̴߳�������ˣ��᳢����������
//add by freeeyes

#define MAX_SERVER_MESSAGE_QUEUE 1000    //���������г���
#define MAX_DISPOSE_TIMEOUT      30      //�������ȴ�����ʱ��  

//��������ͨѶ�����ݽṹ�����հ���
struct _Server_Message_Info
{
	IClientMessage*    m_pClientMessage;
	uint16             m_u2CommandID;
	ACE_Message_Block* m_pRecvFinish;
	_ClientIPInfo      m_objServerIPInfo;

	_Server_Message_Info()
	{
		m_u2CommandID    = 0;
		m_pClientMessage = NULL;
		m_pRecvFinish    = NULL;
	}
};

//�����������ݰ���Ϣ���д������
class CServerMessageTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CServerMessageTask();
	~CServerMessageTask();

	virtual int open(void* args = 0);
	virtual int svc (void);

	bool Start();
	int  Close();
	bool IsRun();

	uint32 GetThreadID();

	bool PutMessage(_Server_Message_Info* pMessage);

	bool CheckServerMessageThread(ACE_Time_Value tvNow);

	void AddClientMessage(IClientMessage* pClientMessage);

	void DelClientMessage(IClientMessage* pClientMessage);

private:
	bool CheckValidClientMessage(IClientMessage* pClientMessage);
	bool ProcessMessage(_Server_Message_Info* pMessage, uint32 u4ThreadID);

private:
	uint32               m_u4ThreadID;  //��ǰ�߳�ID
	bool                 m_blRun;       //��ǰ�߳��Ƿ�����
	uint32               m_u4MaxQueue;  //�ڶ����е�����������
	EM_Server_Recv_State m_emState;     //����״̬
	ACE_Time_Value       m_tvDispose;   //�������ݰ�����ʱ��

	//��¼��ǰ��Ч��IClientMessage����Ϊ���첽�Ĺ�ϵ��
	//������뱣֤�ص���ʱ��IClientMessage�ǺϷ��ġ�
	typedef vector<IClientMessage*> vecValidIClientMessage;
	vecValidIClientMessage m_vecValidIClientMessage;
};

class CServerMessageManager
{
public:
	CServerMessageManager();
	~CServerMessageManager();

	void Init();

	bool Start();
	int  Close();
	bool PutMessage(_Server_Message_Info* pMessage);
	bool CheckServerMessageThread(ACE_Time_Value tvNow);

	void AddClientMessage(IClientMessage* pClientMessage);
	void DelClientMessage(IClientMessage* pClientMessage);

private:
	CServerMessageTask*         m_pServerMessageTask;
	ACE_Recursive_Thread_Mutex  m_ThreadWritrLock; 
};


typedef ACE_Singleton<CServerMessageManager, ACE_Recursive_Thread_Mutex> App_ServerMessageTask;
#endif

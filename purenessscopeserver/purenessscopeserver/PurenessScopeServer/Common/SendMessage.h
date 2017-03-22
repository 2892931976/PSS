#ifndef _SENDMESSAGE_H
#define _SENDMESSAGE_H

#include "define.h"
#include "IBuffPacket.h"
#include "HashTable.h"

using namespace std;

//����һ���������������������첽���Ͷ���
struct _SendMessage
{
	uint32              m_u4ConnectID;    //Ҫ���͵�Զ��ID
	uint16              m_u2CommandID;    //Ҫ���͵�����ID������ͳ�ƹ���
	uint8               m_u1SendState;    //Ҫ���͵�״̬��0���������ͣ�1���Ȼ��治���ͣ�2���������ͺ�ر�
	bool                m_blDelete;       //������ɺ��Ƿ�ɾ����true��ɾ����false�ǲ�ɾ��
	IBuffPacket*        m_pBuffPacket;    //���ݰ�����
	uint8               m_nEvents;        //�������ͣ�0��ʹ��PacketParse��֯�������ݣ�1����ʹ��PacketParse��֯����
	ACE_Time_Value      m_tvSend;         //���ݰ����͵�ʱ���

	ACE_Message_Block*  m_pmbQueuePtr;    //��Ϣ����ָ���

	int                 m_nHashID;        //��ǰ�����HashID

	_SendMessage()
	{
		m_u1SendState = 0;
		m_blDelete    = true;
		m_u4ConnectID = 0;
		m_nEvents     = 0;
		m_u2CommandID = 0;
		m_nHashID     = 0;

		//����������Ϣ����ģ��ָ�����ݣ������Ͳ��ط�����new��delete����������
		//ָ���ϵҲ����������ֱ��ָ��������ʹ�õ�ʹ����ָ��
		m_pmbQueuePtr  = new ACE_Message_Block(sizeof(_SendMessage*));

		_SendMessage** ppMessage = (_SendMessage**)m_pmbQueuePtr->base();
		*ppMessage = this;

	}

	~_SendMessage()
	{
		if(NULL != m_pmbQueuePtr)
		{
			m_pmbQueuePtr->release();
			m_pmbQueuePtr = NULL;
		}
	}

	ACE_Message_Block* GetQueueMessage()
	{
		return m_pmbQueuePtr;
	}

	void SetHashID(int nHashID)
	{
		m_nHashID = nHashID;
	}

	int GetHashID()
	{
		return m_nHashID;
	}
};

class CSendMessagePool
{
public:
	CSendMessagePool(void);
	~CSendMessagePool(void);

	void Init(int nObjcetCount = MAX_MSG_THREADQUEUE);
	void Close();

	_SendMessage* Create();
	bool Delete(_SendMessage* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	CHashTable<_SendMessage>    m_objHashHandleList;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
};
#endif


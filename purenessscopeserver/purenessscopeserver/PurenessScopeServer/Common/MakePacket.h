#ifndef MAKEPACKET_H
#define MAKEPACKET_H

//ר�Ŵ������ݽ������
//�����ݰ���ƴ�Ӻͽ������һ��
//add by freeeyes
//2010-06-11
#pragma once

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "PacketParsePool.h"
#include "MessageService.h"
#include "ProfileTime.h"
#include "HashTable.h"

using namespace std;

struct _MakePacket
{
	uint8          m_PacketType;     //���ݰ����� 
	uint32         m_u4ConnectID;    //����ID
	uint8          m_u1Option;       //��������
	CPacketParse*  m_pPacketParse;   //���ݰ�����ָ��
	ACE_INET_Addr  m_AddrRemote;     //���ݰ�����ԴIP��Ϣ
	ACE_INET_Addr  m_AddrListen;     //���ݰ���Դ����IP��Ϣ 

	int            m_nHashID;        //��Ӧ��¼hash��ID

	_MakePacket()
	{
		m_PacketType        = PACKET_TCP;  //0ΪTCP,1��UDP Ĭ����TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
		m_nHashID          = 0;
	}

	void Clear()
	{
		m_PacketType        = PACKET_TCP;  //0ΪTCP,1��UDP Ĭ����TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
		m_pPacketParse      = NULL;
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

class CMakePacket
{
public:
	CMakePacket(void);
	~CMakePacket(void);

	bool Init();

	bool PutMessageBlock(uint32 u4ConnectID, _MakePacket* pMakePacket, ACE_Time_Value& tvNow);                 //����TCP���ݰ�
	bool PutUDPMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow);                                  //����UDP���ݰ� 
	bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, ACE_Time_Value& tvNow);                      //����ʧ����Ϣ�ص� 

private:
	void SetMessage(_MakePacket* pMakePacket, CMessage* pMessage, ACE_Time_Value& tvNow);                                              //һ�����ݰ���Ϣ
	void SetMessageConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                              //�û��������ݰ���Ϣ
	void SetMessageCDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //�û��Ͽ��������ݰ���Ϣ
	void SetMessageSDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //�������Ͽ��������ݰ���Ϣ
	void SetMessageSendTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                          //���������͸��ͻ������ݳ�ʱ��Ϣ
	void SetMessageCheckTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow);                                         //������������ʱ��Ϣ 
	void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, ACE_Time_Value& tvNow);           //������ʧ�ܻص����ݰ���Ϣ

	bool ProcessMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow);                                   //��ɶ�����Ϣ
private:
	ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket; 

#endif

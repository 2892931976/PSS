#include "MakePacket.h"


CMakePacketPool::CMakePacketPool()
{
}

CMakePacketPool::~CMakePacketPool()
{
	OUR_DEBUG((LM_INFO, "[CMakePacketPool::~CMakePacketPool].\n"));
	Close();
}

void CMakePacketPool::Init(uint32 u4PacketCount)
{
	Close();

	//��ʼ��HashTable
	int nKeySize = 10;
	size_t nArraySize = (sizeof(_Hash_Table_Cell<_MakePacket>) + nKeySize + sizeof(_MakePacket* )) * u4PacketCount;
	char* pHashBase = new char[nArraySize];
	m_objHashHandleList.Set_Base_Addr(pHashBase, (int)u4PacketCount);
	m_objHashHandleList.Set_Base_Key_Addr(pHashBase + sizeof(_Hash_Table_Cell<_MakePacket>) * u4PacketCount, 
																	nKeySize * u4PacketCount, nKeySize);
	m_objHashHandleList.Set_Base_Value_Addr(pHashBase + (sizeof(_Hash_Table_Cell<_MakePacket>) + nKeySize) * u4PacketCount, 
																	sizeof(_MakePacket* ) * u4PacketCount, sizeof(_MakePacket* ));

	for(int i = 0; i < (int)u4PacketCount; i++)
	{
		_MakePacket* pPacket = new _MakePacket();
		if(NULL != pPacket)
		{
			//��ӵ�Hash��������
			pPacket->SetHashID(i);
			char szPacketID[10] = {'\0'};
			sprintf_safe(szPacketID, 10, "%d", i);
			m_objHashHandleList.Add_Hash_Data(szPacketID, pPacket);
		}
	}
	m_u4CulationIndex = 1;
}

void CMakePacketPool::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(int i = 0; i < m_objHashHandleList.Get_Count(); i++)
	{
		_MakePacket* pPacket = m_objHashHandleList.Get_Index(i);
		SAFE_DELETE(pPacket);
	}
	m_u4CulationIndex = 1;
}

int CMakePacketPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return m_objHashHandleList.Get_Count() - m_objHashHandleList.Get_Used_Count();
}

int CMakePacketPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return m_objHashHandleList.Get_Used_Count();
}

_MakePacket* CMakePacketPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	_MakePacket* pPacket = NULL;

	//��Hash���е���һ����ʹ�õ�����
	//�ж�ѭ��ָ���Ƿ��Ѿ��ҵ��˾�ͷ����������0��ʼ����
	if(m_u4CulationIndex >= (uint32)(m_objHashHandleList.Get_Count() - 1))
	{
		m_u4CulationIndex = 0;
	}

	//��һ��Ѱ�ң��ӵ�ǰλ��������
	for(int i = (int)m_u4CulationIndex; i < m_objHashHandleList.Get_Count(); i++)
	{
		pPacket = m_objHashHandleList.Get_Index(i);
		if(NULL != pPacket)
		{
			//�Ѿ��ҵ��ˣ�����ָ��
			int nDelPos = m_objHashHandleList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CMakePacketPool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szHandlerID, i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			return pPacket;
		}
	}

	//�ڶ���Ѱ�ң���0����ǰλ��
	for(int i = 0; i < (int)m_u4CulationIndex; i++)
	{
		pPacket = m_objHashHandleList.Get_Index(i);
		if(NULL != pPacket)
		{
			//�Ѿ��ҵ��ˣ�����ָ��
			int nDelPos = m_objHashHandleList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CMakePacketPool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", pPacket->GetHashID(), i, nDelPos, pPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CProConnectHandlerPool::Create]szHandlerID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szHandlerID, i, nDelPos, pHandler));
			}
			m_u4CulationIndex = i;
			return pPacket;
		}
	}

	//û�ҵ������
	return pPacket;
}

bool CMakePacketPool::Delete(_MakePacket* pMakePacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pMakePacket)
	{
		return false;
	}

	char szHandlerID[10] = {'\0'};
	sprintf_safe(szHandlerID, 10, "%d", pMakePacket->GetHashID());
	//int nPos = m_objHashHandleList.Add_Hash_Data(szHandlerID, pObject);
	int nPos = m_objHashHandleList.Set_Index(pMakePacket->GetHashID(), szHandlerID, pMakePacket);
	if(-1 == nPos)
	{
		OUR_DEBUG((LM_INFO, "[CMakePacketPool::Delete]szHandlerID=%s(0x%08x) nPos=%d.\n", szHandlerID, pMakePacket, nPos));
	}
	else
	{
		//OUR_DEBUG((LM_INFO, "[CMakePacketPool::Delete]szHandlerID=%s(0x%08x) nPos=%d.\n", szHandlerID, pMakePacket, nPos));
	}

	return true;
}


//*********************************************************************

CMakePacket::CMakePacket(void)
{
}

CMakePacket::~CMakePacket(void)
{
	OUR_DEBUG((LM_INFO, "[CMakePacket::~CMakePacket].\n"));
}

bool CMakePacket::Init()
{
	m_MakePacketPool.Init(MAX_PACKET_PARSE);
	return true;
}


bool CMakePacket::PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, _MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutUDPMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_AddrRemote        = AddrRemote;

	ProcessMessageBlock(pMakePacket, tvNow);

	return true;
}

bool CMakePacket::PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, _MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutMessageBlock] Get pMakePacket is NULL.\n"));
		return false;
	}

	pMakePacket->m_u4ConnectID       = u4ConnectID;
	pMakePacket->m_u1Option          = u1Option;
	pMakePacket->m_PacketType        = PACKET_TCP;

	ProcessMessageBlock(pMakePacket, tvNow);

	return true;
}

bool CMakePacket::ProcessMessageBlock(_MakePacket* pMakePacket, ACE_Time_Value& tvNow)
{
	if(NULL == pMakePacket)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMakePacket is NULL.\n"));
		return false;
	}

	//���ݲ���OP��������Ӧ�ķ�����
	CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(pMakePacket->m_u4ConnectID, pMakePacket->m_PacketType);
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR,"[CMakePacket::ProcessMessageBlock] pMessage is NULL.\n"));
		return false;
	}
	
	if(pMakePacket->m_u1Option == PACKET_PARSE)
	{
		if(pMakePacket->m_PacketType == 0)
		{
			//TCP���ݰ�������
			//��¼IP
			sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrRemote.get_host_addr());
			pMessage->GetMessageBase()->m_u4Port = (uint32)pMakePacket->m_AddrRemote.get_port_number(); 
			sprintf_safe(pMessage->GetMessageBase()->m_szListenIP, MAX_BUFF_20, "%s", pMakePacket->m_AddrListen.get_host_addr());
			pMessage->GetMessageBase()->m_u4ListenPort = (uint32)pMakePacket->m_AddrListen.get_port_number(); 

			SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_u4ConnectID, pMessage, tvNow);
		}
		else
		{
			//UDP���ݰ�������
			SetMessage(pMakePacket->m_pPacketParse, pMakePacket->m_AddrRemote, pMessage, tvNow);
		}		
	}
	else if(pMakePacket->m_u1Option == PACKET_CONNECT)
	{
		SetMessageConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_CDISCONNECT)
	{
		SetMessageCDisConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_SDISCONNECT)
	{
		SetMessageSDisConnect(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_SEND_TIMEOUT)
	{
		SetMessageSendTimeout(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}
	else if(pMakePacket->m_u1Option == PACKET_CHEK_TIMEOUT)
	{
		SetMessageSendTimeout(pMakePacket->m_u4ConnectID, pMessage, tvNow);
	}

	//��Ҫ�������Ϣ������Ϣ�����߳�
	if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::ProcessMessageBlock] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
		App_MessageServiceGroup::instance()->DeleteMessage(pMakePacket->m_u4ConnectID, pMessage);
		return false;
	}

	return true;
}

void CMakePacket::SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = UDP_HANDER_ID;
		pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4Port        = (uint32)AddrRemote.get_port_number();
		pMessage->GetMessageBase()->m_u1PacketType  = PACKET_UDP;
		pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
		pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
		sprintf_safe(pMessage->GetMessageBase()->m_szIP, MAX_BUFF_20, "%s", AddrRemote.get_host_addr());

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(pPacketParse->GetMessageHead());
		pMessage->SetPacketBody(pPacketParse->GetMessageBody());
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetMessage] UDP ConnectID, pMessage->GetMessageBase() is NULL.\n"));
	}
}

void CMakePacket::SetMessageConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CONNECT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageCDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_CDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageCDisConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageSDisConnect(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLIENT_LINK_SDISCONNET;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSDisConnect] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageSendTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_SENDTIMEOUT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSendTimeout] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

void CMakePacket::SetMessageCheckTimeout(uint32 u4ConnectID, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = CLINET_LINK_CHECKTIMEOUT;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(NULL);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageCheckTimeout] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}


void CMakePacket::SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, ACE_Time_Value& tvNow)
{
	if(NULL != pMessage->GetMessageBase())
	{
		//��ʼ��װ����
		pMessage->GetMessageBase()->m_u4ConnectID   = u4ConnectID;
		pMessage->GetMessageBase()->m_u2Cmd         = (uint16)CLINET_LINK_SENDERROR;
		pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)tvNow.sec();
		pMessage->GetMessageBase()->m_u4HeadSrcSize = 0;
		pMessage->GetMessageBase()->m_u4BodySrcSize = 0;

		//�����ܵ����ݻ������CMessage����
		pMessage->SetPacketHead(NULL);
		pMessage->SetPacketBody(pBodyMessage);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::SetMessageSendError] ConnectID = %d, pMessage->GetMessageBase() is NULL.\n", u4ConnectID));
	}
}

bool CMakePacket::PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, ACE_Time_Value& tvNow)
{
	CMessage* pMessage = App_MessageServiceGroup::instance()->CreateMessage(u4ConnectID, (uint8)PACKET_TCP);
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMakePacket::PutSendErrorMessage] pMessage is NULL.\n"));
		App_MessageBlockManager::instance()->Close(pBodyMessage);
		return false;
	}
	
	SetMessageSendError(u4ConnectID, pBodyMessage, pMessage, tvNow);
	if(NULL != pMessage)
	{
		//��Ҫ�������Ϣ������Ϣ�����߳�
		if(false == App_MessageServiceGroup::instance()->PutMessage(pMessage))
		{
			OUR_DEBUG((LM_ERROR, "[CMakePacket::PutSendErrorMessage] App_MessageServiceGroup::instance()->PutMessage Error.\n"));
			App_MessageBlockManager::instance()->Close(pBodyMessage);
			App_MessageServiceGroup::instance()->DeleteMessage(u4ConnectID, pMessage);
			return false;
		}
	}
	else
	{
		return false;	
	}

	return true;
}


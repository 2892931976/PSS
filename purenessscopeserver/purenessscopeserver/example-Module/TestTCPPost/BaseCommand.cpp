#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
	CloseClient2Server();
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{	
	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL.\n"));
		return -1;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
		return -1;
	}

	//�������ӽ�����Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
	{
		AddClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
	}

	//�������ӶϿ���Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
	{
		DelClient2Server(pMessage->GetMessageBase()->m_u4ConnectID);
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
	}

	//����������Ϣ
	if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_BASE)
	{
		uint32     u4PacketLen = 0;
		uint16     u2CommandID = 0;
		//VCHARS_STR strsName;
		string     strName;

		IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
		if(NULL == pBodyPacket)
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
			return -1;
		}
		
		_PacketInfo BodyPacket;
		pMessage->GetPacketBody(BodyPacket);

		pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

		(*pBodyPacket) >> u2CommandID;
		//(*pBodyPacket) >> strsName;
		//strName.assign(strsName.text, strsName.u1Len);
		
		//���м������������Ϣ(��������͸��)
		char szPostData[2048] = {'\0'};
		//sprintf_safe(szPostData, MAX_BUFF_100, "hello world");
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CommandID= 0x%04x, m_nDataLen=%d.\n", u2CommandID, BodyPacket.m_nDataLen));
		int nSendSize = BodyPacket.m_nDataLen + sizeof(uint32);
		ACE_OS::memcpy(szPostData, &BodyPacket.m_nDataLen, sizeof(uint32));
		ACE_OS::memcpy(&szPostData[sizeof(uint32)], BodyPacket.m_pData, BodyPacket.m_nDataLen);		

		m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

		//���õ�ǰ�������ݵ�ConnectID�������յ�Զ�̻�Ӧ��Ϣ����
		//m_pPostServerData1->SetConnectID(pMessage->GetMessageBase()->m_u4ConnectID);
		CPostServerData* pPostServerData = GetClient2Server_ServerID(pMessage->GetMessageBase()->m_u4ConnectID);

		if(NULL != pPostServerData)
		{
			if(false == m_pServerObject->GetClientManager()->SendData(pPostServerData->GetServerID(), szPostData, nSendSize, false))
			{
				OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] Send Post Data Error.\n"));
			}
			return 0;
		}
	}


	return 0;
}

void CBaseCommand::InitServer()
{
}

void CBaseCommand::AddClient2Server(uint32 u4ClientID)
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f != m_mapC2S.end())
	{
		return;
	}

	CPostServerData* pPostServerData = new CPostServerData();

	//���÷��ؿͻ�����Ҫ�ķ��Ͷ���
	pPostServerData->SetServerObject(m_pServerObject);
	pPostServerData->SetConnectID(u4ClientID);
	pPostServerData->SetServerID(u4ClientID);

	//��ʼ�����ӹ�ϵ
	m_pServerObject->GetClientManager()->Connect(u4ClientID, "172.21.0.41", 10040, TYPE_IPV4, (IClientMessage* )pPostServerData);

	m_mapC2S.insert(mapc2s::value_type(u4ClientID, pPostServerData));
}

void CBaseCommand::DelClient2Server( uint32 u4ClientID )
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f == m_mapC2S.end())
	{
		return;
	}

	CPostServerData* pPostServerData = (CPostServerData* )f->second;
	if(NULL != pPostServerData)
	{
		uint32 u4ServerID = pPostServerData->GetServerID();
		m_pServerObject->GetClientManager()->Close(u4ServerID);
	}
	m_mapC2S.erase(f);

}

CPostServerData* CBaseCommand::GetClient2Server_ServerID( uint32 u4ClientID )
{
	mapc2s::iterator f = m_mapC2S.find(u4ClientID);
	if(f == m_mapC2S.end())
	{
		return NULL;
	}
	else
	{
		return (CPostServerData* )f->second;
	}

}

void CBaseCommand::CloseClient2Server()
{
	for(mapc2s::iterator b = m_mapC2S.begin(); b != m_mapC2S.end(); b++)
	{
		CPostServerData* pPostServerData = (CPostServerData* )b->second;
		SAFE_DELETE(pPostServerData);
	}
	m_mapC2S.clear();
}

#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
	m_pServerObject = NULL;
	m_nCount        = 0;

	m_szLSIP[0]     = '\0';
	m_u4LSPort      = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
	//__ENTER_FUNCTION���Զ�����ĺ���׷��try�ĺ꣬���ڲ�׽���������ʹ��__THROW_FUNCTION()�׳���Ĵ���
	//����__THROW_FUNCTION("hello"); �����Զ���Log�ļ����µ�assert.log��¼�ļ�������������������������
	//�Ƽ���ʹ������꣬��סһ��Ҫ�ں����Ľ���ʹ��__LEAVE_FUNCTION����__LEAVE_FUNCTION_WITHRETURN��֮��ԡ�
	//__LEAVE_FUNCTION_WITHRETURN��__LEAVE_FUNCTION��ͬ��ǰ�߿���֧�ֺ����˳���ʱ�򷵻�һ������
	//������������Ƿ���һ��int
	//��ôĩβ����__LEAVE_FUNCTION_WITHRETURN(0); 0���Ƿ��ص�int����Ȼ��Ҳ֧�ֱ�����ͱ��������忴���Լ��ĺ�����
	//��������������κα����������ʹ��__LEAVE_FUNCTION���ɡ�
	__ENTER_FUNCTION();

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
	MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
	MESSAGE_FUNCTION(CLIENT_LINK_CONNECT,         Do_Connect,            pMessage);
	MESSAGE_FUNCTION(CLIENT_LINK_CDISCONNET,      Do_DisConnect,         pMessage);
	MESSAGE_FUNCTION(CLINET_LINK_SENDTIMEOUT,     Do_ClientSendTimeout,  pMessage);
	MESSAGE_FUNCTION(COMMAND_LOGIC_CLIENAT_LOGIN, Do_Logic_Client_Login, pMessage);
	MESSAGE_FUNCTION_END;

	return 0;

	__LEAVE_FUNCTION_WITHRETURN(0);
}

int CBaseCommand::Do_Connect(IMessage* pMessage)
{
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect] (%d)TCP CLIENT_LINK_CONNECT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

	//�жϵ�ǰ���������Ƿ�Խ��2000��
	int nConnectCount = m_pServerObject->GetConnectManager()->GetCount();
	if(nConnectCount > 2000)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Connect]connect count is more(%d).\n", nConnectCount));
	}

	return 0;
}

int CBaseCommand::Do_DisConnect(IMessage* pMessage)
{
	//�������ӶϿ��¼�
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLIENT_LINK_CDISCONNET OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

	return 0;
}

int CBaseCommand::Do_ClientSendTimeout(IMessage* pMessage)
{
	//������������Ϳͻ����������ӳ�����ֵ���¼�
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_DisConnect](%d)CLINET_LINK_SNEDTIMEOUT OK.\n", pMessage->GetMessageBase()->m_u4ConnectID));

	return 0;
}

int CBaseCommand::Do_Logic_Client_Login(IMessage* pMessage)
{
	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	if(NULL == pBodyPacket)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
		return -1;
	}

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

	_VCHARS_STR strUserName;
	_VCHARS_STR strUserPass;
	_VCHARS_STR strLSKey;

	(*pBodyPacket) >> strUserName;
	(*pBodyPacket) >> strUserPass;
	(*pBodyPacket) >> strLSKey;

	m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

	uint8  u1Type = 0;  //1��ҪȥLS�����б�0����Ҫ����
	if(ACE_OS::strcmp(strLSKey.text, m_objLSServer.Get_LS_Key()) != 0)
	{
		u1Type = 1;
	}

	//���Ϳͻ�������
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_LOGIC_CLIENAT_LOGIN_R;

	uint32 u4UserID = 0;
	_VCHARS_STR strServerCode;

	char szServerCode[20] = {'\0'};
	sprintf_safe(szServerCode, 20, "%s", LG_SERVER_KEY);
	uint8 u1Len = (uint8)ACE_OS::strlen(szServerCode);
	strServerCode.SetData(szServerCode, u1Len);
	
	//��Ϣ�б���
	uint32 u4SendPacketLen = sizeof(uint32) + sizeof(uint8) + sizeof(uint8) + ACE_OS::strlen(szServerCode);
	(*pResponsesPacket) << pMessage->GetPacketHeadInfo()->m_u2Version;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << u4SendPacketLen; //���ݰ��峤��
	pResponsesPacket->WriteStream(pMessage->GetPacketHeadInfo()->m_szSession, SESSION_LEN);

	(*pResponsesPacket) << u4UserID;
	(*pResponsesPacket) << u1Type;
	(*pResponsesPacket) << strServerCode;

	if(NULL != m_pServerObject->GetConnectManager())
	{
		//����ȫ������
		m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, 
			pResponsesPacket, 
			SENDMESSAGE_JAMPNOMAL, 
			u2PostCommandID, 
			PACKET_SEND_IMMEDIATLY, 
			PACKET_IS_FRAMEWORK_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	}

	//m_pServerObject->GetConnectManager()->CloseConnect(pMessage->GetMessageBase()->m_u4ConnectID);

	return 0;
}

void CBaseCommand::Init(const char* pFileName)
{
	ReadIniFile(pFileName);

	//m_objLSServer.Init(1, "127.0.0.1", 10080, m_pServerObject);
	m_objLSServer.Init(1, m_szLSIP, m_u4LSPort, m_pServerObject);

	vecControlInfo objControlInfo;
	m_pServerObject->GetControlListen()->ShowListen(objControlInfo);

	//�Զ��õ���ǰ���ŵ�IP�Ͷ˿ڣ�Ȼ���¼���������͸�Զ��LS
	m_objLSServer.Set_LG_Info(objControlInfo[0].m_szListenIP, 
							  objControlInfo[0].m_u4Port, 
							  (uint32)m_pServerObject->GetControlListen()->GetServerID());
	m_objLSServer.Connect();

	ActiveTimer* pTimer = m_pServerObject->GetTimerManager();
	m_u4TimeID = (uint32)pTimer->schedule(this, NULL, ACE_OS::gettimeofday() + ACE_Time_Value(MAX_TIMER_INTERVAL), ACE_Time_Value(MAX_TIMER_INTERVAL));
}

int CBaseCommand::handle_timeout( const ACE_Time_Value &tv, const void *arg )
{
	//��ʱ����ʱִ��
	OUR_DEBUG((LM_INFO, "[CBaseCommand::handle_timeout]Run!\n"));
	m_objLSServer.Send_LG_Alive();
	return 0;
}

void CBaseCommand::ReadIniFile(const char* pIniFileName)
{
	dictionary* pDictionary = NULL;
	pDictionary = iniparser_load(pIniFileName);
	if(NULL == pDictionary)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::ReadIniFile](%s)Read Ini fail.\n", pIniFileName));
		return;
	}
	else
	{
		//��ȡIni�ļ�����
		char* pData = iniparser_getstring(pDictionary, "LSServer:IP", NULL);
		if(NULL != pData)
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::ReadIniFile]IP=%s.\n", pData));
			sprintf_safe(m_szLSIP, 50, "%s", pData);
		}

		int nData = iniparser_getint(pDictionary, "LSServer:Port", 0);
		if(0 != nData)
		{
			OUR_DEBUG((LM_INFO, "[CBaseCommand::ReadIniFile]Port=%d.\n", nData));
			m_u4LSPort =(uint32)nData;
		}

		iniparser_freedict(pDictionary);
	}
}

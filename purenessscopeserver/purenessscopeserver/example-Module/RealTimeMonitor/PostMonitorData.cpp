#include "PostMonitorData.h"

CPostServerData::CPostServerData()
{

}

CPostServerData::~CPostServerData()
{

}

void CPostServerData::Init(IMonitorCommand* pMonitorCommand)
{
    m_pMonitorCommand = pMonitorCommand;
}

bool CPostServerData::Need_Send_Format()
{
    return false;
}

bool CPostServerData::Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)
{
    //��ʱ���ô˷���
    ACE_UNUSED_ARG(pData);
    ACE_UNUSED_ARG(u4Len);
    ACE_UNUSED_ARG(pMessageBlockManager);
    ACE_UNUSED_ARG(mbSend);
    return true;
}

bool CPostServerData::Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv)
{
    mbFinishRecv = mbRecv;
    return true;
}

void CPostServerData::ReConnect(int nServerID)
{
    //�������ӶϿ�����Ҫ���·���ע���
    m_pMonitorCommand->SetState(MONITOR_STATE_CONNECT);
}

bool CPostServerData::RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
{
    //�������ݷ��ذ�
    if (mbRecv->length() == sizeof(uint16) + sizeof(uint32))
    {
        char szTemp[MAX_BUFF_50] = { '\0' };
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), szTemp, sizeof(uint32));
        szTemp[sizeof(uint32)] = '\0';
        mbRecv->rd_ptr(sizeof(uint32));
        uint32 u4PacketLength = ACE_OS::atoi(szTemp);
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint16), szTemp, sizeof(uint16));
        szTemp[sizeof(uint16)] = '\0';
        mbRecv->rd_ptr(sizeof(uint16));
        uint16 u2CommandID = ACE_OS::atoi(szTemp);
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), szTemp, sizeof(uint32));
        szTemp[sizeof(uint32)] = '\0';
        mbRecv->rd_ptr(sizeof(uint32));
        uint32 u4State = ACE_OS::atoi(szTemp);

        if (COMMAND_MONITOR_LOGIN_ACK == u2CommandID)
        {
            if (0 == u4State)
            {
                //��¼��֤��ִ�ɹ�
                m_pMonitorCommand->SetState(MONITOR_STATE_LOGINED);
            }
        }
    }

    return true;
}

bool CPostServerData::ConnectError(int nError, _ClientIPInfo objServerIPInfo)
{
    //�������ӶϿ�����
    m_pMonitorCommand->SetState(MONITOR_STATE_DISCONNECT);
    return true;
}


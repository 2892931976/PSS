#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
}

CPacketParsePool::~CPacketParsePool()
{
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool].\n"));
    Close();
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool] End.\n"));
}

void CPacketParsePool::Init(uint32 u4PacketCount)
{
    Close();

    //��ʼ��HashTable
    m_objPacketList.Init(u4PacketCount);
    m_objPacketParseList.Init((int)u4PacketCount);

    for(uint32 i = 0; i < u4PacketCount; i++)
    {
        CPacketParse* pPacket = m_objPacketList.GetObject(i);

        if(NULL != pPacket)
        {
            //��ӵ�HashPool����
            char szPacketID[10] = {'\0'};
            sprintf_safe(szPacketID, 10, "%d", i);
            int nHashPos = m_objPacketParseList.Add_Hash_Data(szPacketID, pPacket);

            if(-1 != nHashPos)
            {
                pPacket->SetHashID(i);
            }
        }
    }
}

void CPacketParsePool::Close()
{
    //���������Ѵ��ڵ�ָ��
    m_objPacketParseList.Close();
}

int CPacketParsePool::GetUsedCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return m_objPacketParseList.Get_Count() - m_objPacketParseList.Get_Used_Count();
}

int CPacketParsePool::GetFreeCount()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    return  m_objPacketParseList.Get_Used_Count();
}

CPacketParse* CPacketParsePool::Create()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    //��Hash���е���һ����ʹ�õ�����
    CPacketParse* pPacketParse = m_objPacketParseList.Pop();

    return pPacketParse;
}

bool CPacketParsePool::Delete(CPacketParse* pPacketParse, bool blDelete)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

    CPacketParse* pBuff = pPacketParse;

    if(NULL == pBuff)
    {
        return false;
    }

    if (true == blDelete)
    {
        //�����ͷ�Ͱ�����ڴ�
        if (NULL != pPacketParse->GetMessageHead())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageHead());
        }

        if (NULL != pPacketParse->GetMessageBody())
        {
            App_MessageBlockManager::instance()->Close(pPacketParse->GetMessageBody());
        }
    }

    pPacketParse->Clear();

    char szHashID[10] = {'\0'};
    sprintf_safe(szHashID, 10, "%d", pPacketParse->GetHashID());
    bool blState = m_objPacketParseList.Push(szHashID, pPacketParse);

    if(false == blState)
    {
        OUR_DEBUG((LM_INFO, "[CPacketParsePool::Delete]HashID=%s(0x%08x).\n", szHashID, pPacketParse));
    }
    else
    {
        //OUR_DEBUG((LM_INFO, "[CSendMessagePool::Delete]HashID=%s(0x%08x) nPos=%d.\n", szHashID, pObject, nPos));
    }

    return true;
}

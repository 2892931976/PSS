#include "PacketParsePool.h"

CPacketParsePool::CPacketParsePool()
{
}

CPacketParsePool::~CPacketParsePool()
{
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool].\n"));
    CObjectPoolManager::Close();
    OUR_DEBUG((LM_INFO, "[CPacketParsePool::~CPacketParsePool] End.\n"));
}

void CPacketParsePool::Init_Callback(int nIndex, CPacketParse* pPacketParse)
{
    pPacketParse->SetHashID(nIndex);
}

void CPacketParsePool::Close()
{
    //���������Ѵ��ڵ�ָ��
    CObjectPoolManager::Close();
}

int CPacketParsePool::GetUsedCount()
{
    return CObjectPoolManager::GetUsedCount();
}

int CPacketParsePool::GetFreeCount()
{
    return  CObjectPoolManager::GetFreeCount();
}

CPacketParse* CPacketParsePool::Create(const char* pFileName, uint32 u4Line)
{
    return CObjectPoolManager::Create(pFileName, u4Line);
}

bool CPacketParsePool::Delete(CPacketParse* pPacketParse, bool blDelete)
{
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

    return CObjectPoolManager::Delete(pPacketParse->GetHashID(), pPacketParse);
}

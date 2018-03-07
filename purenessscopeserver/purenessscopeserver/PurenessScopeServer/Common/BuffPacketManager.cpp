#include "BuffPacketManager.h"

CBuffPacketManager::CBuffPacketManager(void)
{
    //Ĭ��Ϊ������
    m_blSortType = false;
}

CBuffPacketManager::~CBuffPacketManager(void)
{
    OUR_DEBUG((LM_ERROR, "[CBuffPacketManager::~CBuffPacketManager].\n"));
    //Close();
}

IBuffPacket* CBuffPacketManager::Create(const char* pFileName, uint32 u4Line)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    IBuffPacket* pBuffPacket = dynamic_cast<IBuffPacket*>(m_objHashBuffPacketList.Pop_Uint32());

    if (NULL != pBuffPacket)
    {
        pBuffPacket->SetCreateInfo(pFileName, u4Line);
    }

    return pBuffPacket;
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    CBuffPacket* pBuff = dynamic_cast<CBuffPacket*>(pBuffPacket);

    if(NULL == pBuff)
    {
        return false;
    }

    pBuff->Clear();
    pBuff->SetNetSort(m_blSortType);

    int32 nPos = m_objHashBuffPacketList.Push_Uint32(pBuff->GetBuffID(), pBuff);

    if(-1 == nPos)
    {
        OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%d(0x%08x).\n", pBuff->GetBuffID(), pBuff));
    }

    return true;
}

void CBuffPacketManager::GetCreateInfoList(vector<_Packet_Create_Info>& objCreateList)
{
    objCreateList.clear();

    //�����������ʹ�õĶ��󴴽���Ϣ
    uint32 u4Count = m_objBuffPacketList.GetCount();

    for (uint32 i = 0; i < u4Count; i++)
    {
        CBuffPacket* pBuffPacket = m_objBuffPacketList.GetObject(i);

        int nCreateLine       = pBuffPacket->GetCreateLine();
        char* pCreateFileName = pBuffPacket->GetCreateFileName();

        if (strlen(pCreateFileName) > 0 && nCreateLine > 0)
        {
            bool blIsFind = false;

            //����ʹ�õĶ��󣬽���ͳ��
            for (int j = 0; j < (int)objCreateList.size(); j++)
            {
                if (0 == ACE_OS::strcmp(pCreateFileName, objCreateList[j].m_szCreateFileName)
                    && nCreateLine == objCreateList[j].m_u4Line)
                {
                    blIsFind = true;
                    objCreateList[j].m_u4Count++;
                    break;
                }
            }

            if (false == blIsFind)
            {
                _Packet_Create_Info obj_Packet_Create_Info;
                sprintf_safe(obj_Packet_Create_Info.m_szCreateFileName, MAX_BUFF_100, "%s", pCreateFileName);
                obj_Packet_Create_Info.m_u4Line  = nCreateLine;
                obj_Packet_Create_Info.m_u4Count = 1;
                objCreateList.push_back(obj_Packet_Create_Info);
            }
        }
    }
}

void CBuffPacketManager::Close()
{
    //���������Ѵ��ڵ�ָ��
    m_objHashBuffPacketList.Close();
}

void CBuffPacketManager::Init(uint32 u4PacketCount, uint32 u4MaxBuffSize, bool blByteOrder)
{
    Close();

    //��ʼ��Hash��
    m_objBuffPacketList.Init(u4PacketCount);
    m_objHashBuffPacketList.Init((int32)u4PacketCount);

    uint32 u4Size = m_objHashBuffPacketList.Get_Count();

    for(uint32 i = 0; i < u4Size; i++)
    {
        CBuffPacket* pBuffPacket = m_objBuffPacketList.GetObject(i);

        if(NULL != pBuffPacket)
        {
            //����BuffPacketĬ������
            pBuffPacket->Init(DEFINE_PACKET_SIZE, u4MaxBuffSize);
            pBuffPacket->SetNetSort(blByteOrder);
            pBuffPacket->SetBuffID(i);

            //��ӵ�Hash��������
            int32 nHashPos = m_objHashBuffPacketList.Add_Hash_Data_By_Key_Unit32(pBuffPacket->GetBuffID(), pBuffPacket);

            if(-1 != nHashPos)
            {
                pBuffPacket->SetHashID(i);
            }
        }
    }

    //�趨��ǰ����ص�����
    m_blSortType = blByteOrder;

}

uint32 CBuffPacketManager::GetBuffPacketUsedCount()
{
    return (uint32)m_objHashBuffPacketList.Get_Count() - m_objHashBuffPacketList.Get_Used_Count();
}

uint32 CBuffPacketManager::GetBuffPacketFreeCount()
{
    return (uint32)m_objHashBuffPacketList.Get_Used_Count();
}


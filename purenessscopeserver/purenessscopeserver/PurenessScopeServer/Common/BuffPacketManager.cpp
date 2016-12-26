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

IBuffPacket* CBuffPacketManager::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
	CBuffPacket* pBuffPacket = NULL;

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	//�ж�ѭ��ָ���Ƿ��Ѿ��ҵ��˾�ͷ����������0��ʼ����
	if(m_u4CulationIndex + 1 >= (uint32)m_objHashBuffPacketList.Get_Count() - 1)
	{
		m_u4CulationIndex = 0;
	}

	//��һ��Ѱ�ң��ӵ�ǰλ��������
	for(int i = (int)m_u4CulationIndex; i < m_objHashBuffPacketList.Get_Count(); i++)
	{
		pBuffPacket = m_objHashBuffPacketList.Get_Index(i);
		if(NULL != pBuffPacket)
		{
			//�Ѿ��ҵ��ˣ�����ָ��
			char szPacketID[10] = {'\0'};
			sprintf_safe(szPacketID, 10, "%d", pBuffPacket->GetBuffID());
			//int nDelPos = m_objHashBuffPacketList.Del_Hash_Data(szPacketID);
			int nDelPos = m_objHashBuffPacketList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Create]szPacketID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szPacketID, i, nDelPos, pBuffPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Create]szPacketID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szPacketID, i, nDelPos, pBuffPacket));
			}
			m_u4CulationIndex = i;
			return (IBuffPacket* )pBuffPacket;
		}
	}

	//�ڶ���Ѱ�ң���0����ǰλ��
	for(int i = 0; i < (int)m_u4CulationIndex; i++)
	{
		pBuffPacket = m_objHashBuffPacketList.Get_Index(i);
		if(NULL != pBuffPacket)
		{
			//�Ѿ��ҵ��ˣ�����ָ��
			char szPacketID[10] = {'\0'};
			sprintf_safe(szPacketID, 10, "%d", pBuffPacket->GetBuffID());
			int nDelPos = m_objHashBuffPacketList.Set_Index_Clear(i);
			if(-1 == nDelPos)
			{
				OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Create]szPacketID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szPacketID, i, nDelPos, pBuffPacket));
			}
			else
			{
				//OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Create]szPacketID=%s, nPos=%d, nDelPos=%d, (0x%08x).\n", szPacketID, i, nDelPos, pBuffPacket));
			}
			m_u4CulationIndex = i;
			return (IBuffPacket* )pBuffPacket;
		}
	}
	
	m_objHashBuffPacketList.Get_Index(0);
	return (IBuffPacket* )pBuffPacket;
}

bool CBuffPacketManager::Delete(IBuffPacket* pBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	CBuffPacket* pBuff = (CBuffPacket* )pBuffPacket;
	if(NULL == pBuff)
	{
		return false;
	}

	pBuff->Clear();
	pBuff->SetNetSort(m_blSortType);

	char szPacketID[10] = {'\0'};
	sprintf_safe(szPacketID, 10, "%d", pBuff->GetBuffID());
	//int nPos = m_objHashBuffPacketList.Add_Hash_Data(szPacketID, pBuff);
	int nPos = m_objHashBuffPacketList.Set_Index(pBuff->GetHashID(), szPacketID, pBuff);
	if(-1 == nPos)
	{
		OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%s(0x%08x) nPos=%d.\n", szPacketID, pBuff, nPos));
	}
	else
	{
		//OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Delete]szPacketID=%s(0x%08x) nPos=%d.\n", szPacketID, pBuff, nPos));
	}

	return true;
}

void CBuffPacketManager::Close()
{
	//���������Ѵ��ڵ�ָ��
	for(int i = 0; i < m_objHashBuffPacketList.Get_Count(); i++)
	{
		CBuffPacket* pBuffPacket = m_objHashBuffPacketList.Get_Index(i);
		if(NULL != pBuffPacket)
		{
			pBuffPacket->Close();
			SAFE_DELETE(pBuffPacket);
		}
	}

	m_objHashBuffPacketList.Close();
	m_u4CulationIndex = 0;
}

void CBuffPacketManager::Init(uint32 u4PacketCount, bool blByteOrder)
{
	Close();

	//��ʼ��Hash��
	size_t nArraySize = (sizeof(_Hash_Table_Cell<CBuffPacket>)) * u4PacketCount;
	char* pHashBase = new char[nArraySize];
	m_objHashBuffPacketList.Init(pHashBase, (int)u4PacketCount);

	for(int i = 0; i < m_objHashBuffPacketList.Get_Count(); i++)
	{
		CBuffPacket* pBuffPacket = new CBuffPacket();
		if(NULL != pBuffPacket)
		{
			//����BuffPacketĬ������
			pBuffPacket->SetNetSort(blByteOrder);
			pBuffPacket->SetBuffID(i);

			char szPacketID[10] = {'\0'};
			sprintf_safe(szPacketID, 10, "%d", i);

			//��ӵ�Hash��������
			int nHashPos = m_objHashBuffPacketList.Add_Hash_Data(szPacketID, pBuffPacket);
			if(-1 != nHashPos)
			{
				pBuffPacket->SetHashID(nHashPos);
			}
		}
	}

	for(int i = 0; i < m_objHashBuffPacketList.Get_Count(); i++)
	{
		if(NULL == m_objHashBuffPacketList.Get_Index(i))
		{
			OUR_DEBUG((LM_INFO, "[CBuffPacketManager::Init]ERROR(%d).\n", i));
			m_objHashBuffPacketList.Get_Index(i);
		}
	}

	m_u4CulationIndex = 0;
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

#include "DataManager.h"

CDataManager* CDataManager::m_pInstance = NULL;

CDataManager::CDataManager()
{

}
CDataManager::~CDataManager()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//����xml�����ļ�
bool CDataManager::ParseXmlFile(const char* pXmlFile)
{
    return true;
}

//���ӽڵ�����
void CDataManager::AddNodeDate(uint32 u4ConnectId,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
}

//����index html�ļ�
void CDataManager::make_index_html()
{

}

//����detail html�ļ�
void CDataManager::make_detail_html()
{

}

CDataManager* CDataManager::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDataManager();
    }

    return m_pInstance;
}

//��ȡ��������ʱ����
uint32 CDataManager::GetTimeInterval()
{
    return 0;
}
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
void CDataManager::AddNodeDate(const char* pIP,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut)
{

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
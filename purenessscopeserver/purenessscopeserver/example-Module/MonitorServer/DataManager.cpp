#include "DataManager.h"

CDataManager* CDataManager::m_pInstance = NULL;

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{
    Close();
}

void CDataManager::Close()
{
    if(NULL != m_pTiXmlDocument)
    {
        delete m_pTiXmlDocument;
        m_pTiXmlDocument = NULL;
        m_pRootElement   = NULL;
    }

    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//����xml�����ļ�
bool CDataManager::ParseXmlFile(const char* pXmlFile)
{
    m_pTiXmlDocument = new TiXmlDocument(pXmlFile);

    if(NULL == m_pTiXmlDocument)
    {
        return false;
    }

    if(false == m_pTiXmlDocument->LoadFile())
    {
        return false;
    }

    //��ø�Ԫ��
    m_pRootElement = m_pTiXmlDocument->RootElement();

    return true;
}

//���ӽڵ�����
void CDataManager::AddNodeDate(const char* pIP, uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut)
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

char* CDataManager::GetData(const char* pName, const char* pAttrName)
{
    if(m_pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);

    if(NULL != pTiXmlElement)
    {
        return (char* )pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

char* CDataManager::GetData( const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement )
{
    if(m_pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = NULL;

    if(NULL == pNextTiXmlElement)
    {
        pTiXmlElement = m_pRootElement->FirstChildElement(pName);
        pNextTiXmlElement = pTiXmlElement;
    }
    else
    {
        pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
        pNextTiXmlElement = pTiXmlElement;
    }

    if(NULL != pTiXmlElement)
    {
        return (char* )pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
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


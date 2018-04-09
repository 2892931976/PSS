#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "tinyxml.h"
#include "tinystr.h"
#include <vector>
#include <map>
using namespace std;

class CDataManager
{
public:
    ~CDataManager();

    //����xml�����ļ�
    bool ParseXmlFile(const char* pXmlFile);

    //���ӽڵ�����
    void AddNodeDate(uint32 u4ConnectId,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut);

    //����index html�ļ�
    void make_index_html();

    //����detail html�ļ�
    void make_detail_html();

private:
    CDataManager();
public:
    static CDataManager* GetInstance();

    //��ȡ��������ʱ����
    uint32 GetTimeInterval();
private:
    static CDataManager* m_pInstance;

    //���ƶ��߳���
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;

    typedef map<uint32,string> mapConnectId2IP;

    //��������id��IP��ӳ���ϵ
    mapConnectId2IP m_mapConnectId2IP;

};

#endif //_DATAMANAGER_H_

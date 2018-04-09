#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "tinyxml.h"
#include "tinystr.h"

class CDataManager 
{
public:
    CDataManager();
    ~CDataManager();

    //����xml�����ļ�
    bool ParseXmlFile(const char* pXmlFile);

    //���ӽڵ�����
    void AddNodeDate(const char* pIP,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut);

    //����index html�ļ�
    void make_index_html();

    //����detail html�ļ�
    void make_detail_html();

private:

};

#endif //_DATAMANAGER_H_

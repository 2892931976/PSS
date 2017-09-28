#ifndef _PROACOOECTACCEPT_H
#define _PROACOOECTACCEPT_H

#include <vector>

using namespace std;

#include "ForbiddenIP.h"
#include "ProConnectHandle.h"
#include "AceProactorManager.h"
#include "XmlOpeation.h"

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"

//ƽ���ͻ��˵�Acceptor
class ProConnectAcceptor : public ACE_Asynch_Acceptor<CProConnectHandle>
{
public:
    ProConnectAcceptor();
    void InitClientProactor(uint32 u4ClientProactorCount);
    void SetPacketParseInfoID(uint32 u4PaccketParseInfoID);
    uint32 GetPacketParseInfoID();
    CProConnectHandle* file_test_make_handler(void);
private:
    virtual CProConnectHandle* make_handler (void);
    virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
                                     const ACE_INET_Addr& remote,
                                     const ACE_INET_Addr& local);

public:
    void SetListenInfo(const char* pIP, uint32 u4Port);
    char*  GetListenIP();
    uint32 GetListenPort();

private:
    char   m_szListenIP[MAX_BUFF_20];
    uint32 m_u4Port;
    uint32 m_u4AcceptCount;             //���յ���������
    uint32 m_u4ClientProactorCount;     //�ͻ��˷�Ӧ���ĸ���
    uint32 m_u4PacketParseInfoID;       //�����������ģ��ID
};

class CProConnectAcceptManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CProConnectAcceptManager(void);
    ~CProConnectAcceptManager(void);

    bool InitConnectAcceptor(int nCount, uint32 u4ClientProactorCount);
    void Close();
    int GetCount();
    ProConnectAcceptor* GetConnectAcceptor(int nIndex);
    ProConnectAcceptor* GetNewConnectAcceptor();
    const char* GetError();

    bool Close(const char* pIP, uint32 n4Port);
    bool CheckIPInfo(const char* pIP, uint32 n4Port);

private:
    typedef vector<ProConnectAcceptor*> vecProConnectAcceptor;
    vecProConnectAcceptor m_vecConnectAcceptor;
    int                   m_nAcceptorCount;
    char                  m_szError[MAX_BUFF_500];

public:
    //�ļ����Է���
    FileTestResultInfoSt FileTestStart(const char* szXmlFileTestName);      //��ʼ�ļ�����
    int FileTestEnd();        //�����ļ�����
private:
    bool LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult);        //��ȡ���������ļ�
    int  ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo);   //����Ϣ���ļ��������ݽṹ

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //��ʱ�����
private:
    //�ļ����Ա���
    bool m_bFileTesting;          //�Ƿ����ڽ����ļ�����
    int32 m_n4TimerID;            //��ʱ��ID

    CXmlOpeation m_MainConfig;
    string m_strProFilePath;
    int    m_n4TimeInterval;      //��ʱ���¼����
    int    m_n4ConnectCount;      //ģ��������
    uint32 m_u4ParseID;           //������ID
    int    m_n4ContentType;       //Э����������,1�Ƕ�����Э��,0���ı�Э��
    
    typedef vector<FileTestDataInfoSt> vecFileTestDataInfoSt;
    vecFileTestDataInfoSt m_vecFileTestDataInfoSt;
};

typedef ACE_Singleton<CProConnectAcceptManager, ACE_Null_Mutex> App_ProConnectAcceptManager;
#endif

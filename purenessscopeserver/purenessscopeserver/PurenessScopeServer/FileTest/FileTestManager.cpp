#include "FileTestManager.h"

CFileTestManager::CFileTestManager(void)
{
    m_bFileTesting     = false;
    m_n4TimerID        = 0;
    m_n4ConnectCount   = 0;
    m_u4ParseID        = 0;
    m_n4ResponseCount  = 0;
    m_n4ExpectTime     = 1000;
    m_n4ContentType    = 1;
    m_n4TimeInterval   = 0;
}

CFileTestManager::~CFileTestManager(void)
{
    if(m_n4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_n4TimerID);
        m_n4TimerID = 0;
        m_bFileTesting = false;
    }

    return;
}

FileTestResultInfoSt CFileTestManager::FileTestStart(const char* szXmlFileTestName)
{
    FileTestResultInfoSt objFileTestResult;

    if(m_bFileTesting)
    {
        OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]m_bFileTesting:%d.\n",m_bFileTesting));
        objFileTestResult.n4Result = RESULT_ERR_TESTING;
        return objFileTestResult;
    }
    else
    {
        if(!LoadXmlCfg(szXmlFileTestName, objFileTestResult))
        {
            OUR_DEBUG((LM_DEBUG, "[CProConnectAcceptManager::FileTestStart]Loading config file error filename:%s.\n", szXmlFileTestName));
        }
        else
        {
            m_n4TimerID = App_TimerManager::instance()->schedule(this, (void*)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(m_n4TimeInterval), ACE_Time_Value(m_n4TimeInterval));

            if(-1 == m_n4TimerID)
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Start timer error\n"));
                objFileTestResult.n4Result = RESULT_ERR_UNKOWN;
            }
            else
            {
                OUR_DEBUG((LM_ERROR, "[CMainConfig::LoadXmlCfg]Start timer OK.\n"));
                objFileTestResult.n4Result = RESULT_OK;
                m_bFileTesting = true;
            }
        }
    }

    return objFileTestResult;
}

int CFileTestManager::FileTestEnd()
{
    if(m_n4TimerID > 0)
    {
        App_TimerManager::instance()->cancel(m_n4TimerID);
        m_n4TimerID = 0;
        m_bFileTesting = false;
    }

    return 0;
}

void CFileTestManager::HandlerServerResponse(uint32 u4ConnectID)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
    char szConnectID[10] = { '\0' };
    sprintf_safe(szConnectID, 10, "%d", u4ConnectID);

    ResponseRecordSt* pResponseRecord = m_objResponseRecordList.Get_Hash_Box_Data(szConnectID);

    if (NULL == pResponseRecord)
    {
        OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_n4ExpectTimeNo find connectID=%d.\n", u4ConnectID));
        return;
    }

    if(pResponseRecord->m_u1ResponseCount + 1 == m_n4ResponseCount)
    {
        ACE_Time_Value atvResponse = ACE_OS::gettimeofday();

        if(m_n4ExpectTime <= (int)((uint64)atvResponse.get_msec() - pResponseRecord->m_u8StartTime))
        {
            //Ӧ��ʱ�䳬������ʱ������
            OUR_DEBUG((LM_INFO, "[CFileTestManager::HandlerServerResponse]Response time too long m_n4ExpectTime:%d.\n",m_n4ExpectTime));
        }

#ifndef WIN32
        App_ConnectManager::instance()->Close(u4ConnectID);
#else
        App_ProConnectManager::instance()->Close(u4ConnectID);
#endif
        //���ն�������
        m_objResponseRecordList.Del_Hash_Data(szConnectID);
        SAFE_DELETE(pResponseRecord);
    }
    else
    {
        pResponseRecord->m_u1ResponseCount += 1;
    }

}

bool CFileTestManager::LoadXmlCfg(const char* szXmlFileTestName, FileTestResultInfoSt& objFileTestResult)
{
    char* pData = NULL;
    OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]Filename = %s.\n", szXmlFileTestName));

    if(false == m_MainConfig.Init(szXmlFileTestName))
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]File Read Error = %s.\n", szXmlFileTestName));
        objFileTestResult.n4Result = RESULT_ERR_CFGFILE;
        return false;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "Path");

    if(NULL != pData)
    {
        m_strProFilePath = pData;
    }
    else
    {
        m_strProFilePath = "./";
    }

    pData = m_MainConfig.GetData("FileTestConfig", "TimeInterval");

    if(NULL != pData)
    {
        m_n4TimeInterval = (uint8)ACE_OS::atoi(pData);
        objFileTestResult.n4TimeInterval = m_n4TimeInterval;
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4TimeInterval = %d.\n", m_n4TimeInterval));
    }
    else
    {
        m_n4TimeInterval = 10;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ConnectCount");

    if(NULL != pData)
    {
        m_n4ConnectCount = (uint8)ACE_OS::atoi(pData);
        objFileTestResult.n4ConnectNum = m_n4ConnectCount;
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ConnectCount = %d.\n", m_n4ConnectCount));
    }
    else
    {
        m_n4ConnectCount = 10;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ResponseCount");

    if(NULL != pData)
    {
        m_n4ResponseCount = (uint8)ACE_OS::atoi(pData);
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ResponseCount = %d.\n", m_n4ResponseCount));
    }
    else
    {
        m_n4ResponseCount = 1;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ExpectTime");

    if(NULL != pData)
    {
        m_n4ExpectTime = (uint8)ACE_OS::atoi(pData);
        OUR_DEBUG((LM_INFO, "[CProConnectAcceptManager::LoadXmlCfg]m_n4ExpectTime = %d.\n", m_n4ExpectTime));
    }
    else
    {
        m_n4ExpectTime = 1000;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ParseID");

    if(NULL != pData)
    {
        m_u4ParseID = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_u4ParseID = 1;
    }

    pData = m_MainConfig.GetData("FileTestConfig", "ContentType");

    if(NULL != pData)
    {
        m_n4ContentType = (uint8)ACE_OS::atoi(pData);
    }
    else
    {
        m_n4ContentType = 1; //Ĭ���Ƕ�����Э��
    }

    //�������������
    TiXmlElement* pNextTiXmlElementFileName     = NULL;
    TiXmlElement* pNextTiXmlElementDesc         = NULL;

    while(true)
    {
        FileTestDataInfoSt objFileTestDataInfo;
        string strFileName;
        string strFileDesc;

        pData = m_MainConfig.GetData("FileInfo", "FileName", pNextTiXmlElementFileName);

        if(pData != NULL)
        {
            strFileName = m_strProFilePath + pData;
        }
        else
        {
            break;
        }

        pData = m_MainConfig.GetData("FileInfo", "Desc", pNextTiXmlElementDesc);

        if(pData != NULL)
        {
            strFileDesc = pData;
            objFileTestResult.vecProFileDesc.push_back(strFileDesc);
        }
        else
        {
            break;
        }

        //��ȡ���ݰ��ļ�����
        int nReadFileRet = ReadTestFile(strFileName.c_str(), m_n4ContentType, objFileTestDataInfo);

        if(RESULT_OK == nReadFileRet)
        {
            m_vecFileTestDataInfoSt.push_back(objFileTestDataInfo);
        }
        else
        {
            objFileTestResult.n4Result = nReadFileRet;
            return false;
        }
    }

    m_MainConfig.Close();

    //��ʼ�����ӷ�����������
    ResponseRecordList();

    objFileTestResult.n4ProNum = static_cast<int>(m_vecFileTestDataInfoSt.size());
    return true;
}

int CFileTestManager::ReadTestFile(const char* pFileName, int nType, FileTestDataInfoSt& objFileTestDataInfo)
{
    ACE_FILE_Connector fConnector;
    ACE_FILE_IO ioFile;
    ACE_FILE_Addr fAddr(pFileName);

    if (fConnector.connect(ioFile, fAddr) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Open filename:%s Error.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }

    ACE_FILE_Info fInfo;

    if (ioFile.get_info(fInfo) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::ReadTestFile]Get file info filename:%s Error.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }

    if (MAX_BUFF_10240 - 1 < fInfo.size_)
    {
        OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Protocol file too larger filename:%s.\n", pFileName));
        return RESULT_ERR_PROFILE;
    }
    else
    {
        char szFileContent[MAX_BUFF_10240] = { '\0' };
        ssize_t u4Size = ioFile.recv(szFileContent, fInfo.size_);

        if (u4Size != fInfo.size_)
        {
            OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]Read protocol file error filename:%s Error.\n", pFileName));
            return RESULT_ERR_PROFILE;
        }
        else
        {
            if (nType == 0)
            {
                //������ı�Э��
                memcpy_safe(szFileContent, static_cast<uint32>(u4Size), objFileTestDataInfo.m_szData, static_cast<uint32>(u4Size));
                objFileTestDataInfo.m_szData[u4Size] = '\0';
                objFileTestDataInfo.m_u4DataLength = static_cast<uint32>(u4Size);
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]u4Size:%d\n", u4Size));
                OUR_DEBUG((LM_INFO, "[CMainConfig::LoadXmlCfg]m_szData:%s\n", objFileTestDataInfo.m_szData));
            }
            else
            {
                //����Ƕ�����Э��
                CConvertBuffer objConvertBuffer;
                //�����ݴ�ת���ɶ����ƴ�
                int nDataSize = MAX_BUFF_10240;
                objConvertBuffer.Convertstr2charArray(szFileContent, static_cast<int>(u4Size), (unsigned char*)objFileTestDataInfo.m_szData, nDataSize);
                objFileTestDataInfo.m_u4DataLength = static_cast<uint32>(nDataSize);
            }
        }
    }

    return RESULT_OK;
}

int CFileTestManager::ResponseRecordList()
{
    //��ʼ��Hash��
    m_objResponseRecordList.Close();
    m_objResponseRecordList.Init((int)m_n4ConnectCount);

    return 0;
}

int CFileTestManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
    if (NULL != arg)
    {
        OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]arg is not NULL.\n"));
    }

    //���ȱ�����һ�ζ�ʱ����ִ�����ݶ����Ƿ��Ѿ�ȫ���ͷ�
    vector<ResponseRecordSt*> vecList;
    m_objResponseRecordList.Get_All_Used(vecList);

    int nUsedSize = (int)vecList.size();

    if (nUsedSize > 0)
    {
        for (int i = 0; i < nUsedSize; i++)
        {
            //����һ����ѯ���ڣ�û�н����Ķ���
            if(m_n4ExpectTime <= (int)((uint64)tv.get_msec() - vecList[i]->m_u8StartTime))
            {
                //������ִ�з�Χʱ��
                OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Response time too long m_n4ExpectTime:%d.\n", m_n4ExpectTime));
            }
            else
            {
                //�����˶�ʱ��ʱ��
                OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Response time too long m_n4TimeInterval:%d.\n", m_n4TimeInterval));
            }

            char szConnectID[10] = { '\0' };
            sprintf_safe(szConnectID, 10, "%d", vecList[i]->m_u4ConnectID);
            m_objResponseRecordList.Del_Hash_Data(szConnectID);

            //д�������־

        }
    }

#ifndef WIN32
    CConnectHandler* pConnectHandler = NULL;

    for (int iLoop = 0; iLoop < m_n4ConnectCount; iLoop++)
    {
        pConnectHandler = App_ConnectHandlerPool::instance()->Create();

        if (NULL != pConnectHandler)
        {
            //�ļ����ݰ����Բ���Ҫ�õ�pProactor������Ϊ����Ҫʵ�ʷ������ݣ������������ֱ������һ���̶���pProactor
            ACE_Reactor* pReactor = App_ReactorManager::instance()->GetAce_Client_Reactor(0);
            pConnectHandler->reactor(pReactor);
            pConnectHandler->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = pConnectHandler->file_open(dynamic_cast<IFileTestManager*>(this));

            if (0 != u4ConnectID)
            {
                ResponseRecordSt* pResponseRecord = new ResponseRecordSt();
                pResponseRecord->m_u1ResponseCount = 0;
                pResponseRecord->m_u8StartTime     = tv.get_msec();
                pResponseRecord->m_u4ConnectID = u4ConnectID;

                char szConnectID[10] = { '\0' };
                sprintf_safe(szConnectID, 10, "%d", u4ConnectID);

                if (-1 == m_objResponseRecordList.Add_Hash_Data(szConnectID, pResponseRecord))
                {
                    OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Add m_objResponseRecordList error, ConnectID=%d.\n", u4ConnectID));
                }
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::handle_timeout]file_open error\n"));
            }
        }
    }

    //ѭ�������ݰ�ѹ�����Ӷ���
    for (int iLoop = 0; iLoop < (int)m_vecFileTestDataInfoSt.size(); iLoop++)
    {
        FileTestDataInfoSt& objFileTestDataInfo = m_vecFileTestDataInfoSt[iLoop];

        vector<ResponseRecordSt*> vecExistList;
        m_objResponseRecordList.Get_All_Used(vecExistList);

        for (int jLoop = 0; jLoop < (int)vecExistList.size(); jLoop++)
        {
            uint32 u4ConnectID = vecExistList[jLoop]->m_u4ConnectID;
            App_ConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
        }
    }

#else
    CProConnectHandle* ptrProConnectHandle = NULL;

    for (int iLoop = 0; iLoop < m_n4ConnectCount; iLoop++)
    {
        ptrProConnectHandle = App_ProConnectHandlerPool::instance()->Create();

        if (NULL != ptrProConnectHandle)
        {
            //�ļ����ݰ����Բ���Ҫ�õ�pProactor������Ϊ����Ҫʵ�ʷ������ݣ������������ֱ������һ���̶���pProactor
            ACE_Proactor* pPractor = App_ProactorManager::instance()->GetAce_Client_Proactor(0);
            ptrProConnectHandle->proactor(pPractor);
            ptrProConnectHandle->SetPacketParseInfoID(m_u4ParseID);

            uint32 u4ConnectID = ptrProConnectHandle->file_open(dynamic_cast<IFileTestManager*>(this));

            if (0 != u4ConnectID)
            {
                ResponseRecordSt* pResponseRecord = new ResponseRecordSt();
                pResponseRecord->m_u1ResponseCount = 0;
                pResponseRecord->m_u8StartTime = tv.get_msec();
                pResponseRecord->m_u4ConnectID = u4ConnectID;

                char szConnectID[10] = { '\0' };
                sprintf_safe(szConnectID, 10, "%d", u4ConnectID);

                if (-1 == m_objResponseRecordList.Add_Hash_Data(szConnectID, pResponseRecord))
                {
                    OUR_DEBUG((LM_INFO, "[CFileTestManager::handle_timeout]Add m_objResponseRecordList error, ConnectID=%d.\n", u4ConnectID));
                }
            }
            else
            {
                OUR_DEBUG((LM_INFO, "[CMainConfig::handle_timeout]file_open error\n"));
            }
        }
    }

    for (int iLoop = 0; iLoop < (int)m_vecFileTestDataInfoSt.size(); iLoop++)
    {
        FileTestDataInfoSt& objFileTestDataInfo = m_vecFileTestDataInfoSt[iLoop];

        vector<ResponseRecordSt*> vecExistList;
        m_objResponseRecordList.Get_All_Used(vecExistList);

        for (int jLoop = 0; jLoop < (int)vecExistList.size(); jLoop++)
        {
            uint32 u4ConnectID = vecExistList[jLoop]->m_u4ConnectID;
            App_ProConnectManager::instance()->handle_write_file_stream(u4ConnectID, objFileTestDataInfo.m_szData, objFileTestDataInfo.m_u4DataLength, m_u4ParseID);
        }
    }

#endif

    return 0;
}




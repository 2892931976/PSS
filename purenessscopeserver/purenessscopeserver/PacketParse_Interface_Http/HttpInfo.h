#include "define.h"
#include "HashTable.h"

#include <map>

using namespace std;

//������Ǽ�¼���е�Http����״̬��
//add by freeeyes

//���δ���ܰ��ĳ���,�Լ������Ĵ�С��������ȱ���������޸�����
#define MAX_ENCRYPTLENGTH 50*MAX_BUFF_1024
//���������ݰ����ȣ����������ݰ������������չ���ֵ
#define MAX_DECRYPTLENGTH 50*MAX_BUFF_1024

//��¼websokcet������״̬������ǳ������ӣ�������ΪWEBSOCKET_STATE_HANDIN
struct _HttpInfo
{
    uint32                  m_u4ConnectID;                         //���ӵ�ID
    char                    m_szData[MAX_ENCRYPTLENGTH];           //��ǰ���������ݵĳ���
    uint32                  m_u4DataLength;                        //��ǰ������е����ݳ���

    _HttpInfo()
    {
        Init();
    }

    void Init()
    {
        m_u4ConnectID = 0;
        ACE_OS::memset(m_szData, 0, MAX_DECRYPTLENGTH);
        m_u4DataLength     = 0;
    }
};

//�������е�������
class CHttpInfoManager
{
public:
    CHttpInfoManager()
    {
        m_u4Count = 1000;

        //��ʼ��_HttpInfo HashTable
        m_objHttpInfoList.Init(m_u4Count);
    }

    ~CHttpInfoManager()
    {
        Close();
    }

    void Close()
    {
        vector<_HttpInfo*> vecHttpInfo;
        m_objHttpInfoList.Get_All_Used(vecHttpInfo);

        for(int i = 0; i < (int)vecHttpInfo.size(); i++)
        {
            _HttpInfo* ptrHttpInfo = vecHttpInfo[i];

            if(NULL != ptrHttpInfo)
            {
                SAFE_DELETE(ptrHttpInfo);
            }
        }

        m_objHttpInfoList.Close();
    }

    //����һ���µ���������״̬
    bool Insert(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //���Ҳ����
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            //����Ѿ�����
            ptrHttpInfo->Init();
            return true;
        }
        else
        {
            //����µ�����ͳ����Ϣ
            ptrHttpInfo = new _HttpInfo();

            if(NULL != ptrHttpInfo)
            {
                m_objHttpInfoList.Add_Hash_Data(szIndex, ptrHttpInfo);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    //ɾ��һ���µ����ݿ�����״̬
    void Delete(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //���Ҳ����
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            //����Ѿ�����
            ptrHttpInfo->Init();
            return;
        }
    }

    //����ָ��������״̬
    _HttpInfo* GetHttpInfo(uint32 u4ConnectID)
    {
        uint32 u4Index = u4ConnectID%m_u4Count;
        char szIndex[10];
        sprintf(szIndex, "%d", u4Index);
        //���Ҳ����
        _HttpInfo* ptrHttpInfo = m_objHttpInfoList.Get_Hash_Box_Data(szIndex);

        if(NULL != ptrHttpInfo)
        {
            return ptrHttpInfo;
        }
        else 
        {
            return NULL;
        }
    }

private:
    CHashTable<_HttpInfo> m_objHttpInfoList;
    uint32 m_u4Count;
};

typedef ACE_Singleton<CHttpInfoManager, ACE_Null_Mutex> App_HttpInfoManager;

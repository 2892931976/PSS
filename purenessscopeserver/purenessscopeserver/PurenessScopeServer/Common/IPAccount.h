#ifndef _IPACCOUNT_H
#define _IPACCOUNT_H

//��ӶԿͻ������ӵ�ͳ����Ϣ
//add by freeeyes
//2016-11-25
//�������Hash����ķ�ʽ���
//Ĭ���ǵ�ǰ������ӵ�2��


#include <string>
#include "ace/Date_Time.h"
#include "define.h"
#include "HashTable.h"
#include "ace/Recursive_Thread_Mutex.h"

//IP����ͳ��ģ��
struct _IPAccount
{
	string         m_strIP;              //��ǰ���ӵ�ַ
	int            m_nCount;             //��ǰ���Ӵ���
	int            m_nAllCount;          //ָ��IP���Ӵ����ܺ� 
	int            m_nMinute;            //��ǰ������
	ACE_Date_Time  m_dtLastTime;         //�������ʱ��

	_IPAccount()
	{
		m_strIP      = "";
		m_nCount     = 0;
		m_nAllCount  = 0;
		m_dtLastTime.update();
		m_nMinute    = (int)m_dtLastTime.minute();
	}

	void Add(ACE_Date_Time dtNowTime)
	{
		m_dtLastTime = dtNowTime;
		if(m_dtLastTime.minute() != m_nMinute)
		{
			m_nMinute  = (int)m_dtLastTime.minute();
			m_nCount   = 1;
			m_nAllCount++;
		}
		else
		{
			m_nCount++;
			m_nAllCount++;
		}
	}

	//falseΪ�����ѹ���
	bool Check(ACE_Date_Time dtNowTime)
	{
		//���3������û�и��£�������֮
		uint16 u2NowTime = (uint32)dtNowTime.minute();
		if(u2NowTime - m_nMinute  < 0)
		{
			u2NowTime += 60;
		}

		if(u2NowTime  - m_nMinute >= 3)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};

typedef vector<_IPAccount> vecIPAccount;

class CIPAccount
{
public:
	CIPAccount() 
	{ 
		m_nMaxConnectCount = 100;  //Ĭ��ÿ�����100�� 
    }

	~CIPAccount() 
	{
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount].\n"));
		Close(); 
		OUR_DEBUG((LM_INFO, "[CIPAccount::~CIPAccount]End.\n"));
    }

	void Close()
	{
		for(int i = 0; i < m_objIPList.Get_Count(); i++)
		{
			_IPAccount* pIPAccount = m_objIPList.Get_Index(i);
			SAFE_DELETE(pIPAccount);
		}

		m_objIPList.Close();
	}

	void Init(uint32 u4IPCount)
	{
		m_nMaxConnectCount = u4IPCount;

		//��ʼ��HashTable
		size_t nArraySize = (sizeof(_Hash_Table_Cell<_IPAccount>)) * u4IPCount;
		char* pHashBase = new char[nArraySize];
		m_objIPList.Init(pHashBase, (int)u4IPCount);

		ACE_Date_Time  dtNowTime;
		m_u2CurrTime = (uint16)dtNowTime.minute();
    }

	bool AddIP(string strIP)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

		//��鵱ǰʱ�䣬10��������һ�ֵ�ǰHash����
		ACE_Date_Time  dtNowTime;
		uint16 u2NowTime = (uint16)dtNowTime.minute();
		if(u2NowTime - m_u2CurrTime  < 0)
		{
			u2NowTime += 60;
		}

		if(u2NowTime - m_u2CurrTime >= 10)
		{
			//����Hash����
			for(int i = 0; i < m_objIPList.Get_Count(); i++)
			{
				_IPAccount* pIPAccount = m_objIPList.Get_Index(i);
				if(NULL != pIPAccount)
				{
					if(false == pIPAccount)
					{
						SAFE_DELETE(pIPAccount);
						m_objIPList.Set_Index_Clear(i);
					}
				}
			}
		}

		bool blRet = false;
		//������Ҫ����Ҫ�ж��������Ҫ�������IPͳ��
		if(m_nMaxConnectCount > 0)
		{
			_IPAccount* pIPAccount = m_objIPList.Get_Hash_Box_Data(strIP.c_str());
			if(NULL == pIPAccount)
			{
				//û���ҵ������
				pIPAccount = new _IPAccount();
				if(NULL == pIPAccount)
				{
					blRet = true;
				}
				else
				{
					pIPAccount->m_strIP     = strIP;
					pIPAccount->Add(dtNowTime);

					//�鿴�����Ƿ�����
					if(m_objIPList.Get_Count() == m_objIPList.Get_Used_Count())
					{
						//��ʱ������
						return true;
					}
					else
					{
						m_objIPList.Add_Hash_Data(strIP.c_str(), pIPAccount);
					}
				}
			}
			else
			{
				pIPAccount->Add(dtNowTime);
				if(pIPAccount->m_nCount >= m_nMaxConnectCount)
				{
					blRet = false;
				}
			}

			blRet = true;
		}
		else
		{
			blRet = true;
		}

		return blRet;
    }

	int GetCount()
	{
		return m_objIPList.Get_Used_Count();
	}

	void GetInfo(vecIPAccount& VecIPAccount)
	{
		ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
		for(int i = 0; i < m_objIPList.Get_Count(); i++)
		{
			_IPAccount* pIPAccount = m_objIPList.Get_Index(i);
			if(NULL != pIPAccount)
			{
				VecIPAccount.push_back(*pIPAccount);
			}
		}
	}

private:
	CHashTable<_IPAccount>           m_objIPList;                          //IPͳ����Ϣ
	int                              m_nMaxConnectCount;                   //ÿ������������������ǰ����m_nNeedCheck = 0;�Ż���Ч
	uint16                           m_u2CurrTime;                         //��ǰʱ��
	ACE_Recursive_Thread_Mutex       m_ThreadLock;                         //���߳��� 
};

typedef ACE_Singleton<CIPAccount, ACE_Recursive_Thread_Mutex> App_IPAccount;

//��λʱ��������ͳ��
class CConnectAccount
{
public:
	CConnectAccount()
	{
		m_u4ConnectMin     = 0;
		m_u4ConnectMax     = 0;
		m_u4DisConnectMin  = 0;
		m_u4DisConnectMax  = 0;
		m_u4CurrConnect    = 0;
		m_u4CurrDisConnect = 0;
    }

	~CConnectAccount()
	{

	}

	uint32 Get4ConnectMin()
	{
		return m_u4ConnectMin;
	}

	uint32 GetConnectMax()
	{
		return m_u4ConnectMax;
	}

	uint32 GetDisConnectMin()
	{
		return m_u4DisConnectMin;
	}

	uint32 GetDisConnectMax()
	{
		return m_u4DisConnectMax;
	}

	uint32 GetCurrConnect()
	{
		return m_u4CurrConnect;
	}

	uint32 GetCurrDisConnect()
	{
		return m_u4CurrDisConnect;
	}

	void Init(uint32 u4ConnectMin, uint32 u4ConnectMax, uint32 u4DisConnectMin, uint32 u4DisConnectMax)
	{
		m_u4ConnectMin     = u4ConnectMin;
		m_u4ConnectMax     = u4ConnectMax;
		m_u4DisConnectMin  = u4DisConnectMin;
		m_u4DisConnectMax  = u4DisConnectMax;
		m_u4CurrConnect    = 0;
		m_u4CurrDisConnect = 0;

		ACE_Date_Time  dtLastTime;
		m_u1Minute = (uint8)dtLastTime.minute();
	}

	bool AddConnect()
	{
		if(m_u4ConnectMax > 0)
		{
			ACE_Date_Time  dtLastTime;
			if(m_u1Minute != (uint8)dtLastTime.minute())
			{
				//�µ�һ����
				m_u4CurrConnect = 1;
			}
			else
			{
				m_u4CurrConnect++;
			}

			return true;
		}
		else
		{
			return true;
		}
	}

	bool AddDisConnect()
	{
		if(m_u4ConnectMax > 0)
		{
			ACE_Date_Time  dtLastTime;
			if(m_u1Minute != (uint8)dtLastTime.minute())
			{
				//�µ�һ����
				m_u4CurrDisConnect = 1;
			}
			else
			{
				m_u4CurrDisConnect++;
			}

			return true;
		}
		else
		{
			return true;
		}
	}
	
	int CheckConnectCount()
	{
		if(m_u4ConnectMax > 0)
		{
			if(m_u4CurrConnect > m_u4ConnectMax)
			{
				return 1;   //1Ϊ��Խmax����
			}
		}

		if(m_u4ConnectMin > 0)
		{
			if(m_u4CurrConnect < m_u4ConnectMin)
			{
				return 2;    //2Ϊ����min����
			}
		}
		else
		{
			return 0;
		}

		return 0;
	}

	int CheckDisConnectCount()
	{
		if(m_u4DisConnectMax > 0)
		{
			if(m_u4CurrDisConnect > m_u4DisConnectMax)
			{
				return 1;    //1Ϊ��Խmax����
			} 
		}

		if(m_u4ConnectMin > 0)
		{
			if(m_u4CurrDisConnect < m_u4DisConnectMin)
			{
				return 2;    //2Ϊ����min����
			}
		}
		else
		{
			return 0;
		}

		return 0;
	}

private:
	uint32 m_u4CurrConnect;
	uint32 m_u4CurrDisConnect;

	uint32 m_u4ConnectMin;
	uint32 m_u4ConnectMax;
	uint32 m_u4DisConnectMin;
	uint32 m_u4DisConnectMax;
	uint8  m_u1Minute;            //��ǰ������
};

typedef ACE_Singleton<CConnectAccount, ACE_Recursive_Thread_Mutex> App_ConnectAccount;
#endif

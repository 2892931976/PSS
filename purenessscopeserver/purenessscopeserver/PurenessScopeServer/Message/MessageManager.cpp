// MessageService.h
// ������Ϣ������Ϣ���ɸ�������߼�������ȥִ��
// ���쵽�˹���ͼ��ݣ��о��¹���ĺ����ɣ�������д���������˼��
// ��ʱ���������ǵ�֧���£�PSS�Ż��ߵĸ�Զ��
// ���ϵ��кý���������ɼ���Ҷ�������ʹ���С�
// ��ʱ��ƽ������������
// add by freeeyes
// 2009-01-29

#include "MessageManager.h"
#ifdef WIN32
	#include "ProConnectHandle.h"
#else
	#include "ConnectHandler.h"
#endif

bool Delete_CommandInfo(_ClientCommandInfo* pClientCommandInfo)
{
	return pClientCommandInfo->m_u4CurrUsedCount == 0;
}

CMessageManager::CMessageManager(void)
{
	m_u2MaxModuleCount     = 0;
	m_u4MaxCommandCount    = 0;
	m_u4CurrCommandCount   = 0;
}

CMessageManager::~CMessageManager(void)
{
	OUR_DEBUG((LM_INFO, "[CMessageManager::~CMessageManager].\n"));
	//Close();
}

void CMessageManager::Init(uint16 u2MaxModuleCount, uint32 u4MaxCommandCount)
{
	//��ʼ����������
	size_t nArraySize = m_objClientCommandList.Get_Size(u4MaxCommandCount);
	char* pHashBase = new char[nArraySize];
	m_objClientCommandList.Init(pHashBase, (int)u4MaxCommandCount);

	//��ʼ��HashTable
	nArraySize =m_objModuleClientList.Get_Size(u2MaxModuleCount);
	pHashBase = new char[nArraySize];
	m_objModuleClientList.Init(pHashBase, (int)u2MaxModuleCount);

	m_u2MaxModuleCount  = u2MaxModuleCount;
	m_u4MaxCommandCount = u4MaxCommandCount;

}

bool CMessageManager::DoMessage(ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag)
{
	if(NULL == pMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage] pMessage is NULL.\n"));
		return false;
	}

	//�Ÿ���Ҫ�̳е�ClientCommand��ȥ����
	//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d.\n", u2CommandID));

	CClientCommandList* pClientCommandList = GetClientCommandList(u2CommandID);
	if(pClientCommandList != NULL)
	{
		int nCount = pClientCommandList->GetCount();
		for(int i = 0; i < nCount; i++)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->GetClientCommandIndex(i);
			if(NULL != pClientCommandInfo && pClientCommandInfo->m_u1State == 0)
			{
				//�жϵ�ǰ��Ϣ�Ƿ���ָ���ļ����˿�
				if(pClientCommandInfo->m_objListenIPInfo.m_nPort > 0)
				{
					if(ACE_OS::strcmp(pClientCommandInfo->m_objListenIPInfo.m_szClientIP, pMessage->GetMessageBase()->m_szListenIP) != 0 ||
						(uint32)pClientCommandInfo->m_objListenIPInfo.m_nPort != pMessage->GetMessageBase()->m_u4ListenPort)
					{
						continue;
					}
				}

				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d Begin.\n", u2CommandID));
				//����ָ��¼���������
				pClientCommandInfo->m_pClientCommand->DoMessage(pMessage, bDeleteFlag);
				ACE_Time_Value tvCost =  ACE_OS::gettimeofday() - tvBegin;
				u4TimeCost =  (uint32)tvCost.msec();

				//��¼������ô���
				u2Count++;
				//OUR_DEBUG((LM_ERROR, "[CMessageManager::DoMessage]u2CommandID = %d End.\n", u2CommandID));
				
			}
		}		
	}

	return true;
}

CClientCommandList* CMessageManager::GetClientCommandList(uint16 u2CommandID)
{
	char szCommandID[10] = {'\0'};
	sprintf_safe(szCommandID, 10, "%d", u2CommandID);
	return m_objClientCommandList.Get_Hash_Box_Data(szCommandID);
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName)
{


	if(NULL == pClientCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
		return false;
	}

	char szCommandID[10] = {'\0'};
	sprintf_safe(szCommandID, 10, "%d", u2CommandID);
	CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);
	if(NULL != pClientCommandList)
	{
		//�������Ѵ���
		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
		if(NULL != pClientCommandInfo) 
		{
			//���������ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;
				
			//��ӵ�ģ������
			string strModule = pModuleName;
			_ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());
			if(NULL == pModuleClient)
			{
				//�Ҳ����������µ�ģ����Ϣ
				pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
				}
			}
			else
			{
				//�ҵ��ˣ���ӽ�ȥ
				pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
			}
			OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		}
		else
		{
			return false;
		}
	}
	else
	{
		//��������δ���
		pClientCommandList = new CClientCommandList();
		if(NULL != pClientCommandList)
		{
			_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName);
			if(NULL != pClientCommandInfo)
			{
				//���������ID
				pClientCommandInfo->m_u2CommandID = u2CommandID;			
			
				//��ӵ�ģ������
				string strModule = pModuleName;
				_ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());
				if(NULL == pModuleClient)
				{
					//�Ҳ����������µ�ģ����Ϣ
					pModuleClient = new _ModuleClient();
					if(NULL != pModuleClient)
					{
						pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
						m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
					}
				}
				else
				{
					//�ҵ��ˣ���ӽ�ȥ
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
				}

				m_objClientCommandList.Add_Hash_Data(szCommandID, pClientCommandList);
				m_u4CurrCommandCount++;
				OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CMessageManager::AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName, _ClientIPInfo objListenInfo)
{
	if(NULL == pClientCommand)
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d pClientCommand is NULL.\n", u2CommandID));
		return false;
	}

	char szCommandID[10] = {'\0'};
	sprintf_safe(szCommandID, 10, "%d", u2CommandID);
	CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);
	if(NULL != pClientCommandList)
	{
		//�������Ѵ���
		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);
		if(NULL != pClientCommandInfo) 
		{
			//���������ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;

			//��ӵ�ģ������
			string strModule = pModuleName;
			_ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());
			if(NULL == pModuleClient)
			{
				//�Ҳ����������µ�ģ����Ϣ
				pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
				}
			}
			else
			{
				//�ҵ��ˣ���ӽ�ȥ
				pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
			}
			OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		}
		else
		{
			return false;
		}
	}
	else
	{
		//��������δ���
		pClientCommandList = new CClientCommandList();
		if(NULL == pClientCommandList)
		{
			return false;
		}

		_ClientCommandInfo* pClientCommandInfo = pClientCommandList->AddClientCommand(pClientCommand, pModuleName, objListenInfo);
		if(NULL != pClientCommandInfo)
		{
			//���������ID
			pClientCommandInfo->m_u2CommandID = u2CommandID;			

			//��ӵ�ģ������
			string strModule = pModuleName;
			_ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModule.c_str());
			if(NULL == pModuleClient)
			{
				//�Ҳ����������µ�ģ����Ϣ
				pModuleClient = new _ModuleClient();
				if(NULL != pModuleClient)
				{
					pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
					m_objModuleClientList.Add_Hash_Data(strModule.c_str(), pModuleClient);
				}
			}
			else
			{
				//�ҵ��ˣ���ӽ�ȥ
				pModuleClient->m_vecClientCommandInfo.push_back(pClientCommandInfo);
			}
			m_objClientCommandList.Add_Hash_Data(szCommandID, pClientCommandList);
			m_u4CurrCommandCount++;
			OUR_DEBUG((LM_ERROR, "[CMessageManager::AddClientCommand] u2CommandID = %d Add OK***.\n", u2CommandID));
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CMessageManager::DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)
{
	char szCommandID[10] = {'\0'};
	sprintf_safe(szCommandID, 10, "%d", u2CommandID);
	CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Hash_Box_Data(szCommandID);
	if(NULL != pClientCommandList)
	{
		if(true == pClientCommandList->DelClientCommand(pClientCommand))
		{
			SAFE_DELETE(pClientCommandList);
			m_objClientCommandList.Del_Hash_Data(szCommandID);
		}
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d Del OK.\n", u2CommandID));
		return true;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CMessageManager::DelClientCommand] u2CommandID = %d is not exist.\n", u2CommandID));
		return false;
	}
}

bool CMessageManager::UnloadModuleCommand(const char* pModuleName, uint8 u1State)
{
	string strModuleName  = pModuleName;
	string strModuleN     = "";
	string strModulePath  = "";
	string strModuleParam = "";

	//������ض�����ز���
	_ModuleInfo* pModuleInfo = App_ModuleLoader::instance()->GetModuleInfo(pModuleName);
	if(NULL != pModuleInfo)
	{
		//��ȡ������Ϣ
		strModuleN     = pModuleInfo->strModuleName;
		strModulePath  = pModuleInfo->strModulePath;
		strModuleParam = pModuleInfo->strModuleParam;
	}

	_ModuleClient* pModuleClient = m_objModuleClientList.Get_Hash_Box_Data(strModuleName.c_str());
	if(NULL != pModuleClient)
	{
		//�Ӳ��Ŀǰע������������ҵ����иò������Ϣ��һ�����ͷ�
		for(uint32 u4Index = 0; u4Index < (uint32)pModuleClient->m_vecClientCommandInfo.size(); u4Index++)
		{
			_ClientCommandInfo* pClientCommandInfo = pModuleClient->m_vecClientCommandInfo[u4Index];
			if(NULL != pClientCommandInfo)
			{
				uint16 u2CommandID = pClientCommandInfo->m_u2CommandID;
				CClientCommandList* pCClientCommandList = GetClientCommandList(u2CommandID);
				if(NULL != pCClientCommandList)
				{
					for(int i = 0; i < pCClientCommandList->GetCount(); i++)
					{
						//�ҵ��Ǹ�Ψһ
						if(pCClientCommandList->GetClientCommandIndex(i) == pClientCommandInfo)
						{
							//�ҵ��ˣ��ͷ�֮
							pCClientCommandList->DelClientCommand(pClientCommandInfo->m_pClientCommand);

							//�����ָ���µ������Ѿ������ڣ���ɾ��֮
							if(pCClientCommandList->GetCount() == 0)
							{
								SAFE_DELETE(pCClientCommandList);
								char szCommandID[10] = {'\0'};
								sprintf_safe(szCommandID, 10, "%d", u2CommandID);
								m_objClientCommandList.Del_Hash_Data(szCommandID);
								m_u4CurrCommandCount--;
							}
							break;
						}
					}
				}
			}
		}

		//ж�ز����Ϣ
		App_ModuleLoader::instance()->UnLoadModule(pModuleName, true);
	}

	//�����Ƿ�Ҫ���¼���
	if(u1State == 2)
	{
		if(strModulePath.length() > 0)
		{
			//���¼���
			App_ModuleLoader::instance()->LoadModule(strModulePath.c_str(), strModuleN.c_str(), strModuleParam.c_str());
		}
	}

	return true; 
}

int CMessageManager::GetCommandCount()
{
	return (int)m_u4CurrCommandCount;
}

void CMessageManager::Close()
{
	//��رյ�������
	for(int i = 0; i < (int)m_objClientCommandList.Get_Count(); i++)
	{
		CClientCommandList* pClientCommandList = m_objClientCommandList.Get_Index(i);
		 SAFE_DELETE(pClientCommandList);
	}
	m_objClientCommandList.Close();

	for(int i = 0; i < m_objModuleClientList.Get_Count(); i++)
	{
		_ModuleClient* pModuleClient = (_ModuleClient* )m_objModuleClientList.Get_Index(i);
		SAFE_DELETE(pModuleClient);
	}
	m_objModuleClientList.Close();

	m_u2MaxModuleCount  = 0;
	m_u4MaxCommandCount = 0;

}

CHashTable<_ModuleClient>* CMessageManager::GetModuleClient()
{
	return &m_objModuleClientList;
}

uint32 CMessageManager::GetWorkThreadCount()
{
	return App_MessageServiceGroup::instance()->GetWorkThreadCount();
}

uint32 CMessageManager::GetWorkThreadByIndex(uint32 u4Index)
{
	return App_MessageServiceGroup::instance()->GetWorkThreadIDByIndex(u4Index);
}


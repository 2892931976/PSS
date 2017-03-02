#ifndef _LOADMODULE_H
#define _LOADMODULE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"
#include "IObject.h"
#include "HashTable.h"

#include <string>
#include <vector>

using namespace std;

struct _ModuleInfo
{
	string           strModuleName;         //ģ���ļ�����
	string           strModulePath;         //ģ��·��
	string           strModuleParam;        //ģ���������� 
	ACE_Date_Time    dtCreateTime;          //ģ�鴴��ʱ��
	ACE_SHLIB_HANDLE hModule;
	int (*LoadModuleData)(CServerObject* pServerObject);
	int (*UnLoadModuleData)(void);
	const char* (*GetDesc)(void);
	const char* (*GetName)(void);
	const char* (*GetModuleKey)(void);
	int (*DoModuleMessage)(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
	bool (*GetModuleState)(uint32& u4AErrorID);

	_ModuleInfo()
	{
	}
};

class CLoadModule : public IModuleInfo
{
public:
	CLoadModule(void);
	virtual ~CLoadModule(void);

	void Init(uint16 u2MaxModuleCount);

	void Close();

	bool LoadModule(const char* pModulePath, const char* pModuleName, const char* pModuleParam);
	bool UnLoadModule(const char* szModuleName, bool blIsDelete = true);

	int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

	int  GetCurrModuleCount();
	int  GetModulePoolCount();
	_ModuleInfo* GetModuleIndex(int nIndex);
	_ModuleInfo* GetModuleInfo(const char* pModuleName);

	//����ӿ��ṩ��ع���
	bool GetModuleExist(const char* pModuleName);
	const char* GetModuleParam(const char* pModuleName);
	const char* GetModuleFileName(const char* pModuleName);
	const char* GetModuleFilePath(const char* pModuleName);
	const char* GetModuleFileDesc(const char* pModuleName);
	uint16 GetModuleCount();
	const char* GetModuleName(uint16 u2Index);

private:
	bool LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo, const char* pModulePath);    //��ʼ����ģ��Ľӿں�����

private:
	CHashTable<_ModuleInfo>            m_objHashModuleList;
	char                               m_szModulePath[MAX_BUFF_200];
	ACE_Recursive_Thread_Mutex         m_tmModule;
};

typedef ACE_Singleton<CLoadModule, ACE_Null_Mutex> App_ModuleLoader; 
#endif

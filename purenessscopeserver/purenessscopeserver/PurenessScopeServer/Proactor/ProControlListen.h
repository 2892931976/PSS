#ifndef _PROCONTROLLISTEN_H
#define _PROCONTROLLISTEN_H

#include "ProConnectAccept.h"
#include "AceProactorManager.h"

//���ӹ����࣬���ڴ���ɾ���Ͳ鿴����
//add by freeeyes

struct _ProControlInfo
{
	char m_szListenIP[MAX_BUFF_20];
	uint32 m_u4Port;

	_ProControlInfo()
	{
		m_szListenIP[0] = '\0';
		m_u4Port        = 0;
	}
};

typedef vector<_ProControlInfo> vecProControlInfo;

class CProControlListen
{
public:
	CProControlListen();
	~CProControlListen();

	bool AddListen(const char* pListenIP, uint32 u4Port, uint8 u1IPType);  //��һ���µļ����˿�
	bool DelListen(const char* pListenIP, uint32 u4Port);                  //�ر�һ����֪������
	void ShowListen(vecProControlInfo& objProControlInfo);                 //�鿴�Ѵ򿪵ļ����˿�
};

typedef ACE_Singleton<CProControlListen, ACE_Null_Mutex> App_ProControlListen;

#endif

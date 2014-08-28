#ifndef _CONTROLLISTEN_H
#define _CONTROLLISTEN_H

#include "ConnectAccept.h"
#include "AceReactorManager.h"

struct _ControlInfo
{
	char m_szListenIP[MAX_BUFF_20];
	uint32 m_u4Port;

	_ControlInfo()
	{
		m_szListenIP[0] = '\0';
		m_u4Port        = 0;
	}
};

typedef vector<_ControlInfo> vecControlInfo;

class CControlListen
{
public:
	CControlListen();
	~CControlListen();

	bool AddListen(const char* pListenIP, uint32 u4Port, uint8 u1IPType);  //��һ���µļ����˿�
	bool DelListen(const char* pListenIP, uint32 u4Port);                  //�ر�һ����֪������
	void ShowListen(vecControlInfo& objControlInfo);                       //�鿴�Ѵ򿪵ļ����˿�
};

typedef ACE_Singleton<CControlListen, ACE_Null_Mutex> App_ControlListen;

#endif

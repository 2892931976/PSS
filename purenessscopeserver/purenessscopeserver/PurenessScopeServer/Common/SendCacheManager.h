#ifndef _SENDCACHEMANAGER_H
#define _SENDCACHEMANAGER_H

#include "ISendCacheManager.h"
#include "MessageBlockManager.h"

#include <map>
using namespace std;

//�����¼���ͻ���أ����ڷ��������ڴ档
//add by freeeyes

//Ĭ�ϻ������10��
#define MAX_CACHE_POOL_SIZE 10

class CSendCacheManager : public ISendCacheManager
{
public:
	CSendCacheManager();
	~CSendCacheManager();

	void Init(uint32 u4CacheCount, uint32 u4CacheSize);
	void Close();

	uint32 GetFreeCount();
	uint32 GetUsedCount();

	//�õ�ָ����һ�����ӵĻ���
	ACE_Message_Block* GetCacheData(uint32 u4ConnectID);
	//�ͷ�ָ��һ�����ӵĻ���
	void FreeCacheData(uint32 u4ConnectID);

private:
	typedef map<uint32, ACE_Message_Block*> mapUsedCache;
	typedef vector<ACE_Message_Block*> vecFreeCache;

	mapUsedCache               m_mapUsedCache;
	vecFreeCache               m_vecFreeCache;
	uint32                     m_u4CacheSize;
};

#endif

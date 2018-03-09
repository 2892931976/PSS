#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//����������CreateBuffPacket��ǳ�����������Ҫ����������
//ʹ��BuffPacket�أ��ڷ�����������ʱ��һ���Դ����ö��Buffpacket�������ڴ�ص��뷨��
//�����лfreebird�Ĳ��Կͻ��ˣ�������Ϊ�����Ҳ��ܷ�����������⡣��Ҫ�ù�ע�����ʧ�������Ǳ���ġ�
//2010-06-03 freeeyes

#include "ObjectArrayList.h"
#include "HashTable.h"
#include "IPacketManager.h"
#include "BuffPacket.h"
#include "ObjectPoolManager.h"
#include "MainConfig.h"

using namespace std;

class CBuffPacketManager : public CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>, public IPacketManager
{
public:
    static void Init_Callback(int nIndex, CBuffPacket* pBuffPacket);

    uint32 GetBuffPacketUsedCount();
    uint32 GetBuffPacketFreeCount();
    void OutputCreateInfo();

    IBuffPacket* Create(const char* pFileName = __FILE__, uint32 u4Line = __LINE__);
    bool Delete(IBuffPacket* pBuffPacket);
    void GetCreateInfoList(vector<_Packet_Create_Info>& objCreateList);
};

typedef ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex> App_BuffPacketManager;
#endif

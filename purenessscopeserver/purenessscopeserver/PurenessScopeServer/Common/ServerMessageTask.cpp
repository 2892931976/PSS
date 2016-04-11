#include "ServerMessageTask.h"

Mutex_Allocator _msg_server_message_mb_allocator; 

CServerMessageTask::CServerMessageTask()
{
	m_u4ThreadID = 0;
	m_blRun      = false;
	m_u4MaxQueue = MAX_SERVER_MESSAGE_QUEUE;
	m_emState    = SERVER_RECV_INIT;
}

CServerMessageTask::~CServerMessageTask()
{
	OUR_DEBUG((LM_INFO, "[CServerMessageTask::~CServerMessageTask].\n"));
}

bool CServerMessageTask::IsRun()
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_RunMutex);

	return m_blRun;
}

bool CServerMessageTask::Start()
{
	if(0 != open())
	{
		return false;
	}

	return true;
}

int CServerMessageTask::open(void* args /*= 0*/)
{
	if(args != NULL)
	{
		OUR_DEBUG((LM_INFO,"[CServerMessageTask::open]args is not NULL.\n"));
	}

	m_blRun = true;

	if(activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED | THR_SUSPENDED, MAX_MSG_THREADCOUNT) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CServerMessageTask::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
		m_blRun = false;
		return -1;
	}

	resume();

	return 0;
}

int CServerMessageTask::Close()
{
	m_blRun = false;
	msg_queue()->deactivate();
	msg_queue()->flush();
	OUR_DEBUG((LM_INFO, "[CServerMessageTask::Close] Close().\n"));
	return 0;
}

int CServerMessageTask::svc(void)
{
	ACE_Message_Block* mb = NULL;

	//��΢��Ϣһ�£���һ�������߳�������ѭ��
	ACE_Time_Value tvSleep(0, MAX_MSG_SENDCHECKTIME*MAX_BUFF_1000);
	ACE_OS::sleep(tvSleep);

	while(IsRun())
	{
		mb = NULL;
		//xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CMessageService::svc] PutMessage error errno = [%d].\n", errno));
			m_blRun = false;
			break;
		}

		if (mb == NULL)
		{
			continue;
		}

		_Server_Message_Info* msg = *((_Server_Message_Info**)mb->base());
		if (! msg)
		{
			OUR_DEBUG((LM_ERROR,"[CMessageService::svc] mb msg == NULL CurrthreadNo=[%d]!\n", m_u4ThreadID));
			mb->release();
			continue;
		}

		this->ProcessMessage(msg, m_u4ThreadID);

		mb->release();
	}

	OUR_DEBUG((LM_INFO,"[CMessageService::svc] svc finish!\n"));
	return 0;
}

uint32 CServerMessageTask::GetThreadID()
{
	return m_u4ThreadID;
}

bool CServerMessageTask::PutMessage(_Server_Message_Info* pMessage)
{
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN(mb, 
		static_cast<ACE_Message_Block*>( _msg_server_message_mb_allocator.malloc(sizeof(ACE_Message_Block))),
		ACE_Message_Block(sizeof(_Server_Message_Info*), // size
		ACE_Message_Block::MB_DATA, // type
		0,
		0,
		&_msg_server_message_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_msg_server_message_mb_allocator,
		&_msg_server_message_mb_allocator
		));

	if(NULL != mb)
	{
		_Server_Message_Info** ppMessage = (_Server_Message_Info **)mb->base();
		*ppMessage = pMessage;

		//�ж϶����Ƿ����Ѿ����
		int nQueueCount = (int)msg_queue()->message_count();
		if(nQueueCount >= (int)m_u4MaxQueue)
		{
			OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
			mb->release();
			return false;
		}

		ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, 100000);
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
			mb->release();
			return false;
		}
	}
	else
	{
		OUR_DEBUG((LM_ERROR,"[CServerMessageTask::PutMessage] mb new error.\n"));
		return false;
	}

	return true;
}

bool CServerMessageTask::ProcessMessage(_Server_Message_Info* pMessage, uint32 u4ThreadID)
{
	m_tvDispose = ACE_OS::gettimeofday();

	m_emState   = SERVER_RECV_BEGIN;
	//�������ݰ�����
	pMessage->m_pClientMessage->RecvData(pMessage->m_u2CommandID, pMessage->m_pRecvFinish, pMessage->m_objServerIPInfo);
	//���մ����
	App_MessageBlockManager::instance()->Close(pMessage->m_pRecvFinish);
	SAFE_DELETE(pMessage);
	m_emState = SERVER_RECV_END;
	return true;
}

bool CServerMessageTask::CheckServerMessageThread(ACE_Time_Value tvNow)
{
	ACE_Time_Value tvIntval(tvNow - m_tvDispose);
	if(m_emState == SERVER_RECV_BEGIN && tvIntval.sec() > MAX_DISPOSE_TIMEOUT)
	{
		return false;
	}
	else
	{
		return true;
	}

}



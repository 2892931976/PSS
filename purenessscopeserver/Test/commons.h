#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <errno.h>  
#include <assert.h>  
#include <fcntl.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h> 

#define MAX_BUFF_200  200
#define MAX_BUFF_300  300
#define MAX_BUFF_1024 1024

#define COMMAND_AUTOTEST_HEAD 0x1000

//�������ݼ�
struct _ClientInfo
{
	char  m_szServerIP[30];
	int   m_nPort; 
	char* m_pSendBuffer;
	int   m_nSendLength;
	int   m_nRecvLength;

	_ClientInfo()
	{
		m_szServerIP[0] = '\0';
		m_nPort         = 0;
		m_pSendBuffer   = NULL;
		m_nSendLength   = 0;
		m_nRecvLength   = 0;
	}
};

//��������
struct _ResultInfo
{
	char m_szTestName[MAX_BUFF_300];     //���Լ�����
	char m_szResult[MAX_BUFF_1024];      //���Խ������ 
	int  m_nRet;                         //���Խ�� 0 �ɹ� 1 ʧ��  
	
	_ResultInfo()
	{
		m_szTestName[0] = '\0';
		m_szResult[0]   = '\0';
		m_nRet          = 0;
	}
};
#endif

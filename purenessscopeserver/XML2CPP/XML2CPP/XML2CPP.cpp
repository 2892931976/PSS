// XML2CPP.cpp : �������̨Ӧ�ó������ڵ㡣
//
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#endif

#include "Write_Packet_Parse_cpp.h"
#include "Write_Plugin_Main_cpp.h"
#include "Write_Plugin_Command.h"
#include "Write_Plugin_Make.h"
#include "Write_Plugin_Logic.h"

int main(int argc, char* argv[])
{
	char szFileName[255] = {'\0'};

	if(NULL == argv[1])
	{
		sprintf_safe(szFileName, 255, "protocol.xml");
	}
	else
	{
		sprintf_safe(szFileName, 255, "%s", argv[1]);
	}

	//��ȡ�����ļ�
	_Project_Info objProjectInfo;
	bool blState = Read_Project_File(PROJECT_FILE_NAME, objProjectInfo);
	if(false == blState)
	{
		printf("[error]Project XML is error.\n");
		return 0;
	}

	//��ȡ�����ļ�
	vecXmlInfo objvecXmlInfo;
	blState = Read_Command_File(szFileName, objvecXmlInfo);
	if(false == blState)
	{
		printf("[error]Command XML is error.\n");
		return 0;
	}

	//��������Ŀ¼
	if(strlen(objProjectInfo.m_szProjectName) == 0)
	{
		printf("[error]m_szProjectName is null.\n");
		return 0;
	}

	//��������Ŀ¼
	mkdir(objProjectInfo.m_szProjectName);

	//д�����ļ�
	Gen_2_Cpp_Main(objProjectInfo, objvecXmlInfo);

	//д�����ݰ��ļ�
	Gen_2_Cpp_Packet(objProjectInfo.m_szProjectName, objvecXmlInfo);

	//д��Commandͷ�ļ�
	Gen_2_Cpp_Command_H(objProjectInfo);

	//д��Command���ļ�
	Gen_2_Cpp_Command_Cpp(objProjectInfo, objvecXmlInfo);

	//����make�ļ�
	Gen_2_Make_define(objProjectInfo.m_szProjectName);
	Gen_2_Make(objProjectInfo.m_szProjectName, objProjectInfo);
	Gen_2_Mpc(objProjectInfo.m_szProjectName, objProjectInfo);
	Gen_2_ZZZ(objProjectInfo.m_szProjectName);
	Gen_2_Cpp_Logic_H(objProjectInfo, objvecXmlInfo);
	Gen_2_Cpp_Logic_Cpp(objProjectInfo, objvecXmlInfo);

	printf("[success]OK.\n");
	getchar();
	return 0;
}


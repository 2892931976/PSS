#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>

using namespace std;

//��XMLת��ΪMAP�ṹ
//add by freeeyes

//֧������
enum PROPERTY_TYPE
{
	PROPERTY_UNKNOW,
	PROPERTY_CHAR,
	PROPERTY_STRING,
	PROPERTY_UINT8,
	PROPERTY_UINT16,
	PROPERTY_UINT32,
	PROPERTY_UINT64,
	PROPERTY_INT8,
	PROPERTY_INT16,
	PROPERTY_INT32,
	PROPERTY_FLOAT32,
	PROPERTY_FLOAT64,
};

enum PROPERTY_CLASS
{
	CLASS_SINGLE,
	CLASS_VECTOR,
	CLASS_MAP,
};

enum COMMAND_TYPE
{
	COMMAND_NONE,
	COMMAND_IN,
	COMMAND_OUT,
};

//������Ϣ
struct _Property
{
	char           m_szPropertyName[50];   //��������
	char           m_szClassName[50];      //��������
	char           m_szDesc[100];          //������Ϣ  
	PROPERTY_TYPE  m_emType;               //��������
	PROPERTY_CLASS m_emClass;              //�������
	int            m_nLength;              //����
	int            m_nNeedHeadLength;      //�Ƿ���Ҫ�Զ�׷�����ݳ��� 0׷�ӣ�1��׷��
	char           m_szKeyName[50];        //KeyName
	PROPERTY_TYPE  m_emKeyType;            //key��������

	_Property()
	{
		m_szPropertyName[0] = '\0';
		m_szClassName[0]    = '\0';
		m_szDesc[0]         = '\0';
		m_szKeyName[0]      = '\0';
		m_emType            = PROPERTY_UNKNOW;
		m_emKeyType         = PROPERTY_UNKNOW;
		m_emClass           = CLASS_SINGLE;
		m_nLength           = 0;
		m_nNeedHeadLength   = 0;
	}
};

typedef vector<_Property> vecProperty;

//����Ϣ
struct _Xml_Info
{
	char           m_szXMLName[60];
	char           m_szDesc[100];
	char           m_szMacroName[50];
	int            m_nCommandID;
	COMMAND_TYPE   m_emCommandType;        //�������� 
	vecProperty    m_vecProperty;

	_Xml_Info()
	{
		m_nCommandID     = 0;
		m_szXMLName[0]   = '\0';
		m_szDesc[0]      = '\0';
		m_szMacroName[0] = '\0';
		m_emCommandType  = COMMAND_NONE;
	}
};
typedef vector<_Xml_Info> vecXmlInfo;

struct _Command_Relation_info
{
	char m_szCommandFuncName[100];
	int  m_nCommandInID;
	int  m_nCommandOutID;
	int  m_nOutPcket;        //0Ϊ��PacketParse 1Ϊֱ�ӷ���  

	_Command_Relation_info()
	{
		m_szCommandFuncName[0] = '\0';
		m_nCommandInID         = 0;
		m_nCommandOutID        = 0;
		m_nOutPcket            = 0;
	}
};
typedef vector<_Command_Relation_info> vecCommandRelationinfo;

struct _Project_Info
{
	char m_szProjectName[100];   //��������
	char m_szProjectDesc[200];   //��������
	char m_szProjectKey[100];    //����key

	vecCommandRelationinfo m_objCommandList;    //�����ϵ

	_Project_Info()
	{
		m_szProjectName[0] = '\0';
		m_szProjectDesc[0] = '\0';
		m_szProjectKey[0]  = '\0';
	}
};

inline void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(szText, nLen, fmt, ap);
	szText[nLen - 1] = '\0';

	va_end(ap);
};

class CXmlOpeation
{
public:
  CXmlOpeation(void);
  ~CXmlOpeation(void);

  bool Init(const char* pFileName);
  bool Init_String(const char* pXMLText);

  char* GetData(const char* pName, const char* pAttrName);
  char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

  char* GetData_Text(const char* pName);
  char* GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement);

  bool Parse_XML(char* pText, _Xml_Info& objxmlInfo);
  bool Parse_XML_File(const char* pFileName, vecXmlInfo& objvecXmlInfo);
  bool Parse_XML_File_Project(const char* pFileName, _Project_Info& objProjectInfo);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif

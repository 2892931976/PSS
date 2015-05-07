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

//������Ϣ
struct _Property
{
	char          m_szPropertyName[50];   //��������
	char          m_szDesc[100];          //������Ϣ  
	PROPERTY_TYPE m_emType;               //��������
	int           m_nLength;              //���� 

	_Property()
	{
		m_szPropertyName[0] = '\0';
		m_szDesc[0]         = '\0';
		m_emType            = PROPERTY_UNKNOW;
		m_nLength           = 0;
	}
};

typedef vector<_Property> vecProperty;

//����Ϣ
struct _Xml_Info
{
	char m_szXMLName[60];
	char m_szDesc[100];
	int  m_nCommandID;
	vecProperty m_vecProperty;

	_Xml_Info()
	{
		m_nCommandID   = 0;
		m_szXMLName[0] = '\0';
		m_szDesc[0]    = '\0';
	}
};
typedef vector<_Xml_Info> vecXmlInfo;

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
  bool Parse_XML_File(char* pFileName, vecXmlInfo& objvecXmlInfo);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif

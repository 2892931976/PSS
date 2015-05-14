#ifndef _WRITE_PACKET_PARSE_H
#define _WRITE_PACKET_PARSE_H

#include "XmlOpeation.h"

#define STRUCT_DATA_FILE   "DataFormat.h"
#define PROTOCAL_DATA_FILE "Protocol.h"

bool Read_Command_File(char* pFileName, vecXmlInfo& objvecXmlInfo)
{
	CXmlOpeation objXmlOpeation;
	return objXmlOpeation.Parse_XML_File(pFileName, objvecXmlInfo);
}

void Display(_Xml_Info& objxmlInfo)
{
	printf("[Display]structName=%s.\n", objxmlInfo.m_szXMLName);
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		printf("[Display]PrpertyName=%s, TYPE=%d, length=%d.\n", 
			objxmlInfo.m_vecProperty[i].m_szPropertyName,
			objxmlInfo.m_vecProperty[i].m_emType,
			objxmlInfo.m_vecProperty[i].m_nLength);
	}
}

//����vec��map�������ṹ
void Gen_2_Cpp_Class(FILE* pFile, _Xml_Info& objxmlInfo)
{
	char szTemp[200] = {'\0'};
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UNKNOW)
		{
			if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_VECTOR)
			{
				sprintf_safe(szTemp, 200, "typedef vector<%s> vec%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_MAP)
			{
				if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_STRING)
				{
					sprintf_safe(szTemp, 200, "typedef map<string, %s> map%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT16)
				{
					sprintf_safe(szTemp, 200, "typedef map<uint16, %s> map%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT32)
				{
					sprintf_safe(szTemp, 200, "typedef map<uint32, %s> map%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
		}
	}
};

//�������ݽṹ
void Gen_2_Cpp_Struct(FILE* pFile, _Xml_Info& objxmlInfo)
{
	char szTemp[200] = {'\0'};
	sprintf_safe(szTemp, 200, "//%s \n", objxmlInfo.m_szDesc);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "struct %s \n", objxmlInfo.m_szXMLName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			sprintf_safe(szTemp, 200, "\tchar m_sz%s[%d];  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName, 
				objxmlInfo.m_vecProperty[i].m_nLength,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
		{
			sprintf_safe(szTemp, 200, "\tstring m_str%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT8)
		{
			sprintf_safe(szTemp, 200, "\tuint8 m_u1%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT16)
		{
			sprintf_safe(szTemp, 200, "\tuint16 m_u2%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT32)
		{
			sprintf_safe(szTemp, 200, "\tuint32 m_u4%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT64)
		{
			sprintf_safe(szTemp, 200, "\tuint64 m_u8%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT8)
		{
			sprintf_safe(szTemp, 200, "\tint8 m_n1%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT16)
		{
			sprintf_safe(szTemp, 200, "\tint16 m_n2%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT32)
		{
			sprintf_safe(szTemp, 200, "\tint32 m_n4%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT32)
		{
			sprintf_safe(szTemp, 200, "\tfloat32 m_f4%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT64)
		{
			sprintf_safe(szTemp, 200, "\tfloat64 m_f8%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else
		{
			if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_SINGLE)
			{
				sprintf_safe(szTemp, 200, "\t%s m_obj%s;  //%s\n", 
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szDesc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_VECTOR)
			{
				sprintf_safe(szTemp, 200, "\tvec%s m_objvec%s;  //%s\n", 
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szDesc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_MAP)
			{
				sprintf_safe(szTemp, 200, "\tmap%s m_objmap%s;  //%s\n", 
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szDesc);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}
	sprintf_safe(szTemp, 200, "};\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
}

//����д����
void Gen_2_Cpp_In_Stream(FILE* pFile, _Xml_Info& objxmlInfo)
{
	char szTemp[200] = {'\0'};
	sprintf_safe(szTemp, 200, "//%s In BuffPacket \n", objxmlInfo.m_szXMLName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "inline bool %s_In_Stream(%s& obj%s, IBuffPacket* pBuffPacket)\n", 
		objxmlInfo.m_szXMLName, 
		objxmlInfo.m_szXMLName,
		objxmlInfo.m_szXMLName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{ \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tif(NULL == pBuffPacket)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{ \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\treturn false; \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t} \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//�ȴ����ַ���������
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			if(objxmlInfo.m_vecProperty[i].m_nLength <= 255)
			{
				sprintf_safe(szTemp, 200, "\t_VCHARS_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_nLength <= 56635)
			{
				sprintf_safe(szTemp, 200, "\t_VCHARM_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t_VCHARB_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}

	//����������
	if(objxmlInfo.m_nCommandID > 0)
	{
		sprintf_safe(szTemp, 200, "\tuint16 u2CommandID = (uint16)%d;\n", 
			objxmlInfo.m_nCommandID);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << u2CommandID;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//��������д����
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			if(objxmlInfo.m_vecProperty[i].m_nLength < 255)
			{
				sprintf_safe(szTemp, 200, "\tuint8 u1%sLen = (uint8)strlen(obj%s.m_sz%s);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tvs%s.SetData(obj%s.m_sz%s, u1%sLen);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << vs%s;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_nLength < 65535 && objxmlInfo.m_vecProperty[i].m_nLength >= 255)
			{
				sprintf_safe(szTemp, 200, "\tuint16 u2%sLen = (uint16)strlen(obj%s.m_sz%s);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tvs%s.SetData(obj%s.m_sz%s, u2%sLen);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << vs%s;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tuint32 u4%sLen = (uint32)strlen(obj%s.m_sz%s);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tvs%s.SetData(obj%s.m_sz%s, u4%sLen);\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << vs%s;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_str%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_u1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_u2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_u4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_u8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << %s.m_n1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_n2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_n4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_f4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << obj%s.m_f8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else
		{
			if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_SINGLE)
			{
				//���࣬������֮ǰ�ķ���ʵ��֮
				sprintf_safe(szTemp, 200, "\t%s_In_Stream(obj%s.m_obj%s, pBuffPacket);\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_VECTOR)
			{
				//��vector
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << (uint16)obj%s.m_objvec%s.size();\n",
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < (int)obj%s.m_objvec%s.size(); i++)\n",
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t%s_In_Stream((%s)obj%s.m_objvec%s[i], pBuffPacket);\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//��map
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) << (uint16)obj%s.m_objmap%s.size();\n",
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tfor(map%s::iterator itorFreeB = obj%s.m_objmap%s.begin(); itorFreeB != obj%s.m_objmap%s.end(); itorFreeB++)\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_STRING)
				{
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) << (string)itorFreeB->first;\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT16)
				{
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) << (uint16)itorFreeB->first;\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT32)
				{
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) << (uint32)itorFreeB->first;\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				sprintf_safe(szTemp, 200, "\t\t%s& obj%s = (%s)itorFreeB->second;\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t%s_In_Stream(obj%s, pBuffPacket);\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}

	sprintf_safe(szTemp, 200, "\treturn true;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "} \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
}

//���ɶ�����
void Gen_2_Cpp_Out_Stream(FILE* pFile, _Xml_Info& objxmlInfo)
{
	char szTemp[200] = {'\0'};
	sprintf_safe(szTemp, 200, "//%s Out BuffPacket \n", objxmlInfo.m_szXMLName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "inline bool %s_Out_Stream(%s& obj%s, IBuffPacket* pBuffPacket)\n", 
		objxmlInfo.m_szXMLName, 
		objxmlInfo.m_szXMLName,
		objxmlInfo.m_szXMLName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{ \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "\tif(NULL == pBuffPacket)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{ \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\treturn false; \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t} \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	//�ȴ����ַ���������
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			if(objxmlInfo.m_vecProperty[i].m_nLength <= 255)
			{
				sprintf_safe(szTemp, 200, "\t_VCHARS_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_nLength <= 56635)
			{
				sprintf_safe(szTemp, 200, "\t_VCHARM_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\t_VCHARB_STR vs%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
		{
			sprintf_safe(szTemp, 200, "\t_VCHARM_STR vs%s;\n", 
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
	}

	//����������
	if(objxmlInfo.m_nCommandID > 0)
	{
		sprintf_safe(szTemp, 200, "\tuint16 u2CommandID = 0;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> u2CommandID;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//�������ж�����
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> vs%s;\n",
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tsprintf_safe(obj%s.m_sz%s, %d, \"%%s\", vs%s.text);\n",
				objxmlInfo.m_szXMLName, 
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_nLength,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_str%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_u1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_u2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_u4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_u8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_n1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_n2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_n4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_f4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> obj%s.m_f8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else
		{
			if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_SINGLE)
			{
				//���࣬������֮ǰ�ķ���ʵ��֮
				sprintf_safe(szTemp, 200, "\t%s_Out_Stream(obj%s.m_obj%s, pBuffPacket);\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else if(objxmlInfo.m_vecProperty[i].m_emClass == CLASS_VECTOR)
			{
				//��vector
				sprintf_safe(szTemp, 200, "\tuint16 u2%sCount = 0;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> u2%sCount;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < (int)u2%sCount; i++)\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t%s obj%s;\n", 
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t%s_Out_Stream(obj%s, pBuffPacket);\n",
					objxmlInfo.m_vecProperty[i].m_szClassName,
					objxmlInfo.m_vecProperty[i].m_szClassName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tobj%s.m_objvec%s.push_back(obj%s);\n",
					objxmlInfo.m_szXMLName,
					objxmlInfo.m_vecProperty[i].m_szPropertyName,
					objxmlInfo.m_vecProperty[i].m_szClassName); 
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//��map
				sprintf_safe(szTemp, 200, "\tuint16 u2%sCount = 0;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t(*pBuffPacket) >> u2%sCount;\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < (int)u2%sCount; i++)\n",
					objxmlInfo.m_vecProperty[i].m_szPropertyName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_STRING)
				{
					sprintf_safe(szTemp, 200, "\t\tstring str%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s obj%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) >> str%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s_Out_Stream(obj%s, pBuffPacket);\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tobj%s.m_objmap%s.insert(map%s::value_type(str%s, obj%s));\n",
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szPropertyName,
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szKeyName, 
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT16)
				{
					sprintf_safe(szTemp, 200, "\t\tuint16 u2%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s obj%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) >> u2%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s_Out_Stream(obj%s, pBuffPacket);\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tobj%s.m_objmap%s.insert(map%s::value_type(u2%s, obj%s));\n",
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szPropertyName,
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szKeyName, 
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else if(objxmlInfo.m_vecProperty[i].m_emKeyType == PROPERTY_UINT32)
				{
					sprintf_safe(szTemp, 200, "\t\tuint32 u4%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s obj%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t(*pBuffPacket) >> u4%s;\n", 
						objxmlInfo.m_vecProperty[i].m_szKeyName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t%s_Out_Stream(obj%s, pBuffPacket);\n",
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tobj%s.m_objmap%s.insert(map%s::value_type(u4%s, obj%s));\n",
						objxmlInfo.m_szXMLName,
						objxmlInfo.m_vecProperty[i].m_szPropertyName,
						objxmlInfo.m_vecProperty[i].m_szClassName,
						objxmlInfo.m_vecProperty[i].m_szKeyName, 
						objxmlInfo.m_vecProperty[i].m_szClassName); 
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}

	sprintf_safe(szTemp, 200, "\treturn true;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "} \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
}

//ת��ΪCPP�ļ��ķ���
void Gen_2_Cpp_Packet(const char* pPath, vecXmlInfo& objvecXmlInfo)
{
	char szTemp[200]     = {'\0'};
	char szPathFile[200] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/%s", pPath, STRUCT_DATA_FILE);

	//�������������ļ���
	FILE* pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	//д����Ҫ�����ͷ�ļ�
	sprintf_safe(szTemp, 200, "#include \"define.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include <vector>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include <map>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "using namespace std;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)objvecXmlInfo.size(); i++)
	{
		//Display(objvecXmlInfo[i]);
		//����������
		Gen_2_Cpp_Class(pFile, objvecXmlInfo[i]);

		//�������ݽṹ�ļ�
		Gen_2_Cpp_Struct(pFile, objvecXmlInfo[i]);

		sprintf_safe(szTemp, 200, "\n", objvecXmlInfo[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	fclose(pFile);

	sprintf_safe(szPathFile, 200, "%s/%s", pPath, PROTOCAL_DATA_FILE);

	pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	sprintf_safe(szTemp, 200, "#include \"%s\"\n", STRUCT_DATA_FILE);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"IBuffPacket.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	//���ɶ�ȡ��д���ļ�
	for(int i = 0; i < (int)objvecXmlInfo.size(); i++)
	{
		//��������д����
		Gen_2_Cpp_In_Stream(pFile, objvecXmlInfo[i]);

		sprintf_safe(szTemp, 200, "\n", objvecXmlInfo[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		//���ɶ�����
		Gen_2_Cpp_Out_Stream(pFile, objvecXmlInfo[i]);

		sprintf_safe(szTemp, 200, "\n", objvecXmlInfo[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	fclose(pFile);
}

#endif

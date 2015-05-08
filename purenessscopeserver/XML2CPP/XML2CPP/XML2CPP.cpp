// XML2CPP.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "XmlOpeation.h"

#define STRUCT_DATA_FILE   "DataFormat.h"
#define PROTOCAL_DATA_FILE "Protocol.h"

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
			sprintf_safe(szTemp, 200, "\t%s m_obj%s;  //%s\n", 
				objxmlInfo.m_vecProperty[i].m_szClassName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_vecProperty[i].m_szDesc);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
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
	sprintf_safe(szTemp, 200, "bool %s_In_Stream(%s& obj%s, IBuffPacket* pBuffPacket)\n", 
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
		sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << u2CommandID;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//��������д����
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			sprintf_safe(szTemp, 200, "\tvs%s.SetData(obj%s.m_sz%s, (int)strlen(obj%s.m_sz%s));\n",
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName,
				objxmlInfo.m_szXMLName, 
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << vs%s;\n",
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_str%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_u1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_u2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_u4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_u8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << %s.m_n1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_n2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_n4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_f4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) << obj%s.m_f8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else
		{
			//���࣬������֮ǰ�ķ���ʵ��֮
			sprintf_safe(szTemp, 200, "\t%s_In_Stream(obj%s.m_obj%s, pBuffPacket);\n",
				objxmlInfo.m_vecProperty[i].m_szClassName,
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szClassName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
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
	sprintf_safe(szTemp, 200, "bool %s_Out_Stream(%s& obj%s, IBuffPacket* pBuffPacket)\n", 
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
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_STRING)
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
		sprintf_safe(szTemp, 200, "\tuint16 u2CommandID = 0;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> u2CommandID;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	//�������ж�����
	for(int i = 0; i < (int)objxmlInfo.m_vecProperty.size(); i++)
	{
		if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_CHAR)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> vs%s;\n",
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
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_str%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_u1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_u2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_u4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_UINT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_u8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT8)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_n1%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT16)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_n2%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_INT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_n4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT32)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_f4%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else if(objxmlInfo.m_vecProperty[i].m_emType == PROPERTY_FLOAT64)
		{
			sprintf_safe(szTemp, 200, "\t(*pBuffPacketvs) >> obj%s.m_f8%s;\n",
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szPropertyName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
		else
		{
			//���࣬������֮ǰ�ķ���ʵ��֮
			sprintf_safe(szTemp, 200, "\t%s_Out_Stream(obj%s.m_obj%s, pBuffPacket);\n",
				objxmlInfo.m_vecProperty[i].m_szClassName,
				objxmlInfo.m_szXMLName,
				objxmlInfo.m_vecProperty[i].m_szClassName); 
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
	}

	sprintf_safe(szTemp, 200, "\treturn true;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "} \n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
}

//ת��ΪCPP�ļ��ķ���
void Gen_2_Cpp(vecXmlInfo& objvecXmlInfo)
{
	char szTemp[200]     = {'\0'};

	//�������������ļ���
	FILE* pFile = fopen(STRUCT_DATA_FILE, "w");
	if(NULL == pFile)
	{
		return;
	}

	//д����Ҫ�����ͷ�ļ�
	sprintf_safe(szTemp, 200, "#include \"define.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)objvecXmlInfo.size(); i++)
	{
		//Display(objvecXmlInfo[i]);

		//�������ݽṹ�ļ�
		Gen_2_Cpp_Struct(pFile, objvecXmlInfo[i]);

		sprintf_safe(szTemp, 200, "\n", objvecXmlInfo[i].m_szDesc);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	fclose(pFile);

	pFile = fopen(PROTOCAL_DATA_FILE, "w");
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

int main(int argc, char* argv[])
{
	CXmlOpeation objXmlOpeation;
	_Xml_Info    objxmlInfo;

	char szFileName[255] = {'\0'};

	if(NULL == argv[1])
	{
		sprintf_safe(szFileName, 255, "protocol.xml");
	}
	else
	{
		sprintf_safe(szFileName, 255, "%s", argv[1]);
	}

	vecXmlInfo objvecXmlInfo;

	bool blState = objXmlOpeation.Parse_XML_File(szFileName, objvecXmlInfo);
	if(true == blState)
	{
		Gen_2_Cpp(objvecXmlInfo);
		printf("[success]OK.\n");
	}
	else
	{
		printf("[error]XML is error.\n");
	}

	getchar();
	return 0;
}


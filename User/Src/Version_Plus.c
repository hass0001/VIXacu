/************************************************************************************************************
**
**	@file		version_model.c
**	@date	2012/07/03
**	@brief	 Firmware ���� ���� ��
**			
**						
*************************************************************************************************************/
#define    VERSION_GLOBALS 
#include <string.h>
#include "Version_Plus.h"

/**
	@name	VS_GetVersionStr
	
	@param	versionStr : OUT, ��Ʈ�� ���� �����͸� ���� �ּ� (20bytes �����͸� ���� �� �־�� �Ѵ�)
	
	@return	void
	
	@brief	������ ��Ʈ������ ���´�. 
	 
*/
void VS_GetVersionStr(char* versionStr)
{
	int i,j,k;
	char temp[100];
	char version[3];
	char stable[3];
	char development[5];
	char* custom1 = (char*)VS_CUSTOMIZING1;
	char* custom2 = (char*)VS_CUSTOMIZING2;
	char length = 0;

	version[0] = (char)(((unsigned char)VS_VERSION/100)+'0');
	version[1] = (char)((((unsigned char)VS_VERSION%100)/10)+'0');
	version[2] = (char)(((unsigned char)VS_VERSION%10)+'0');

	stable[0] = (char)(((unsigned char)VS_STABLE/100)+'0');
	stable[1] = (char)((((unsigned char)VS_STABLE%100)/10)+'0');
	stable[2] = (char)(((unsigned char)VS_STABLE%10)+'0');

	development[0] = (char)(((unsigned short)VS_DEVELOPMENT/10000)+'0');
	development[1] = (char)((((unsigned short)VS_DEVELOPMENT%10000)/1000)+'0');
	development[2] = (char)((((unsigned short)VS_DEVELOPMENT%1000)/100)+'0');
	development[3] = (char)((((unsigned short)VS_DEVELOPMENT%100)/10)+'0');
	development[4] = (char)(((unsigned short)VS_DEVELOPMENT%10)+'0');

	for(i = 0; (i<3&&version[i] == '0'); i++){}	
	for(j = 3,k=i; j>i ;j--,k++)
	{
		temp[length++] = version[k];
	}
	if(i == 3)	temp[length++] = version[2];
	temp[length++] = '.';
	
	for(i = 0; (i<3&&stable[i] == '0'); i++){}
	for(j = 3,k=i; j>i ;j--,k++)
	{
		temp[length++] = stable[k];
	}
	if(i == 3)	temp[length++] = stable[2];
	temp[length++] = '.';
	
	for(i = 0; (i<5&&development[i] == '0'); i++){}
	for(j = 5,k=i; j>i ;j--,k++)
	{
		temp[length++] = development[k];
	}
	if(i == 5)	temp[length++] = development[4];
	temp[length++] = '.';

	if(custom1 != 0 && custom2 != 0)
	{
		temp[length++] = custom1[0];
		temp[length++] = custom1[1];
		temp[length++] = '.';
		temp[length++] = custom2[0];
		temp[length++] = custom2[1];
		temp[length++] = '\0';		
	}
	else if(custom1 != 0 && custom2 == 0)
	{
		temp[length++] = custom1[0];
		temp[length++] = custom1[1];
		temp[length++] = '\0';				
	}
	else if(custom1 == 0 && custom2 != 0)
	{
		temp[length++] = custom2[0];
		temp[length++] = custom2[1];
		temp[length++] = '\0';
	}
	else
	{
		temp[length-1] = '\0';
	}

	for(i = 0; i<length; i++)
	{
		versionStr[i] = temp[i];
	}
	
	strncpy( (char *)fwCompileTime, (const char *)__TIME__, 8); 
	fwCompileTime[8] = 0;
}

/**
	@name	VS_GetVersionStrFromData
	
	@param	data : IN, ���� ������ �ּ� (8bytes �����͸� ���� �� �־�� �Ѵ�)
			versionStr : OUT, ���� ��Ʈ�� ������ �ּ� (20bytes �����͸� ���� �� �־�� �Ѵ�)
			
	@return	void
	
	@brief	���� �����͸� ��Ʈ������ ��ȯ�Ͽ� �ش�.
*/
void VS_GetVersionStrFromData(unsigned char* data, char* versionStr)
{
	int i,j,k;
	char temp[100];
	char version[3];
	char stable[3];
	char development[5];
	unsigned short developmentData;
	char custom1[2];
	char custom2[2];
	char length = 0;	
	
	version[0] = (char)((data[0]/100)+'0');
	version[1] = (char)(((data[0]%100)/10)+'0');
	version[2] = (char)((data[0]%10)+'0');

	stable[0] = (char)((data[1]/100)+'0');
	stable[1] = (char)(((data[1]%100)/10)+'0');
	stable[2] = (char)((data[1]%10)+'0');

	developmentData = (unsigned short)(data[2]<<8|data[3]);
	
	development[0] = (char)((developmentData/10000)+'0');
	development[1] = (char)(((developmentData%10000)/1000)+'0');
	development[2] = (char)(((developmentData%1000)/100)+'0');
	development[3] = (char)(((developmentData%100)/10)+'0');
	development[4] = (char)((developmentData%10)+'0');
	
	custom1[0] = (char)data[4];
	custom1[1] = (char)data[5];
	
	custom2[0] = (char)data[6];
	custom2[1] = (char)data[7];

	for(i = 0; (i<3&&version[i] == '0'); i++){}	
	for(j = 3,k=i; j>i ;j--,k++)
	{
		temp[length++] = version[k];
	}
	if(i == 3)	temp[length++] = version[2];
	temp[length++] = '.';
	
	for(i = 0; (i<3&&stable[i] == '0'); i++){}
	for(j = 3,k=i; j>i ;j--,k++)
	{
		temp[length++] = stable[k];
	}
	if(i == 3)	temp[length++] = stable[2];
	temp[length++] = '.';
	
	for(i = 0; (i<5&&development[i] == '0'); i++){}
	for(j = 5,k=i; j>i ;j--,k++)
	{
		temp[length++] = development[k];
	}
	if(i == 5)	temp[length++] = development[4];
	temp[length++] = '.';

	if(custom1[0] != '0' && custom2[0] != '0')
	{
		temp[length++] = custom1[0];
		temp[length++] = custom1[1];
		temp[length++] = '.';
		temp[length++] = custom2[0];
		temp[length++] = custom2[1];
		temp[length++] = '\0';		
	}
	else if(custom1[0] != '0' && custom2[0] == '0')
	{
		temp[length++] = custom1[0];
		temp[length++] = custom1[1];
		temp[length++] = '\0';				
	}
	else if(custom1[0] == '0' && custom2[0] != '0')
	{
		temp[length++] = custom2[0];
		temp[length++] = custom2[1];
		temp[length++] = '\0';
	}
	else
	{
		temp[length-1] = '\0';
	}

	for(i = 0; i<length; i++)
	{
		versionStr[i] = temp[i];
	}
	
}


/**
	@name	VS_GetVersionData
	
	@param	data : OUT, ���� �����͸� ���� ������ �ּ� (8bytes �����͸� ���� �� �־�� �Ѵ�)
	
	@return	void
	
	@brief	���� �����͸� ���´�. 
			������ Ÿ ����̽��� ������ �� ����ϸ� �ȴ�. (���� ũ�� 8bytes�̴�)
*/
void VS_GetVersionData(unsigned char* data)
{
	unsigned char* custom1 = (unsigned char*)VS_CUSTOMIZING1;
	unsigned char* custom2 = (unsigned char*)VS_CUSTOMIZING2;
	
	data[0] = (unsigned char)VS_VERSION;
	data[1] = (unsigned char)VS_STABLE;
	data[2] = (unsigned char)((((unsigned short)VS_DEVELOPMENT)>>8)&0xFF);
	data[3] = (unsigned char)((unsigned short)VS_DEVELOPMENT&0xFF);

	if(custom1 != 0 && custom2 != 0)
	{
		data[4] = custom1[0];
		data[5] = custom1[1];
		data[6] = custom2[0];
		data[7] = custom2[1];		
	}
	else if(custom1 != 0 && custom2 == 0)
	{
		data[4] = custom1[0];
		data[5] = custom1[1];
		data[6] = '0';
		data[7] = '0';	
	}
	else if(custom1 == 0 && custom2 != 0)
	{
		data[4] = '0';
		data[5] = '0';
		data[6] = custom2[0];
		data[7] = custom2[1];		
	}
	else
	{
		data[4] = '0';
		data[5] = '0';
		data[6] = '0';
		data[7] = '0';
	}
}

/**
	@name	VS_CompareVersion
	
	@param 	data1 : IN, ���� ������ 1 �ּ�(������ size�� ���� 8byte�� �Ǿ�� �Ѵ�.)
			data2 : IN, ���� ������ 2 �ּ�(������ size�� ���� 8byte�� �Ǿ�� �Ѵ�.) 
			
	@return	-1 : data1�� data2���� ������ ���� ��� 
			0 : data1�� data2 ������ ���� ���
			1 : data2�� data1���� ������ ���� ���
			
	@brief	������ ������ ���Ѵ�. 
			Ŀ���͸���¡ ������ ������ ������, ���� Version, Stable, Development ������ ���ϰ� �ȴ�. 
*/
char VS_CompareVersion(unsigned char* data1, unsigned char* data2)
{
	unsigned short developmentData[2];
	
	developmentData[0] = (unsigned short)(data1[2]<<8|data1[3]);
	developmentData[1] = (unsigned short)(data2[2]<<8|data2[3]);
	
	if(data1[0] > data2[0])		return -1;
	else if(data1[0] < data2[0])	return 1;
	else
	{
		if(data1[1] > data2[1])		return -1;
		else if(data1[1] < data2[1])	return 1;
		else
		{
			if(developmentData[0] < developmentData[1])		return -1;
			else if(developmentData[0] > developmentData[1])	return 1;
			else											return 0;
		}
	}
}



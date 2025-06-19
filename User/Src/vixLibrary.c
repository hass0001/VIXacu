/*
 * vixLibrary.c
 *
 *  Created on: Nov 1, 2024
 *      Author: intellivix
 */


#include "vixConfig.h"



/*
@Name: xCONVERT_u32Tou8Array
@Brief: 
@Param: 
@Param: 
@Param: 
@Return: 
@Note: 
*/
ErrState xCONVERT_u32Tou8Array(const uint32_t Src, uint8_t* pDest, uint16_t Length, uint8_t ByteOrder)
{
	if(Length == 0 || Length > 4)
		return _LenErr;

	if(ByteOrder == _LSB)
	{
		pDest[0] = (uint8_t)(Src & 0x000000FF);
		pDest[1] = (uint8_t)((Src & 0x0000FF00) >> 8);
		pDest[2] = (uint8_t)((Src & 0x00FF0000) >> 16);
		pDest[3] = (uint8_t)((Src & 0xFF000000) >> 24);
	}
	else			// _MSB
	{
		pDest[0] = (uint8_t)((Src & 0xFF000000) >> 24);
		pDest[1] = (uint8_t)((Src & 0x00FF0000) >> 16);
		pDest[2] = (uint8_t)((Src & 0x0000FF00) >> 8);
		pDest[3] = (uint8_t)(Src & 0x000000FF);
	}

	return _Ok;
}


uint32_t bCONVERT_u8ArrayTou32(const uint8_t* pSrc)
{
	if(pSrc == NULL)
		return 0;

	return (uint32_t)(pSrc[0] << 24 | pSrc[1] << 16 | pSrc[2] << 8 | pSrc[3]);
}


/*
@Name: xCONVERT_ArraryToString
@Brief: Array�� String���� ��ȯ
@Param: ���� Array
@Param: ���� Array ����
@Param: NULL�� �ٿ� String���� ��ȯ�� Array
@Return: ó�� ��� 
@Note: Dest �� �ڿ� NULL�� �ٿ� ��ȯ
*/
ErrState xCONVERT_ArraryToString(const char* pSrc, uint16_t Length, uint8_t* pDest)
{
	uint16_t u16Cnt;

	if(pSrc == NULL || pDest == NULL)
		return _NullErr;

	if(Length > 1024)
		return _LenErr;

	for(u16Cnt=0; u16Cnt < Length; u16Cnt++)
	{
		pDest[u16Cnt] = pSrc[u16Cnt];
	}
	pDest[u16Cnt] = '\0';		

	return _Ok;
}


/*
@Name: xCONVERT_StringToArray
@Brief: 
@Param: 
@Param: 
@Param: 
@Return: ó�� ���
@Note: 
*/
uint16_t bCONVERT_StringToArray(const char* pSrc, uint8_t* pDest)
{
	uint16_t u16Cnt, msgLen;
	char temp[3];

	if(pSrc == NULL || pDest == NULL)
		return _NullErr;

	msgLen = (int)strlen((char*)pSrc);
	if((msgLen % 2) != 0)
		return _LenErr;

	for(u16Cnt=0; u16Cnt < msgLen; u16Cnt++)
	{
		memcpy(temp, &pSrc[u16Cnt*2], 2);
		temp[2] = 0;
		pDest[u16Cnt] = (uint8_t)strtoul(temp, NULL, 16);
	}

	return _Ok;
}



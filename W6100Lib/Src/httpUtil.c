/**
 * @file	httpUtil.c
 * @brief	HTTP Server Utilities	
 * @version 1.0
 * @date	2014/07/15
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vixConfig.h"

#include "httpUtil.h"
#include "httpClient.h"


extern _HTTPC_GetPostTypeDef HTTP_RestCmd[MAX_NUM_DOOR];

uint8_t Door1ValidTZ;

uint8_t http_get_cgi_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;

	if(predefined_get_cgi_processor(uri_name, buf, &len))
	{
		;
	}
	else if(strcmp((const char *)uri_name, "example.cgi") == 0)
	{
		// To do
		;
	}
	else
	{
		// CGI file not found
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}

uint8_t http_post_cgi_handler(uint8_t * uri_name, st_http_request * p_http_request, uint8_t * buf, uint32_t * file_len)
{
	uint8_t ret = HTTP_OK;
	uint16_t len = 0;
	uint8_t val = 0;

	if(predefined_set_cgi_processor(uri_name, p_http_request->URI, buf, &len))
	{
		;
	}
	else if(strcmp((const char *)uri_name, "example.cgi") == 0)
	{
		// To do
		val = 1;
		len = sprintf((char *)buf, "%d", val);
	}
	else
	{
		// CGI file not found
		ret = HTTP_FAILED;
	}

	if(ret)	*file_len = len;
	return ret;
}

void HTTPC_OldDecodeTime(uint32_t tt, uint8_t *tBuffer)
{
	*(tBuffer+5) = (uint8_t)(tt % 60); tt /= 60;   // sec
	*(tBuffer+4) = (uint8_t)(tt % 60); tt /= 60;    // min
	*(tBuffer+3) =(uint8_t)((tt % 24)+9); tt /= 24;	// hour   +9 UTC
	*(tBuffer+2) = (uint8_t)(tt % 31 + 1); tt /= 31;	// day
	*(tBuffer+1) = (uint8_t)(tt % 12 + 1); tt /= 12; 	//mon
//	*(tBuffer+0) =(uint8_t)(tt + 2000);				// year
	*(tBuffer+0) =(uint8_t)(tt);				// year
}

uint32_t HTTPC_GetSecFromDateTime( uint8_t *chrTime)
{
	uint32_t n32Sec = 0;

	n32Sec += (*(chrTime+8)-0x30) * 100000000;
	n32Sec += (*(chrTime+7)-0x30) * 10000000;
	n32Sec += (*(chrTime+6)-0x30) * 1000000;
	n32Sec += (*(chrTime+5)-0x30) * 100000;
	n32Sec += (*(chrTime+4)-0x30) * 10000;
	n32Sec += (*(chrTime+3)-0x30) * 1000;
	n32Sec += (*(chrTime+2)-0x30) * 100;
	n32Sec += (*(chrTime+1)-0x30) * 10;
	n32Sec += (*(chrTime+0)-0x30) * 1;

	return n32Sec;
}

void CmdSetOptionDateTime( void )
{
}

/**
@brief	CONVERT CHAR INTO HEX
@return	HEX
This function converts HEX(0-F) to a character
*/
uint8_t A2H( uint8_t c)
{	/**< is a character('0'-'F') to convert to HEX */
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}

void parseDurString(uint8_t * encodedStr, uint8_t * startHour, uint8_t * startMinute, uint8_t * endHour, uint8_t * endMinute)
{
    uint8_t *endptr;
    uint32_t  encoded = strtol(encodedStr, (char **)&endptr, 10);

    uint16_t startInt = (uint16_t)(encoded / 0x10000);
    uint16_t endInt   = (uint16_t)(encoded % 0x10000);

#ifdef DEBUG_MODE
	printf( "parseDurString[%s],startInt=%d,endInt=%d\r\n",encodedStr, startInt, endInt);
#endif

    *startHour = startInt / 100;
    *startMinute = startInt % 100;

    *endHour = endInt / 100;
    *endMinute = endInt % 100;

#ifdef DEBUG_MODE
	printf( "parseDur result[s],startInt=%d:%d,endInt=%d:%d\r\n",encodedStr, *startHour, *startMinute, *endHour, *endMinute);
#endif
}

/////////// 커맨드 처리 영역 /////////////
void Httpc_TreatUpgrade( uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t* ptr;
	uint8_t * ptrId;
 	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;

	uint8_t * ptrData;
	uint8_t * ptrChecksum;
	uint8_t * prtUrl;
	uint8_t * ptrSize;

//		 nHttpcCmd = 384;
	memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Command, "UPGRADE", 7 );

	ptrChecksum = strstr( (char const*)recvBuf, "checksum");
	if( ptrChecksum != NULL)
	{
		prtUrl = strstr( (char const*)recvBuf, "url");
		if( prtUrl != NULL)
		{
			ptrData = ptrChecksum+9;
			nPtrSize = (prtUrl-ptrChecksum)-10;
			memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum, ptrData, nPtrSize);

			ptrSize = strstr( (char const*)recvBuf, "size");
			if( ptrSize != NULL)
			{
				ptrData = prtUrl+4;
				nPtrSize = (ptrSize-prtUrl)-5;    // ,url=http....,size=
				memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Url, ptrData, nPtrSize);

				ptrData = ptrSize+1;
				sscanf( ptrData, "%d", &ulBinLen);

				Firmware_Upgrade = SPI1_UPGRADE_YES;
				ftimeSpiOut = 0;
				PrepareUpdatesFlash(UPDATE_FROM_NEW); // Should be prior to send response, make ulBinLen 0
				SET_HTTPC_HeartType(UHS_Type_GetFirmware);
			}
			else  // Upgrade CMD No 사이즈 에러
			{
				HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -20;
			}
		}
		else  //  Upgrade CMD No URL 에러
		{
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -10;
		}
	}
	else  // Upgrade CMD No 첵섬 에러
	{
		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
	}
	SET_HTTPC_HeartType(UHS_Type_SendCmdReturn);
}

void Httpc_TreatControlDevice( uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t* ptr;
	uint8_t * ptrId;
 	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;

	 ptrId = strstr( (char const*)recvBuf, "C:");
	 memcpy(HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, ptrId+2, 17);
	 sprintf(HTTP_RestCmd[HttpcSubModeStep].Httpc_Command, "CONTROL DEVICE");
	 ptrDevice = strstr( (char const*)recvBuf, "DEVICE");
    if( ptrDevice != NULL)
    {
		ptrCmd = ptrDevice+7;
		nPtrSize = 8;
		memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum, ptrCmd, nPtrSize);

        EEPROMSetData((INT16U)(EEPROM_ADDRESS_FireFlag), 0);//eeprom 화재신호 지우기..

		if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[0] == '0' &&
			HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[1] == '1' )
		{  /// Control the output
			if(  HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[2] == '0' &&
				 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[3] == '0' )
			{  //FIre Alert  01 00 01 FF
		        EEPROMSetData((INT16U)(EEPROM_ADDRESS_FireFlag), 119);//eeprom 화재신호 만들기..
				FireOccurrence = TRUE; //화재 개방..
				FireLockFlag = 0;
			}
			else  // 01
			{
				if(  HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[6] == 'F' &&
					 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[7] == 'F' )
				{
					if(  HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[4] == '0' &&
						 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[5] == '1' )
					{  // 수동 개방  010101FF
						ChangeLockMode( LockMode_Manual_Unlock, HttpcSubModeStep);
					}
					else  // 02 // 화재 개방 Control the auxiliary output
					{	  // 01 01 02 FF
				        EEPROMSetData((INT16U)(EEPROM_ADDRESS_FireFlag), 119);//eeprom 화재신호 만들기..
						FireOccurrence = TRUE; //화재시..
						FireLockFlag = 0;
					}
				}
				else  // 01 임시 개방
				{
				    uint8_t *endptr;
					uint8_t nOpen = (uint8_t)strtol(&HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[6], (char **)&endptr, 16);
			        ControlDoor( nOpen, HttpcSubModeStep);
				}
			}
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[0] == '0' &&
			HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[1] == '3' )
		{
			NVIC_SystemReset();	// System Reset
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[0] == '0' &&
				 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[1] == '4' )
		{  //  // 스케듈 얼락 다음에 다시 와서 무시(v1.7) 하면 수동 개방 안되고 잠김.
			if( Door1ValidTZ == 1)
				ChangeLockMode( LockMode_Schedule_Lock, HttpcSubModeStep);
			else if(Door1ValidTZ == 2)
				ChangeLockMode( LockMode_Schedule_Unlock, HttpcSubModeStep);
			else
				ChangeLockMode( LockMode_Normal, HttpcSubModeStep);
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[0] == '0' &&
				 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[1] == '6' )
		{  // Control Normal Lock
			if(  HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[5] == '1' )
			{  //수동 잠김  // 06010100
				ChangeLockMode( LockMode_Manual_Lock, HttpcSubModeStep);
			}
			else
			{  //일반 잠김  // 06010000
				ChangeLockMode( LockMode_Normal, HttpcSubModeStep);
			}
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[0] == '1' &&
				 HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[1] == '5' )
		{  /// Set Fire action of the output
			for( uint8_t nLock = 0; nLock < MAX_NUM_RELAY; nLock++)
			{
				FireParameter.FireClose[nLock] = HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[nLock+2] - 0x30;  //0x10
			}
			EEPROMSetDataBlock(E2P_Para_FIRE, &FireParameter.FireClose[0], MAX_NUM_RELAY);
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else  // Upgrade CMD No 사이즈 에러
		{
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -20;
		}
    }
    else  // Upgrade CMD No 첵섬 에러
    {
   	 HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
    }
	 SET_HTTPC_HeartType(UHS_Type_SendCmdReturn);
}


void Httpc_TreatDataDelete( uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t* ptr;
	uint8_t * ptrId;
 	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;

	ptrCmd = strstr( (char const*)recvBuf, "C:");   // C:20250326105130643:DATA DELETE user Cardno=896106578
	memcpy(HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, ptrCmd+2, 17);  // 커맨드 ID
	sprintf(HTTP_RestCmd[HttpcSubModeStep].Httpc_Command, "DATA DELETE");

	if((ptr = strstr( (char const*)recvBuf, "userauthorize")) != NULL)
	{  // 권한 지움  DATA DELETE userauthorize Pin=8888 Cardno=89610578
		if( (ptrId = strstr( (char const*)recvBuf, "Pin=")) != NULL)
		{
			if ((ptrCmd = strstr( (char const*)recvBuf, "CardNo=")) != NULL)
			{
				// 권한 지움  DATA DELETE user Cardno=89610578
				uint8_t tempStr[16];
				nPtrSize = ptrCmd - (ptrId+4) - 1; // 변수 사이에 탬이 하나씩 있슴.
				memcpy( CardParameter.stParam.Pin, ptrId+4, nPtrSize);

				ptrDevice = ptrCmd + 7;
				nPtrSize = recvLen - (uint16_t)(ptrDevice-recvBuf);

				if(nPtrSize <= 8)
					memcpy( tempStr, ptrDevice, nPtrSize);
				else
					memcpy( tempStr, ptrDevice, 8);  // Card number  ??

				CardParameter.stParam.CardId[0] = (A2H(tempStr[0])*0x10) + A2H(tempStr[1]);
				CardParameter.stParam.CardId[1] = (A2H(tempStr[2])*0x10) + A2H(tempStr[3]);
				CardParameter.stParam.CardId[2] = (A2H(tempStr[4])*0x10) + A2H(tempStr[5]);
				CardParameter.stParam.CardId[3] = (A2H(tempStr[6])*0x10) + A2H(tempStr[7]);

				 DeleteAuthorityFromSerialFlash( CardParameter.stParam.Pin, CardParameter.stParam.CardId, HttpcSubModeStep);
			}
		}
 		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
   }
	else if ((ptr = strstr( (char const*)recvBuf, "user")) != NULL)
	{
		if ((ptrDevice = strstr( (char const*)recvBuf, "Cardno=")) != NULL)
		{
			// 권한 지움  DATA DELETE user Cardno=89610578
			uint8_t tempStr[16];
			nPtrSize = recvLen - ((ptrDevice - recvBuf) + 7);
			ptrCmd = ptrDevice + 7;
			if(nPtrSize <= 8)
				memcpy(tempStr, ptrCmd, nPtrSize);   // Card number
			else
				memcpy(tempStr, ptrCmd, 8);   // Card number  ??

			CardParameter.stParam.CardId[0] = (A2H(tempStr[0])*0x10) + A2H(tempStr[1]);
			CardParameter.stParam.CardId[1] = (A2H(tempStr[2])*0x10) + A2H(tempStr[3]);
			CardParameter.stParam.CardId[2] = (A2H(tempStr[4])*0x10) + A2H(tempStr[5]);
			CardParameter.stParam.CardId[3] = (A2H(tempStr[6])*0x10) + A2H(tempStr[7]);

			DeleteRfCardDataOnSerialFlash( CardParameter.stParam.CardId, 1);
		}
		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
	}
    else  // Upgrade CMD No 첵섬 에러
    {
		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
    }
    SET_HTTPC_HeartType(UHS_Type_SendCmdReturn);
}


void Httpc_TreatSetOption( uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t* ptr;
	uint8_t * ptrId;
 	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;
//    		printf("1C %s\r\n", recvBuf);
	ptrId = strstr( (char const*)recvBuf, "C:");
	memcpy(HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, ptrId+2, 17);
	sprintf(HTTP_RestCmd[HttpcSubModeStep].Httpc_Command, "SET OPTIONS");

	if( (ptrDevice = strstr( (char const*)recvBuf, "DateTime")) != NULL)
	{
//				CmdSetOptionDateTime( recvBuf, ptrDevice);
		 uint8_t ReceiveTime[7];
		 nPtrSize = 9;
		 memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum, ptrDevice+9, nPtrSize);

//		 uint32_t nTotalSec = HTTPC_GetSecFromDateTime(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum);

		 uint8_t *endptr;
		 uint32_t  encoded = strtol(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum, (char **)&endptr, 10);
 		 HTTPC_OldDecodeTime( encoded, ReceiveTime);

		 RTC_SetTimeAndDate( &toDayTime, &ReceiveTime[0] );
		 EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime, (INT8U*)ReceiveTime, 7);

		 HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
	}
	else if( (ptrDevice = strstr( (char const*)recvBuf, "Door")) != NULL)
	{
		 nPtrSize = 14;
		 memcpy(HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum, ptrDevice, nPtrSize);

		 if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[13] == '1')  // Door1ValidTZ=1
		 {
			 Door1ValidTZ = 1;
// 			 ChangeLockMode( LockMode_Schedule_Lock, HttpcSubModeStep);
		 }
		 else if( HTTP_RestCmd[HttpcSubModeStep].Httpc_Checksum[13] == '2')
		 {
			 Door1ValidTZ = 2;
//			 ChangeLockMode( LockMode_Schedule_Unlock, HttpcSubModeStep);
		 }
		 else
		 {
			 ChangeLockMode( LockMode_Normal, HttpcSubModeStep);
		 }
		 HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
	}
	else
	{
		  HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
	}
	SET_HTTPC_HeartType(UHS_Type_SendCmdReturn);
}


void Httpc_TreatDataUpdate( uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t * ptrId;
	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;

	ptrId = strstr( (char const*)recvBuf, "C:");   // C:20250326105130643:DATA UPDATE user Cardno=896106578
	memcpy(HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, ptrId+2, 17);  // 커맨드 ID
	sprintf(HTTP_RestCmd[HttpcSubModeStep].Httpc_Command, "DATA UPDATE");

	ptrDevice = strstr( (char const*)recvBuf, "userauthorize");
    if( ptrDevice != NULL)
    {  // 권한 업데이트
		ptrId = strstr( (char const*)recvBuf, "Pin=");
		if( ptrId != NULL)
		{
			ptrCmd = strstr( (char const*)recvBuf, "AuthorizeTimezoneId");
			if( ptrCmd != NULL)
			{
				ptrDevice = ptrId + 4;
				nPtrSize = ptrCmd- ptrDevice-1; // 변수 사이에 탬이 하나씩 있슴.
				if(( nPtrSize > 0) && (nPtrSize < 9))
				{  // 사용자 카드 업데이트
					memcpy(CardParameter.stParam.Pin, ptrDevice, nPtrSize);   // Pin

					ptrCmd = strstr( (char const*)recvBuf, "CardNo=");
					if( ptrCmd != NULL)
					{
						uint8_t tempStr[16];
						ptrId = ptrCmd + 7;
						nPtrSize = 8; // 카드번호 8자리.
						memcpy(tempStr, ptrId, nPtrSize);   // Card number

						CardParameter.stParam.CardId[0] = (A2H(tempStr[0])*0x10) + A2H(tempStr[1]);
						CardParameter.stParam.CardId[1] = (A2H(tempStr[2])*0x10) + A2H(tempStr[3]);
						CardParameter.stParam.CardId[2] = (A2H(tempStr[4])*0x10) + A2H(tempStr[5]);
						CardParameter.stParam.CardId[3] = (A2H(tempStr[6])*0x10) + A2H(tempStr[7]);
#ifdef DEBUG_MODE
						printf( "Result Card%02X%02X%02X%002X,Pin%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
								CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
								CardParameter.stParam.Pin[0], CardParameter.stParam.Pin[1],CardParameter.stParam.Pin[2],CardParameter.stParam.Pin[3],
								CardParameter.stParam.Pin[4],CardParameter.stParam.Pin[5],CardParameter.stParam.Pin[6],CardParameter.stParam.Pin[7]);
#endif
						AddAuthorityToSerialFlash( CardParameter.stParam.Pin, CardParameter.stParam.CardId, HttpcSubModeStep);
					}
				}
			}
		}
 		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
	}
    else if( (ptrDevice = strstr( (char const*)recvBuf, "user CardNo")) != NULL)
    {
		memset( CardParameter.u8strParam, 0, 16);
		uint8_t tempStr[16];
		ptrCmd = ptrDevice+12;
		ptrId = strstr( (char const*)recvBuf, "Pin=");
		if( ptrId != NULL)
		{
			nPtrSize = ptrId - ptrCmd -1;   // 변수 사이에 탬이 하나씩 있슴.
			if(( nPtrSize > 0) && (nPtrSize < 9))
			{  // 사용자 카드 업데이트
				memcpy(tempStr, ptrCmd, nPtrSize);   // Card number

				CardParameter.stParam.CardId[0] = (A2H(tempStr[0])*0x10) + A2H(tempStr[1]);
				CardParameter.stParam.CardId[1] = (A2H(tempStr[2])*0x10) + A2H(tempStr[3]);
				CardParameter.stParam.CardId[2] = (A2H(tempStr[4])*0x10) + A2H(tempStr[5]);
				CardParameter.stParam.CardId[3] = (A2H(tempStr[6])*0x10) + A2H(tempStr[7]);

				ptrCmd = strstr( (char const*)recvBuf, "Password=");
				if( ptrCmd != NULL)
				{
					ptrDevice = ptrId + 4;
					nPtrSize = ptrCmd-ptrDevice-1; // 변수 사이에 탬이 하나씩 있슴.
					memcpy(CardParameter.stParam.Pin, ptrDevice, nPtrSize);   // Pin=

#ifdef DEBUG_MODE
					printf( "Result Card%02X%02X%02X%002X,Pin%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						CardParameter.stParam.CardId[0], CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3],
						CardParameter.stParam.Pin[0],    CardParameter.stParam.Pin[1],    CardParameter.stParam.Pin[2],    CardParameter.stParam.Pin[3],
						CardParameter.stParam.Pin[4],    CardParameter.stParam.Pin[5],    CardParameter.stParam.Pin[6],    CardParameter.stParam.Pin[7]);
#endif
				}

				//TA_INDEX_DAT 7, 총 16바이트 카드데이터 중 카드 4바이트 방식은 0~11 : 0으로 채워서 옴
				CardParameter.stParam.DoorAuth_CardKind = 0xF1;  //  All door 1111 권한  || 1:상시 ID.  2:일시 ID.  3:당일카드 ID.
				CardParameter.stParam.PassCode[0] = 0x99; // 사용기한 자리에 지정카드용 ACU 번호가 두개 들어감
				CardParameter.stParam.PassCode[1] = 0x31; // 사용기한 자리에 지정카드용 ACU 번호가 두개 들어감
				CardParameter.stParam.Location = 2;

				uint8_t nResult = WriteRfCardDataToSerialFlash(CardParameter.u8strParam);
			   /*
			   #define Card_Registerd_OK     0
			   #define Card_No_Granted    	 1

			   #define Card_Deleted_OK   	 0
			   #define Card_No_Space         1
			   #define Card_Already_Exist    2
			   #define Card_Not_Registerd    3
			   #define Card_Error_Registerd  4
			   */
			}
		}
 		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
    }
	else if((ptrDevice = strstr( (char const*)recvBuf, "timezone")) != NULL)
	{
		uint8_t tempStr[8][10];
		uint8_t * ptrDay[8];
		uint16_t nPtrSize[8];

		ptrDay[0] = strstr( (char const*)recvBuf, "SunTime1=");
		if( ptrDay[0] != NULL)
		{
			ptrDay[0] += 9;
			ptrDay[1] = strstr( (char const*)recvBuf, "MonTime1=");
			if( ptrDay[1] != NULL)
			{
				nPtrSize[0] = ptrDay[1] - ptrDay[0] - 1;   // 변수 사이에 탬이 하나씩 있슴.
				memcpy(tempStr[0], ptrDay[0], nPtrSize[0]);   // 일요일 시간
				tempStr[0][nPtrSize[0]] = 0;

				ptrDay[1] += 9;
				ptrDay[2] = strstr( (char const*)recvBuf, "TueTime1=");
				if( ptrDay[2] != NULL)
				{
					nPtrSize[1] = ptrDay[2] - ptrDay[1] - 1;   // 변수 사이에 탬이 하나씩 있슴.
					memcpy(tempStr[1], ptrDay[1], nPtrSize[1]);   // 월요일 시간
					tempStr[1][nPtrSize[1]] = 0;

					ptrDay[2] += 9;
					ptrDay[3] = strstr( (char const*)recvBuf, "WedTime1=");
					if( ptrDay[3] != NULL)
					{
						nPtrSize[2] = ptrDay[3] - ptrDay[2] - 1;   // 변수 사이에 탬이 하나씩 있슴.
						memcpy(tempStr[2], ptrDay[2], nPtrSize[2]);   // 화요일 시간
						tempStr[2][nPtrSize[2]] = 0;

						ptrDay[3] += 9;
						ptrDay[4] = strstr( (char const*)recvBuf, "ThuTime1=");
						if( ptrDay[4] != NULL)
						{
							nPtrSize[3] = ptrDay[4] - ptrDay[3] - 1;   // 변수 사이에 탬이 하나씩 있슴.
							memcpy(tempStr[3], ptrDay[3], nPtrSize[3]);   // 수요일 시간
							tempStr[3][nPtrSize[3]] = 0;

							ptrDay[4] += 9;
							ptrDay[5] = strstr( (char const*)recvBuf, "FriTime1=");
							if( ptrDay[5] != NULL)
							{
								nPtrSize[4] = ptrDay[5] - ptrDay[4] - 1;   // 변수 사이에 탬이 하나씩 있슴.
								memcpy(tempStr[4], ptrDay[4], nPtrSize[4]);   // 목요일 시간
								tempStr[4][nPtrSize[4]] = 0;

								ptrDay[5] += 9;
								ptrDay[6] = strstr( (char const*)recvBuf, "SatTime1=");
								if( ptrDay[6] != NULL)
								{
									nPtrSize[5] = ptrDay[6] - ptrDay[5] - 1;   // 변수 사이에 탬이 하나씩 있슴.
									memcpy(tempStr[5], ptrDay[5], nPtrSize[5]);   // 금요일 시간
									tempStr[5][nPtrSize[5]] = 0;

									ptrDay[6] += 9;
									ptrDay[7] = strstr( (char const*)recvBuf, "Hol1Time1=");
									if( ptrDay[7] != NULL)
									{
										nPtrSize[6] = ptrDay[7] - ptrDay[6] - 1;   // 변수 사이에 탬이 하나씩 있슴.
										memcpy(tempStr[6], ptrDay[6], nPtrSize[6]);   // 토요일 시간
										tempStr[6][nPtrSize[6]] = 0;

										ptrDay[7] += 10;   ///  마지막 스트링은 Hol1Time1= 까지 읽음
										if( ptrDay[7] < (recvBuf + recvLen))
										{
											nPtrSize[7] = (recvBuf + recvLen) - ptrDay[7];   // 최종 길이에서 포인트까지
											memcpy(tempStr[7], ptrDay[7], nPtrSize[7]);   // 휴일 시간
											tempStr[7][nPtrSize[7]] = 0;
										}
										else
										{
											HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
										}
									}
									else
									{
										HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
									}
								}
								else
								{
									HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
								}
							}
							else
							{
								HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
							}
						}
						else
						{
							HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
						}
					}
					else
					{
						HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
					}
				}
				else
				{
					HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
				}
			}
			else
			{
				HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
			}

#ifdef DEBUG_MODE
			printf( "timezone Door = %d\r\n", HttpcSubModeStep);
#endif

			// Time code 2  로 항상 적용 시킨다.
			for(int i=0; i<3; i++)
			{
				CMSParameter.DoorTimeCode[HttpcSubModeStep][i] = 2;    // 1,2,3       MAX_NUM_TIMECODE 128
				EEPROMSetData((INT16U)(E2P_Para_CMS + EEPROM_PARAM_INDEX_DOORTIMECODE  + HttpcSubModeStep * 3 + i), CMSParameter.DoorTimeCode[HttpcSubModeStep][i]);

#ifdef DEBUG_MODE
			printf( "timezone Timecode = %d, Value=%d\r\n", i , CMSParameter.DoorTimeCode[HttpcSubModeStep][i]);
#endif
			}

		    uint8_t startHour, startMin, endHour, endMin;

			for(int ni = 0; ni < MAX_NUM_WEEK_HOLIDAY; ni++)
			{// 일 부터 토 , 휴까지
				parseDurString( tempStr[ni], &startHour,  &startMin,  &endHour, &endMin);
				ScheduleParameter[HttpcSubModeStep][2][ni].StartHh = startHour;//StartHh;
				ScheduleParameter[HttpcSubModeStep][2][ni].StartMm = startMin;//StartMm;
				ScheduleParameter[HttpcSubModeStep][2][ni].EndHh =   endHour;//EndHh;
				ScheduleParameter[HttpcSubModeStep][2][ni].EndMm =   endMin;//EndMm;

#ifdef DEBUG_MODE
			printf( "timezone schedule = start=%d:%d, end=%d:%d\r\n",
					ScheduleParameter[HttpcSubModeStep][2][ni].StartHh, ScheduleParameter[HttpcSubModeStep][2][ni].StartMm,
					ScheduleParameter[HttpcSubModeStep][2][ni].EndHh, 	ScheduleParameter[HttpcSubModeStep][2][ni].EndMm);
#endif
			}
			SaveScheduleData();   // ScheduleParameter Sector에 써 넣음 45

			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = 0;
		}
		else
		{
			HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
		}
	}
    else  // Upgrade CMD No 첵섬 에러
    {
		HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn = -19;
    }
	SET_HTTPC_HeartType(UHS_Type_SendCmdReturn);
}






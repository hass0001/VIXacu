
/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: EEPROM.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
/******************************************************************************
**   Include Files
******************************************************************************/
#define    EEPROM_GLOBALS
#include   "I2C_Eeprom.h"

const INT8U DefaultSystemFlag[8] = {'B','r','u','c','e','I','v','x'}; //INT8U SystemFlag[8];
const INT8U DefaultSerial[8] = {'J','A','2','5','0','1','3','5'}; //INT8U SiteInfo[8];

const INT8U DefaultCmsParameter[LEN_E2ROM_CMS] = {
  192,168, 0, 123,
  255,255,255,  0,
  192,168,  0,  1,//INT8U GatewayAddress[4]; 16
  168,126, 63,  1, //KT dns
  0x70,0xDA,0x9C, 0x02,0x07,0x08, //INT8U MacAddress[6];
  5,5,5, 5,5,5,//DoorOpenTime [MAX_NUM_RELAY];  12
  0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, //DoorTimeCode[MAX_NUM_RELAY][3];
  0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, //LimitCardTimeCode[MAX_NUM_RELAY][3];
  0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0  //SpecialCardTimeCode[MAX_NUM_RELAY][3]; 54
};

const INT8U DefaultSensorParameter[LEN_E2ROM_SENSOR] =
{//Sensor Parameter 0~8
//  3,3,,    //OpenTime
  0,0,0,0,
  0,0,0,0,
  0,0,0,0      // Normal Open
};

const INT8U DefaultButtonParameter[LEN_E2ROM_BUTTON] =
{//Button Parameter 0~8
//  3,3,3,3,3,3,3,3,    //OpenTime
//  0,0,0,0,0,0,0,0,      //Status
  0,0,0,0,0,0,0,0       //ButtonCnt
};

const INT8U DefaultLockParameter[LEN_E2ROM_LOCK] =
{//lock Parameter 0~8
  0,0,0,0,0,0,//DoorTimerSet
  0,0,0,0,0,0,	//DoorTimerCount
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01,//DoorLockMode
  //#define LockMode_Normal           0x01 #define LockMode_Manual_Unlock    0x10
  0x80, 0x40, 0x20, 0x10, 0x08, 0x04 //SelectWieDoor 0, 1, 2, 3, 4, 5, 6,
};

const INT8U DefaultConsoleParameter[LEN_E2ROM_CONSOLE] =
{//Console Parameter 0~8
  5,5,5,5,5,5,5,5,//AccessOpenTime 의미엇음..
  1,1,1,1,0,0,0,0,//Type        등록된 리더기.. 기본은 1번만..
  0,1,2,3,0,1,2,3, //DoorNumber 릴레이를 작동시킬 대상.. 이건 711에서 사용하는 것임.. 의미 부활 시킴..
  1,2,3,4,5,6,7,8//AttendControl        //성능인증에서 임시 사둉.. 향후 재검토 후 적용..
};

const INT8U DefaultWiegandParameter[LEN_E2ROM_WIEGAND] =
{//Wiegand Parameter 0~3
  3,3,3,3,	//CommType    wiegand34
  0,0,0,0,	//AntiPassback
  1,1,1,1,	//OkRelayGroupNum
  1,1,1,1	//ErrRelayGroupNum
};

const INT8U DefaultHttpcParameter[LEN_E2ROM_HTTPC] =
{//httpc Parameter
  192,168,0,100,   // TAIP1[4] 172.16.3.43
  0x1F,0x99,     // TAPort[2] 0x1F,0x99, // 8089  // 0x53,0x4E,  // 21326
  4,0,0,0,0,0,0,0       //ScheduleCode[8]  heart second,
};

const INT8U DefaultFireParameter[LEN_E2ROM_FIRE] =
{//fire setting Parameter
  0,0,0,0,0,0		// default door open
};

void nItem(INT32U nCardData)
{
	uint8_t buff[4];
	xCONVERT_u32Tou8Array(nCardData, buff, sizeof(buff), _MSB);
	xWRITE_DataToEeprom(EEPROM_ADDRESS_NumCardData, buff, 4);
	return;
}


uint32_t ReadNumCardFromEeprom(void)
{
	uint8_t buff[4];
	xREAD_DataFromEeprom(EEPROM_ADDRESS_NumCardData, buff, 4);
	return bCONVERT_u8ArrayTou32(buff);
}


uint8_t EEPROMGetData(uint16_t ReadAddr)
{
	uint8_t buff = 0;

	xREAD_DataFromEeprom(ReadAddr, &buff, 1);
	return buff;
}


uint8_t EEPROMSetData(uint16_t WriteAddr, uint8_t Data)
{
	uint8_t * pData = &Data;

	if(_Ok != xWRITE_DataToEeprom(WriteAddr, pData, 1))
		return FALSE;
	else
		return TRUE;
}

uint8_t EEPROMGetDataBlock(uint16_t ReadAddr, uint8_t* pData, uint16_t Length)
{
	if(_Ok != xREAD_DataFromEeprom(ReadAddr, pData, Length))
		return FALSE;
	else
		return TRUE;
}

#define TEST_RW_EEPROM
uint8_t EEPROMSetDataBlock(uint16_t WriteAddr, uint8_t* pData, uint16_t Length)
{
#ifdef TEST_RW_EEPROM
	uint8_t rData[256];
	uint16_t ReadAddr = WriteAddr;
	uint16_t rLength = Length;
	uint16_t nFor;
#endif

	if(_Ok != xWRITE_DataToEeprom(WriteAddr, pData, Length))
		return FALSE;
	else {
#ifdef TEST_RW_EEPROM
		xREAD_DataFromEeprom(ReadAddr, rData, rLength);
		for( nFor = 0; nFor < rLength; nFor++) {
			if( *(pData+nFor) != *(rData+nFor) )
				printf( " EEPORM Write Error %d\r\n", nFor);
		}
#endif
	}
	return TRUE;
}



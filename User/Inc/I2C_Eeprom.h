/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: EEPROM.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _EEPROM_
#define  _EEPROM_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   EEPROM_GLOBALS
#define  EEPROM_EXT
#else
#define  EEPROM_EXT   extern
#endif


#define  I2C_EEPROM_SLAVE_ADDR   		0xA0

#define  EEPROM_PAGE0           		0
#define  EEPROM_PAGE1   				0x0100  // Parameters   256   //
#define  EEPROM_PAGE2  					0x0200  // For Event Data 1 Parameter
#define  EEPROM_PAGE3   				0x0300  // For Event Data Temp save... 0x0200 ~ 0x11ff 512
#define  EEPROM_PAGE4   				0x0400  // For Event Data 768
#define  EEPROM_PAGE5   				0x0500  // For Event Data 1024
#define  EEPROM_PAGE6   				0x0600  // For Event Data
#define  EEPROM_PAGE7   				0x0700  // For Event Data
#define  EEPROM_PAGE8   				0x0800  // For Event Data
#define  EEPROM_PAGE9   				0x0900  // For Event Data 2k
#define  EEPROM_PAGE10  				0x0A00  // For Event Data
#define  EEPROM_PAGE11  				0x0B00  // For Event Data
#define  EEPROM_PAGE12  				0x0C00  // For Event Data
#define  EEPROM_PAGE13  				0x0D00  // For Event Data 3k
#define  EEPROM_PAGE14  				0x0E00  // For Event Data
#define  EEPROM_PAGE15  				0x0F00  // For Event Data
#define  EEPROM_PAGE16  				0x1000  // For Event Data 4k
#define  EEPROM_PAGE17  				0x1100  // For Event Data Temp save... 0x0200 ~ 0x11ff
#define  EEPROM_PAGE18  				0x1200
#define  EEPROM_PAGE19  				0x1300
#define  EEPROM_PAGE20  				0x1400
#define  EEPROM_PAGE21  				0x1500		
#define  EEPROM_PAGE22  				0x1600
#define  EEPROM_PAGE23  				0x1700
#define  EEPROM_PAGE29					0x1D00
#define  EEPROM_PAGE30					0x1E00
#define  EEPROM_PAGE31					0x1F00
#define  EEPROM_PAGE32					0x2000
#define  EEPROM_PAGE33					0x2100
#define  EEPROM_PAGE34					0x2200
#define  EEPROM_PAGE35					0x2300
#define  EEPROM_PAGE36					0x2400
#define  EEPROM_PAGE44					0x2C00
#define  EEPROM_PAGE48					0x3000
#define  EEPROM_PAGE60					0x3C00	 		
#define  EEPROM_PAGE61          		0x3D00
#define  EEPROM_PAGE62          		0x3E00
#define  EEPROM_PAGE63          		0x3F00		// For Log Event


/******************************************************************************
**    EEPROM MAP  16Kbytes
******************************************************************************/
// PAGE 0
#define EEPROM_ADDRESS_SystemFlag        EEPROM_PAGE0    		//  uint8_t SystemFlag[8];
#define EEPROM_ADDRESS_SiteInfo         (EEPROM_PAGE0 + 8)  	//  uint8_t Serial Number];
#define EEPROM_ADDRESS_AteStart         (EEPROM_PAGE0 + 8) 	//EEPROM_ADDRESS_SiteInfo 와 함께 씀.. 테스트용이니..
#define EEPROM_ADDRESS_AteEnd			(EEPROM_PAGE0 + 16)
#define EEPROM_ADDRESS_NumCardData    	(EEPROM_PAGE0 + 16)  // 4 bytes..

#define EEPROM_ADDRESS_FireFlag         	(EEPROM_PAGE0 + 20) 	// 1 byte..
#define EEPROM_ADDRESS_DoorOpendTooLong 	(EEPROM_PAGE0 + 21)  // 1byte..
//22~23
#define EEPROM_ADDRESS_DateTime			(EEPROM_PAGE0 + 24)//7bytes.. 현재시간 저장.. YYMMDDhhmmssWW
#define EEPROM_ADDRESS_HolidayCount     (EEPROM_PAGE0 + 31)//1byte 0~64
#define EEPROM_ADDRESS_HolidayDate      (EEPROM_PAGE0 + 32) //2bytes * 64 == 128

#define MAX_NUM_HOLIDAY  64 //공휴일저장 최대치..

// 900Mhz 설정 저장
#define EEPROM_ADDRESS_900mhz_port      		(EEPROM_PAGE0 + 160)    //== EEPROM_ADDRESS_HolidayDate(32) + 128
#define EEPROM_ADDRESS_900mhz_buzzer    		(EEPROM_PAGE0 + 161)
#define EEPROM_ADDRESS_900mhz_portpower 		(EEPROM_PAGE0 + 162)
#define EEPROM_ADDRESS_CountPin_HIGH         	(EEPROM_PAGE0 + 163)
#define EEPROM_ADDRESS_CountPin_LOW        		(EEPROM_PAGE0 + 164)
#define EEPROM_ADDRESS_reserve3         		(EEPROM_PAGE0 + 165)
#define EEPROM_ADDRESS_reserve4         		(EEPROM_PAGE0 + 166)
#define EEPROM_ADDRESS_reserve5         		(EEPROM_PAGE0 + 167)

#define EEPROM_ADDRESS_ConsoleInfoSave  		(EEPROM_PAGE0 + 168)//MAX_NUM_SYSTEM_PORT(8) * 6bytes == 48bytes //콘솔을 8개만 할 때..
#define EEPROM_ADDRESS_ConsoleInfoSave_End      (EEPROM_ADDRESS_ConsoleInfoSave + 48)//216
//PowerOn 시간을 저장하는건데 우리는 필요없을지 모름..
//216~255

#define  LEN_E2ROM_CMS                        	82		//   (EEPROM_PARAM_INDEX_SPECIALCARDTIMECODE + MAX_NUM_RELAY * 3) //(82)
#define  LEN_E2ROM_SENSOR       				(MAX_NUM_SENSOR*3) // 4*3   12
#define  LEN_E2ROM_BUTTON       				(MAX_NUM_BUTTON*2) // 4*2    8
#define  LEN_E2ROM_LOCK         				(MAX_NUM_RELAY*4)    // 6*4 24 이지만 4*4 로 하고 Relay 재설정 필요
#define  LEN_E2ROM_CONSOLE      				(MAX_NUM_485CS*4)  // 8*4   32
#define  LEN_E2ROM_WIEGAND      				(MAX_NUM_WIEGAND*4)  // 4*4 16
#define  LEN_E2ROM_HTTPC                        14      //  188
#define  LEN_E2ROM_FIRE                         MAX_NUM_RELAY      //  188

#define  EEPROM_OFFSET_DoorLockMode   			(MAX_NUM_RELAY*2)  // 6*2  12

// PAGE 1 Parameter
#define E2P_Para_CMS            				(EEPROM_PAGE1)
#define E2P_Para_SENSOR         				(E2P_Para_CMS + LEN_E2ROM_CMS) 			//82
#define E2P_Para_LOCK           				(E2P_Para_SENSOR + LEN_E2ROM_SENSOR) 	// 82 + 16
#define E2P_Para_BUTTON         				(E2P_Para_LOCK + LEN_E2ROM_LOCK) 		//98 + 30
#define E2P_Para_CONSOLE        				(E2P_Para_BUTTON + LEN_E2ROM_BUTTON) 	//148+ 16
#define E2P_Para_WIEGAND        				(E2P_Para_CONSOLE + LEN_E2ROM_CONSOLE) 	//164 + 32
#define E2P_Para_HTTPC        					(E2P_Para_WIEGAND + LEN_E2ROM_WIEGAND) 	//212 + 16
#define E2P_Para_FIRE        					(E2P_Para_HTTPC + LEN_E2ROM_HTTPC) 	//212 + 16
#define EEPROM_ADDRESS_FlashSectorCount         (E2P_Para_FIRE + LEN_E2ROM_FIRE) 	//228 + 14 = 242 < 0x100
#define EEPROM_ADDRESS_EEPromPageCount          (EEPROM_ADDRESS_FlashSectorCount + 1)		//243
#define EEPROM_ADDRESS_EventCount              	(EEPROM_ADDRESS_FlashSectorCount + 2)		//244
#define EEPROM_ADDRESS_InstallSiteNumber       	(EEPROM_ADDRESS_FlashSectorCount + 3)		//245

// PAGE 2 ~ 17 Parameter
#define EEPROM_ADDRESS_EventStart          		EEPROM_PAGE2
#define EEPROM_ADDRESS_EventEnd		        	EEPROM_PAGE18  // 16 PAGES == 0x1000 == sFlash 1 Sector

#define  E2P_Para_SDP       					EEPROM_ADDRESS_EventEnd
#define  LEN_E2ROM_SDP      					47
#define  E2P_Para_GWP       					(EEPROM_PAGE + LEN_E2ROM_SDP)
#define  LEN_E2ROM_GWP      					88



// PAGE 18 ~ 63 Parameter 46page 남음.. 향후 지정카드 저장과 antipassback에 활용..
// 지정카드는 특정카드번호만 지정된 시간에 출입문 출입자격이 있는것.. 특별카드와 다른점은 특정문에만 적용된다는 점..
// antipassback 은 들어간 카드를 여기다 저장해서 같은카드로 다시 들어가거나 안들어간 카드가 나갈 때 경고 처리함..
//카드정보에 antipassback index번호를 넣어서 비트플래그로 체크함..

/******************************************************************************
**    EEPROM Type Define CMS_Parameter
******************************************************************************/
#define  EEPROM_PARAM_INDEX_IP         						(0)
#define  EEPROM_PARAM_INDEX_SNMASK      					(4)
#define  EEPROM_PARAM_INDEX_GW         						(8)
#define  EEPROM_PARAM_INDEX_DNS        						(12)
#define  EEPROM_PARAM_INDEX_MAC        						(16)

#define  EEPROM_PARAM_INDEX_DOOROPENTIME        			(EEPROM_PARAM_INDEX_MAC + MAX_NUM_RELAY)   //(22)
#define  EEPROM_PARAM_INDEX_DOORTIMECODE        			(EEPROM_PARAM_INDEX_DOOROPENTIME        + MAX_NUM_RELAY) //(28)
#define  EEPROM_PARAM_INDEX_LIMITCARDTIMECODE   			(EEPROM_PARAM_INDEX_DOORTIMECODE        + MAX_NUM_RELAY * 3) //(46)
#define  EEPROM_PARAM_INDEX_SPECIALCARDTIMECODE 			(EEPROM_PARAM_INDEX_LIMITCARDTIMECODE   + MAX_NUM_RELAY * 3) //(64)

/******************************************************************************
**    System Parameter Type Define
******************************************************************************/
typedef struct  {//CMS_Parameter
	uint8_t IPAddress[4];         // 0   인덱스 상 0 부터 처리 해야 됨
  	uint8_t SubnetMask[4];        // 4
  	uint8_t GatewayAddress[4];    // 8
  	uint8_t DNSAddress[4];        // 12
  	uint8_t MacAddress[6];        // 16, 22 bytes from IPAddress
  	uint8_t DoorOpenTime [MAX_NUM_RELAY]; // 22     6 //default == 5초   //MAX_NUM_SYSTEM_PORT 6
  	uint8_t DoorTimeCode[MAX_NUM_RELAY][3]; // 28  6*3
  	uint8_t LimitCardTimeCode[MAX_NUM_RELAY][3]; // 46   6*3
  	uint8_t SpecialCardTimeCode[MAX_NUM_RELAY][3]; // 64  6*3
} __attribute__((packed)) CMS_Parameter;            // 82       bytes..

/******************************************************************************/
typedef struct  {//CMS_Parameter
	INT8U TAIP1[4];       // 0
	INT8U TAPort[2];      // 4
	INT8U ScheduleCode[8];       // 6
} __attribute__((packed)) Http_Parameter;            // 14       116 bytes..

typedef struct  {//Sensor_Parameter
//  	uint8_t OpenTime[MAX_NUM_SYSTEM_PORT];
	uint8_t Status[MAX_NUM_SENSOR];
	uint8_t SensCnt[MAX_NUM_SENSOR];
	uint8_t NoNc[MAX_NUM_SENSOR];
}__attribute__((packed)) Sensor_Parameter;


typedef struct  {//Button_Parameter
//  uint8_t OpenTime[MAX_NUM_SYSTEM_PORT];
	uint8_t Status[MAX_NUM_BUTTON];
	uint8_t ButtonCnt[MAX_NUM_BUTTON];
} __attribute__((packed)) Button_Parameter;


typedef struct  {//Lock_Parameter
	uint8_t DoorTimerSet[MAX_NUM_RELAY];//락이 풀려있는 시간..
  	uint8_t DoorTimerCount[MAX_NUM_RELAY];//위의 시간까지 누적 후 락을 다시 잠금..
  	uint8_t DoorLockMode[MAX_NUM_RELAY];
  	uint8_t SelectDoor[MAX_NUM_RELAY];
} __attribute__((packed)) Lock_Parameter;


typedef struct  {//Console_Parameter
	uint8_t AccessOpenTime[MAX_NUM_485CS];
	uint8_t Type[MAX_NUM_485CS];//존재여부.. ack를 보내야 하는가??
  	uint8_t DoorNumber[MAX_NUM_485CS];
  	uint8_t AttendControl[MAX_NUM_485CS];
}Console_Parameter;


typedef struct  {//Wiegand_Parameter
	uint8_t CommType[MAX_NUM_WIEGAND];
	uint8_t AntiPassback[MAX_NUM_WIEGAND];
	uint8_t OkRelayGroupNum[MAX_NUM_WIEGAND];
	uint8_t ErrRelayGroupNum[MAX_NUM_WIEGAND];
} __attribute__((packed)) Wiegand_Parameter;



typedef struct  {//Fire_Setting_Parameter
	uint8_t FireClose[MAX_NUM_RELAY];
} __attribute__((packed)) FireSet_Parameter;


/******************************************************************************
**    Local Values
******************************************************************************/
EEPROM_EXT CMS_Parameter        		CMSParameter;           	// 80
EEPROM_EXT Http_Parameter     			HttpcParameter;       		// 14
EEPROM_EXT Sensor_Parameter     		SensorParameter;       		// 12
EEPROM_EXT Lock_Parameter       		LockParameter;        		// 24
EEPROM_EXT Button_Parameter     		ButtonParameter;        	// 12
EEPROM_EXT Console_Parameter    		ConsoleParameter;        	// 16
EEPROM_EXT Wiegand_Parameter    		WiegandParameter;        	// 16
EEPROM_EXT FireSet_Parameter 			FireParameter;        		// 32
//EEPROM_EXT Gwp_Parameter 				GwpParameter;        		// 32

/******************************************************************************
**    Global Values
******************************************************************************/
//EEPROM_EXT OS_RSEMA EEPROMSem;
EEPROM_EXT const uint8_t DefaultSystemFlag[8];// = {'B','r','u','c','e','I','v','X'}; //INT8U SystemFlag[8];
EEPROM_EXT const uint8_t DefaultSerial[8];// =  //INT8U SiteInfo[12];

EEPROM_EXT const uint8_t DefaultCmsParameter[LEN_E2ROM_CMS];
EEPROM_EXT const uint8_t DefaultHttpcParameter[LEN_E2ROM_HTTPC];
EEPROM_EXT const uint8_t DefaultSensorParameter[LEN_E2ROM_SENSOR];
EEPROM_EXT const uint8_t DefaultLockParameter[LEN_E2ROM_LOCK];
EEPROM_EXT const uint8_t DefaultButtonParameter[LEN_E2ROM_BUTTON];
EEPROM_EXT const uint8_t DefaultConsoleParameter[LEN_E2ROM_CONSOLE];
EEPROM_EXT const uint8_t DefaultWiegandParameter[LEN_E2ROM_WIEGAND];
EEPROM_EXT const uint8_t DefaultFireParameter[LEN_E2ROM_FIRE];
//EEPROM_EXT const uint8_t DefaultGwpParameter[LEN_E2ROM_GWP];


void WriteNumCardToEeprom(INT32U nCardData);
uint32_t ReadNumCardFromEeprom(void);
uint8_t EEPROMGetData(uint16_t ReadAddr);
uint8_t EEPROMSetData(uint16_t WriteAddr, uint8_t Data);
uint8_t EEPROMGetDataBlock(uint16_t ReadAddr, uint8_t* pData, uint16_t Length);
uint8_t EEPROMSetDataBlock(uint16_t WriteAddr, uint8_t* pData, uint16_t Length);


#endif

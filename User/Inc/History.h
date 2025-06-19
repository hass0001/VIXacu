/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: History.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  __HISTORY_H_
#define  __HISTORY_H_


/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define 
******************************************************************************/
#ifdef   HIS_GLOBALS
#define  HIS_EXT
#else
#define  HIS_EXT   extern
#endif

/******************************************************************************
**    Type Define 
******************************************************************************/
#define LOG_CIRCULAR_MAX_SIZE	(5)
#define LOG_EVENT_OCCUR_SIZE	(22)
#define LOG_EVENT_RESULT_SIZE	(7)	
#define LOG_EVENT_MAX_SIZE		(LOG_EVENT_OCCUR_SIZE+LOG_EVENT_RESULT_SIZE)
#define LOG_EVENT_MAX_CNT		(2250)	//2250 * 29 = 65250
#define LOG_ACCESS_MAX_SIZE	(17)
#define LOG_ACCESS_MAX_CNT	(2250)	//2250 * 17 = 38250

#define LOG_TO_PC_POLL_SIZE	(250)
#define Indicator_Save   0xF0
#define Indicator_Send   0x00

#define CARD_ARMOK 		0
#define CARD_ARMFAIL	1
#define CARD_OK			2
#define CARD_ClosedDoor	3
#define CARD_LEVELFAIL	4
#define CARD_FAIL		5


#define EVENT_SEND_OK		0
#define EVENT_SEND_FAIL	1

#define	Event_Alarm     		0
#define	Event_Access    		1
#define	Event_Alarm_Rst	2

#define EventOccur      1
#define EventRestore    3

#define CodeInstant    		 0
#define CodeInterior   		 1
#define CodeFire       		 2
#define CodePanic   		 3
#define CodeWater    		 4
#define CodeGugb     		 5
#define CodeGas         	 6
#define CodeEASAlarm    	 7
#define CodeEASFire     	 8
#define CodeEASAcloss   	 9
#define CodeDoor        	 10
#define CodeExit        	 11
#define CodeFireTrouble   	 12
#define CodeCD365   		 13
#define CodePatrol   		 14
#define CodeProgramTamper	 15
#define CodePanelReset 		 16
#define CodeBypass  		 17
#define CodeBypass24 		 18
#define CodeBypassEAS 		 19
#define CodeDisArm  		 20
#define CodeArm		 		 21
#define CodeStayArm 		 22
#define CodeAutoArm 		 23
#define CodeACLoss  		 24
#define CodeBattery 		 25
#define CodeLineTest 		 26
#define CodeTamper  		 27				 
#define CodeConsoleTamper    28
#define CodeConsoleTrouble   29
#define CodeSchArmFail 		 30
#define CodeKeyPAdPanic      31
#define CodeTALineTest		 32
#define CodeTABackLineTest	 33
#define CodePSTNLineCut		 34
#define CodeTimeLimitArm  	 35
#define CodeNoneBattery		 36
#define CodeIPMTF			 37
#define CodeProgramTamper_DLS	38
#define CodeSensorTrouble		 39

#define CMS_EVENT_BUF_SIZE (64)

typedef enum
{
	LOG_FIRST_BLOCK=0,
	LOG_SECOND_BLOCK
}E_LOG_BLOCK_ADDR;

typedef enum
{
	ACCESS_SUCCESS = 0,
	ACCESS_AUTHORITY_FAIL,
	ACCESS_CARD_NONREG
}E_ACCESS_RESULT;


typedef struct
{
	INT8U year;			// �̺�Ʈ �߻� �⵵
	INT8U month;		// �̺�Ʈ �߻� ��
	INT8U day;			// �̺�Ʈ �߻� ��
	INT8U hour;			// �̺�Ʈ �߻� ��
	INT8U minute;		// �̺�Ʈ �߻� ��	
	INT8U second;		// �̺�Ʈ �߻� ��
	INT8U contactID[16];	// Contact ID Data
} __attribute__((packed)) S_EVENT_OCCUR, *P_S_EVENT_OCCUR;

typedef struct
{
	INT8U	blockFlag;		//for block change
	INT8U 	eepAddr;		//eeprom
	INT16U	flashAddr;		//Flash
	S_EVENT_OCCUR s_occur;
} __attribute__((packed)) S_LOG_EVENT_OCCUR, *P_S_LOG_EVENT_OCCUR;

typedef struct
{
	INT8U day;			// �̺�Ʈ ��ȣ ���� ��� ��
	INT8U hour;			// �̺�Ʈ ��ȣ ���� ��� ��
	INT8U minute;		// �̺�Ʈ ��ȣ ���� ��� ��
	INT8U second;		// �̺�Ʈ ��ȣ ���� ��� ��
	INT8U result;		// �̺�Ʈ ��ȣ ���� ��� ��
	INT8U retryCount;	// �̺�Ʈ ��ȣ ���� �۽� Ƚ��
	INT8U tryLine;			// �̺�Ʈ ��ȣ ���� ȸ�� ����
}S_EVENT_RST, *P_S_EVENT_RST;

typedef struct
{
	INT8U	blockFlag;		//for block change
	INT8U 	eepAddr;		//eeprom
	INT16U	flashAddr;		//Flash
	S_EVENT_RST s_rst;
} __attribute__((packed)) S_LOG_EVENT_RST, *P_S_LOG_EVENT_RST;

typedef struct
{
	INT8U year;			// ���� �߻� �⵵
	INT8U month;		// ���� �߻� ��
	INT8U day;			// ���� �߻� ��
	INT8U hour;		// ���� �߻� ��
	INT8U minute;		// ���� �߻� ��
	INT8U second;		// ���� �߻� ��
	INT8U cardSerial[8];	// ���� �߻� ī��ø��� �ѹ�
	INT8U number[2];	// ���� �߻� ���� �ѹ� 
	INT8U result;		// ���� ���� ��� �� 
} __attribute__((packed)) S_ACCESS, *P_S_ACCESS;

typedef struct
{
	INT8U	blockFlag;		//for block change
	INT8U 	eepAddr;		//eeprom
	INT16U	flashAddr;		//Flash
	S_ACCESS s_access;
} __attribute__((packed)) S_LOG_ACCESS, *P_S_LOG_ACCESS;

/******************************************************************************
**    Card Check Info
******************************************************************************/
HIS_EXT INT8U *SavePtr;
HIS_EXT INT8U CmsRamBuf[16],*CmsRamPtr;
HIS_EXT INT32U HistoryIndex_Save, HistoryIndex_Send; 
HIS_EXT INT32U HistoryIndex_Upload;
//HIS_EXT BOOLEAN EventSendFlag;
HIS_EXT BOOLEAN EventLogSendFlag;
HIS_EXT INT32U EventLogUpIndex;

HIS_EXT INT8U CMSSendBuf[CMS_EVENT_BUF_SIZE][16];
HIS_EXT INT8U CMSSend_IN;
HIS_EXT INT8U CMSSend_OUT;

/******************************************************************************
**    Global Value 
******************************************************************************/
HIS_EXT S_LOG_EVENT_OCCUR g_s_logEventOccur;
HIS_EXT S_LOG_EVENT_RST g_s_logEventRst;
HIS_EXT S_LOG_ACCESS g_s_logAccess;

/******************************************************************************
**    Function Define 
******************************************************************************/
//HIS_EXT void SaveHistory(INT8U, INT8U, INT8U, INT8U, INT8U, INT8U, INT8U);
HIS_EXT void HistoryInitialAction(void);

HIS_EXT void PatrolEventSave(INT8U Partition, INT8U Event);
HIS_EXT void ProgramTamperEventSave(INT8U Partition);
HIS_EXT void ProgramTamperEventSave_DLS(INT8U Partition);
HIS_EXT void PanelResetEventSave(INT8U Partition, INT8U Event);
HIS_EXT void PartBypassEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartBypass24EventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartBypassEASEventSave(INT8U partition,INT8U Event,INT8U SensorNum);

HIS_EXT void PartExitEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartInstantEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartInteriorEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartFireEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartFireTroubleEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartPanicEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartWaterEventSave(INT8U partition, INT8U Event,INT8U SensorNum);
HIS_EXT void PartGugbEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartGasEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartEASAlarmEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartEASFireEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartEASAclossEventSave(INT8U partition,INT8U Event,INT8U SensorNum);
HIS_EXT void PartCD365EventSave(INT8U partition,INT8U Event,INT8U SensorNum);

HIS_EXT void PartDisArmEventSave(INT8U partition,INT8U Event,INT8U user_Hi, INT8U user_Low, INT8U originator);
HIS_EXT void PartArmEventSave(INT8U partition,INT8U Event,INT8U user_Hi, INT8U user_Low, INT8U originator);
HIS_EXT void PartStayArmEventSave(INT8U partition,INT8U Event,INT8U user_Hi, INT8U user_Low, INT8U originator);
HIS_EXT void PartAutoArmEventSave(INT8U partition,INT8U Event,INT8U user_Hi, INT8U user_Low);
HIS_EXT void AcLossEventSave(INT8U Partition, INT8U Event);
HIS_EXT void Battery1EventSave(INT8U Partition, INT8U Event);
HIS_EXT void SensorBatteryEventSave(INT8U Partition, INT8U Event);
HIS_EXT void LineTestEventSave(INT8U Partition);
HIS_EXT void LineTestTAEventSave(INT8U Partition);
HIS_EXT void LineTestTABackEventSave(INT8U Partition);
HIS_EXT void TamperEventSave(INT8U Partition, INT8U Event);
HIS_EXT void PartConsoleTamperEventSave(INT8U partition,INT8U Event,INT8U Num);
HIS_EXT void PartConsoleTroubleEventSave(INT8U partition,INT8U Event,INT8U Num);
HIS_EXT void PartSchArmFailEventSave(INT8U partition,INT8U Event);
HIS_EXT void KeyPadPanic( INT8U Part);
HIS_EXT void PartTimeLimitArmEventSave(INT8U partition);
HIS_EXT void PartIPMSendFailEventSave(INT8U partition, INT8U event);
HIS_EXT void PartWSensorTroubleEventSave(INT8U partition,INT8U Event,INT8U Num);
HIS_EXT void CmsPartHeader(INT8U partition);
HIS_EXT void CmsSave(INT8U Data);									  
HIS_EXT void CheckSum685(void);
HIS_EXT void Hex2Ascii2(INT16U value);
HIS_EXT void SaveHistory(INT8U Type, INT8U Data1, INT8U Data2, INT8U Data3, INT8U Data4, INT8U Data5, INT8U Data6);
HIS_EXT void LOG_Initialization(void);
HIS_EXT INT16U LOG_GetOffsetAddr(INT8U eType, INT8U* tempData);
HIS_EXT void LOG_SetAddr(INT8U eType);
HIS_EXT void PartPSTNLineCut(INT8U partition, INT8U event);
HIS_EXT INT8U getYear(void);
HIS_EXT INT8U getDate(void);
HIS_EXT INT8U getMonth(void);
HIS_EXT INT8U getHour(void);
HIS_EXT INT8U getMinute(void);
HIS_EXT INT8U getSeconds(void);
HIS_EXT void LoadLogData(INT8U type, INT16U index, INT8U* data);
HIS_EXT INT16U GetLogPollCount(INT8U type);
HIS_EXT void SaveEventLog2EEprom_sFlash(void);
HIS_EXT void RestoreEventLogFromEEpromORsFlash(void);
HIS_EXT INT8U CheckEventCardAndLog( uint8_t nPort, uint8_t * u8CardData, uint16_t u16CardResult);
HIS_EXT bool CheckInSchedule(uint8_t u8StartHh, uint8_t uStartMm8, uint8_t u8EndHh, uint8_t u8EndMm);

HIS_EXT void CopyYYMMDDhhmmss(uint8_t *u8ArrTarget);
#endif

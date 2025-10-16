/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: InTask.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _INTASK_
#define  _INTASK_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   IN_GLOBALS
#define  INP_EXT
#else
#define  INP_EXT   extern
#endif
/******************************************************************************
**    Type Define		   		      
******************************************************************************/
// #define TIMEMIN_ONEDAY (24*60)
#define ACCESS_NORMAL 0
#define ACCESS_PATROL 1
#define ACCESS_SENSOR 2
#define ACCESS_BUTTON 3
#define ACCESS_FIRE 4

//Door LockMode Define for Lock Operation..
#define LockMode_NotUse		  0x00
#define LockMode_Normal           0x01
#define LockMode_Manual_Unlock    0x10
#define LockMode_Manual_Lock      0x11
#define LockMode_Schedule_Unlock  0x20
#define LockMode_Schedule_Lock    0x21
#define LockMode_Unknown          0xFF

#define CNT_MENU_BOUNCE	    7
#define CNT_BTN_BOUNCE	    6

#define MAX_MAIN_MENU	    3
#define MAX_SONE_MENU	    4
#define MAX_STWO_MENU	    2
#define MAX_STHR_MENU	    12

#define ALARM_BuzzerRelayOn()  HAL_GPIO_WritePin( ALARM_C6_GPIO_Port, ALARM_C6_Pin, GPIO_PIN_SET);// Relay5 On
#define ALARM_BuzzerRelayOff() HAL_GPIO_WritePin( ALARM_C6_GPIO_Port, ALARM_C6_Pin, GPIO_PIN_RESET);// Relay5 Off

#define ALARM_BellRelayOn()  HAL_GPIO_WritePin( ALARM_G8_GPIO_Port, ALARM_G8_Pin, GPIO_PIN_SET);// Relay6 On
#define ALARM_BellRelayOff() HAL_GPIO_WritePin( ALARM_G8_GPIO_Port, ALARM_G8_Pin, GPIO_PIN_RESET);// Relay6 Off

/******************************************************************************
**    Global Values						    
******************************************************************************/

INP_EXT INT8U gU8InstallSiteNumber;

INP_EXT INT8U DoorLockMode;
INP_EXT INT8U DoorTimerFlag;
INP_EXT INT16U DoorTimerCnt;
INP_EXT INT16U DoorTimerSet;

INP_EXT INT8U FireOccurrence;
INP_EXT INT8U FireLockFlag; //화재발생작동모드이면 119 아니면 0, 비작동모드이면 99
INP_EXT INT8U FireAlertCnt;
INP_EXT INT8U isFireAlertOccur;

INP_EXT INT8U TamperOccurrence;
INP_EXT INT8U TamperLockFlag;
INP_EXT INT8U TamperAlertCnt;
INP_EXT INT8U isTamperAlertOccur;

INP_EXT INT8U gU8ACUnum;
INP_EXT INT8U gU8SensorValue;

INP_EXT INT8U DateTimeSaveFlag;
INP_EXT INT8U DisplayFlag;

INP_EXT INT8U gU8HolidayFlag;

INP_EXT INT8U ConsoleTroubleTimer_1Min[MAX_NUM_485CS];
INP_EXT INT8U ConsoleTrouble[MAX_NUM_485CS];
INP_EXT INT8U ConsoleTamper[MAX_NUM_485CS];

INP_EXT INT8U gU8ArrDoorOpenedTooLongCount[MAX_NUM_RELAY];// = {0,0,0,0,0,0,0,0};
INP_EXT INT8U gU8DoorOpenedTooLong;//��� �������� �ϴ� ����..

INP_EXT INT16U gU16IntaskAliveCount;
INP_EXT INT16U gU16nDelayCnt_Count;
/******************************************************************************
**    Prototype        	   		    
******************************************************************************/
INP_EXT void InTask(void);
INP_EXT void Timer2_Init(void);
INP_EXT void InputInit(void);
INP_EXT void InProcess(void);
INP_EXT void CheckSensor(void);
INP_EXT void SensorScan(INT8U nSensor);
INP_EXT void CheckTamper(void);

INP_EXT void ReleaseLockSelectDoor(INT8U OpenTime, INT8U nLock, INT8U nAuth);
INP_EXT void ControlDoor(INT8U OpenTime, INT8U nLock);
INP_EXT void ChangeLockMode(INT8U LockMode, INT8U NumLock);

INP_EXT void DoorTimerProcess(void);
INP_EXT void ExecuteLockOnOff( INT8U nLock);

INP_EXT void SensorProcess(INT8U SensorIndex);

INP_EXT void CheckButton(void);
INP_EXT void ButtonScan(INT8U nSw);

INP_EXT void CheckFireAlert(void);

INP_EXT void CheckLockStatus(void);
INP_EXT void TestLockOut(void);

INP_EXT void SetLockOnOff( INT8U NoRelay, bool bON);
//INP_EXT void SetLockOff( INT8U NoRelay);

INP_EXT void PrepareMenuParams(void);
INP_EXT void CompareAndSetMenuParams(void);

INP_EXT void CheckMenu(void);
INP_EXT void CheckPlus(void);
INP_EXT void SetPlus2ThrSub(void);
INP_EXT void SetNetIpOnly(void);

INP_EXT void CheckMinus(void);
INP_EXT void SetMinus2ThrSub(void);
INP_EXT void CheckSelect(void);
INP_EXT void SaveDateTime(void);

INP_EXT void CheckMenuItem(void);
INP_EXT void DisplayMenu(void);
INP_EXT void DisplaySchedule(void);
INP_EXT void DisplayIpSubMenu(void);
INP_EXT void DisplayBeepSubMenu(void);
INP_EXT void DisplayRfSubMenu(void);

INP_EXT void CountLockTimerEach1Sec(void);

INP_EXT void CheckDoorOpenedTooLongEach1Sec(void);

INP_EXT void ScheduleInitial(void);
INP_EXT void GetScheduleData(INT8U nLock, INT8U nCode);
INP_EXT void LoadScheduleData(void);
INP_EXT void SaveScheduleData(void);
INP_EXT void ResetDoorStatus(void);

INP_EXT void SetMenuParameter(uint8_t nSelMain, uint8_t nSelSub);

#endif

		   

/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: StartTask.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#define   STA_GLOBALS

#include  "StartTask.h"

extern uint16_t gnCountPin;

void SystemTask(void)
{
	INT8U versionStr[120];
	uint16_t eepCnt = 0;
	//ㅇ듀ㅕㅎ
	//	Ethernet_END();
	_STATE_LED_ON();
	_RUN_LED_ON();
	_RS485_LED_ON();
//	_RESET_LED_ON();
	_CAN_LED_ON();

//	EEPROM_Init();
	ResetStatus();

//  FAC_Test_Eeprom(&eepCnt);
//  EEPROMSetData(EEPROM_ADDRESS_SystemFlag, 't');//"BruceIvx"로 써넣기.. CheckSystemInitial() 에서 원래대로 돌려 놓는다.. 초기화 할 때 임시로 사용..
////////////////
//  readExtFlashRDID(versionStr);
//  rprintf( "FlashRDID : %s\r\n", versionStr);
   ////////////////
  CheckSystemInitial();
  ReloadParameter();	
  InitRTC();

//  Desmain();
  //	LOG_Initialization();
  //	FireStatusReload();
//  ResetDoorStatus();    //도대체 이걸 왜하는데????????
//  ResetConsoleStatus();    //도대체 이걸 왜하는데????????
  {
    VS_GetVersionStr((char*)versionStr);
    rprintf( "========================================================\r\n");
    rprintf( "\t[VIXacu] Start-Up INTELLIVIX Access Control System\r\n");
    rprintf( "========================================================\r\n");
    rprintf( "\tRelease Version:\t%s\r\n", versionStr);
    rprintf( "\tRelease Date:\t%s, %s\r\n", __DATE__, fwCompileTime);
    rprintf( "========================================================\r\n");
  }
	_STATE_LED_OFF();
	_RUN_LED_OFF();
	_RS485_LED_OFF();
//	_RESET_LED_OFF();
	_CAN_LED_OFF();

   EEPROMGetDataBlock(E2P_Para_CMS ,       (INT8U*)&versionStr[0],    LEN_E2ROM_CMS);
   rprintf("E2P_Para_CMS:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_CMS; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_SENSOR ,    (INT8U*)&versionStr[0], LEN_E2ROM_SENSOR);
   rprintf("\r\nE2P_Para_SENSOR:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_SENSOR; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_LOCK ,      (INT8U*)&versionStr[0],   LEN_E2ROM_LOCK);
   rprintf("\r\nE2P_Para_LOCK:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_LOCK; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_BUTTON ,    (INT8U*)&versionStr[0], LEN_E2ROM_BUTTON);
   rprintf("\r\nE2P_Para_BUTTON:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_BUTTON; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_CONSOLE ,   (INT8U*)&versionStr[0],LEN_E2ROM_CONSOLE);
   rprintf("\r\nE2P_Para_CONSOLE:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_CONSOLE; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_WIEGAND ,   (INT8U*)&versionStr[0],LEN_E2ROM_WIEGAND);
   rprintf("\r\nE2P_Para_WIEGAND:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_WIEGAND; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_HTTPC ,   (INT8U*)&versionStr[0],LEN_E2ROM_HTTPC);
   rprintf("\r\nE2P_Para_HTTPC:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_HTTPC; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   EEPROMGetDataBlock(E2P_Para_FIRE ,       (INT8U*)&versionStr[0],	   LEN_E2ROM_FIRE);
   rprintf("\r\E2P_Para_FIRE:");
   for( eepCnt = 0; eepCnt < LEN_E2ROM_FIRE; eepCnt++)
   {
	   rprintf("%02X ", versionStr[eepCnt]);
   }
   rprintf("\r\nStart System Task!\r\n");


// You need to create at least one task before calling OS_Start().
  // Task 우선순위 넘버가 큰것이 우선순위가 높음
  //Usart1 (TCBSCI1FwdTask)를 cp710, cp711 에서는 스마트폰 케이블 연결로 이용, ta4000, aru8은 J1 Debug 포트로 이용함..
  //Usart2 (SCI2Rs485Task)를 모두 RS485로 이용함..
  //Usart3 (TCBFacTask)를 cp710, cp711 에서는 PSTN 으로 이용, ta4000, aru8은 J2 ATE 테스트 포트로 이용함..
  //Usart4 (TCBSCI4DebugCdcTask) 를 cp710에서는 debug, cp711에서는 cdma.. ta4000 에서는 LCD로 사용함.. aru8은 연결안됨..
  //Usart5 cp710에서는 TP3,4로, ta4000 에서는 Wiegand out 으로 사용함.. aru8은 연결안됨..   
//  OS_CREATETASK(&TCBWiegandTask,      "WiegandTask",          WiegandTask,     99,     WIEGAND_TASK_STK);
//  OS_CREATETASK(&TCBFacTask, 	      "FacTask",              FacTask,         90,     FAC_TASK_STK);  // ATE   In order to check task running.
//  OS_CREATETASK(&TCBInTask,           "InTask",               InTask,          98,     IN_TASK_STK);
//  OS_CREATETASK(&TCBSPI1NetTask,      "SPI1NetTask",          SPI1NetTask,     99,     SPI1_NET_STK);
//  OS_CREATETASK(&TCBSCI1FwdTask,      "SCI1FwdTask",          SCI1FwdTask,     78,     SCI1_FWD_STK);   //USART1        ta2000 ������ 1踰� 485
//  OS_CREATETASK(&TCBSCI2Rs485Task,    "SCI2Rs485Task",        SCI2Rs485Task,   77,     SCI2_RS485_STK); //USART2        ta2000 ������ 2踰� 485
}

void FireStatusReload(void)
{
	FireOccurrence = FALSE;
}

void ResetStatus(void)
{
	isFireAlertOccur = FALSE;
	FireOccurrence = FALSE;
	g_DebugMode = TRUE;
}

void CheckSystemInitial(void)
{
	uint8_t u8SystemFlag[8];
	EEPROMGetDataBlock((INT16U)EEPROM_ADDRESS_SystemFlag, u8SystemFlag, 8);
	if( memcmp(u8SystemFlag, DefaultSystemFlag, 8) != 0){//"BruceIvx"
		//    rprintf("Erase all memory & Save default Data\r\n");
		EEPROMSetDataBlock(EEPROM_ADDRESS_SystemFlag, (INT8U*)DefaultSystemFlag,    8);//"BruceIvx"로 써넣기..
		EEPROMSetDataBlock(EEPROM_ADDRESS_SiteInfo, (INT8U*)DefaultSerial,    8);//"BruceIvx"로 써넣기..
		EEPROMSetDataBlock(E2P_Para_CMS ,       (INT8U*)&DefaultCmsParameter[0],    LEN_E2ROM_CMS);  // 82 -> 64
		EEPROMSetDataBlock(E2P_Para_SENSOR ,    (INT8U*)&DefaultSensorParameter[0], LEN_E2ROM_SENSOR);  //
		EEPROMSetDataBlock(E2P_Para_LOCK ,      (INT8U*)&DefaultLockParameter[0],   LEN_E2ROM_LOCK);
		EEPROMSetDataBlock(E2P_Para_BUTTON ,    (INT8U*)&DefaultButtonParameter[0], LEN_E2ROM_BUTTON);
		EEPROMSetDataBlock(E2P_Para_CONSOLE ,   (INT8U*)&DefaultConsoleParameter[0],LEN_E2ROM_CONSOLE);
		EEPROMSetDataBlock(E2P_Para_WIEGAND ,   (INT8U*)&DefaultWiegandParameter[0],LEN_E2ROM_WIEGAND);
		EEPROMSetDataBlock(E2P_Para_HTTPC ,     (INT8U*)&DefaultHttpcParameter[0],  LEN_E2ROM_HTTPC);
		EEPROMSetDataBlock(E2P_Para_FIRE ,   	(INT8U*)&DefaultFireParameter[0],	LEN_E2ROM_FIRE);

		EEPROMSetData(EEPROM_ADDRESS_CountPin_HIGH,      0);     //163
		EEPROMSetData(EEPROM_ADDRESS_CountPin_LOW,      0);      //164

		EEPROMSetData(EEPROM_ADDRESS_FlashSectorCount,      0);     //242
		EEPROMSetData(EEPROM_ADDRESS_EEPromPageCount,       0);     //243
		EEPROMSetData(EEPROM_ADDRESS_EventCount,            0);     //244
		EEPROMSetData(EEPROM_ADDRESS_InstallSiteNumber,     0);     //245

		ScheduleInitial();

//		EraseSFlash(FLASH_MpCardData_Start, FLASH_MpCardData_End);//== ClearCardDataOfSerialFlashAll();
		EraseSFlash(FLASH_RfCardData_Start, FLASH_RfCardData_End);//== ClearCardDataOfSerialFlashAll();
	}
	
	if((EEPROMGetData(EEPROM_ADDRESS_HolidayDate)>12)||(EEPROMGetData(EEPROM_ADDRESS_HolidayDate+1)>31)
			||(EEPROMGetData(EEPROM_ADDRESS_HolidayCount)>MAX_NUM_HOLIDAY)) { //MAX_NUM_HOLIDAY 60
		EEPROMSetData(EEPROM_ADDRESS_HolidayCount, 0); //카운트 지우고..
		EEPROMSetData(EEPROM_ADDRESS_HolidayDate,  0);//데이터 지운다..
		EEPROMSetData(EEPROM_ADDRESS_HolidayDate+1,0);//데이터 지운다..
	}
}

void ReloadParameter(void)
{
  INT8U *ReadPtr;
  INT16U ReadAddr = 0;

  rprintf("Reload Parameter.\r\n");	

  ReadAddr = (INT16U)E2P_Para_CMS;   //  +EEPROM_PARAM_INDEX_IP
  ReadPtr = &CMSParameter.IPAddress[0];//시작 포인트이므로..
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_CMS);

  //타임코드를 검증..
  for(int i=0; i < MAX_NUM_RELAY; i++){  //#define MAX_NUM_RELAY   6  // Lock  갯수
    for(int j=0; j < 3; j++){
      if(CMSParameter.DoorTimeCode[i][j] >= MAX_NUM_TIMECODE) //  #define MAX_NUM_TIMECODE  128  //63개면 64로.. 30개면 31로.. 배열 0은 그냥 공란..
        CMSParameter.DoorTimeCode[i][j] = 0;
      if(CMSParameter.LimitCardTimeCode[i][j] >= MAX_NUM_TIMECODE) //  #define MAX_NUM_TIMECODE  128  //63개면 64로.. 30개면 31로.. 배열 0은 그냥 공란..
        CMSParameter.LimitCardTimeCode[i][j] = 0;
      if(CMSParameter.SpecialCardTimeCode[i][j] >= MAX_NUM_TIMECODE) //  #define MAX_NUM_TIMECODE  128  //63개면 64로.. 30개면 31로.. 배열 0은 그냥 공란..
        CMSParameter.SpecialCardTimeCode[i][j] = 0;
    }
  }

  ReadAddr = (INT16U)E2P_Para_SENSOR;
  ReadPtr = &SensorParameter.Status[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_SENSOR);

  ReadAddr = (INT16U)E2P_Para_BUTTON;
  ReadPtr = &ButtonParameter.Status[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_BUTTON);

  ReadAddr = (INT16U)E2P_Para_LOCK;
  ReadPtr = &LockParameter.DoorTimerSet[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_LOCK);

  for(int i=0; i < MAX_NUM_RELAY; i++) {//MAX_NUM_SYSTEM_PORT 8
    if ((LockParameter.DoorLockMode[i] != LockMode_Normal) && 
        (LockParameter.DoorLockMode[i] != LockMode_Manual_Unlock) &&
        (LockParameter.DoorLockMode[i] != LockMode_Manual_Lock) &&
        (LockParameter.DoorLockMode[i] != LockMode_Schedule_Unlock) &&
        (LockParameter.DoorLockMode[i] != LockMode_Schedule_Lock)){//평소사용모드가 아니면..
      LockParameter.DoorLockMode[i] = LockMode_Manual_Unlock;  //0x10 수동풀림으로 놓는다..
    }
  }

  ReadAddr = (INT16U)E2P_Para_CONSOLE;
  ReadPtr = &ConsoleParameter.AccessOpenTime[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_CONSOLE);

  ReadAddr = (INT16U)E2P_Para_WIEGAND;
  ReadPtr = &WiegandParameter.CommType[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_WIEGAND);

  ReadAddr = (INT16U)E2P_Para_HTTPC;
  ReadPtr = &HttpcParameter.TAIP1[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, LEN_E2ROM_HTTPC);

  LoadScheduleData();
  GVTAEventData.u8LogCount           = EEPROMGetData(EEPROM_ADDRESS_EventCount); //read log count
  GVTAEventData.u8EEPromPageCount    = EEPROMGetData(EEPROM_ADDRESS_EEPromPageCount); //read page count
  GVTAEventData.u8SFlashSectorCount  = EEPROMGetData(EEPROM_ADDRESS_FlashSectorCount); //read FlashSector Count

  gU8InstallSiteNumber = EEPROMGetData(EEPROM_ADDRESS_InstallSiteNumber); //설치 고객사 번호..
  
  ReadAddr = (INT16U)EEPROM_ADDRESS_EventStart;
  ReadPtr = &GVTAEventData.pU8EventLog[0];
  EEPROMGetDataBlock(ReadAddr, ReadPtr, GVTAEventData.u8LogCount * 16);

  gnCountPin  = (EEPROMGetData((INT16U) EEPROM_ADDRESS_CountPin_HIGH) * 256);//
  gnCountPin += EEPROMGetData((INT16U) EEPROM_ADDRESS_CountPin_LOW);//
  if(gnCountPin == 0xFFFF )
  {
		EEPROMSetData(EEPROM_ADDRESS_CountPin_HIGH,      0);     //163
		EEPROMSetData(EEPROM_ADDRESS_CountPin_LOW,      0);      //164
		gnCountPin = 0;
  }

  FireLockFlag = EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag);//화재발생작동모드이면 119 아니면 0, 비작동모드이면 99
  if(FireLockFlag == 119){//119 이면 화재발생작동모드에 화재상태 119 이기 떄문에
    FireOccurrence = TRUE;
    FireLockFlag = 0;//화재작동모드 0 으로
  }//화재발생작동모드에 화재아님 0 이거나 비작동모드이면 99일 것이기 때문에 FireOccurrence == 0으로 가도 문제 없음..
}

void ReadTimeCodeFromEeprom(void) {
  for(int i=0; i<MAX_NUM_RELAY; i++){
    EEPROMGetDataBlock((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DOORTIMECODE + i * 3),        &CMSParameter.DoorTimeCode[i][0],        3);
    EEPROMGetDataBlock((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_LIMITCARDTIMECODE + i * 3),   &CMSParameter.LimitCardTimeCode[i][0],   3);
    EEPROMGetDataBlock((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_SPECIALCARDTIMECODE + i * 3), &CMSParameter.SpecialCardTimeCode[i][0], 3);
  }
}

//void WriteTimeCodeToEeprom(INT8U nlock){
//  for(int i=0; i<3; i++){
//    EEPROMSetData((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DOORTIMECODE + nlock * MAX_NUM_SYSTEM_PORT + i),        CMSParameter.DoorTimeCode[nlock][i]);
//    EEPROMSetData((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_LIMITCARDTIMECODE + nlock * MAX_NUM_SYSTEM_PORT + i),   CMSParameter.LimitCardTimeCode[nlock][i]);
//    EEPROMSetData((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_SPECIALCARDTIMECODE + nlock * MAX_NUM_SYSTEM_PORT + i), CMSParameter.SpecialCardTimeCode[nlock][i]);
//  }
//}

//void WriteDoorOpenTimeToEeprom(INT8U nlock)
//{

//EEPROMSetDataBlock((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DOOROPENTIME), data, MAX_NUM_LOCK);
//  INT8U i = 0;
//  INT16U WriteAddr = (INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DOOROPENTIME);
//	
//  for(i=0; i < MAX_NUM_LOCK; i++)
//    EEPROMSetData(WriteAddr++, data[i]);
//}

void InitRTC(void)  {
  INT8U TimePacket[7];
  EEPROMGetDataBlock(EEPROM_ADDRESS_DateTime, (INT8U*)TimePacket, 7);
//  SetToDay( &TimePacket[0] );
	 RTC_SetTimeAndDate( &toDayTime, &TimePacket[0] );
}



/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: InTask.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#define    IN_GLOBALS
#include   "InTask.h"

/******************************************************************************
**  IN TASK
******************************************************************************/
#define OS_Delay	MyMsDelay
// #define TEST_SEC_READ_ON
// // void InTask(void *pdata)
//uint8_t u8FireStatusFlag=0;
unsigned short int  uiCrcValue = 0xFFFF;
uint16_t u16Timer6_count=0;
uint16_t u16Sec = 0;

extern IWDG_HandleTypeDef hiwdg;
uint8_t u8DoorOpendTooLong;
extern stRtLog stRealtimeLog;

void InputInit(void)
{

  gU8DoorOpenedTooLong = EEPROMGetData((INT16U) EEPROM_ADDRESS_DoorOpendTooLong);
  u8DoorOpendTooLong = gU8DoorOpenedTooLong;
  FireAlertCnt = 0;
  
  gU16IntaskAliveCount = 0;
  gU16nDelayCnt_Count = 0;

  /*TACheckJumperSetting
  Advanced-control timers (TIM1 and TIM8)
  General-purpose timers (TIM2, TIM3, TIM4 and TIM5)
  Basic timers TIM6 and TIM7
  타이머가 매우 정확하지는 않지만 10ms 간격으로 돌게 처리함..
  ******************************************************************************/
  HAL_TIM_Base_Start_IT(&htim2);
// 1ms 주기로.. 높이면 주기가 짧아짐
// 1 은 화재 시 닫힘을 의미.
/*
 	FireParameter.FireClose[0] = 0;
	FireParameter.FireClose[1] = 0;
	FireParameter.FireClose[2] = 1;
	FireParameter.FireClose[3] = 1;

	EEPROMSetDataBlock(E2P_Para_FIRE, &FireParameter.FireClose[0], MAX_NUM_RELAY);
*/
}

uint8_t idInput;
void InProcess(void)
{
 //    OS_Delay(10);
    gU16IntaskAliveCount ++;    //이건 계속 바뀌어야만 intask가 살아있다는 것이 됨..
    CheckFireAlert();
	idInput = TACheckJumperSetting();
/*
    if( idInput == 0xf0) {
    	if( nNetPartMode == TA_SERVER ) {
    		nNetPartMode = PUSH_CLIENT;
    		OPEN_Ethernet();
    	}
    }
    else {
    	if( nNetPartMode == PUSH_CLIENT) {
    		nNetPartMode = TA_SERVER;
    		OPEN_Ethernet();
    	}
    }
*/
    CheckSensor();   
    CheckButton();         
    CheckTamper();

    DoorTimerProcess(); //도어의 lock mode를 체크해서 time code 확인으로 door 상태 변경 진행.. 

    SaveDateTime();

    if(u16Sec > 3600)
    {//3600초마다 저장 1시간마다.. 타이머에선 eeprom 쓰기 안됨..
//      EEPROMSetDataBlock(E2P_Para_SENSOR , (uint8_t*)&SensorParameter.OpenTime[0], LEN_E2ROM_SENSOR);
    	EEPROMSetDataBlock(E2P_Para_LOCK , (uint8_t*)&LockParameter.DoorTimerSet[0], LEN_E2ROM_LOCK);
//      EEPROMSetDataBlock(E2P_Para_BUTTON , (uint8_t*)&ButtonParameter.OpenTime[0], LEN_E2ROM_BUTTON);
//        SaveInStatusToEeprom(); INP_EXT void SaveInStatusToEeprom(void);
    	u16Sec = 0;
    }
    
    if(u16Sec % 2)
    {//1초마다
    	_RUN_LED_TOGGLE();     // LED OnOff
    }
    
    if(u8DoorOpendTooLong != gU8DoorOpenedTooLong)
    {//값이 다르면 저장해 놓고 또 비교해본다..
		u8DoorOpendTooLong = gU8DoorOpenedTooLong;
		EEPROMSetData((INT16U) EEPROM_ADDRESS_DoorOpendTooLong, u8DoorOpendTooLong);
    }

    TaskRunCnt(ENUM_IN_TASK);          // Notice Activated.  20150210 Jylee
//    HAL_IWDG_Refresh(&hiwdg);
}

//==============================================================================
//Timer2 Interrupt Handler
//==============================================================================
void INT_TIM2HandlerProc(void)
{
    u16Timer6_count++;
    if(u16Timer6_count > 130)
    {//1초마다 체크
		u16Sec++;
		CountLockTimerEach1Sec();
		CheckDoorOpenedTooLongEach1Sec();//오래열림 체크..
		u16Timer6_count = 0;
    }
}

void CountLockTimerEach1Sec(void)
{
  for(int nLock=0; nLock < MAX_NUM_RELAY; nLock++)
  {
    if (LockParameter.DoorTimerSet[nLock] > 0)
    {//if timer is runned..
        LockParameter.DoorTimerCount[nLock]++;	
    }
  }
}

void CheckDoorOpenedTooLongEach1Sec(void){//오래열림 체크..
//#ifdef TACS_V244
//        gU8DoorOpenedTooLong = 0;//문오래열림 적용안함..
//        return;
//#endif
	for(int nSensor=0; nSensor < MAX_NUM_SENSOR; nSensor++)
	{
		if(SensorParameter.Status[nSensor] == 0x00)
		{//센서가 열려 있고..
			if (gU8ArrDoorOpenedTooLongCount[nSensor] < DOOR_OPENED_TOO_LONG_SEC)
			{//#define DOOR_OPENED_TOO_LONG_SEC 20
				gU8ArrDoorOpenedTooLongCount[nSensor]++; //증가시킴..
			}
			else if(gU8ArrDoorOpenedTooLongCount[nSensor] >= DOOR_OPENED_TOO_LONG_SEC)
			{//#define DOOR_OPENED_TOO_LONG_SEC 20
				gU8ArrDoorOpenedTooLongCount[nSensor] = 0;//다시 돌려~
				gU8DoorOpenedTooLong |= 0x80>>nSensor;//해당비트만 1 로 만들기..
			}
			else
			{//열림시간+5 보다 오래 열려있으면..
				//아무짓도 안함.. 다시 전송하는 루틴은 위에서 처리했음..
			}
		}
		else
		{//센서가 닫혀 있으면..
			gU8DoorOpenedTooLong &= 0xff^(0x80>>nSensor);//해당비트만 0 으로 만들기.. xor 0^0 == 1^1 == 0, 1^0 == 1
			gU8ArrDoorOpenedTooLongCount[nSensor] = 0;
		}
	}
}

void ReleaseLockSelectDoor(INT8U OpenTime, INT8U nLock, INT8U nAuth)
{
  for(int i = 0; i < MAX_NUM_RELAY; i++)
  { //모델별로 가지고 있는 최대 락까지만 진행함..
    if((LockParameter.SelectDoor[nLock] << i) & 0x80)
    {//1234 5678
      if((nAuth << i) & 0x80)
      {//1234 5678
        ControlDoor(OpenTime, i);
//        LockParameter.DoorTimerSet[i] = OpenTime;
//        LockParameter.DoorTimerCount[i] = 0;
      }
    }
  }
}

void ControlDoor(uint8_t OpenTime, uint8_t nLock){
  LockParameter.DoorTimerSet[nLock] = OpenTime;
  LockParameter.DoorTimerCount[nLock] = 0;
//  for(int i = 0; i < MAX_NUM_RELAY; i++){ //모델별로 가지고 있는 최대 락까지만 진행함..
//    if((LockParameter.SelectDoor[nLock]<<i)&0x80){//1234 5678
//      LockParameter.DoorTimerSet[i] = OpenTime;
//      LockParameter.DoorTimerCount[i] = 0;
//    }
//  }
}

void ChangeLockMode(uint8_t LockMode, uint8_t nLock)  {
  if(FireOccurrence != TRUE){
    switch(LockMode)  {
      case LockMode_Normal ://0x01
      case LockMode_Manual_Unlock ://0x10
      case LockMode_Manual_Lock ://0x11
      case LockMode_Schedule_Unlock ://0x20
      case LockMode_Schedule_Lock :{//0x21
        LockParameter.DoorLockMode[nLock] = LockMode;
        break;
      }
      case LockMode_NotUse :// 무조건 개방상태로..
      case LockMode_Unknown :
      default :{
        LockParameter.DoorLockMode[nLock] = LockMode_Manual_Unlock;  //0x10
        break;
      }
    }
    EEPROMSetData(E2P_Para_LOCK + EEPROM_OFFSET_DoorLockMode + nLock, LockParameter.DoorLockMode[nLock]);
  }
}

/******************************************************************************
**  Door Timer Process - Call this function every 100ms in InTask
******************************************************************************/
void DoorTimerProcess(void)
{
  uint8_t nLock, nCode[3], nWeek, u8LockModeChangeFlag;
//  INT16U nSchedStartMin, nSchedEndMin, nCurrTimeMin ;
	
//  nWeek = toDayTime.WeekDAY;
//  if(gU8HolidayFlag) 	nWeek = 7;  //공휴일 플래그가 걸리면..
//  else 	nWeek = toDayTime.WeekDAY;
  nWeek = (gU8HolidayFlag)?7:toDayTime.WeekDAY;//공휴일 플래그가 걸리면..
  for(nLock=0; nLock < MAX_NUM_DOOR; nLock++)
  {
    memcpy(nCode,CMSParameter.DoorTimeCode[nLock],3);//새끼줄 값 가져와서..
//    for(int i=0; i<3; i++){
//      nCode[i] = CMSParameter.DoorTimeCode[nLock][i];//새끼줄 값 가져와서..
//    }
//    nCode[1] = CMSParameter.DoorTimeCode2[nLock];//새끼줄 값 가져와서..
//    nCode[2] = CMSParameter.DoorTimeCode3[nLock];//새끼줄 값 가져와서..
    u8LockModeChangeFlag = LockParameter.DoorLockMode[nLock];

    if((LockParameter.DoorLockMode[nLock] & 0xf0) == 0x20)
    { // schedule mode이면..

      for(int i=0; i < 3; i++)
      {
        if(nCode[i] > 0)
        {//timecode가 있다면.. Check Time And Compare Open Schdule
          if(CheckInSchedule(ScheduleParameter[nLock][nCode[i]][nWeek].StartHh, ScheduleParameter[nLock][nCode[i]][nWeek].StartMm,
                             ScheduleParameter[nLock][nCode[i]][nWeek].EndHh, ScheduleParameter[nLock][nCode[i]][nWeek].EndMm))
          {
#ifdef DEBUG_MODE
//			printf( "스케률 인 nLock=%d,nCode=%d,i=%d,week=%d,start=%d:%d, end=%d:%d\r\n", nLock, nCode[i], i, nWeek,
//					ScheduleParameter[nLock][nCode[i]][nWeek].StartHh,ScheduleParameter[nLock][nCode[i]][nWeek].StartMm,
 //                   ScheduleParameter[nLock][nCode[i]][nWeek].EndHh, ScheduleParameter[nLock][nCode[i]][nWeek].EndMm);
#endif
             i=10;//for 문 빠져 나갈 수 있도록..
            u8LockModeChangeFlag=LockMode_Schedule_Unlock;//ChangeLockMode(LockMode_Schedule_Unlock,nLock);
          }
          else
          {
            u8LockModeChangeFlag=LockMode_Schedule_Lock;//ChangeLockMode(LockMode_Schedule_Lock,nLock);
          }
        }
        else
        {//timecode가 없다면..
          u8LockModeChangeFlag=LockMode_Schedule_Lock;//ChangeLockMode(LockMode_Schedule_Lock,nLock);
        }
      }//end for..
    }//end if..

    if(LockParameter.DoorLockMode[nLock] != u8LockModeChangeFlag)
    {
      ChangeLockMode(u8LockModeChangeFlag,nLock);
    }
    ExecuteLockOnOff(nLock);
  }
}

void ExecuteLockOnOff( uint8_t nLock)
{
//  INT16U  DoorTimer = 0;
  if(FireOccurrence == TRUE)
  { //화재시..
    if(FireLockFlag != 99){//EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
    	if( FireParameter.FireClose[nLock] == 1)
    		SetLockOnOff(nLock, false);// Relay off == door is locked..
    	else
    		SetLockOnOff(nLock, true);// Relay on == door is unlocked..
      LockParameter.DoorTimerSet[nLock]   = 0;
      LockParameter.DoorTimerCount[nLock]   = 0;
      return;
    }
  }
//  else
//			FW_TACS_STM_V1.4.06_SC_210311.bin 에서 적용.. 화재작동안함 선택의 경우 화재시 모든버튼과 카드입력도 처리안했으나
//  	  	  그건 문제 있으므로 버튼과 카드는 출입할 수 있도록 함
//  {  		//화재 해제시.. 또는           FireOccurrence = 99;//화재인데 화재작동 안하는 경우는 여기에 해당함.. 노말모드로..
    if (LockParameter.DoorTimerSet[nLock] > 0)
    {//if DoorTimer is running..
      SetLockOnOff(nLock, true); // Relay on == door is unlocked..
      if(LockParameter.DoorTimerCount[nLock] >= LockParameter.DoorTimerSet[nLock])
      { //door open time 이 지나면 닫는다..
        LockParameter.DoorTimerSet[nLock] = 0;
        LockParameter.DoorTimerCount[nLock] = 0;		            
      }
    }
    else
    {  //if DoorTimer is stoped..
      switch(LockParameter.DoorLockMode[nLock])
      {
        case LockMode_Normal ://0x01
        case LockMode_Manual_Lock ://0x11
        case LockMode_Schedule_Lock :
        {//0x21
          SetLockOnOff(nLock, false);// Relay off == door is locked..
          break;
        }
        case LockMode_Manual_Unlock ://0x10
        case LockMode_Schedule_Unlock :
        {//0x20
          SetLockOnOff(nLock, true); // Relay on == door is unlocked..
          break;
        }
        case LockMode_NotUse ://0x00
        case LockMode_Unknown ://0xff
        default :
        {
          SetLockOnOff(nLock, false);// Relay off == door is locked..
          LockParameter.DoorLockMode[nLock]         = LockMode_Manual_Unlock;
          LockParameter.DoorTimerSet[nLock] = 0;
          LockParameter.DoorTimerCount[nLock] = 0;		            
          break;
        }
      }
    }
//  }
}

void CheckSensor(void)
{
	uint8_t SensorIndex, u8SensorValue=0;

	for(SensorIndex=0; SensorIndex < MAX_NUM_SENSOR; SensorIndex++)
	{
		SensorScan(SensorIndex);
		if(SensorParameter.Status[SensorIndex] == TRUE)
		{//문닫힘 상태이면..
			u8SensorValue += (0x80>>SensorIndex); //1234 5678
		}
	}

	if(gU8SensorValue != u8SensorValue)
	{//변화가 있었다면..
		gU8SensorValue = u8SensorValue;

		if( (gU8SensorValue & 0x80) == 0x80)    	SensorIndex = 0;
		if( (gU8SensorValue & 0x40) == 0x40)    	SensorIndex = 1;
		if( (gU8SensorValue & 0x20) == 0x20)    	SensorIndex = 2;
		if( (gU8SensorValue & 0x10) == 0x10)    	SensorIndex = 3;

		TABuf_Tx[TA_INDEX_DAT+0] = '1';//정상아님.. 없음..
		TABuf_Tx[TA_INDEX_DAT+1] = gU8SensorValue;  //1234 5678
		TABuf_Tx[TA_INDEX_DAT+2] = gU8ConsoleAlive; //
		TABuf_Tx[TA_INDEX_DAT+3] = gU8DoorOpenedTooLong;  //1234 5678
		TABuf_Tx[TA_INDEX_DAT+4] = TACheckJumperSetting(); //1234 0000 나중에 쓸지 모름..
		CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_EVENT_TIME);//funcCopyEventDateTime_to_TABuf_Tx();
		memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);

		if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속
		{
			HTTP_MakeHttpPacketRtLog( SensorIndex, HTTP_EVENT_LONG_OPEN_CODE, HTTP_VERIFYTYPE_EVENT , Result_OK);
		}
		//							// 소켓통신 TCP/IP 인터페이스로 접속
		{
			TAMakeResponsePacket(TA_StrCmdGetEvent);//TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
		}
#ifdef TEST_SEC_READ_ON

#ifdef LOG_DEBUG
	printf( "TEST_SEC_READ_ON CheckSensor SensorIndex=%02X\r\n",	SensorIndex);
#endif
		ConsoleResultPack(3); //  {  IDX_AUDIOMENT_DOOR_OPEN, IDX_AUDIOMENT_MUTE_100MS}, // 0x01
#endif

	}
}

void SensorScan( uint8_t nSensor)
{//MAX_NUM_SYSTEM_PORT 8 메모리는 모조리..
  uint16_t gSensorPin[MAX_NUM_SENSOR]       = {ADC1_IN1_Pin,        ADC1_IN4_Pin,       ADC1_IN12_Pin,      ADC1_IN15_Pin   };
  GPIO_TypeDef *gSensorPort[MAX_NUM_SENSOR] = {ADC1_IN1_GPIO_Port,  ADC1_IN4_GPIO_Port, ADC1_IN12_GPIO_Port, ADC1_IN15_GPIO_Port   };
  uint8_t readPort = 0;
  
  if( nSensor < MAX_NUM_SENSOR)    
  {
	readPort = HAL_GPIO_ReadPin(gSensorPort[nSensor], gSensorPin[nSensor]);    // 센서값 readPort == 0 일 때 닫힘 상태.
	if( SensorParameter.NoNc[nSensor] == 1) // NC
		readPort = !readPort;

    if(SensorParameter.Status[nSensor] == FALSE) //TRUE)    // //문열림 상태이면.. Already Detected
    { 
      if(readPort == FALSE) //!= FALSE) { // 문이 열려있는 상태에서 센서값이 닫힘 상태이면.. 10을 누적하고 계속이면 문닫힘 상태로 바꿈..
      {
        if(SensorParameter.SensCnt[nSensor] < 10 )  //카운트하고 10이상이 되면 상태바꿈..
        { 
          SensorParameter.SensCnt[nSensor]++;
        }
        else //  if(SensorParameter.SensCnt[nSensor] >= 10) {
        { 
          SensorParameter.SensCnt[nSensor] = 0;
          SensorParameter.Status[nSensor] = TRUE;   //FALSE; //닫힘상태로 바꾼다..
          PCALED_SetSensorLed( nSensor, TRUE);
        }
      }
      else  
      {
        SensorParameter.SensCnt[nSensor] = 0;
      }	
    }
    else //if(SensorParameter.Status[nSensor] == TRUE)    // //문닫힘 상태이면..
    {  
      if(readPort == TRUE) // 문이 닫혀있는 상태에서 센서값이 열림 상태이면.. 10을 누적하고 계속이면 문열림상태로 바꿈..
      {
        if(SensorParameter.SensCnt[nSensor] < 10 )
        {
          SensorParameter.SensCnt[nSensor]++;
        } 
        else 
        {  //if(SensorParameter.SensCnt[nSensor] >= 10)
          SensorParameter.SensCnt[nSensor] = 0;
          SensorParameter.Status[nSensor] = FALSE;//TRUE;	
          PCALED_SetSensorLed( nSensor, FALSE);
        }
      }
      else 
      {
        SensorParameter.SensCnt[nSensor] = 0;
      }
    }
  }
  else  
  {
    rprintf( "[Error] Sensor %d is not set. \r\n", nSensor);
  }
}

void CheckButton(void)
{
  uint8_t SwIndex;
  for(SwIndex=0; SwIndex < MAX_NUM_BUTTON; SwIndex++)  
  {
    ButtonScan(  SwIndex);
  }
}

void ButtonScan( uint8_t nSw)//MAX_NUM_SYSTEM_PORT 8 메모리는 모조리.. 
{
  uint16_t gButtonPin[MAX_NUM_BUTTON]       =  {ADC1_IN2_Pin,        ADC1_IN5_Pin,       ADC1_IN8_Pin,       ADC1_IN14_Pin   };
  GPIO_TypeDef *gButtonPort[MAX_NUM_BUTTON] =  {ADC1_IN2_GPIO_Port,  ADC1_IN5_GPIO_Port, ADC1_IN8_GPIO_Port, ADC1_IN14_GPIO_Port   };

  uint8_t readPort = HAL_GPIO_ReadPin(gButtonPort[nSw], gButtonPin[nSw]);
  
  if(ButtonParameter.Status[nSw] == TRUE)
  { // Already Detected
    if(readPort != FALSE) { //안눌렸을 때..
      if(ButtonParameter.ButtonCnt[nSw]< 10 )
      {
          ButtonParameter.ButtonCnt[nSw]++;
      } 
      else if(ButtonParameter.ButtonCnt[nSw] >= 10)
      {
        ButtonParameter.ButtonCnt[nSw] = 0;
        ButtonParameter.Status[nSw] = FALSE;	
      } 
      else
      {
           //Do not Anything
      }
    } 
    else
    {//눌려 있다면..
      LockParameter.DoorTimerCount[nSw] = 0;//시간을 연장.. FW_TACS_STM_V1.4.03_SC_210224.bin       버튼입력이 계속될 때 도어오픈은 계속 연장..
      ButtonParameter.ButtonCnt[nSw] = 0;
      ControlDoor(CMSParameter.DoorOpenTime[nSw],  nSw); //버튼은 무조건 열림.. FW_TACS_STM_V1.4.05_SC_210311.bin 에서 추가함
    }
  }
  else {
    if(readPort == FALSE)
    { //눌렸을 때..
      if(ButtonParameter.ButtonCnt[nSw] < 10 )
      {
          ButtonParameter.ButtonCnt[nSw]++;
      }
      else if(ButtonParameter.ButtonCnt[nSw] >= 10)
      {
        ButtonParameter.ButtonCnt[nSw] = 0;
        ButtonParameter.Status[nSw] = TRUE;	

        ControlDoor(CMSParameter.DoorOpenTime[nSw],  nSw); //버튼은 무조건 열림..

        uint32_t nDelayCnt = 0;
        if(gU8LogCountFlag > 0)
        {
          while(1){ //다른데서 쓰고 있는중이면 멈춤..
			  if(nDelayCnt++ > 100000)
			  {
				gU16nDelayCnt_Count ++;
				break;
			  }
          }
        }
//        gU8LogCountFlag = 1;//멈춤 설정..

        memcpy(GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16,"\x1A\x30\x45\x42\x00",5); //26 length, '0' "EB" 0(발생)
        GVTAEventData.pU8EventLog[GVTAEventData.u8LogCount*16+ 5] = nSw; //data-event내용2  출입문 번호 0~3
        CopyYYMMDDhhmmss(GVTAEventData.pU8EventLog + GVTAEventData.u8LogCount*16 + 6);
        memset(GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16+12,0,4);//카드자리 4바이트 클리어
        SaveEventLog2EEprom_sFlash();//2. log count는 이 함수에서 올림..
//        gU8LogCountFlag = 0;//멈춤 해제..
        
    	if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속
    	{
    		HTTP_MakeHttpPacketRtLog( nSw, HTTP_EVENT_EXIT_CODE, HTTP_VERIFYTYPE_EVENT , Result_OK);
    	}
    //							// 소켓통신 TCP/IP 인터페이스로 접속
		if(TASocketConnected > 0)
		{// 커넥트 되어 있고 패킷전송할게 있으면.. 업뎃중이면 안해야 하지 않을까?
		  TAMakeResponsePacket(TA_StrCmdAckEvent);//TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
		}
      }
      else
      {
           //Do not Anything
      }
    }
    else
    {
      ButtonParameter.ButtonCnt[nSw] = 0;
    }	
  }	
}

/******************************************************************************
**  Fire Alert CHECK
******************************************************************************/
void CheckFireAlert(void)
{
  uint8_t readPort = 0;

  readPort = HAL_GPIO_ReadPin(FIRE_ALARM_GPIO_Port, FIRE_ALARM_Pin);

  if(isFireAlertOccur == TRUE)
  {// Fire Occured already.. 사실 화재상태면 체크할 필요가 없을텐데.. 실제로 로직자체도 그렇고..
    if(readPort != FALSE)
    {     // Fire Not Occur
      if(FireAlertCnt < 5 )
      {
        FireAlertCnt++;
      } 
      else if(FireAlertCnt >= 5)
      {
        FireAlertCnt = 0;
        isFireAlertOccur = FALSE;

        FireOccurrence = FALSE;
       // 네트웍으로 화재 해제 신호가 안들어 와서 강제 해제
        EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag, 0);//eeprom 화재신호 지우기..

      } 
      else
      {
         //Do nothing
      }
    } 
    else
    {
      FireAlertCnt = 0;
    }	
  } 
  else	{// Fire not Occured..
    if(readPort == FALSE)
    {      // Fire Occur
      if(FireAlertCnt < 5 )
      {
        FireAlertCnt++;
      } 
      else if(FireAlertCnt >= 5)
      {
        FireAlertCnt = 0;
        isFireAlertOccur = TRUE;	
        if(FireLockFlag != 99)
        {//EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
          FireOccurrence = TRUE;
          EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag,   119);
        }
        else
        {
          FireOccurrence = 99;//화재인데 화재작동 안하는 경우는 여기에 해당함.. 노말모드로..
        }

    	if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속에서 연결되었으면 보내는 걸로 바꿈.
    	{
			HTTP_MakeHttpPacketRtLog( 0, HTTP_EVENT_FIRE_CODE, HTTP_VERIFYTYPE_EVENT, Result_OK);
    	}
//    	else						// 소켓통신 TCP/IP 인터페이스로 접속
    	{
			memcpy(TABuf_Tx+TA_INDEX_DAT,"\x30\x49\x46\x00\x00",5);//"IF"
			CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_EVENT_TIME);//funcCopyEventDateTime_to_TABuf_Tx();
			memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);

			TAMakeResponsePacket(TA_StrCmdGetEvent);//TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
    	}

		SetSSDPSendFireAlarm();
        printf( "Fire Flag is Set. \r\n");
      }
      else
      {
         //Do not Anything
      }
    }
    else
    {
      FireAlertCnt = 0;
    }
  }	
}


/******************************************************************************
**   CHECKING TAMPER
******************************************************************************/
void CheckTamper(void)
{
  uint8_t readPort = 0;

  readPort = HAL_GPIO_ReadPin( TAMPER_GPIO_Port, TAMPER_Pin);

  if(isTamperAlertOccur == TRUE)
  {// Tamper Occured already.. 사실 탬퍼 상태면 체크할 필요가 없을텐데.. 실제로 로직자체도 그렇고..
    if(readPort != FALSE){     // Tamper Not Occur
      if(TamperAlertCnt < 5 )
      {
    	  TamperAlertCnt++;
      }
      else if(TamperAlertCnt >= 5)
      {
    	  TamperAlertCnt = 0;
        isTamperAlertOccur = FALSE;
      }
      else
      {
         //Do nothing
      }
    }
    else {
    	TamperAlertCnt = 0;
    }
  }
  else {// Fire not Occured..
    if(readPort == FALSE)
    {      // Fire Occur
      if(TamperAlertCnt < 5 )
      {
    	  TamperAlertCnt++;
      }
      else if(TamperAlertCnt >= 5)
      {
    	  TamperAlertCnt = 0;
    	  isTamperAlertOccur = TRUE;
    	  TamperOccurrence = TRUE;

        memcpy(TABuf_Tx+TA_INDEX_DAT,"\x30\x49\x46\x00\x00",5);//"IF"
        CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_EVENT_TIME);//funcCopyEventDateTime_to_TABuf_Tx();
        memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);


    	if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속
    	{
    		HTTP_MakeHttpPacketRtLog( readPort, HTTP_EVENT_TAMPER_CODE, HTTP_VERIFYTYPE_TAMPER , Result_OK);
    	}
    //							// 소켓통신 TCP/IP 인터페이스로 접속
    	{
	        TAMakeResponsePacket(TA_StrCmdGetEvent);//TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
    	}

        rprintf( "Tamper Flag is Set. \r\n");
      }
      else {
         //Do not Anything
      }
    }
    else {
    	TamperAlertCnt = 0;
    }
  }
}


/******************************************************************************
**   TestLockStatus 
******************************************************************************/
void TestLockOut(void)
{
  uint8_t i;
  for(i=0; i < MAX_NUM_RELAY; i++)
  {
    SetLockOnOff(i, true);
    OS_Delay(200);
  }

  for(i=0; i < MAX_NUM_RELAY; i++)
  {
    SetLockOnOff(i, false);
    OS_Delay(200);
  }
}

void SetLockOnOff( uint8_t NoRelay, bool bON)
{    //MAX_NUM_SYSTEM_PORT 8 메모리는 모조리..
  uint16_t gRelayPin[MAX_NUM_RELAY]       = {RELAY1_Pin,       RELAY2_Pin,       RELAY4_Pin,       RELAY3_Pin,       ALARM_C6_Pin,       ALARM_G8_Pin};
  GPIO_TypeDef *gRelayPort[MAX_NUM_RELAY] = {RELAY1_GPIO_Port, RELAY2_GPIO_Port, RELAY4_GPIO_Port, RELAY3_GPIO_Port, ALARM_C6_GPIO_Port, ALARM_G8_GPIO_Port};

  if(bON)
  {
	  HAL_GPIO_WritePin(gRelayPort[NoRelay % MAX_NUM_RELAY], gRelayPin[NoRelay % MAX_NUM_RELAY], GPIO_PIN_SET);// Relay1 On
  }
  else
  {
	  HAL_GPIO_WritePin(gRelayPort[NoRelay % MAX_NUM_RELAY], gRelayPin[NoRelay % MAX_NUM_RELAY], GPIO_PIN_RESET);// Relay1 Off
  }
}

void SaveDateTime(void)
{
    uint8_t TimePacket[7];

    if(DateTimeSaveFlag)
    {
        if(((toDayTime.MIN%30)==0)&&(toDayTime.SEC==0))
        {//0,30분 0초이면.. pass
        }
        else
        {
            DateTimeSaveFlag = 0; //이제 다시 체크하그라..
        }
    }
    else
    {
        if(((toDayTime.MIN%30)==0)&&(toDayTime.SEC==0))
        {//0,30분 0초이면.. time save..
            DateTimeSaveFlag = 1;
            CopyYYMMDDhhmmss(TimePacket);
            TimePacket[6] = toDayTime.WeekDAY;
            EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime, (uint8_t*)TimePacket, 7);
        }
        else
        {
            DateTimeSaveFlag = 0; //안해도 상관없지만.. 그래도.. 노파심에...
        }
    }
}

void ScheduleInitial(void)
{//#define	FLASH_ScheduleData			0x00376000L	//  4bytes * 4 Lock * 8 weekday&holiday * 64 timecode = 8192 bytes
	xERASE_ExFlash4kSector(FLASH_ScheduleData);
}

void GetScheduleData(INT8U nLock, INT8U nCode)
{
	uint8_t nWeek;
	INT16U i = 0;
	INT32U startAddr = FLASH_ScheduleData + (LEN_TIMECODE_LOCK*nLock) + (nCode*LEN_TIMECODE);
	for( nWeek = 0; nWeek < MAX_NUM_WEEK_HOLIDAY; nWeek++)
	{

#ifdef DEBUG_MODE
			printf( "GetScheduleData startAddr=%08X+%d, nWeek=%02X\r\n", startAddr, i , nWeek);
#endif

//		xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].EnWeek, 1);
//		i++;
		xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].StartHh, 1);
		i++;
		xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].StartMm, 1);
		i++;
		xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].EndHh, 1);
		i++;
		xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].EndMm, 1);
		i++;
	}
}

void LoadScheduleData(void)
{
	INT16U i = 0;
	INT32U startAddr;
	uint8_t nLock, nCode, nWeek;

	startAddr = FLASH_ScheduleData;
	for( nLock = 0; nLock < MAX_NUM_DOOR; nLock++)
	{
		for( nCode = 0; nCode < MAX_NUM_TIMECODE; nCode++)
		{
			for( nWeek = 0; nWeek < MAX_NUM_WEEK_HOLIDAY; nWeek++)
			{
#ifdef DEBUG_MODE
			printf( "GetScheduleData startAddr=%08X+%d, nLock=%d, nCode=%d, nWeek=%d, ", startAddr, i , nLock, nCode,nWeek);
#endif
				xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].StartHh, 1);
				i++;
				xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].StartMm, 1);
				i++;
				xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].EndHh, 1);
				i++;
				xREAD_DataFromExNorFlash(startAddr+i,&ScheduleParameter[nLock][nCode][nWeek].EndMm, 1);
				i++;
#ifdef DEBUG_MODE
			printf( "Schedule start=%d:%d, End=%d:%d\r\n",
					ScheduleParameter[nLock][nCode][nWeek].StartHh,	ScheduleParameter[nLock][nCode][nWeek].StartMm,
					ScheduleParameter[nLock][nCode][nWeek].EndHh,   ScheduleParameter[nLock][nCode][nWeek].EndMm);
#endif
			}
		}
	}
}

void SaveScheduleData(void)
{//스케쥴을 flash 에 저장..
	uint16_t ulFor;
	uint32_t wrAddr;
	uint32_t rdAddr;
	xERASE_ExFlash4kSector(FLASH_ScheduleData);    //   0x00081000L  // 타임코드, Event History 영역

	wrAddr = FLASH_ScheduleData;
	rdAddr = ScheduleParameter;
	for( ulFor = 0; ulFor < QSPI_SUBSECTOR_SIZE; ulFor+=MX25L51245G_PAGE_SIZE)
	{

#ifdef DEBUG_MODE
			printf( "SaveScheduleData wrAddr=%08X, ulFor=%04X, addr=%08X\r\n", wrAddr , ulFor, (rdAddr+ulFor) );
#endif
		xWRITE_DataToExNorFlash(wrAddr, (uint8_t*)(rdAddr+ulFor), MX25L51245G_PAGE_SIZE);
		wrAddr += MX25L51245G_PAGE_SIZE;
	}
}

void ResetDoorStatus(void)
{
  memset(LockParameter.DoorTimerSet,0,  MAX_NUM_RELAY);
  memset(LockParameter.DoorTimerCount,0,MAX_NUM_RELAY);
//  memset(LockParameter.DoorLockMode,0,  MAX_NUM_SYSTEM_PORT);//초기화는 이해 안됨..
//  memset(LockParameter.SelectDoor,0,    MAX_NUM_SYSTEM_PORT);//초기화는 이해 안됨..
}

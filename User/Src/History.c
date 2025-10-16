/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: History.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/

/******************************************************************************
**
**   Include Files
**
******************************************************************************/
#define    HIS_GLOBALS

#include   "History.h"

extern unionRtLog_Parameter stRealtimeLog;
extern uint8_t gSetEventRtlog;

void SaveEventLog2EEprom_sFlash(void)
{
  INT8U /*u8SFlashSectorCount, */u8EEPromPageCount, u8LogCount;
 
/*typedef struct  {
  uint8_t u8SFlashSectorCount;  //sFlash Sector Size 0x1000 4096
  uint8_t u8EEPromPageCount;    //eeprom Page size   0x100  256
  uint8_t u8LogCount;
  uint8_t pU8EventLog[256];//16bytes * 16 log..
}TA_EventData;*/
  u8EEPromPageCount = EEPROMGetData(EEPROM_ADDRESS_EEPromPageCount); //read page count
  u8LogCount = EEPROMGetData(EEPROM_ADDRESS_EventCount); //read log count

  if(u8EEPromPageCount*16+u8LogCount < GVTAEventData.u8EEPromPageCount*16+GVTAEventData.u8LogCount)
  { //EEProm에 저장된 카운트가 작으면 페이지 단위로 모조리 저장한다..
    for(uint8_t i=0;i<GVTAEventData.u8EEPromPageCount-u8EEPromPageCount;i++)
    {
      EEPROMSetDataBlock(EEPROM_ADDRESS_EventStart+(GVTAEventData.u8EEPromPageCount+i)*0x100,  GVTAEventData.pU8EventLog,
        (GVTAEventData.u8EEPromPageCount-u8EEPromPageCount > 0)?0x100:GVTAEventData.u8LogCount*16+16);  // count size 만큼...
    }
  }
  else
  {  //아니면 하나만 추가 저장한다..
        EEPROMSetDataBlock(EEPROM_ADDRESS_EventStart+GVTAEventData.u8EEPromPageCount*0x100+GVTAEventData.u8LogCount*16,\
                       GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16, 16);  //1. 데이터 eeprom에 저장하고..
  }

  GVTAEventData.u8LogCount++;//2. log count 를 올림..
  if(GVTAEventData.u8LogCount >= 16 )
  {//over 1 page size 0x100
    GVTAEventData.u8EEPromPageCount++;
    if(GVTAEventData.u8EEPromPageCount >= 16 )
    {//over 1 sector size 0x1000
      GVTAEventData.u8SFlashSectorCount++;
      uint8_t sFlashBuffer[256];                 //TACS_Stm_Vx.0.21_UDP_190904.bin 
      xERASE_ExFlash4kSector (FLASH_EventHisory_Start+((GVTAEventData.u8SFlashSectorCount-1)%32)*0x1000);//지우고..
      for(int i =0; i<16 ; i++)
      {
        EEPROMGetDataBlock(EEPROM_ADDRESS_EventStart+i*256, sFlashBuffer, 256);//eeprom 16개 page 모두 버퍼로 복사..
        xWRITE_DataToExNorFlash(FLASH_EventHisory_Start+((GVTAEventData.u8SFlashSectorCount-1)%32)*0x1000+i*256,sFlashBuffer,256);
      }
      GVTAEventData.u8EEPromPageCount = 0;
      EEPROMSetData(EEPROM_ADDRESS_FlashSectorCount,GVTAEventData.u8SFlashSectorCount);
    }
    EEPROMSetData(EEPROM_ADDRESS_EEPromPageCount, GVTAEventData.u8EEPromPageCount);
    GVTAEventData.u8LogCount = 0;
  }
  EEPROMSetData(EEPROM_ADDRESS_EventCount, GVTAEventData.u8LogCount);
}

void RestoreEventLogFromEEpromORsFlash(void)
{
/*typedef struct  {
  uint8_t u8SFlashSectorCount;  //sFlash Sector Size 0x1000 4096
  uint8_t u8EEPromPageCount;    //eeprom Page size   0x100  256
  uint8_t u8LogCount;
  uint8_t pU8EventLog[256];//16bytes * 16 log..
}TA_EventData;*/
  if(GVTAEventData.u8EEPromPageCount > 0)
  { //eeprom counter 확인하고..
    GVTAEventData.u8EEPromPageCount--;
    GVTAEventData.u8LogCount = 16;
    EEPROMGetDataBlock(EEPROM_ADDRESS_EventStart+GVTAEventData.u8EEPromPageCount*0x100, GVTAEventData.pU8EventLog, 0x100);// 한 page 를 읽어들인다..
  }
  else if(GVTAEventData.u8SFlashSectorCount > 0)
  {  //sflash counter도 확인해서..
    uint8_t sFlashBuffer[4096];
    GVTAEventData.u8SFlashSectorCount--;
    xREAD_DataFromExNorFlash(FLASH_EventHisory_Start+GVTAEventData.u8SFlashSectorCount*0x1000,sFlashBuffer,0x1000);//flash 1sector를 읽어서...
    EEPROMSetDataBlock(EEPROM_ADDRESS_EventStart, sFlashBuffer, 0x1000);//eeprom 16개 page에 복사..
    GVTAEventData.u8EEPromPageCount = 15;
    GVTAEventData.u8LogCount = 16;
    EEPROMGetDataBlock(EEPROM_ADDRESS_EventStart+0xF00, GVTAEventData.pU8EventLog, 0x100);// 한 page 를 읽어들인다..
  }
}

/*
카드 종류
0: 'M' 마스터카드 - 어떤 시간이라도 모든 출입문을 출입할 수 있는 카드..
1: 'C' 상시출입카드 - 특별카드timecode 에 지정된 시간을 제외한 시간에 출입이 가능.
2: 'P' 단기(방문)카드 - 임시카드. 기간이 지정되어 있는카드..
3: 'F' 당일카드 - 임시카드와 같지만 기간이 보통 당일로만 되어 있음..
4: 'L' 출입시간제한카드 - 출입시간제한timecode 에 설정된 시간만 출입이 가능
5: 'S' 특별카드 - 특별카드 timecode에 설정된 시간외에도 출입이 가능 (마스터카드를 제외하고 나머지는 그 시간외에는 출입 못함)
6: 'D' 미정..
7: 'A' 출입지정카드 - Appointed Card..해당 출입문에 지정한 시간대에 출입할 수 있는 카드.. 이건 좀 다르게 관리해야 함.. 개인별로 출입문마다 개별 저장해야 함..
*/

/*
  GVTAEventData.pU8EventLog[16];
  2A  length  0 //carddata 0x2A, no carddata 0x1A
  30	data-status 1 '0'
  4343  data-event종류  2
  00	data-event내용1 4
  04	data-event내용2 5
  0F0B01000927	data-time 6
  3824A75C	data-ID 12
*/

const uint8_t u8ArrCardCode[8] = {'M', 'C', 'P', 'F', 'L', 'S', 'D', 'A'};

uint8_t CheckEventCardAndLog( uint8_t nPort, uint8_t * u8CardData, uint16_t u16CardResult)
{
  uint8_t nResult = 0xff, u8DoorAuth, u8CardType = 0, u8ArrLogdata[16]={0x2a,'0','C',0,0,0,0,0, 0,0,0,0,0,0,0,0};
                                     //  memcpy(u8ArrLogdata,"\x2a\x30\x43",3);
  uint8_t nAutho;

  _STATE_LED_TOGGLE();   // LED On
 // MyMsDelay(500); // 500 mS
#ifdef ALLCARD_GRANT_MODE
  ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
#endif
// Door Auth(8), antiPass(1), cardType(3), resultvalue(4) == 검색결과(0있음, 1기간지났음, 2이미존재?(쓸 때), 3없음)
  if((u16CardResult&0x000F) == Card_Registerd_OK )
  {  //Card_Registerd_OK 0
    u8DoorAuth = (uint8_t) (u16CardResult >> 8);
//    if(LockParameter.SelectDoor[nPort] & u8DoorAuth)   // 락과 연동된 출입문 번호의 권한을 체크
    uint8_t nauth = (0x80 >> nPort);
    if(nauth & u8DoorAuth)   // 락과 연동된 출입문 번호의 권한을 체크
    {	//배정된 출입문에 해당 권한이 있으면 아래 진행.. SelectDoor 에는 해당 입력포트(485id, wiegand port)에 대응하는 lock 번호가 저장되어 있음..
//        if(u16CardResult & (0x8000 >> nPort)) {//해당 출입문에 출입권한이 있으면..8개

      u8CardType = (uint8_t)((u16CardResult>>4)&0x07); // ???? ???? ?111 ???? 이기 때문에..
      u8ArrLogdata[3] = u8ArrCardCode[u8CardType];//{'M', 'C', 'P', 'F', 'L', 'S', 'D', 'A'};

      printf( "CheckEventCardAndLog  Card_Registerd_OK u16CardResult=%04X, u8DoorAuth=%02X,  u8CardType=%02X", u16CardResult, u8DoorAuth, u8CardType);
      switch(u8CardType)
      {		//	u16CardResult&0x0070) {
        case 0: 
		{	//0x0000: {//0000 0000 ?000 0000 종류 : 마스터카드(0)
          ReleaseLockSelectDoor(CMSParameter.DoorOpenTime[nPort], nPort, u8DoorAuth);
//              ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
          u8ArrLogdata[4] = 0x00; //data-event내용1 정상
          break;
        }
        case 1: //0x0010 : {//0000 0000 ?001 0000 종류 : 상시카드(1)
        case 2: //0000 0000 ?010 0000 종류 : 단기(방문)카드, 임시카드(2)
        case 3: //0000 0000 ?011 0000 종류 : 당일카드(3)
        case 4:
        {	//0000 0000 ?100 0000 종류 : 출입시간제한카드(4)
          INT8U nWeek, nCode, i=0, u8HaveTimeCode=0;
          nWeek = (gU8HolidayFlag)?7:toDayTime.WeekDAY;//공휴일 플래그가 걸리면 공휴일 7번으로, 아니면 현재 요일로..
          for(; i < 3; i++)
          {
            nCode = CMSParameter.SpecialCardTimeCode[i][nPort];//새끼줄 값 가져와서..
            if(nCode > 0)
            {//모든카드를 특별카드 타임코드 지정되어 있으면 시간비교를 한다.
              u8HaveTimeCode=10;
              if(CheckInSchedule(ScheduleParameter[nPort][nCode][nWeek].StartHh, ScheduleParameter[nPort][nCode][nWeek].StartMm,
                                 ScheduleParameter[nPort][nCode][nWeek].EndHh,   ScheduleParameter[nPort][nCode][nWeek].EndMm))
              {
                i=10;//for 문 빠져 나갈 수 있도록..
              }
              else
              {
                u8ArrLogdata[4] = 0x0E; //data-event내용1 - 출입시간아님.
              }
            }
            else
            {//특별카드 타임코드가 없다면
              u8HaveTimeCode=0;
            }
          }//end for
          
          if((u8HaveTimeCode > 0) && (i < 10))
        	  break; //특별카드 타임코드가 적용되어 있는데 범위 안에 들어가 있다면 아래 루틴을 들어가고.. 안들어가 있으면 출입시간 아님 처리..
          
          switch(LockParameter.DoorLockMode[nPort])
		  {
            case LockMode_Manual_Unlock ://0x10
              u8ArrLogdata[4] = 0x0B; //data-event내용1 수동풀림상태
              break;
            case LockMode_Schedule_Unlock ://0x20
              u8ArrLogdata[4] = 0x09; //data-event내용1 스케줄풀림상태
              break;
            case LockMode_Manual_Lock ://0x11
              u8ArrLogdata[4] = 0x0C; //data-event내용1,  0x0C: 수동잠금 해제권한없음
              break;
            case LockMode_Normal ://0x01
            case LockMode_Schedule_Lock ://0x21

//              if(u8CardType != 4)
			  { //종류 : 출입시간제한카드(4)가 아니라면..
                ReleaseLockSelectDoor(CMSParameter.DoorOpenTime[nPort], nPort, u8DoorAuth);
//                    ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
                u8ArrLogdata[4] = 0x00; //data-event내용1 정상
                break;
              }
/*              else
			  {        //출입시간제한카드(4) 일 때 타임코드를 체크해서 출입시간이면 열어줌..

                 for(i=0; i<3; i++)
				 {
                  nCode = CMSParameter.LimitCardTimeCode[nPort][i];//새끼줄 값 가져와서..
                  if(nCode > 0)
				  {//출입시간제한카드가 출입시간타임코드가 있으면 시간비교를 한다.
                    if(CheckInSchedule(ScheduleParameter[nPort][nCode][nWeek].StartHh, ScheduleParameter[nPort][nCode][nWeek].StartMm,
                                       ScheduleParameter[nPort][nCode][nWeek].EndHh,   ScheduleParameter[nPort][nCode][nWeek].EndMm))
					{
                      ReleaseLockSelectDoor(CMSParameter.DoorOpenTime[nPort], nPort, u8DoorAuth);
//                          ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
                      u8ArrLogdata[4] = 0x00; //data-event내용1 - 정상
                      i=10;//for 문 빠져 나갈 수 있도록..
                    }
                    else
					{
                      u8ArrLogdata[4] = 0x0E; //data-event내용1 - 출입시간아님.
                    }
                  }
                  else  
				  {//타임코드가 없다면 출입이 안됨..
                    u8ArrLogdata[4] = 0x0E; //data-event내용1 - 출입시간아님.
                  }
                }//end for
 //             }
  * */
              break;
            case LockMode_NotUse ://0x00
            case LockMode_Unknown ://0xFF
            default:
              u8ArrLogdata[4] = 0x0F; //data-event내용1,  0x0F: 알수없음(F)
              break;
          }
          break;
        }
        case 5 :
        {//0000 0000 ?101 0000 종류 : 특별카드(5)
          switch(LockParameter.DoorLockMode[nPort])
          {
            case LockMode_Manual_Unlock ://0x10
              u8ArrLogdata[4] = 0x0B; //data-event내용1 수동풀림상태
              break;
            case LockMode_Schedule_Unlock ://0x20
              u8ArrLogdata[4] = 0x09; //data-event내용1 스케줄풀림상태
              break;
            case LockMode_Manual_Lock ://0x11
              u8ArrLogdata[4] = 0x0C; //data-event내용1,  0x0C: 수동잠금 해제권한없음
              break;
            case LockMode_Normal ://0x01
            case LockMode_Schedule_Lock ://0x21
              ReleaseLockSelectDoor(CMSParameter.DoorOpenTime[nPort], nPort, u8DoorAuth);
//                  ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
              u8ArrLogdata[4] = 0x00; //data-event내용1 정상
              break;
            case LockMode_NotUse ://0x00
            case LockMode_Unknown ://0xFF
            default:
              u8ArrLogdata[4] = 0x0F; //data-event내용1,  0x0F: 알수없음(F)
              break;
          }
          break;
        }
        case 6 :
        {//0000 0000 ?110 0000 종류 : 미정카드(6)//아직 없음..
        }
        case 7 :
        {//0000 0000 ?111 0000 종류 : 지정카드(7)//지정
        }
        default :
        {
          u8ArrLogdata[4] = 0x0F; //data-event내용1,  0x0F: 알수없음(F)
          break;
        }
      }
      nResult = Result_OK;
    }
    else
    {  // acu 에 카드값은 있으나 출입문 권한이 없음..
       printf( "CheckEventCardAndLog  LockParameter.SelectDoor[%02ㅌ]=%02X, u8DoorAuth=%02X\r\n", nPort, LockParameter.SelectDoor[nPort], u8DoorAuth);
       printf( "CheckEventCardAndLog  Card_No_Autho u16CardResult=%04X, u8DoorAuth=%02X,  u8CardType=%02X", u16CardResult, u8DoorAuth, u8CardType);
      u8ArrLogdata[3] = 'X';//0x58; //'X' 권한없음.. "CX"
      u8ArrLogdata[4] = 0x0D; //출입 권한없음..
#ifdef ALLCARD_GRANT_MODE
    nResult = Result_OK;
#else
	#ifdef GUUI_PORT_GRANTED
		ReleaseLockSelectDoor(CMSParameter.DoorOpenTime[nPort], nPort, u8DoorAuth);
	//              ControlDoor(CMSParameter.DoorOpenTime[nPort], nPort);
		 u8ArrLogdata[4] = 0x00; //data-event내용1 정상
	     nResult = Result_OK;
	 	#else
		 nResult = Result_Wrong_Zone;
	#endif
#endif
    }
  }
  else if((u16CardResult&0x000F) == Card_No_Granted )
  {  //Card_No_Granted 1
    printf( "CheckEventCardAndLog  Card_No_Granted u16CardResult=%04X, u8DoorAuth=%02X,  u8CardType=%02X", u16CardResult, u8DoorAuth, u8CardType);
    u8ArrLogdata[3] = (u8CardType == 2) ? 'P':'F'; //'P' 임시,단기(방문), 'F' 당일..
    u8ArrLogdata[4] = 0x06; //data-event내용1,    0x06 : "-출입기간X"
#ifdef ALLCARD_GRANT_MODE
    nResult = Result_OK;
#else
    nResult = Card_No_Granted;
 #endif
  }
  else
  {    //Card_Not_Registerd 3
	printf( "CheckEventCardAndLog  Card_Not_Registerd u16CardResult=%04X, u8DoorAuth=%02X,  u8CardType=%02X", u16CardResult, u8DoorAuth, u8CardType);
#ifdef ALLCARD_GRANT_MODE
    u8ArrLogdata[3] = 'C';// //'C' 등록..
    u8ArrLogdata[4] = 0x00; //data-event내용1 - 정상
    nResult = Result_OK;
#else
    u8ArrLogdata[3] = 'N';//0x4E; //'N' 미등록..
    u8ArrLogdata[4] = 0x00; //data-event내용
    nResult = Card_Not_Registerd;
#endif

  }

  u8ArrLogdata[5] = nPort; //data-event내용2  출입문 번호 0~3
  CopyYYMMDDhhmmss(u8ArrLogdata + 6);
  memcpy(u8ArrLogdata+12, u8CardData, 4);
  
  int nDelayCnt = 0;
  if(gU8LogCountFlag > 0)
  {
    while(1){//다른데서 쓰고 있는중이면 멈춤...OS 설치 시 필요. RTOS 에서 오버랩 시켜야,.
      if(nDelayCnt++ > 10000)
      {
        gU16nDelayCnt_Count++;
        break;
      }
    }
  }

  printf( "CheckEventCardAndLog  GVTAEventData=%s\r\n", u8ArrLogdata);

//      gU8LogCountFlag = 1;//멈춤 설정..
  memcpy(&GVTAEventData.pU8EventLog[GVTAEventData.u8LogCount*16], u8ArrLogdata, 16);
  SaveEventLog2EEprom_sFlash();//2. log count는 이 함수에서 올림..
//      gU8LogCountFlag = 0;//멈춤 해제..

//	if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속
	{
		HTTP_MakeHttpPacketRtLog( nPort, HTTP_EVENT_ACCESS_CODE, HTTP_VERIFYTYPE_CARD , nResult);
	}
		// 소켓통신 TCP/IP 인터페이스로 접속
	{
	    if(TASocketConnected > 0)
	    {// 커넥트 되어 있고 패킷전송할게 있으면.. 업뎃중이면 안해야 하지 않을까?
			nDelayCnt = 0;
			if(GVTAComData.SetPacket == TRUE)
			{
				while(1)
				{//패킷 보내는 중이면.. 잠시 멈춤
				  if(nDelayCnt++ > 10000)
				  {
					gU16nDelayCnt_Count++;
					break;
				  }
				}
			}
			TAMakeResponsePacket(TA_StrCmdAckEvent);//  //  ("EA")  //  EVENT ping Ack.. EB �޽����� ���������..TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
		  }
	  }

	  _STATE_LED_TOGGLE();    // LED On
	  return nResult;
}
          
void CopyYYMMDDhhmmss(uint8_t *u8ArrTarget)
{
  u8ArrTarget[0] = toDayTime.YEAR;
  u8ArrTarget[1] = toDayTime.MON;
  u8ArrTarget[2] = toDayTime.DATE;
  u8ArrTarget[3] = toDayTime.HOUR;
  u8ArrTarget[4] = toDayTime.MIN;
  u8ArrTarget[5] = toDayTime.SEC;
}

bool CheckInSchedule(uint8_t u8StartHh, uint8_t u8StartMm, uint8_t u8EndHh, uint8_t u8EndMm)
{
  INT16U nSchedStartMin, nSchedEndMin, nCurrTimeMin ;
  nSchedStartMin = (u8StartHh*60) + u8StartMm;//시작시간을 분으로..
  nSchedEndMin =   (u8EndHh*60)   + u8EndMm;//종료시간을 분으로...
  if (nSchedStartMin == nSchedEndMin)
  {//시작시간과 종료시간이 같으면.. 잠김시간, 스케줄 안쓴다는..
    return false;//스케줄에 들어가지 않음..
  }
  else
  {
    nCurrTimeMin =   (toDayTime.HOUR*60)+toDayTime.MIN; //현재시간을 분으로..
    
    if(( (nSchedStartMin < nSchedEndMin) &&
    	 (nCurrTimeMin >= nSchedStartMin && nCurrTimeMin <= nSchedEndMin) ) ||
       ( (nSchedStartMin > nSchedEndMin) &&
         (nCurrTimeMin >= nSchedStartMin ||  nCurrTimeMin <= nSchedEndMin) ))
    {
      return true;//스케줄에 들어감..
    }
    else
    {//스케줄 안에 들어와 있지 않다면..
      return false;//스케줄에 들어가지 않음..
    }
  }
}

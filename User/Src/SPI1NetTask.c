/******************************************************************************
**
**   CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: SPI1Task.c
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
#define    SPI1_GLOBALS

#include   "SPI1NetTask.h"
#include "Version_Plus.h"

const uint16_t con16JulianDateArray[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};//365
                                //      31,+28,+31, +30, +31, +30, +31, +31, +30, +31, +30,  +31  // 占쏙옙占쏙옙 占싹쇽옙..

INT8U udp_send_buffer[UDP_SEND_BUFFER_SIZE]={'\0'};    /**< Send Buffer */
INT8U udp_recv_buffer[UDP_RECV_BUFFER_SIZE]={'\0'};    /**< Receive Buffer */

INT8U  bcast_addr[4];
INT8U  recv_addr[4];
INT16U recv_port;

INT16U RSR_len = 0;
INT16U SSD_len = 0;

// #define AUTO_SEND_EVENT
#define SIZE_OF_TOKEN_EVENT 128
#define OS_Delay	MyMsDelay

#define DELETE_OPERATION

extern _HTTPC_GetPostTypeDef HTTP_RestCmd[MAX_NUM_DOOR];
extern int32_t TCPS_Handler(uint8_t SockNum);
//jsmn_parser jParser;
//jsmntok_t tokens[SIZE_OF_TOKEN_EVENT]; // a number >= total number of tokens

extern TIM_HandleTypeDef htim6;
extern uint16_t t50msTim6_Net;
extern uint16_t t50msTim6_Tcp;
/******************************************************************************
**
******************************************************************************/
void SPI1NetTask( void)
{
	uint32_t checkCrc = 0;
	static uint16_t nNetTaskCycle = 0;

	UdpServerProcess();

    // 타이머는 증가만 하고 Http 네트웍이 연결이 안되면 SPI가 딜레이로 기다리게 된다. 연결되면 1초 이내로 통신 가능.
	if( (t50msTim6_Net < 20) || (t50msTim6_Net > 60) )
	{ /// 1 sec ~ 3 sec 네트웍이 연결 되어 씨는 지 아닌 지 확인.
		t50msTim6_Net = 0;
		HTTPClient_Handler();
	}

	TCPS_Handler(Socket_MANG);
	if( (fOpenEthernet == 0) && (t50msTim6_Tcp > 5000) )
	{ // 연결되었었는데 통신 안된 상태가 10초 이상이면 리셋으로 다른 연결을 받아 드림.
		t50msTim6_Tcp = 0;
   	    OPEN_Ethernet();
	}

	NetworkAlertProcess();

    TALEDControl();

    if( bRequestNeed == MON_NEED_SPI1_FWUD) 
	{//업뎃완료후.. case  FWD_TYP_END:    // (7)
		  checkCrc = getExtFlashCRC( FIRM_START_NEW_ADDR, ulBinLen );
		  if( checkCrc == ulBinCrc)
		  {
			  WriteUpdateInfoFlag( UPDATE_FROM_NEW, UPDATE_NEED );
			  rprintf("[NOTICE]System Reset for Upgrade!\r\n");
			  NVIC_SystemReset();  // 맞으면.. System Reset
		  }
		  else
		  {
			  bRequestNeed = MON_FROM_USER;
			  rprintf("[NOTICE]Checksum Error!\r\n");
		  }
		  Firmware_Upgrade = USER_UPGRADE_NONE;
		//      OS_ResumeAllSuspendedTasks();
		/*
		*/
    }
    else 
	{
#ifdef DEBUG_MODE        
      if (nNetTaskCycle > 50000)
	  {  // 100 mS * 50000 = 5000 Sec
    	  nNetTaskCycle = 0;
 //   	  if(httpc_isConnected)
 //   		  HTTP_TestHttpPacketRtLog();
      }
      else
	  {
        nNetTaskCycle++;
      }
#endif
      if( Firmware_Upgrade == SPI1_UPGRADE_YES)
	  {
        ftimeSpiOut++;
        if( ftimeSpiOut > 5000)
		{//2000) { /// About 20 seconds
          bRequestNeed = MON_FROM_USER;
          rprintf("[NOTICE]Firmware Update Waiting Time is over!\r\n");
          Firmware_Upgrade = USER_UPGRADE_NONE;
 //         OS_ResumeAllSuspendedTasks();
        }
	  }
    }
    
    if( Firmware_Upgrade != SPI1_UPGRADE_YES)
    {
        TaskRunCnt(ENUM_SPI1NET_TASK);          // Notice Activated.  20150210 Jylee
    }
    
#ifdef DEBUG_MODE        
    if ((nNetTaskCycle % 2000) == 0) 
    {  // 100 mS * 10000 = 1000 Sec               
//        rprintf(" SPI1 Net Task is Alive.\r\n");
//        RESTClient_SetPacketReady( UHS_Type_ConsoleTest100);
    }        
#else
//    IWDG_ReloadCounter();
#endif    
}

/******************************************************************************
**  TA LED Controller
******************************************************************************/
void TALEDControl(void)  
{
    static uint8_t netLEDCounter = 0;
    if( netLEDCounter == 250)
	{
    	_CAN_LED_TOGGLE();      // LED OnOff
        netLEDCounter = 0;
    }
    else  
	{
        netLEDCounter++;
    }
}

/******************************************************************************
**  Ehternet / Wifi Communication Process(ADT Plus, I-CMS)
******************************************************************************/
void EthernetCommunication(void)  
{
	//  static INT16U TA_Port = 0;
	static INT8U EthernetSetDelay;
	static INT16U EthernetSetCounter;

	//  INT8U i = 0;
//	nNetPartMode = PUSH_CLIENT;
	UDP_Step = UDP_Start;

	SSDPSendReadyFlag = FALSE;
	TASocketConnected = FALSE;
	UHSSocketConnected = FALSE;

	HttpcSubModeStep = 0;
	for( uint8_t nf = 0; nf < MAX_NUM_DOOR; nf++)
		HTTP_RestCmd[nf].HttpcMainModeStep = UHS_Type_ReqDevConnection;  // cdata 전송

	EthernetSetDelay = FALSE;
	HAL_TIM_Base_Start_IT(&htim6);

	rprintf("Network Start OK\r\n");
	ReadMacAddress();

	EthernetSetCounter = 0;
	EthernetSetDelay = TRUE;
	NetworkFailAlertFlag |= NetworkFail_Internet;
}
 
INT8U UHSPacketCheck(void)
{
	INT16U Index;
	INT16U Length;
	INT8U SourceData[16];
	INT8U DecryptData[16];
	int resultCode;
	Index = 0;
	
	rprintf("UHS: Check Packet. \r\n");
//  halt after print buffer 
//    GPIO_SetBits(LED_CALM_PORT, LED_CALM_PIN);

        // Http JSON Parsing
 /*
    resultCode = jsmn_parse(&jParser, UHSRxPacket, Internet_UHSBuf_Size, tokens, SIZE_OF_TOKEN_EVENT);
        
 		if (resultCode < 0) { 
 			return 3; 
 		} else { 
                int ni;
                char keyString[128];    
                for ( ni = 0; ni<SIZE_OF_TOKEN_EVENT; ni++)
                {
                    jsmntok_t this_key = tokens[ni];

                    if( this_key.size > 0)
                    {
                        if( this_key.type == JSMN_STRING)
                        {
                            unsigned int length = this_key.end - this_key.start;

                            memcpy(keyString, &UHSRxPacket[this_key.start], length);
                            keyString[length] = '\0';
                            rprintf("Json Key %d: %s\r\n", this_key.type, keyString);
                        
                            rprintf("Json Dat: %s\r\n", jsmn_get_value( UHSRxPacket, &this_key));
                        }
                    }
                }
    
// 			dump(UHSRxPacket, tokens, jParser.toknext, 0); 
 			return 1; 
 		} 
   */ 

	return TRUE;
}

/******************************************************************************
**  Network Alert Process
******************************************************************************/

void NetworkAlertProcess(void)
{
  static INT16U NetworkCheckTimer = 0;
  if(NetworkCheckTimer++ > 10000)  //every 2 seconds
  {
      NetworkCheckTimer = 0;

      if(NetModuleCheck() == FALSE)  // 현재는 무조건 TRUE
      {
        if(!(NetworkFailAlertFlag & NetworkFail_SIP))
        {
          NetworkFailAlertFlag |= NetworkFail_SIP;          
          rprintf("Network Check: Error Flag Set 1!(Module Error)\r\n");
        }
      }

      if( EthernetLinkCheck() == FALSE)  //
      {
        if(!(NetworkFailAlertFlag & NetworkFail_Link))
        {
          NetworkFailAlertFlag |= NetworkFail_Link;
          rprintf("Network Check: Error Flag Set 1!(Link Error)\r\n");
        }
       }

      if( NetRJ45Check() == FALSE)  // 포트 잭이 연결이 안되어 있거나 빠진 경우
      {
          rprintf("NetRJ45Check Check: Error Flag Set OPEN_Ethernet.\r\n");
       	    OPEN_Ethernet();
            NetworkFailAlertFlag |= NetworkFail_Internet;
      }
   }
}

BOOLEAN NetModuleCheck(void)
{
        return TRUE;
}


BOOLEAN NetRJ45Check(void)
{
    static uint8_t  ConnDelay = 0;

    if( EthernetRJ45Check() == FALSE)
    	ConnDelay++;
    else
    	ConnDelay = 0;
    if(ConnDelay > 1)
    {
    	ConnDelay = 0;
        return FALSE;
    }
    return TRUE;
}
/******************************************************************************
****  TA Packet Receive Functions  **
******************************************************************************/
INT8U TAPacketCheck(void)
{
  if(TABuf_Rx[TA_INDEX_STX] == TA_MSGE_STX)
  {
    INT16U Length = 0, CrcXor = 0;
    Length = TABuf_Rx[TA_INDEX_LENH] * 256 + TABuf_Rx[TA_INDEX_LENL];
    if((Length + 2) > Internet_TABuf_Size)
    {
//      rprintf("Packet Check: Error length %02X.\r\n", Length);
      return FALSE;
    }
    else if((TABuf_Rx[TA_INDEX_CMD1] < 'A')||(TABuf_Rx[TA_INDEX_CMD1] > 'Z'))
    { // 0x5A == 'Z' 占쏙옙占쏙옙占쏙옙 CMD2占쏙옙 체크占쌔억옙 占싹놂옙..
//      rprintf("Packet Check: Error ControlHi Code %02X.\r\n", TABuf_Rx[TA_INDEX_CMD1]);
      return FALSE;
    }
    else
    {
      for(INT16U i = 2; i<Length; i++)
      {
        CrcXor ^= TABuf_Rx[i];
      }
      if(TABuf_Rx[Length+1] == CrcXor)
        return TRUE;
      else
        return FALSE;
    }
  }
//  rprintf("Packet Check: Error STX %02X.\r\n", TABuf_Rx[0]);
  return FALSE;
}

INT16U NetMessageCheck(byte *RxBuffer )
{
	INT16U nId = 0;
	INT16U ni = 0;
    char msgid[10] = {0};
    
	for( ni=0; ni<Internet_TABuf_Size; ni++)
    {
       if( (RxBuffer[ni+0] == 'm') && (RxBuffer[ni+1] == 's') && (RxBuffer[ni+2] == 'g') && (RxBuffer[ni+3] == 'i') && (RxBuffer[ni+4] == 'd') )
        {
            msgid[0] = RxBuffer[ni+6]; msgid[1] = RxBuffer[ni+7];  msgid[2] = RxBuffer[ni+8];  msgid[3] = 0;
            break;
        }
        if( (RxBuffer[ni+0] == 'H') && (RxBuffer[ni+1] == 'T') && (RxBuffer[ni+2] == 'T') && (RxBuffer[ni+3] == 'P') && (RxBuffer[ni+4] == '/') )
        {
            break;
        }
	}
    
    sscanf( msgid, "%d", &nId);
    rprintf("Message Check: ID %d.\r\n", nId);

	return nId;
}

void TAMakeServerPacking(void)  // 4C47
{
 	GVTAComData.length = (uint16_t)strlen(TABuf_Tx);
	GVTAComData.SetPacket = TRUE;
 	rprintf( "[SVR]Set %d -> ", GVTAComData.length);
}

void TAPacketAnalysis(void)
{
  INT16U ProtocolType = 0xFFFF;
  INT8U ReceiveTime[7];
  INT8U nResult = 0;
  
  unionCard_Parameter TcpCardParam;        // 39

  t50msTim6_Tcp = 0;  // 계속 통신이 되고 있으면 새로 시작. 10초가 지나면 TCP서버를 리셋안다.

  ProtocolType = (INT16U)((TABuf_Rx[TA_INDEX_CMD1]*256)+TABuf_Rx[TA_INDEX_CMD2]);
  GVTAComData.GetPacket = TRUE;
      
  switch(ProtocolType)
  {
    case TA_StrCmdCheckAcuStatus:
    { // 0x4141  //  ("AA")  //  ACU의 통신 상태 확인
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
        TABuf_Tx[TA_INDEX_DAT+1] = gU8SensorValue;  //1234 5678
        TABuf_Tx[TA_INDEX_DAT+2] = gU8ConsoleAlive; //
        TABuf_Tx[TA_INDEX_DAT+3] = gU8DoorOpenedTooLong;  //1234 5678
        TABuf_Tx[TA_INDEX_DAT+4] = TACheckJumperSetting(); //1234 0000 나중에 쓸지 모름..
//        memcpy(TABuf_Tx+TA_INDEX_DAT+5, gReaderMsgSeq, 4);//의미없는 듯 한데..
        break;
    }

    case TA_StrCmdSetAcuTime:
    {      // 0x4142  //  ("AB")  //  ACU의 시간설정.
      ReceiveTime[0] = (TABuf_Rx[TA_INDEX_DAT+0]  >99)?99:TABuf_Rx[TA_INDEX_DAT+0];  // 년(1)   : 0x09 -> 09년
      ReceiveTime[1] = ((TABuf_Rx[TA_INDEX_DAT+1] == 0)||(TABuf_Rx[TA_INDEX_DAT+1] >12))? 1:TABuf_Rx[TA_INDEX_DAT+1];  // 월(1)   : 0x0b -> 11월
      ReceiveTime[2] = ((TABuf_Rx[TA_INDEX_DAT+2] == 0)||(TABuf_Rx[TA_INDEX_DAT+2] >31))? 1:TABuf_Rx[TA_INDEX_DAT+2];  // 일(1)   : 0x0b -> 11일
      ReceiveTime[3] = (TABuf_Rx[TA_INDEX_DAT+4]  >23)? 0:TABuf_Rx[TA_INDEX_DAT+4];  // 시(1)    : 0x0b -> 11시
      ReceiveTime[4] = (TABuf_Rx[TA_INDEX_DAT+5]  >59)? 0:TABuf_Rx[TA_INDEX_DAT+5];  // 분(1)    : 0x0b -> 11분
      ReceiveTime[5] = (TABuf_Rx[TA_INDEX_DAT+6]  >59)? 0:TABuf_Rx[TA_INDEX_DAT+6];  // 초(1)    : 0x0b -> 11초
      ReceiveTime[6] = (TABuf_Rx[TA_INDEX_DAT+3]  > 6)? 0:TABuf_Rx[TA_INDEX_DAT+3];  // 요일(1) : 일(0x00) 월(0x01) 화(0x02) 수(0x03) 목(0x04) 금(0x05) 토(0x06).
      
   //   SetToDay(ReceiveTime);
  	  RTC_SetTimeAndDate( &toDayTime, &ReceiveTime[0] );
      EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime, (INT8U*)ReceiveTime, 7);
//      rprintf( "[TA]->Time 20%02x.%02x.%02x %02x:%02x:%02x(%x)\r\n",
//             toDayTime.YEAR, toDayTime.MON, toDayTime.DATE, 
//             toDayTime.HOUR, toDayTime.MIN, toDayTime.SEC, toDayTime.WeekDAY);
      TABuf_Tx[TA_INDEX_DAT+0] = '0';//상태.. 0x30
      TABuf_Tx[TA_INDEX_DAT+1] = VS_VERSION;
      TABuf_Tx[TA_INDEX_DAT+2] = VS_STABLE;
      TABuf_Tx[TA_INDEX_DAT+3] = VS_DEVELOPMENT;
      TABuf_Tx[TA_INDEX_DAT+4] = gU16EEpromFailCount/0x100;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...// 0x4142  //	("AB")
      TABuf_Tx[TA_INDEX_DAT+5] = gU16EEpromFailCount%0x100;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...// 0x4142  //	("AB")
      TABuf_Tx[TA_INDEX_DAT+6] = gU16sFlashFailCount/0x100;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...// 0x4142  //	("AB")
      TABuf_Tx[TA_INDEX_DAT+7] = gU16sFlashFailCount%0x100;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...// 0x4142  //	("AB")

      break;
    }

    case TA_StrCmdCheckAcuTime:
    {   // 0x4143  //  ("AC")  //  ACU의 시간설정 확인.. 안씀..
      TABuf_Tx[TA_INDEX_DAT+0] = '0';           
      CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_DAT+1);
      break;
    }

    case TA_StrCmdResetAcu:
    {       // 0x4144  //  ("AD")  //  ACU의 초기화.
      rprintf("[NOTICE]System Reset for initializing!\r\n");
      EEPROMSetData(EEPROM_ADDRESS_SystemFlag, 't');//"IntelivS"로 써넣기..if( memcmp(u8SystemFlag, DefaultSystemFlag, 8) != 0){//
      //[22] 부터 지우면 ip정보 바꾸지 않음.. 다른 문자로 바꾸면 모조리 지움..
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      OS_Delay(500); // 500 mS
      NVIC_SystemReset();  // System Reset
      break;
    }

    case TA_StrCmdGetAcuInfo:
    {      // 0x4145  //  ("AE")  //  ACU의 정보 확인.
      uint8_t prodSn[10];
      EEPROMGetDataBlock( EEPROM_ADDRESS_SiteInfo, prodSn, 8);
      memcpy( (char *)(TABuf_Tx+TA_INDEX_DAT), (const char *)"0SN:12345678/Date:20250225", 26);
      for(int ni = 0; ni < 8; ni++)
    	  TABuf_Tx[TA_INDEX_DAT+4+ni] = prodSn[ni];
      TABuf_Tx[TA_INDEX_DAT+21] = INT_ASCII_HI(toDayTime.YEAR);
      TABuf_Tx[TA_INDEX_DAT+22] = INT_ASCII_LO(toDayTime.YEAR);
      TABuf_Tx[TA_INDEX_DAT+23] = INT_ASCII_HI(toDayTime.MON);
      TABuf_Tx[TA_INDEX_DAT+24] = INT_ASCII_LO(toDayTime.MON);
      TABuf_Tx[TA_INDEX_DAT+25] = INT_ASCII_HI(toDayTime.DATE);
      TABuf_Tx[TA_INDEX_DAT+26] = INT_ASCII_LO(toDayTime.DATE);
     break;
    }

    case TA_StrCmdAcuReboot:
    {       //0x4146  //  ("AF")  //  ACU의 reboot
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      TAMakeResponsePacket( ProtocolType);  // 부팅하면 동신 안되므로 먼저 보내고...
      OS_Delay(500); // 500 mS
      NVIC_SystemReset();  // System Reset
      break;
    }

    case TA_StrCmdAcuInstallSite:
    {       //0x4147  //  ("AG")  //  ACU의 사이트값 설정
      gU8InstallSiteNumber = TABuf_Rx[TA_INDEX_DAT+0]; 
      EEPROMSetData(EEPROM_ADDRESS_InstallSiteNumber, gU8InstallSiteNumber);     //223
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }

    case TA_StrCmdAcuSetting :
    {//            0x4153  //  ("AS")  //  ACU Setting
      if(/*(g_bBeepValue != TABuf_Rx[TA_INDEX_DAT+0]) && */!(TABuf_Rx[TA_INDEX_DAT+0] > 1))
      {//기존값과 다르고 0,1 인 경우에..
        g_bBeepValue = TABuf_Rx[TA_INDEX_DAT+0]; 
        EEPROMSetData(EEPROM_ADDRESS_900mhz_buzzer, g_bBeepValue);      //Backup900Beep(g_bBeepValue);
        Rs232Set900BuzzerOnOff();
      }
      if(/*(g_nChannelValue != TABuf_Rx[TA_INDEX_DAT+1]) && */(TABuf_Rx[TA_INDEX_DAT+1] != 0) && ( TABuf_Rx[TA_INDEX_DAT+1] < 16))
      {//기존값과 다르고 1~0xf 인 경우에..
        g_nChannelValue = TABuf_Rx[TA_INDEX_DAT+1];
        EEPROMSetData(EEPROM_ADDRESS_900mhz_port, g_nChannelValue);  //Backup900Channel(g_nChannelValue);
        Rs232Set900Channel();
      }
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }

    case TA_StrCmdSetIDsg:
    {         // 0x4941  //  ("IA")  //  ID자료 개별 저장.
		memcpy(TcpCardParam.stParam.CardId, TABuf_Rx+TA_INDEX_DAT+12, 4);//TA_INDEX_DAT 7, 총 16바이트 카드데이터 중 카드 4바이트 방식은 0~11 : 0으로 채워서 옴
		if( TABuf_Rx[TA_INDEX_LENL] >= 48)
			memcpy(TcpCardParam.stParam.Pin, TABuf_Rx+TA_INDEX_DAT+28, 8);//TA_INDEX_DAT 7, 총 8바이트 pin 마지막에 추가
		else
			memcpy(TcpCardParam.stParam.Pin, "Untitled", 8);//TA_INDEX_DAT 7, 총 8바이트 pin 마지막에 추가

		//TA를 기준으로 cardData[2], cardData[3], {Door Auth(4), antiPass(1), cardType(3)}, u16SavedJulianDate(16)
		TcpCardParam.stParam.DoorAuth_CardKind = (TABuf_Rx[TA_INDEX_DAT+19]&0xf0)|((TABuf_Rx[TA_INDEX_DAT+16])&0x07);
		//■ 카드종류/도어/안티패스 형식 : 1111 도어, 1 안티패스, 111 종류
		if(((TcpCardParam.stParam.DoorAuth_CardKind&0x07) == 2)||((TcpCardParam.stParam.DoorAuth_CardKind&0x07) == 3))
		{ //일시 or 당일카드이면..
			//TABuf_Rx[TA_INDEX_DAT+16] //■ 종류/도어/안티패스 형식 : 1111 도어, 1 안티패스, 111 카드종류 [마스터(0) 상시ID(1), 일시ID(2), 당일카드ID(3), 출입제한카드(4)]
			uint16_t u16JulianDate; //MMDDYY
			//    const uint16_t con16JulianDateArray[13] = {0, 1:0, 2:31, 3:59, 4:90, 5:120, 6:151, 7:181, 8:212, 9:243, 10:273, 11:304, 12:334};//365
			u16JulianDate = (con16JulianDateArray[TABuf_Rx[TA_INDEX_DAT+26]] + TABuf_Rx[TA_INDEX_DAT+27])*100+TABuf_Rx[TA_INDEX_DAT+25];
			// 올해 julian date * 100 + YY
			TcpCardParam.stParam.PassCode[0] = u16JulianDate/0x100;//여기에 사용기한이 julian date 로 2바이트가 들어간다..
			TcpCardParam.stParam.PassCode[1] = u16JulianDate%0x100;//여기에 사용기한이 julian date 로 2바이트가 들어간다..
		}
		else
		{
			TcpCardParam.stParam.PassCode[0] = TABuf_Rx[TA_INDEX_DAT+0];//사용기한 자리에 지정카드용 ACU 번호가 두개 들어감
			TcpCardParam.stParam.PassCode[1] = TABuf_Rx[TA_INDEX_DAT+1];//사용기한 자리에 지정카드용 ACU 번호가 두개 들어감
		}
		//      CardParameter.stParam.Reserved   = 0;//TABuf_Rx[TA_INDEX_DAT+30];//보내오나 안씀.
		nResult = WriteRfCardDataToSerialFlash(TcpCardParam.u8strParam);
      /*
#define Card_Registerd_OK   0
#define Card_No_Granted    1
#define Card_Deleted_OK   0
#define Card_No_Space       1
#define Card_Already_Exist    2
#define Card_Not_Registerd    3
#define Card_Error_Registerd  4
*/
		TABuf_Tx[TA_INDEX_DAT+0] = nResult+0x30;
		break;
    }

    case TA_StrCmdSetIDblocksg :
    {   // 0x4942  //  ("IB")  //  ID자료 블럭 저장 (블럭을 통채로 대체하는 방식
      nResult = WriteRfCardDataBlockToSerialFlash(TABuf_Rx + TA_INDEX_DAT);       //TA_INDEX_DAT 7
      break;
    }

    case TA_StrCmdCheckIDsg :
    {   // 0x4943  //  ("IC")  //  ID자료 검색,  권한 체크
    	uint8_t wiegand[4];
    	memcpy( wiegand, &TABuf_Rx[TA_INDEX_DAT], 4);  //
    	uint16_t gu16CardResult = CheckRfCardDataInFlash( wiegand, 1);

        TABuf_Tx[TA_INDEX_DAT+0] = (uint8_t)(gu16CardResult & 0x0F)+0x30;
		TABuf_Tx[TA_INDEX_DAT+1] = (uint8_t)(gu16CardResult / 0x100);
		TABuf_Tx[TA_INDEX_DAT+2] = TcpCardParam.stParam.DoorAuth_CardKind;     // 0xF1
		TABuf_Tx[TA_INDEX_DAT+3] = TcpCardParam.stParam.PassCode[0];     //
		TABuf_Tx[TA_INDEX_DAT+4] = TcpCardParam.stParam.PassCode[1];     //
		memcpy( &TABuf_Tx[TA_INDEX_DAT+5], TcpCardParam.stParam.Pin, 8);
    	break;
    }
    
    case TA_StrCmdDeleteIDsg:
    {      // 0x4944  //  ("ID")  //  ID자료 개별 삭제.
      memcpy(TcpCardParam.stParam.CardId, TABuf_Rx+TA_INDEX_DAT+12, 4);//TA_INDEX_DAT 7, 0~11 : 0으로 채워서 옴
#ifdef DELETE_OPERATION
      nResult = DeleteRfCardDataOnSerialFlash(TcpCardParam.u8strParam, 0);
#endif
      TABuf_Tx[TA_INDEX_DAT+0] = nResult+0x30;
      break;
    }
    
    case TA_StrCmdDeleteIDAll:
    {  // 0x4945  //  ("IE")  //  ID자료 전체 삭제.
 //     EraseSFlash(FLASH_MpCardData_Start, FLASH_MpCardData_End);
   //== ClearCardDataOfSerialFlashAll();
      ClearRfCardIndexOnSerialFlash();  // 데이터를 전체 지우는 대신 인덱스를 전부 지운다.
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }
    
    case TA_StrCmdCheckIDCnt:
    {  //  0x4946  //  ("IF")  //  ID 전체 count.
    	uint32_t nCount = GetRfCardCountInFlash();
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      TABuf_Tx[TA_INDEX_DAT+1] = (uint8_t)((nCount >> 24) & 0xFF);
      TABuf_Tx[TA_INDEX_DAT+2] = (uint8_t)((nCount >> 16) & 0xFF);
      TABuf_Tx[TA_INDEX_DAT+3] = (uint8_t)((nCount >> 8)  & 0xFF);
      TABuf_Tx[TA_INDEX_DAT+4] = (uint8_t)((nCount >> 0)  & 0xFF);
      break;
    }

    case TA_StrCmdDeleteIDBlock:
    {// 0x494A  //	("IJ")  //  ID자료 특정블럭만 삭제.
      memcpy(NewCardParam.stParam.CardId, TABuf_Rx+TA_INDEX_DAT+12, 4); //TA_INDEX_DAT 7, 0~11 : 0으로 채워서 옴
      nResult = DeleteRfCardDataOnSerialFlash(NewCardParam.u8strParam, 1);
      TABuf_Tx[TA_INDEX_DAT+0] = nResult +'0';
      break;
    }

    case TA_StrCmdGetIDContent:
    {		// 0x494C  //	("IL")  //  ID자료 내용을 전달
    	uint8_t wiegand[4];
    	memcpy( wiegand, &TABuf_Rx[TA_INDEX_DAT], 4);  //
    	uint8_t u8CardCount = GetRfCardDataInFlash( wiegand, &TABuf_Tx[TA_INDEX_DAT+1]);

    	TABuf_Tx[TA_INDEX_DAT+0] = u8CardCount;
//    	 u8CardCount * SIZE_OF_RFCARD_DATA;
    	break;
    }

    case TA_StrCmdSetTimeCode :
    {    // 0x5441  //  ("TA")  //  TIME CODE 개별 설정.
//Time Code Number(1) : 0~32 사이의 수
//5 bytes(== nWeek(1)+sHHMM(2)+eHHMM(2)) * 8일 (== 요일(7) + 휴일(1)) == 40
//Lock 번호 : Lock 별로 설정함.. (의미없음. 그냥 넣음)
      INT8U  nLock, nCode;
      nCode = TABuf_Rx[TA_INDEX_DAT+0];
      nLock = TABuf_Rx[TA_INDEX_DAT+40];

      if((nCode < MAX_NUM_TIMECODE)){
        for(int ni = 0; ni < MAX_NUM_WEEK_HOLIDAY; ni++)  {// 원래는 순서대로 넣지만.. nWeek 값이 유효하게 오게 되어 있으므로..
          ScheduleParameter[nLock][nCode][ni].StartHh = TABuf_Rx[TA_INDEX_DAT+(5*ni)+2];//StartHh;
          ScheduleParameter[nLock][nCode][ni].StartMm = TABuf_Rx[TA_INDEX_DAT+(5*ni)+3];//StartMm;
          ScheduleParameter[nLock][nCode][ni].EndHh =   TABuf_Rx[TA_INDEX_DAT+(5*ni)+4];//EndHh;
          ScheduleParameter[nLock][nCode][ni].EndMm =   TABuf_Rx[TA_INDEX_DAT+(5*ni)+5];//EndMm;
        }
        SaveScheduleData();   // ScheduleParameter Sector에 써 넣음 45
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
      break;
    }

    case TA_StrCmdChangeTimeCode :
    { // 0x5442  //  ("TB")  //  출입문 TIME CODE 변경 스케쥴 변경..
      INT8U nlock;//nCode1, nCode2, nCode3, nCode4, nCode5, nCode6;
      nlock = TABuf_Rx[TA_INDEX_DAT+0];
      for(int i=0; i<3; i++) {
        CMSParameter.DoorTimeCode[nlock][i] =         (TABuf_Rx[TA_INDEX_DAT+1+i] < MAX_NUM_TIMECODE) ? TABuf_Rx[TA_INDEX_DAT+1+i]:0;//1,2,3       MAX_NUM_TIMECODE 128
        EEPROMSetData((INT16U)(E2P_Para_CMS + EEPROM_PARAM_INDEX_DOORTIMECODE        + nlock * 3 + i),        CMSParameter.DoorTimeCode[nlock][i]);
        CMSParameter.LimitCardTimeCode[nlock][i] =    (TABuf_Rx[TA_INDEX_DAT+4+i] < MAX_NUM_TIMECODE) ? TABuf_Rx[TA_INDEX_DAT+4+i]:0;//4,5,6       MAX_NUM_TIMECODE 128
        EEPROMSetData((INT16U)(E2P_Para_CMS + EEPROM_PARAM_INDEX_LIMITCARDTIMECODE   + nlock * 3 + i),   CMSParameter.LimitCardTimeCode[nlock][i]);
        CMSParameter.SpecialCardTimeCode[nlock][i] =  (TABuf_Rx[TA_INDEX_DAT+7+i] < MAX_NUM_TIMECODE) ? TABuf_Rx[TA_INDEX_DAT+7+i]:0;//7,8,9       MAX_NUM_TIMECODE 128
        EEPROMSetData((INT16U)(E2P_Para_CMS + EEPROM_PARAM_INDEX_SPECIALCARDTIMECODE + nlock * 3 + i), CMSParameter.SpecialCardTimeCode[nlock][i]);
      }
//      WriteTimeCodeToEeprom(nlock);
      TABuf_Tx[TA_INDEX_DAT+0] = '0';//타임코드가 범위를 벗어나면 0으로 바꿔버리고 성공으로 리턴해버림..
      break;
    }

    case TA_StrCmdSetHoliday  :
    {        // 0x4841  //  ("HA")  //  HOLIDAY 개별 설정.
      INT8U i, j, u8Month, u8Day, u8CountAll, u8arHoliday[MAX_NUM_HOLIDAY][2], u8arHolidayTemp[MAX_NUM_HOLIDAY][2];
      u8Month = TABuf_Rx[TA_INDEX_DAT+0];
      u8Day   = TABuf_Rx[TA_INDEX_DAT+1];

      u8CountAll = EEPROMGetData(EEPROM_ADDRESS_HolidayCount);
      if(u8CountAll > MAX_NUM_HOLIDAY){ //60이 넘으면 초기화..
        EEPROMSetData(EEPROM_ADDRESS_HolidayCount, 0); //카운트 지우고..
        EEPROMSetData(EEPROM_ADDRESS_HolidayDate,  0);//데이터 지운다..
        EEPROMSetData(EEPROM_ADDRESS_HolidayDate+1,0);//데이터 지운다..
        u8CountAll = 0;
      }
//      rprintf("[NOTICE]Set holiday!\r\n");
      if((u8Month <= 12)&&(u8Month > 0)&&(u8Day <= 31)&&(u8Day > 0)&&(u8CountAll < MAX_NUM_HOLIDAY)){//우선 월일만 검사해서 맞으면.. toDayTime.YEAR~SEC 은 일반포맷과 같음..
        if(u8CountAll > 0){  //처음 저장이 아니라면..
          EEPROMGetDataBlock(EEPROM_ADDRESS_HolidayDate, (INT8U*)u8arHoliday, u8CountAll*2); //#define EEPROM_ADDRESS_HolidayDate  (EEPROM_PAGE7 + 16) //0x710
          EEPROMGetDataBlock(EEPROM_ADDRESS_HolidayDate, (INT8U*)u8arHolidayTemp, u8CountAll*2);//임시로 복사한다..
          for(i=0; i<u8CountAll; i++){//기존 저장일에서 같은 값이 있는지 찾는다..
            if((u8arHoliday[i][0] == u8Month)&&(u8arHoliday[i][1] == u8Day)){
              break;
            }
          }
          if(i >= u8CountAll){ //끝까지 돌고 없었다는 뜻이므로..
            for(i=0; i<(u8CountAll+1); i++){//기존 저장값에 새 값을 순차배열로 저장한다..
              if(i == u8CountAll){//복사할 필요가 없음.. (u8arHoliday[i][0] < 1)||(u8arHoliday[i][0] > 12)||(u8arHoliday[i][1] < 1)||(u8arHoliday[i][1] > 31)){
                u8arHoliday[i][0] = u8Month;
                u8arHoliday[i][1] = u8Day;
                break;
              }
              else  {
                if((u8arHoliday[i][0] < u8Month)||//월이 작거나..
                  ((u8arHoliday[i][0] == u8Month)&&(u8arHoliday[i][1] < u8Day))) //같은달의 작은날 통과..
                  continue;
                u8arHoliday[i][0] = u8Month;
                u8arHoliday[i][1] = u8Day;
                for(j=i+1;j<(u8CountAll+1);j++){
                  u8arHoliday[j][0] = u8arHolidayTemp[j-1][0];
                  u8arHoliday[j][1] = u8arHolidayTemp[j-1][1];
                }
                break;
              }
            }
            u8CountAll++;
            EEPROMSetData(EEPROM_ADDRESS_HolidayCount,  u8CountAll);
            EEPROMSetDataBlock(EEPROM_ADDRESS_HolidayDate,(INT8U*)u8arHoliday,u8CountAll*2);
            TABuf_Tx[TA_INDEX_DAT+0] = '0';
          }
          else{ //중간에 같은 값이 있었다면 그냥 나가버리자..
            TABuf_Tx[TA_INDEX_DAT+0] = '1';
          }
        }
        else { //처음이므로..
          u8CountAll=1;
          EEPROMSetData(EEPROM_ADDRESS_HolidayCount,1);
          u8arHoliday[0][0] = u8Month;
          u8arHoliday[0][1] = u8Day;
          EEPROMSetDataBlock(EEPROM_ADDRESS_HolidayDate,(INT8U*)u8arHoliday,u8CountAll*2);
          TABuf_Tx[TA_INDEX_DAT+0] = '0';
        }
      }
      else {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
      break;
    }

    case TA_StrCmdDelHoliday  : 
	{        // 0x4843  //  ("HC")  //  HOLIDAY 개별 삭제.
      INT8U i, j, u8Month, u8Day, u8CountAll, u8arHoliday[MAX_NUM_HOLIDAY][2], u8arHolidayTemp[MAX_NUM_HOLIDAY][2];
      u8Month = TABuf_Rx[TA_INDEX_DAT+0];
      u8Day   = TABuf_Rx[TA_INDEX_DAT+1];
      u8CountAll = EEPROMGetData(EEPROM_ADDRESS_HolidayCount);

      if(u8CountAll > MAX_NUM_HOLIDAY)
	  { //60이 넘으면 초기화..
        EEPROMSetData(EEPROM_ADDRESS_HolidayCount,0); //카운트 지우고..
        EEPROMSetData(EEPROM_ADDRESS_HolidayDate,0);//데이터 지운다..
        EEPROMSetData(EEPROM_ADDRESS_HolidayDate+1,0);//데이터 지운다..
        u8CountAll = 0;
      }
//      rprintf("[NOTICE]Delete holiday!\r\n");
      if((u8Month <= 12)&&(u8Month > 0)&&(u8Day <= 31)&&(u8Day > 0)&&(u8CountAll <= MAX_NUM_HOLIDAY))
	  {//우선 월일만 검사해서 맞으면.. toDayTime.YEAR~SEC 은 일반포맷과 같음..
        if(u8CountAll > 0)
		{
          EEPROMGetDataBlock(EEPROM_ADDRESS_HolidayDate, (INT8U*)u8arHoliday, u8CountAll*2); //#define EEPROM_ADDRESS_HolidayDate  (EEPROM_PAGE7 + 16) //0x710
          EEPROMGetDataBlock(EEPROM_ADDRESS_HolidayDate, (INT8U*)u8arHolidayTemp, u8CountAll*2);//임시로 복사한다..
          for(i=0; i<u8CountAll; i++){//기존 저장값과 비교한다..
            if((u8arHoliday[i][0] == u8Month)&&(u8arHoliday[i][1] == u8Day)){//찾으면..
              for(j=i; j<(u8CountAll-1); j++){//다음의 기존값을 옮긴다..
                u8arHoliday[j][0] = u8arHolidayTemp[j+1][0];
                u8arHoliday[j][1] = u8arHolidayTemp[j+1][1];
              }
              u8CountAll--;
              EEPROMSetData(EEPROM_ADDRESS_HolidayCount,  u8CountAll);
              EEPROMSetDataBlock(EEPROM_ADDRESS_HolidayDate,(INT8U*)u8arHoliday,u8CountAll*2);
              TABuf_Tx[TA_INDEX_DAT+0] = '0';
              break;
            }
          }
          if(i >= u8CountAll)
		  { //끝까지 돌고 지울게 없었다는 뜻이므로..
            TABuf_Tx[TA_INDEX_DAT+0] = '1';
         }
        }
        else  
		{ //저장된 값이 없으면..
          TABuf_Tx[TA_INDEX_DAT+0] = '1';
       }
      }
      else 
	  {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
      break;
    }

    case TA_StrCmdDelHolidayAll  :    
	{// 0x4844  //  ("HD")  //  HOLIDAY 전체 삭제. 
      EEPROMSetData(EEPROM_ADDRESS_HolidayCount, 0); //카운트 지우고..
      EEPROMSetData(EEPROM_ADDRESS_HolidayDate,  0);//데이터 지운다..
      EEPROMSetData(EEPROM_ADDRESS_HolidayDate+1,0);//데이터 지운다..
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }

    case TA_StrCmdSetRelay  :
    {   // 0x5941  //  ("YA")  //  RELAY 개별 설정.
      INT8U nlock, nLockMode, nDoorOpenTime;
      nlock         = TABuf_Rx[TA_INDEX_DAT+0];
      nDoorOpenTime = TABuf_Rx[TA_INDEX_DAT+1];
      if((nDoorOpenTime == 0) && (nlock < MAX_NUM_RELAY))
      {
//        rprintf("[NOTICE] Lock Mode Change!\r\n");
        nLockMode     = TABuf_Rx[TA_INDEX_DAT+2];
        if( nLockMode == 0)
        {//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          ChangeLockMode( LockMode_Manual_Unlock, nlock);
        }
        else if( nLockMode == 1)
        {//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          ChangeLockMode( LockMode_Manual_Lock, nlock);
        }
        else if( nLockMode == 2)
        {//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          if(     (CMSParameter.DoorTimeCode[nlock][0] > 0)||
        		  (CMSParameter.DoorTimeCode[nlock][1] > 0)||
				  (CMSParameter.DoorTimeCode[nlock][2] > 0)  )
          {//타임코드가 있으면..
            ChangeLockMode( LockMode_Schedule_Lock, nlock);//일단 스케줄 Lock 상태로 바꿔버림..
          }
          else
          {
            ChangeLockMode( LockMode_Normal, nlock);
          }
        }
        else
        {  // if( nLockMode == 3)  {//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
            ChangeLockMode( LockMode_Normal, nlock);
        }
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else if((nDoorOpenTime <= 250) && (nlock < MAX_NUM_RELAY))
      {
//        rprintf("[NOTICE] Door Open Time Change!\r\n");
        CMSParameter.DoorOpenTime[nlock] = nDoorOpenTime;
        EEPROMSetData((INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DOOROPENTIME+nlock), nDoorOpenTime);

        TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
      break;
    }

    case TA_StrCmdRelayUserSet  :   
	{  // 0x5949  //  ("YI")   //  RELAY USER 명령. 
      INT8U nlock, nDoorOpenTime;
      nlock         = TABuf_Rx[TA_INDEX_DAT+0];
      nDoorOpenTime = TABuf_Rx[TA_INDEX_DAT+1];
//      rprintf("[NOTICE]Door Contorl each!\r\n");
      if((nDoorOpenTime <= 250) &&(nlock < MAX_NUM_RELAY))
      {
        ControlDoor( nDoorOpenTime, nlock);
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else if(nlock == 0x0F)
      {
    	  if( nDoorOpenTime == 0xFF)   // 전체 개방
    	  {
    		  for( uint8_t ni = 0; ni < MAX_NUM_RELAY; ni++)
				ChangeLockMode( LockMode_Manual_Unlock, ni);
    	  }
    	  else
    	  {    // 전체 닫힘
    		  for( uint8_t ni = 0; ni < MAX_NUM_RELAY; ni++)
				ChangeLockMode( LockMode_Manual_Lock, ni);
    	  }
         TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
     break;
    }

    case TA_StrCmdSetReader  :  
	{     // 0x5241  //  ("RA")  //  READER 개별 설정. 
      INT8U nReader, nCommType;
      nReader   = TABuf_Rx[TA_INDEX_DAT+0];
      nCommType = TABuf_Rx[TA_INDEX_DAT+1];

      //  rprintf("[NOTICE] Setting Reader !\r\n");
      if(nReader < MAX_NUM_RELAY)
      {// MAX_NUM_SYSTEM_PORT 8 리더와 상관없이 SelectDoor에서 lock 을 선택하므로.. MAX_NUM_LOCK){ //&0x7f ? 최상위비트는 순/역방향..
        LockParameter.SelectDoor[nReader] = TABuf_Rx[TA_INDEX_DAT+5];//리더당 컨트롤 하는 락번호..
        EEPROMSetData(E2P_Para_LOCK + MAX_NUM_RELAY * 3 + nReader, LockParameter.SelectDoor[nReader]);//리더당 컨트롤 하는 락번호..

        if(nReader < MAX_NUM_WIEGAND)
        {//MAX_NUM_WIEGAND 4 wiegand는 4포트까지 이므로..
          if((nCommType&0x7f) < 6)
          { //0: Wiegand 26BIT  1: Wiegand 26BIT MOTO Password   2: Wiegand 26BIT HID Password
                                    //3 : Wiegand 34BIT  4: Wiegand 64BIT   5 : Wiegand 128BIT.
            WiegandParameter.CommType[nReader] = nCommType;
          }
          else
          {
            WiegandParameter.CommType[nReader] = 0x03;  //0x06 : Wiegand 34BIT  
          }
          Wiegand_reset_port(nReader, nCommType&0x7f);
          EEPROMSetData(E2P_Para_WIEGAND + nReader, WiegandParameter.CommType[nReader]);

  //        WiegandParameter.AntiPassback[nReader]      = TABuf_Rx[TA_INDEX_DAT+2];//안쓰임
  //        WiegandParameter.OkRelayGroupNum[nReader]   = TABuf_Rx[TA_INDEX_DAT+3];//안쓰임
  //        WiegandParameter.ErrRelayGroupNum[nReader]  = TABuf_Rx[TA_INDEX_DAT+4];//안쓰임
  //        EEPROMSetData(E2P_Para_WIEGAND+4 +nReader,WiegandParameter.AntiPassback[nReader]);//안쓰임
  //        EEPROMSetData(E2P_Para_WIEGAND+8 +nReader,WiegandParameter.OkRelayGroupNum[nReader]);//안쓰임
  //        EEPROMSetData(E2P_Para_WIEGAND+12+nReader,WiegandParameter.ErrRelayGroupNum[nReader]);//안쓰임
        }
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
      }
      else
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
      }
      break;
    }

    case  TA_StrCmd485Reader:      
	{//    0x5253  //  ("RS")  //  RS485 리더기 세팅
      for(int i = 0; i < MAX_NUM_485CS; i++)
	  { //매니저에는 10개를 해두었으나.. 현재 펌웨어에서는 8개가 최대이므로..
        ConsoleParameter.Type[i] = TABuf_Rx[TA_INDEX_DAT + i];
        ConsoleParameter.DoorNumber[i] = TABuf_Rx[TA_INDEX_DAT + MAX_NUM_485CS + i];
      }
      EEPROMSetDataBlock(E2P_Para_CONSOLE, (INT8U*)&ConsoleParameter.AccessOpenTime[0], LEN_E2ROM_CONSOLE);//그냥 모조리 저장하셈..
      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }

    case  TA_StrCmd485Status:
	{//    0x5254  //  ("RT")  //  RS485 리더기 tjfejf
      for(int i = 0; i < MAX_NUM_485CS; i++)
	  { //매니저에는 10개를 해두었으나.. 현재 펌웨어에서는 8개가 최대이므로..
    	  TABuf_Tx[TA_INDEX_DAT+i] = ConsoleParameter.Type[i];
    	  TABuf_Tx[TA_INDEX_DAT+MAX_NUM_485CS+i] = ConsoleParameter.DoorNumber[i];
      }

 //    TABuf_Tx[TA_INDEX_DAT+0] = '0';
      break;
    }
  
    case TA_StrCmdGetEvent  :  
	{        // 0x4542  //  ("EB")  //  EVENT UPLOAD (EVENT 개별 삭제 필요 없음 / 자동 삭제).
      INT16U SendFlag=0;// CrcXor = 0, len, SendFlag=0;
      gU8ACUnum = TABuf_Rx[TA_INDEX_ADDR];  //#define TA_INDEX_ADDR (2)      // ACU Address
      
      if(GVTAEventData.u8LogCount == 0)
	  {//보낼 내용이 있는지 확인해서... 있으면 restore
        if(GVTAEventData.u8EEPromPageCount > 0)
		{ //eeprom counter 확인하고.. 참고 : SaveEventLog2EEprom_sFlash()
          GVTAEventData.u8EEPromPageCount--;
          GVTAEventData.u8LogCount = 16;
          
          EEPROMGetDataBlock(EEPROM_ADDRESS_EventStart+GVTAEventData.u8EEPromPageCount*0x100, GVTAEventData.pU8EventLog, 0x100);// 한 page 를 읽어들인다..
          
          EEPROMSetData(EEPROM_ADDRESS_EventCount,      GVTAEventData.u8LogCount);//저장하기..
          EEPROMSetData(EEPROM_ADDRESS_EEPromPageCount, GVTAEventData.u8EEPromPageCount);//저장하기..
        }
        else if(GVTAEventData.u8SFlashSectorCount > 0)
		{  //sflash counter도 확인해서..
          uint8_t sFlashBuffer[4096];
          GVTAEventData.u8SFlashSectorCount--;
          xREAD_DataFromExNorFlash(FLASH_EventHisory_Start+GVTAEventData.u8SFlashSectorCount*0x1000,sFlashBuffer,0x1000);//flash 1sector를 읽어서..
          EEPROMSetDataBlock(EEPROM_ADDRESS_EventStart, sFlashBuffer, 0x1000);//eeprom 16개 page에 복사..
          GVTAEventData.u8EEPromPageCount = 15;
          GVTAEventData.u8LogCount = 16;
          EEPROMGetDataBlock(EEPROM_ADDRESS_EventStart+0xF00, GVTAEventData.pU8EventLog, 0x100);// 한 page 를 읽어들인다..
          EEPROMSetData(EEPROM_ADDRESS_EventCount,      GVTAEventData.u8LogCount);//저장하기..
          EEPROMSetData(EEPROM_ADDRESS_EEPromPageCount, GVTAEventData.u8EEPromPageCount);//저장하기..
          EEPROMSetData(EEPROM_ADDRESS_FlashSectorCount,GVTAEventData.u8SFlashSectorCount);//저장하기..
        }
      }
      
      if(GVTAEventData.u8LogCount > 0)
	  {// 0x4542  ("EB")  //  EVENT UPLOAD (EVENT 개별 삭제 필요 없음 / 자동 삭제).
//        while(gU8LogCountFlag > 0) OS_Delayus(1);//다른데서 쓰고 있는중이면 멈춤..
        gU8LogCountFlag = 1;//멈춤 설정..
        GVTAEventData.u8LogCount--;
/*  GVTAEventData.pU8EventLog[16];
2A  length  0 //carddata 0x2A, no carddata 0x1A  ==> 0x2A  42로 통일..
30  data-status 1 //이건 거의 30임.. 확인필요..
4343  data-event종류  2 압축 가능..
00  data-event내용1 4 압축가능..
04  data-event내용2 5 압축가능..
0F0B01000927  data-time 6~11 (6) 압축가능.. julian date 이용..
3824A75C  data-ID 12~15 (4) 늘릴 수 있으면 늘려보자..
*/
        TABuf_Tx[TA_INDEX_DAT + 0] = '0';//정상..
        memcpy(TABuf_Tx + TA_INDEX_EVENT_CMD1,  GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16+ 2, 4);//8 1+7        copy event data to tx buffer
        memcpy(TABuf_Tx + TA_INDEX_EVENT_TIME,  GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16+ 6, 6);//12 5+7       날짜  복사
        memset(TABuf_Tx + TA_INDEX_EVENT_CARD, 0, 12);//18 11+7       cardID 앞자리 클리어.. 현재로썬 4바이트 데이터만 취급하므로..
        //////////////////////////////////////// 임시로 intask 카운트를 내보냄.. ////////////////////////////////////////
        TABuf_Tx[TA_INDEX_EVENT_CARD + 2] = gU16nDelayCnt_Count/0x100;
        TABuf_Tx[TA_INDEX_EVENT_CARD + 3] = gU16nDelayCnt_Count%0x100;

        TABuf_Tx[TA_INDEX_EVENT_CARD + 6] = gU16IntaskAliveCount/0x100;
        TABuf_Tx[TA_INDEX_EVENT_CARD + 7] = gU16IntaskAliveCount%0x100;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        memcpy(TABuf_Tx + TA_INDEX_EVENT_CARD+12,  GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16+12, 4);//카드ID 4byte 복사
        memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);//34 27+7

        memset(GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16, 0, 16);//Event data Clear..
        if((GVTAEventData.u8EEPromPageCount > 0)||(GVTAEventData.u8SFlashSectorCount>0))
        {//EEprom 이나 sFlash에 저장되어 있는 log가 있으면..
            EEPROMSetData(EEPROM_ADDRESS_EventCount,      GVTAEventData.u8LogCount);//저장하기..
        }
        else if(GVTAEventData.u8LogCount == 0)
        {        // 로그카운트 모두 0이고 마지막 보냈으니 EEProm 에 마지막 저장...
            if(EEPROMGetData(EEPROM_ADDRESS_EventCount) > 0){//체크 안하면 이벤트 발생할 때마다 계속 쓰게 됨..
                EEPROMSetData(EEPROM_ADDRESS_EventCount, GVTAEventData.u8LogCount);//저장하기..
            }
        }
        gU8LogCountFlag = 0;//멈춤 해제..
      }
      else 
	  {  // 저장된 로그가 없다면.. 즉 아무데이터 없다면.. 센서 상태 이벤트 보냄..
        if(SendFlag == 0)
		{//아무것도 보낸 적이 없으면.. 하나는 보내야지..
          TABuf_Tx[TA_INDEX_DAT+0] = '1';//정상아님.. 없음..
          TABuf_Tx[TA_INDEX_DAT+1] = gU8SensorValue;//1234 5678
          TABuf_Tx[TA_INDEX_DAT+2] = gU8ConsoleAlive; //
          TABuf_Tx[TA_INDEX_DAT+3] = gU8DoorOpenedTooLong;//1234 5678
          TABuf_Tx[TA_INDEX_DAT+4] = TACheckJumperSetting(); //1234 0000 나중에 쓸지 모름..
        }
        else
        {
        }
        CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_EVENT_TIME);//funcCopyEventDateTime_to_TABuf_Tx();
        memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);
        TABuf_Tx[TA_INDEX_EVENT_LOGCOUNT + 0] = 0;    //남은 로그의 개수를 넣는다..
        TABuf_Tx[TA_INDEX_EVENT_LOGCOUNT + 1] = GVTAEventData.u8SFlashSectorCount;
        TABuf_Tx[TA_INDEX_EVENT_LOGCOUNT + 2] = GVTAEventData.u8EEPromPageCount;
        TABuf_Tx[TA_INDEX_EVENT_LOGCOUNT + 3] = GVTAEventData.u8LogCount;
      }
      break;
    }

    case  TA_StrCmdSetFire:      //    0x4641  //  ("FA")  //  화재 동작 설정.
    {
    	for( uint8_t nLock = 0; nLock < MAX_NUM_RELAY; nLock++)
    	{
    		FireParameter.FireClose[nLock] = TABuf_Rx[TA_INDEX_DAT+nLock];  //0x10
    	}
    	EEPROMSetDataBlock(E2P_Para_FIRE, &FireParameter.FireClose[0], MAX_NUM_RELAY);
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
    	break;
    }

    case  TA_StrCmdChkFire:      //    0x4642  //  ("FB")  //  화재 동작 설정확인.
    {
    	TABuf_Tx[TA_INDEX_DAT+0] = '0';
    	for( uint8_t nLock = 0; nLock < MAX_NUM_RELAY; nLock++)
    	{
    		TABuf_Tx[TA_INDEX_DAT+nLock+1] = FireParameter.FireClose[nLock];
    	}
    	TABuf_Tx[TA_INDEX_DAT+MAX_NUM_RELAY+1] = 0;
    	break;
    }

    case TA_FWUPDATE_START:
    {    //#define TA_FWUPDATE_START      0x4653      //"FS"
      rprintf("[NOTICE] Fwd TA_FWUPDATE_START Result!\r\n");
/*      OS_Suspend(&TCBSCI2Rs485Task);
      OS_Suspend(&TCBSCI1FwdTask);
#if defined (MODEL_NUM) && (MODEL_NUM == 1)//1:TA000 cp710占쏙옙占쏙옙占쏙옙 debug, cp711占쏙옙占쏙옙占쏙옙 cdma.. ta4000 占쏙옙占쏙옙占쏙옙 LCD占쏙옙 占쏙옙占쏙옙占�?..
      OS_Suspend(&TCBSCI4DebugCdcTask);
#elif defined (MODEL_NUM) && (MODEL_NUM == 8)        //8: aru8
      OS_Suspend(&TCBFacTask);
#else //#elif defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000
      OS_Suspend(&TCBWiegandTask);
      OS_Suspend(&TCBFacTask);
#endif	//MODEL_NUM
      OS_Suspend(&TCBInTask);*/
      
      bRequestNeed = MON_FROM_USER;  

      Firmware_Upgrade = SPI1_UPGRADE_YES;
      ftimeSpiOut = 0;

      TABuf_Tx[TA_INDEX_DAT+0] = '0';
      TABuf_Tx[TA_INDEX_DAT+1] = 0;
      break;
    }

    case TA_FWUPDATE_HEADER:
    {   //#define TA_FWUPDATE_HEADER     0x4648      //"FH"
      if(Firmware_Upgrade != SPI1_UPGRADE_YES)
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
        TABuf_Tx[TA_INDEX_DAT+1] = 101;
      }
      else
      {
//        rprintf("[NOTICE] Fwd TA_FWUPDATE_HEADER Result!\r\n");
        ftimeSpiOut = 0;
                          
        PrepareUpdatesFlash(UPDATE_FROM_NEW); // Should be prior to send response
        
        ulBinLen  = (TABuf_Rx[TA_INDEX_DAT+0] << 24);
        ulBinLen += (TABuf_Rx[TA_INDEX_DAT+1] << 16);
        ulBinLen += (TABuf_Rx[TA_INDEX_DAT+2] << 8);
        ulBinLen += (TABuf_Rx[TA_INDEX_DAT+3] << 0);
              
        ulBinCrc  = (TABuf_Rx[TA_INDEX_DAT+4] << 24);
        ulBinCrc += (TABuf_Rx[TA_INDEX_DAT+5] << 16);
        ulBinCrc += (TABuf_Rx[TA_INDEX_DAT+6] << 8);
        ulBinCrc += (TABuf_Rx[TA_INDEX_DAT+7] << 0);      

        TABuf_Tx[TA_INDEX_DAT+0] = '0';
        TABuf_Tx[TA_INDEX_DAT+1] = 0;
      }
     break;
    }

    case TA_FWUPDATE_DATA:
    {  //#define TA_FWUPDATE_DATA       0x4644      //"FD"
      if(Firmware_Upgrade != SPI1_UPGRADE_YES)
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
        TABuf_Tx[TA_INDEX_DAT+1] = 102;
      }
      else
      {
//        rprintf("[NOTICE] Fwd TA_FWUPDATE_DATA Result!\r\n");
        ftimeSpiOut = 0;
        uint16_t u16DataLength = TABuf_Rx[TA_INDEX_LENH]*0x100 + TABuf_Rx[TA_INDEX_LENL] - 7; //stx(1)add(1)length(2)command(2)data(?)etx(1)
        if( u16DataLength > MX25L51245G_PAGE_SIZE)
        {

			uint16_t n1st, n2nd, n3rd;

			n1st = (uint16_t) (256 - (ulFlashAddr % 256));
			if( n1st == 256)
			{
				xWRITE_DataToExNorFlash(ulFlashAddr, TABuf_Rx+TA_INDEX_DAT, MX25L51245G_PAGE_SIZE);  // 256

				xWRITE_DataToExNorFlash(ulFlashAddr+256, TABuf_Rx+TA_INDEX_DAT+256, MX25L51245G_PAGE_SIZE);  // 256
			}
			else
			{
				n2nd = (uint16_t) 256;
				n3rd = (uint16_t) (256 - n1st);
				xWRITE_DataToExNorFlash(ulFlashAddr, TABuf_Rx+TA_INDEX_DAT, n1st);
				xWRITE_DataToExNorFlash(ulFlashAddr+n1st, TABuf_Rx+TA_INDEX_DAT+n1st, 256);
				if(n3rd > 0)
				xWRITE_DataToExNorFlash(ulFlashAddr+n1st+256, &TABuf_Rx[TA_INDEX_DAT+n1st+256], n3rd);
			}
        }
        else
        {
            uint16_t n1st, n2nd;
            n1st = (uint16_t) (256 - (ulFlashAddr % 256));
            if( n1st == 256)
            {
            	xWRITE_DataToExNorFlash(ulFlashAddr, TABuf_Rx+TA_INDEX_DAT, u16DataLength);
            }
            else
            {
                n2nd = (uint16_t) (u16DataLength - n1st);
                xWRITE_DataToExNorFlash(ulFlashAddr, TABuf_Rx+TA_INDEX_DAT, n1st);
                if(n2nd > 0)
                xWRITE_DataToExNorFlash(ulFlashAddr+n1st, &TABuf_Rx[TA_INDEX_DAT+n1st], n2nd);
            }
        }
 //       xREAD_DataFromExNorFlash(ulFlashAddr, TABuf_Tx + TA_INDEX_DAT, u16DataLength);
        ulFlashAddr += u16DataLength;
        ulDataLen += u16DataLength;

 /*       uint16_t nIndex = 0;
        for( nIndex = 0; nIndex < u16DataLength; nIndex++) {  // CRC and ETX
        	if( *(TABuf_Rx+TA_INDEX_DAT+nIndex) != *(TABuf_Tx+TA_INDEX_DAT+nIndex) )
        		printf("differnt %d[%02X : %02X] ", nIndex, *(TABuf_Rx+TA_INDEX_DAT+nIndex), *(TABuf_Tx+TA_INDEX_DAT+nIndex));
        }
*/
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
        TABuf_Tx[TA_INDEX_DAT+1] = (uint8_t)(ulDataLen*100/ulBinLen);//진형비율.. %
      }
      break;
    }

    case TA_FWUPDATE_TAIL:
    {  //#define TA_FWUPDATE_TAIL       0x4654      //"FT"
      if(Firmware_Upgrade != SPI1_UPGRADE_YES)
      {
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
        TABuf_Tx[TA_INDEX_DAT+1] = 103;
      }
      else
      {
//        rprintf("[NOTICE] Fwd TA_FWUPDATE_TAIL Result!\r\n");
        ftimeSpiOut = 0;
        ulFlashAddr = FIRM_START_NEW_ADDR;
        nAddedFf = TABuf_Rx[TA_INDEX_DAT];
        ulDataLen -= nAddedFf;

  //      TABuf_Tx[TA_INDEX_DAT] = TA_FWUPDATE_TAIL;
  //      if( ulBinLen == ulDataLen)  TABuf_Tx[4] = 1;            
  //      else                        TABuf_Tx[4] = 0;          
  //
        TABuf_Tx[TA_INDEX_DAT+0] = ( ulBinLen == ulDataLen)? '0': '1';
        TABuf_Tx[TA_INDEX_DAT+1] = 100;
      }
      break;
    }

    case TA_FWUPDATE_END:
    {  //#define TA_FWUPDATE_END       0x4645      //"FE"
/*      if(Firmware_Upgrade != SPI1_UPGRADE_YES){
        TABuf_Tx[TA_INDEX_DAT+0] = '1';
        TABuf_Tx[TA_INDEX_DAT+1] = 104;
      }
      else {*/
        rprintf("[NOTICE] Fwd TA_FWUPDATE_END Result!\r\n");
        ftimeSpiOut = 0;
        bRequestNeed = MON_NEED_SPI1_FWUD;

        TABuf_Tx[TA_INDEX_DAT+0] = '0';
        TABuf_Tx[TA_INDEX_DAT+1] = 100;
 //     }
      break;
    }

    case TA_StrCmdFireOccur  :
    {       // 0x4643  //  ("FC")  //  화재 발생 / 복구
        if (TABuf_Rx[TA_INDEX_DAT+0] == '0')
        { //발생
          //isFireAlertOccur = TRUE;
//  #ifdef ENABLE_FIRE_ANNOUNCE
          if(FireLockFlag != 99)
          {//EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
             FireOccurrence = TRUE;
             EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag, 119);
             TABuf_Tx[TA_INDEX_DAT+1] = '0';//발생..
          }
          else
          {
             FireOccurrence = 99;//화재인데 화재작동 안하는 경우는 여기에 해당함.. 노말모드로..
             TABuf_Tx[TA_INDEX_DAT+1] = '3';//발생안시킴..
          }
          TABuf_Tx[TA_INDEX_DAT+0] = '0';//성공~
//  #else
//          TABuf_Tx[TA_INDEX_DAT+0] = '0';//성공~
//          TABuf_Tx[TA_INDEX_DAT+1] = '3';//발생안시킴..
//  #endif
      }
      else if(TABuf_Rx[TA_INDEX_DAT+0] == '1')
      {  //복구
        isFireAlertOccur = FALSE;
        FireOccurrence = FALSE;
//        if(FireLockFlag != 99)
//        {//EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
          EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag, 0);//eeprom 화재신호 지우기..
//        }
        TABuf_Tx[TA_INDEX_DAT+0] = '0';//성공~
        TABuf_Tx[TA_INDEX_DAT+1] = '1';//복구~
      }
      else if (TABuf_Rx[TA_INDEX_DAT+0] == '2')
      { //화재시 LOCK 작동하게.. 화재 무드 무시
        FireLockFlag = 99;//화재작동모드 0, 화재시비작동모드 99
        TABuf_Tx[TA_INDEX_DAT+0] = '0';//성공~
        TABuf_Tx[TA_INDEX_DAT+1] = '2';//화재 LOCK 작동 모드 변경완료..
      }
      else if (TABuf_Rx[TA_INDEX_DAT+0] == '3')
      { //화재시 LOCK 작동하지 않게 모드 변경..원래 대로 복귀
        FireLockFlag = 0;//화재인데 화재작동 안하는 경우는 여기에 해당함.. 노말모드로..
        TABuf_Tx[TA_INDEX_DAT+0] = '0';//성공~
        TABuf_Tx[TA_INDEX_DAT+1] = '3';//화재 LOCK 비작동 모드 변경완료..
      }
      else
      {//'1' 복구명령인데 화재상태가 아닌 경우
        TABuf_Tx[TA_INDEX_DAT+0] = '1';//실패
        TABuf_Tx[TA_INDEX_DAT+1] = TABuf_Rx[TA_INDEX_DAT+0];//'0'//발생~ 뭔가 문제있으면 발생명령에 대한 실패로..
      }
//      rprintf("[NOTICE] Fire Command Occur or Restore\r\n");
      break;
    }
    case TA_StrCmdSetInput :
    {      //          0x5541  //  ("UA")  //  INPUT ���� ����.
    	uint8_t nInput = TABuf_Rx[TA_INDEX_DAT];
    	if(nInput>0)
    		nInput -= 1;
    	SensorParameter.NoNc[nInput] = TABuf_Rx[TA_INDEX_DAT + 1];
        EEPROMSetDataBlock(E2P_Para_SENSOR, (INT8U*)&SensorParameter.Status[0], LEN_E2ROM_SENSOR);//그냥 모조리 저장하셈..
        TABuf_Tx[TA_INDEX_DAT+0] = '0';
    	break;
    }
    case TA_StrCmdChkInput: {  //            0x5542  //  ("UB")  //  INPUT ���� ���� Ȯ��.
    	uint8_t nInput = TABuf_Rx[TA_INDEX_DAT];
    	if(nInput>0)
    		nInput -= 1;
    	TABuf_Tx[TA_INDEX_DAT+0] = '0';
    	TABuf_Tx[TA_INDEX_DAT+1] = nInput;
    	TABuf_Tx[TA_INDEX_DAT+2] = SensorParameter.NoNc[nInput];
    	TABuf_Tx[TA_INDEX_DAT+3] = 3;
    	TABuf_Tx[TA_INDEX_DAT+4] = 2;
    	TABuf_Tx[TA_INDEX_DAT+5] = 1;
    	TABuf_Tx[TA_INDEX_DAT+6] = 0;
    	TABuf_Tx[TA_INDEX_DAT+7] = LockParameter.SelectDoor[nInput];
    	break;
    }
    default: {//보낸놈 그대로.. 실패다로 보냄..
      TABuf_Tx[TA_INDEX_DAT] = '1';//정상아님.. 없음..
      break;
    }
  }
//  if( ProtocolType != TA_StrCmdCheckAcuStatus) // 계속 응답을 함.
	  TAMakeResponsePacket( ProtocolType);
}


void TATreatControl(INT8U Action, INT8U InputId)
{
  if( Action == 0)
  {// 0xFC
    ChangeLockMode( LockMode_Manual_Unlock, InputId);
  }
  else if( Action == 1)
  {  // 0xFD
    ChangeLockMode( LockMode_Manual_Lock, InputId);
  }
  else if( Action == 2)
  {// 0x01 스케줄 복귀..
    if((CMSParameter.DoorTimeCode[InputId][0] > 0)||(CMSParameter.DoorTimeCode[InputId][1] > 0)||(CMSParameter.DoorTimeCode[InputId][2] > 0))
    {//타임코드가 있으면..
      ChangeLockMode( LockMode_Schedule_Lock, InputId);//일단 스케줄 Lock 상태로 바꿔버림..
    }
    else
    {
      ChangeLockMode( LockMode_Normal, InputId);
    }
  }
  else if( Action == 3)
  {// 0x01 일반모드
      ChangeLockMode( LockMode_Normal, InputId);
  }
  else if( Action <= 243 )
  {// 4 <= OpenTime <= 243
    ControlDoor( Action - 3, InputId); //4초 -> 1초 ~ 240초 -> 237초로 바꿈..
  }
//  else
//    return;
}

INT8U TACheckJumperSetting( void)
{ //if Set Jumper 1,2 is closed == FALSE, is opend == TRUE
  INT8U nPort = 0;

  uint16_t JumperPin[4]  = {SET_0_Pin, SET_1_Pin, SET_2_Pin, SET_3_Pin};
  GPIO_TypeDef *JumperPort[4] = {GPIOB, GPIOB, GPIOB, GPIOB};
  for(int i=0; i<4; i++){
    if( HAL_GPIO_ReadPin(JumperPort[i], JumperPin[i]) == FALSE){
      nPort |= 0x80>>i;
    }
  }

  return nPort;
}

void TAMakeResponsePacket( INT16U SendCommand)
{
  INT16U i;
    
  INT16U CrcXor = 0, len;

  TABuf_Tx[TA_INDEX_HDR] = TA_HEADER_TA_MSG; //0     AA
  TABuf_Tx[TA_INDEX_STX] = TA_MSGE_STX;      //1     02
  TABuf_Tx[TA_INDEX_ADDR] = gU8ACUnum;       //2     TABuf_Rx[TA_INDEX_ADDR];  //2   01

  if( SendCommand != TA_StrCmdCheckAcuStatus) // 계속 응답을 함.
	  len = 0;
  len = TAGetPacketLength( SendCommand);
  TABuf_Tx[TA_INDEX_LENH] = len/256;  //3
  TABuf_Tx[TA_INDEX_LENL] = len%256;  //4
  TABuf_Tx[TA_INDEX_CMD1] = SendCommand/0x100;        //5     TABuf_Rx[TA_INDEX_CMD1];
  TABuf_Tx[TA_INDEX_CMD2] = SendCommand%0x100;        //6     TABuf_Rx[TA_INDEX_CMD2];
    // 0xAA 0x02 0x?? 0x???? 0x????     0x30 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x03 0xE0 
  TABuf_Tx[len] = TA_MSGE_ETX;
   
  for(i = 2; i<len; i++) {
    CrcXor ^= TABuf_Tx[i];
  }
  TABuf_Tx[len+1] = CrcXor;

  GVTAComData.length = len + 2;
  GVTAComData.SetPacket = TRUE;
}

INT16U TAGetPacketLength( INT16U SendCommand)
{
   INT16U uLength = 0;
   
   switch(SendCommand) {
   case  TA_StrCmdCheckAcuStatus:      //   0x4141  //  ("AA")  //  ACU의 통신 상태 확인
        uLength = 12 ;   //5 + 7
        break;
    case  TA_StrCmdSetAcuTime:      //   0x4142  //  ("AB")  //  ACU의 시간설정.
        uLength = 15;   //4 + 7 + 4 바이트 추가 -> gU16EEpromFailCount = 0;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...// 0x4142  //	("AB")
        break;
    case  TA_StrCmdCheckAcuTime:      //    0x4143  //  ("AC")  //  ACU의 시간설정 확인..
        uLength = 15;   //6 + 7
        break;
    case  TA_StrCmdGetAcuInfo:      //    0x4145  //  ("AE")  //  ACU의 정보 확인.
        uLength = 34;   //27 + 7
        break;
    case  TA_StrCmdGetEvent:      //    0x4542  //  ("EB")  //  EVENT UPLOAD (EVENT 개별 삭제 필요 없음 / 자동 삭제).
        uLength = 46;    // 39 + 7
        break;
    case  TA_StrCmdChkFire:      //    0x4642  //  ("FB")  //  화재 동작 설정확인.
        uLength = 15;   //8 + 7
    	break;
     case TA_FWUPDATE_START:       //#define TA_FWUPDATE_START      0x4653      //"FS"
     case TA_FWUPDATE_HEADER:       //#define TA_FWUPDATE_HEADER     0x4648      //"FH"
     case TA_FWUPDATE_DATA:       //#define TA_FWUPDATE_DATA       0x4644      //"FD"
     case TA_FWUPDATE_TAIL:       //#define TA_FWUPDATE_TAIL       0x4654      //"FT"
     case TA_FWUPDATE_END:       //#define TA_FWUPDATE_END       0x4645      //"FE"
     case TA_StrCmdFireOccur  :    // 0x4643  //  ("FC")  //  화재 발생 / 복구
          uLength =  9;//2 + 7 return value 1byte.. '0' or '1' and 현재 진행정도 %
          break;
     case  TA_StrCmdChkInput:      //    0x5542  //  ("UB")  //  INPUT 개별 설정 확인.
         uLength =  15;// 8 + 7
     	 break;
     case  TA_StrCmd485Status:      //    0x5452  //  ("RT")  //  485 설정 확인.
         uLength =  23;// 16 + 7
     	 break;
     case  TA_StrCmdCheckIDsg:      //    0x4943  //  ("IC")  //  ID자료 개별 내용 확인.
         uLength =  20;// 13 + 7
     	 break;
     case TA_StrCmdCheckIDCnt:      //    0x4946  //  ("IF")  //  ID자료 등록 건수 확인.
         uLength =  12;// 5 + 7
     	 break;
     case TA_StrCmdGetIDContent:	//    0x494C  //  ("IL")  //  ID자료 등록 메모리 내용 확인.
         uLength =  258;// 251 + 7
     	 break;
     case  	TA_StrCmdLogDisp:
		 uLength =  39;// 32 + 7
		 break;
		case  TA_StrCmdResetAcu:      //    0x4144  //  ("AD")  //  ACU의 초기화.
    case  TA_StrCmdAcuReboot  :    //0x4146  //  ("AF")  //  ACU의 reboot
    case  TA_StrCmdAcuSetting:    //        0x4153  //  ("AS")  //  ACU Setting
    case  TA_StrCmdSetIDsg:      //    0x4941  //  ("IA")  //  ID자료 개별 저장.
    case  TA_StrCmdSetIDblocksg :    // 0x4942  //  ("IB")  //  ID자료 블럭 저장 (블럭을 통채로 대체하는 방식
    case  TA_StrCmdDeleteIDsg:      //    0x4944  //  ("ID")  //  ID자료 개별 삭제. 
    case  TA_StrCmdDeleteIDAll:      //    0x4945  //  ("IE")  //  ID자료 전체 삭제. 
    case  TA_StrCmdSetApbID:      //    0x4947  //  ("IG")  //  ID자료 Anti-passback 설정.
    case  TA_StrCmdCheckApbID:      //    0x4948  //  ("IH")  //  ID자료 Anti-passback 설정 확인.
    case  TA_StrCmdDeleteIDBlock: 	// 0x494A  //	("IJ")  //  ID자료 특정블럭만 삭제. 
    case  TA_StrCmdInitApbIDAll:      //    0x4949  //  ("II")   //  ID자료 Anti-passback 전체 초기화.
    case  TA_StrCmdSetDoorZone:      //    0x4441  //  ("DA")  //  DOOR ZONE 개별 설정.
    case  TA_StrCmdChkDoorzone:      //  0x4442  //  ("DB")  //  DOOR ZONE 개별 확인. 
    case  TA_StrCmdDelDoorZone:      //      0x4443  //  ("DC")  //  DOOR ZONE 개별 삭제.
    case  TA_StrCmdDelDoorZoneAll:      //    0x4444  //  ("DD")  //  DOOR ZONE 전체 삭제.
    case  TA_StrCmdSetDoorGroup:      //    0x4445  //  ("DE")  //  DOOR GROUP 개별 설정. 
    case  TA_StrCmdChkDoorGroup:      //    0x4446  //  ("DF")  //  DOOR GROUP 개별 확인. 
    case  TA_StrCmdDelDoorGroup:      //    0x4447  //  ("DG")  //  DOOR GROUP 개별 삭제. 
    case  TA_StrCmdDelDoorGroupAll:      //  0x4448  //  ("DH")  //  DOOR GROUP 전체 삭제.
    case  TA_StrCmdSetTimeCode:      //    0x5441  //  ("TA")  //  TIME CODE 개별 설정.
    case  TA_StrCmdChangeTimeCode:      //  0x5442  //  ("TB")  //  출입문 TIME CODE 변경
    case  TA_StrCmdDelTimeCode:      //    0x5443  //  ("TC")  //  TIME CODE 개별 삭제.
    case  TA_StrCmdDelTimeCodeAll:      //    0x5444  //  ("TD")  //  TIME CODE 전체 삭제. 
    case  TA_StrCmdSetTimeZone:      //    0x5445  //  ("TE")  //  TIME ZONE 개별 설정. 
    case  TA_StrCmdChkTimeZone:      //    0x5446  //  ("TF")  //  TIME CODE 확인. 
    case  TA_StrCmdDelTimeZone:      //    0x5447  //  ("TG")  //  TIME ZONE 개별 삭제. 
    case  TA_StrCmdDelTimeZoneAll:      //    0x5448  //  ("TH")  //  TIME ZONE 전체 삭제.
    case  TA_StrCmdSetHoliday:      //    0x4841  //  ("HA")  //  HOLIDAY 개별 설정.
    case  TA_StrCmdChkHoliday:      //    0x4842  //  ("HB")  //  HOLIDAY 개별 확인.
    case  TA_StrCmdDelHoliday:      //    0x4843  //  ("HC")  //  HOLIDAY 개별 삭제.
    case  TA_StrCmdDelHolidayAll:      //    0x4844  //  ("HD")  //  HOLIDAY 전체 삭제. 
    case  TA_StrCmdSetRelay:      //    0x5941  //  ("YA")  //  RELAY 개별 설정.
    case  TA_StrCmdChkRelay:      //    0x5942  //  ("YB")  //  RELAY 개별 확인.
    case  TA_StrCmdDelRelay:      //    0x5943  //  ("YC")  //  RELAY 개별 삭제.
    case  TA_StrCmdDelRelayAll  :      // 0x5944  //  ("YD")  //  RELAY 전체 삭제.
    case  TA_StrCmdSetRelayGroup:      //    0x5945  //  ("YE")  //  RELAY GROUP 개별 설정.
    case  TA_StrCmdChkRelayGroup:      //    0x5946  //  ("YF")  //  RELAY GROUP 개별 확인. 
    case  TA_StrCmdDelRelayGroup:      //    0x5947  //  ("YG")  //  RELAY GROUP 개별 삭제. 
    case  TA_StrCmdDelRelayGroupAll:      //    0x5948  //  ("YH")  //  RELAY GROUP 전체 삭제. 
    case  TA_StrCmdRelayUserSet:      //    0x5949  //  ("YI")   //  RELAY USER 명령. 
    case  TA_StrCmdSetInput:      //    0x5541  //  ("UA")  //  INPUT 개별 설정. 
    case  TA_StrCmdDelInput:      //    0x5543  //  ("UC")  //  INPUT 개별 삭제.
    case  TA_StrCmdDelInputAll:      //    0x5544  //  ("UD")  //  INPUT 전체 삭제. 
    case  TA_StrCmdSetReader:      //    0x5241  //  ("RA")  //  READER 개별 설정. 
    case  TA_StrCmdChkReader:      //    0x5242  //  ("RB")  //  READER 개별 설정 확인. 
    case  TA_StrCmdDelReader:      //    0x5243  //  ("RC")  //  READER 개별 삭제. 
    case  TA_StrCmd485Reader:      //    0x5253  //  ("RS")  //  RS485 리더기 세팅
    case  TA_StrCmdDelReaderAll:      //    0x5244  //  ("RD")  //  READER 개별 삭제. 
//    case  TA_StrCmdChkEvent:      //    0x4541  //  ("EA")  //  EVENT UPLOAD (EVENT 개별 삭제 필요).
    case  TA_StrCmdDelEvent:      //    0x4543  //  ("EC")  //  EVENT 개별 삭제. 
    case  TA_StrCmdDelEventAll:      //    0x4544  //  ("ED")  //  EVENT 전체 삭제. 
    case  TA_StrCmdChkEventCnt:      //    0x4545  //  ("EE")  //  EVENT 건수 확인. 
    case  TA_StrCmdSetSubCnt:      //    0x5341  //  ("SA")  //  SUB_BOARD 수 설정. 
    case  TA_StrCmdChkSubCnt:      //    0x5342  //  ("SB")  //  SUB_BOARD 수 설정확인.
    case  TA_StrCmdSetSubType:      //    0x5343  //  ("SC")  //  SUB_BOARD 형태 설정.
    case  TA_StrCmdChkSubVersion:      //    0x5344  //  ("SD")  //  SUB_BOARD 버젼 확인. 
    case  TA_StrCmdSetFire:      //    0x4641  //  ("FA")  //  화재 동작 설정.
    case  TA_StrCmdSetAntiPassRule:      //    0x5041  //  ("PA")  //  Anti-passback rule 개별설정. 
    case  TA_StrCmdChkAntiPassRule:      //    0x5042  //  ("PB")  //  Anti-passback rule 개별설정확인. 
    case  TA_StrCmdDelAntiPassRule:      //    0x5043  //  ("PC")  //  Anti-passback rule 개별삭제.
    case  TA_StrCmdDelAntiPassAll:      //    0x5044  //  ("PD")  //  Anti-passback rule 전체설정.
    default://모조리 단순 리턴..
        uLength =  8; //1 + 7 return value 1byte.. '0' or '1' and 현재 진행정도 %
    	break;
    }
      
    return uLength;
}

void SetSSDPSendFireAlarm(void)
{
  strncpy( udp_send_buffer, "Fire!", 5);
  SSD_len = 5;
  SSDPSendReadyFlag = TRUE;
}

void SSDPReportMyMAC(void)
{
  strncpy( udp_send_buffer, "MyMc!", 5);
  udp_send_buffer[5] = CMSParameter.MacAddress[0];
  udp_send_buffer[6] = CMSParameter.MacAddress[1];
  udp_send_buffer[7] = CMSParameter.MacAddress[2];
  udp_send_buffer[8] = CMSParameter.MacAddress[3];
  udp_send_buffer[9] = CMSParameter.MacAddress[4];
  udp_send_buffer[10] = CMSParameter.MacAddress[5];
  SSD_len = 11;
  SSDPSendReadyFlag = TRUE;
}

void SSDPReportMyIp(void)
{
  strncpy( udp_send_buffer, "MyIp!", 5);
  udp_send_buffer[5] = CMSParameter.MacAddress[0];
  udp_send_buffer[6] = CMSParameter.MacAddress[1];
  udp_send_buffer[7] = CMSParameter.MacAddress[2];
  udp_send_buffer[8] = CMSParameter.MacAddress[3];
  udp_send_buffer[9] = CMSParameter.MacAddress[4];
  udp_send_buffer[10] = CMSParameter.MacAddress[5];

  udp_send_buffer[11] = CMSParameter.IPAddress[0];
  udp_send_buffer[12] = CMSParameter.IPAddress[1];
  udp_send_buffer[13] = CMSParameter.IPAddress[2];
  udp_send_buffer[14] = CMSParameter.IPAddress[3];

  udp_send_buffer[15] = 0x38;//TACS_TCP_PORT 14443    //CMSParameter.TAPort[0];
  udp_send_buffer[16] = 0x6b;//TACS_TCP_PORT 14443    //CMSParameter.TAPort[1];
  
  udp_send_buffer[17] = CMSParameter.SubnetMask[0];
  udp_send_buffer[18] = CMSParameter.SubnetMask[1];
  udp_send_buffer[19] = CMSParameter.SubnetMask[2];
  udp_send_buffer[20] = CMSParameter.SubnetMask[3];

  udp_send_buffer[21] = CMSParameter.GatewayAddress[0];
  udp_send_buffer[22] = CMSParameter.GatewayAddress[1];
  udp_send_buffer[23] = CMSParameter.GatewayAddress[2];
  udp_send_buffer[24] = CMSParameter.GatewayAddress[3];

  SSD_len = 25;
  SSDPSendReadyFlag = TRUE;
}

void SSDPSetMyIp(void)
{
//  INT8U BufData[6];
  TACommTaskStep = TAComm_End;

  memcpy(&CMSParameter.MacAddress[0],     &udp_recv_buffer[9],  6);
  memcpy(&CMSParameter.IPAddress[0],      &udp_recv_buffer[15], 4);
  memcpy(&CMSParameter.SubnetMask[0],     &udp_recv_buffer[21], 4);
  memcpy(&CMSParameter.GatewayAddress[0], &udp_recv_buffer[25], 4);
  EEPROMSetDataBlock(E2P_Para_CMS, (INT8U*)&CMSParameter.IPAddress[0], 22);
//  
//  memcpy( &BufData[0], &udp_recv_buffer[9], 6);
//  WriteMacAddress((INT8U *)BufData);
//  
//  memcpy( &BufData[0], &udp_recv_buffer[15], 4);
//  WriteIPToEeprom((INT8U *)BufData);
//  
//  memcpy( &BufData[0], &udp_recv_buffer[19], 2);
////  WritePortToEeprom((INT8U *)BufData);
//  
//  memcpy( &BufData[0], &udp_recv_buffer[21], 4);
//  WriteSnToEeprom((INT8U *)BufData);
//  
//  memcpy( &BufData[0], &udp_recv_buffer[25], 4);
//  WriteGwToEeprom((INT8U *)BufData);
            
  ReloadParameter();
  OPEN_Ethernet();
}


void UdpSetDNSServer(void)
{
    TACommTaskStep = TAComm_End;

//    INT16U WriteAddr = (INT16U)(E2P_Para_CMS+EEPROM_PARAM_INDEX_DNSS);
    if( *(udp_recv_buffer+6) == 1) {
 //       EEPROMSetData(WriteAddr, SMART_DIAPER_DNSSERVER);
    }
    else {
//        EEPROMSetData(WriteAddr, SMART_DIAPER_DESKTOP);
    }                      
    
    ReloadParameter();
    OPEN_Ethernet();
}
/******************************************************************************
**  UDP Process
******************************************************************************/
void UdpServerProcess(void)
{
	static INT8U UdpServerProcessTimer = 0;
	static INT16U UDPResetCounter = 0;

	INT8U Step_Result = FALSE;
	INT16U Recv_len = 0;

	if(UdpServerProcessTimer++ < 2)
		return; //every 20ms
	UdpServerProcessTimer = 0;

	switch(UDP_Step)
	{
	case UDP_Start:
	{
		UDPResetCounter = 0;
		EthernetSocketDisconnect(Socket_FIRE);
		EthernetSocketClose(Socket_FIRE);

		rprintf("UDP: UDP Process Start...\r\n");
		UDP_Step = UDP_Init;
		break;
	}
	case UDP_Init:
	{
	  Step_Result = UdpProcess(Socket_FIRE, UDP_Init); // Read Status
	  switch (Step_Result)
	  {
	  case SOCK_UDP:
		UDP_Step = UDP_Receive;
		UDPResetCounter = 0;
		break;
	  case SOCK_CLOSED:          /* if a socket is closed */
		Step_Result = UdpProcess(Socket_FIRE, UDP_Open);
		UDPResetCounter = 0;
		rprintf("UDP: %d : UDP Started. port : %d...\r\n", Socket_FIRE, UDP_SRV_PORT_FIRE );
		break;
	  }
	  if(UDPResetCounter++ > 1200)   // 10mSec * 1200 * 20mSec  = 240Sec
	  {
		UDPResetCounter = 0;
		rprintf("UDP: UDP Process Reset\r\n");
		UDP_Step = UDP_Start;
	  }
	  break;
	}
	case UDP_Send:
	{
		if (UdpProcess(Socket_FIRE, UDP_Send) == TRUE)
			rprintf("UDP: UDP Send Success...\r\n");
		UDP_Step = UDP_Init;
		break;
	}
	case UDP_Receive:
	{
	  Recv_len = UdpProcess(Socket_FIRE, UDP_Receive);
	  if( (Recv_len != 0xFF) && (Recv_len > 0) )
	  {
		rprintf("UDP: UDP Receive (%d) Success [%s].\r\n", Recv_len, udp_recv_buffer);
		if( strncmp((const char *)udp_recv_buffer , "Fire!", 5) == 0)
		{
			if(FireLockFlag != 99)
			{  //EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
				FireOccurrence = TRUE;
				EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag,  119);
			}
			else
			{
				FireOccurrence = 99; //화재인데 화재작동 안하는 경우는 여기에 해당함.. 노말모드로..
			}

#ifdef ENABLE_FIRE_ANNOUNCE  //udp 로 화재 전파기능이 있을 때..

	//			if( httpc_isConnected)  // http 서버 모드 이면 httpclient로 접속에서 연결되었으면 보내는 걸로 바꿈.
			{
				HTTP_MakeHttpPacketRtLog( 0, HTTP_EVENT_FIRE_CODE, HTTP_VERIFYTYPE_EVENT, Result_OK);
			}
			//    	else						// 소켓통신 TCP/IP 인터페이스로 접속
			{
				memcpy(TABuf_Tx+TA_INDEX_DAT,"\x30\x49\x46\x00\x01",5);//"IF"
				CopyYYMMDDhhmmss(TABuf_Tx+TA_INDEX_EVENT_TIME);//funcCopyEventDateTime_to_TABuf_Tx();
				memcpy(TABuf_Tx + TA_INDEX_EVENT_INPUT, LockParameter.DoorLockMode, 8);

				TAMakeResponsePacket(TA_StrCmdGetEvent);//TA_StrCmdGetEvent  0x4542  //  ("EB")  //  EVENT UPLOAD
			}
#endif
			rprintf("UDP: Set Fire %s\r\n", udp_recv_buffer);
		}
		else if( strncmp((const char *)udp_recv_buffer , "GetMc", 5) == 0)
		{
		  TANeedUdpCheck = TRUE;
		  SSDPReportMyMAC();
		  rprintf("UDP: Requested MAC From %d:%d:%d:%d port %d.\r\n", recv_addr[0], recv_addr[1], recv_addr[2], recv_addr[3], recv_port);
		}
		else if( strncmp(udp_recv_buffer , "GetIp", 5) == 0)
		{
		  TANeedUdpCheck = TRUE;
		  SSDPReportMyIp();
		  rprintf("UDP: Requested IP From %d:%d:%d:%d port %d.\r\n", recv_addr[0], recv_addr[1], recv_addr[2], recv_addr[3], recv_port);
		}
		else if( strncmp((const char *)udp_recv_buffer , "SetIp", 5) == 0)
		{
	/*          if( (udp_recv_buffer[5] == CMSParameter.IPAddress[0]) &&
			  (udp_recv_buffer[6] == CMSParameter.IPAddress[1]) &&
			  (udp_recv_buffer[7] == CMSParameter.IPAddress[2]) &&
			  (udp_recv_buffer[8] == CMSParameter.IPAddress[3]) )
		  {*/
			TANeedUdpCheck = TRUE;
			SSDPSetMyIp();
			rprintf("UDP: Requested Set Same TA %d.%d.%d.%d port %d.\r\n", recv_addr[0], recv_addr[1], recv_addr[2], recv_addr[3], recv_port);

	//       	NVIC_SystemReset();	// System Reset
	/*         }
		  else
		  {
			rprintf("UDP: Requested Set another TA %d.%d.%d.%d.\r\n", udp_recv_buffer[5], udp_recv_buffer[6], udp_recv_buffer[7], udp_recv_buffer[8]);
		  }*/
		}
		else if( strncmp((const char *)udp_recv_buffer , "SetSrv", 6) == 0)
		{
			UdpSetDNSServer();
			TANeedUdpCheck = TRUE;
			rprintf("UDP: Requested Set IP From %d:%d:%d:%d port %d.\r\n", recv_addr[0], recv_addr[1], recv_addr[2], recv_addr[3], recv_port);
		}
		else
		{
		  rprintf("UDP: Requested Unknown data.\r\n");
		}
	  }
	  UDP_Step = UDP_Send;
	  break;
	}
	}
}

/**
 * @brief  This function processes the SSDP message.
 * @return  0: success, -1: reply packet timeout, 1: received SSDP parse error
 */
INT8U UdpProcess(INT8U s, INT8U Action)
{
	uint8_t status;
	datasize_t ret;
	static uint8_t addr_len;

  if(Action == UDP_Init) // Read State
  {
//    return EthernetRegisterRead(Sn_SR(s));
	  getsockopt(s, SO_STATUS,&status);
	  return status;
  }
  else if(Action == UDP_Send)  // 
  {
    if( SSDPSendReadyFlag != FALSE)
    {
      if( TANeedUdpCheck == TRUE)
      {
        bcast_addr[0] = 255;
        bcast_addr[1] = 255;
        bcast_addr[2] = 255;
        bcast_addr[3] = 255;
      }
      else 
      {
        bcast_addr[0] = CMSParameter.IPAddress[0];
        bcast_addr[1] = CMSParameter.IPAddress[1];
        bcast_addr[2] = CMSParameter.IPAddress[2];
        bcast_addr[3] = 255;
      }
      // Broadcasting Setting
      //  if(UDPSend(s, (const INT8U*)SSDP, strlen(SSDP), mcast_addr, 15693) <= 0) 
      //    rprintf(DEBUG_IPM,"SSDP Send error!!!!!!!\r\n");
      
      uint8_t sentsize = 0;
      while(sentsize != SSD_len)
      {
    	  ret = sendto(s, udp_send_buffer+sentsize, SSD_len-sentsize, bcast_addr, UDP_SRV_PORT_FIRE, addr_len);
		  if(ret < 0)	break;
		  sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
      }

      SSDPSendReadyFlag = FALSE;
      TANeedUdpCheck = FALSE;
      return TRUE;
    }
    return FALSE;
  }
  else if(Action == UDP_Receive) // 
  {
    //  while (UDPRecv(s, (INT8U*)udp_recv_buffer, UDP_RECV_BUFFER_SIZE, recv_addr, &recv_port) <= 0 && my_time < endTime);  // Check Receive Buffer of W5200
    getsockopt(s, SO_RECVBUF, &RSR_len);
    if(RSR_len == 0)
        return 0xFF;

    if (RSR_len > UDP_RECV_BUFFER_SIZE) 
        RSR_len = UDP_RECV_BUFFER_SIZE;

    ret = recvfrom(s, udp_recv_buffer, RSR_len, (uint8_t*)recv_addr, (uint16_t*)&recv_port, &addr_len);
	if(ret <= 0)
		return ret;
  }
  else // UDP_Open
  {
    // UDP Socket Open
    //  if(UDPOpen(s, UDP_SRV_PORT_FIRE) == FALSE) rprintf(DEBUG_IPM,"UDP Socket Error!!\r\n");
    EthernetSocketOpen(s, Sn_MR_UDP, UDP_SRV_PORT_FIRE);

//          OS_Delay(500);
    memset(udp_recv_buffer, '\0', UDP_RECV_BUFFER_SIZE);
    return TRUE;
  }
  return ret;
}


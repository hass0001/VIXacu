/******************************************************************************
**
**   CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: SCI0Task.c
**   Project: Low Cost CP(730)
**
**   Copyright 2014 TECSEN
**
******************************************************************************/

/******************************************************************************
**
**   Include Files
**
******************************************************************************/
#define    SCI2_485_GLOBALS     

#include   "SCI2Rs485Task.h"

//const INT8U LogOnPassword[4] = {'0','0','0','0'};
//const INT8U CardRange[3] = {8,4,7};
extern DMA_HandleTypeDef hdma_usart1_tx;
extern uint8_t DMA_IsTxComplete(void);
extern void MakeReaderToReportCard(uint8_t * cardId, uint8_t nRes);

uint8_t gGoVect;
int nLoopDelay;
uint8_t bGot485Data;
INT8U ConsoleCommvect;
//INT8U g_ConFpSyncFlag = 0;
//INT8U g_fpUserIndex = 0;
//INT16U gU16ReceiveWaitTime=0;

INT8U gU16RefreshConsoleTimeFlag[MAX_NUM_485CS];

void  RS485_Init(void)
{
    gF485SentChannel = 0;//900mhz용..
    guCsNormalCommand = CS_CMD_ACK;

    Rs485Tx2Enable = 0;
    nRs485PacketIndex = 0;
    ConsoleCommvect = 0;
    gGoVect = 1;
    bGot485Data = 0;
	Console485Disable();
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	HAL_TIM_Base_Start_IT(&htim3);
}

void RS485_SetReceived485Buffer( uint8_t cRs485RecevedChar)
{
    uint16_t   ucCpLength;
    if ( Rs485Tx2Enable == 0 )
    {
 //   	if( nRs485PacketIndex < COMM_RS485_RX_BUF_LEN)
    	nLoopDelay = 0;
    	Rs485CommRx2Buf[nRs485PacketIndex] = cRs485RecevedChar;

        if( Rs485CommRx2Buf[CS_INDEX_STX] == StxStart485_RX)   //0xDD          StxStart485)     //0x7e
        {
              nRs485PacketIndex++;
              if( nRs485PacketIndex > 4 )
              {
                  ucCpLength = (uint16_t)Rs485CommRx2Buf[CS_INDEX_LENGTH]+3; //STX + Length + Checksum ==  3
                  if( ucCpLength > 4 && ucCpLength < 200)
                  {       // NUM_CP_ZONE_NAME 45+4
                      if( nRs485PacketIndex >= ucCpLength )
                      { // consider last ff

							// RS485_TreatCpData();
                    	  	memcpy( CopyRx2Rs485Buf, Rs485CommRx2Buf, nRs485PacketIndex);
						//	memset(CommRx4Rs485Buf, 0, nRs485PacketIndex);
                    	  	Rs485Rx2Length = nRs485PacketIndex;
							nRs485PacketIndex = 0;
							Rs485Tx2Enable = 1;
////							if( Rs485CommRx2Buf[CS_INDEX_CONTROL] != CS_CMD_ACK)
//								printf("ch %02x, idx %d. len %d\r\n", cRs485RecevedChar, nRs485PacketIndex, Rs485Rx2Length);
                    //      osSemaphoreRelease(binSem48503Handle);
                      } // length is different
                      else
                      {   // wait until length
                      }
                  }  // length is available
                  else
                  {
                       nRs485PacketIndex = 0;
                  }
              } // length is less.
              else
              {
              }
        }
        else
        { // handle middle data
            if( nRs485PacketIndex < COMM_CS_DATA_RX_BUF_LEN)
            {
                if( Rs485CommRx2Buf[nRs485PacketIndex] == StxStart485_RX )
                {  // 0xBB
                	Rs485CommRx2Buf[0] = StxStart485_RX;
                    nRs485PacketIndex = 1;
                }
                else if( Rs485CommRx2Buf[nRs485PacketIndex] == 0xFF)
                {  // 0xFF  last
                    nRs485PacketIndex = 0;
                }
                else
                	nRs485PacketIndex++;
            }
            else
            {
                nRs485PacketIndex = 0;  // ignore middle dat, start from the first.
            }
        }
    }
    else
    {  // still in processing previous data
          printf("still %02x, idx %d. en %d\r\n", cRs485RecevedChar, nRs485PacketIndex, Rs485Tx2Enable);
    }
}

void SCI2Rs485Process(void)
{
    uint32_t checkCrc = 0;
#ifdef DEBUG_MODE
    uint16_t n485TaskCnt = 0;
#endif

 //     ConsoleCommProcess();
    if( (bGot485Data == 1) && (ConsoleCommStat[ConsolePollIndex] == CommOK) )
    {
      AnalysisConsoleRxData();    //데이터처리..
      gGoVect = 1;			// 처리가 끝나고 타이머가 다시 호출 해도 됨
    }

	if( bRequestNeed == MON_NEED_SCI2_FWUD)
	{
		checkCrc = getExtFlashCRC( FIRM_START_NEW_ADDR, ulBinLen );
		if( checkCrc == ulBinCrc)
		{
			WriteUpdateInfoFlag( UPDATE_FROM_NEW, UPDATE_NEED );
			rprintf("[NOTICE]System Reset for Upgrade!\r\n");
			NVIC_SystemReset();  // System Reset
		}
		else
		{
			bRequestNeed = MON_FROM_USER;
			rprintf("[NOTICE]Checksum Error!\r\n");
		}
		Firmware_Upgrade = USER_UPGRADE_NONE;
 //               OS_ResumeAllSuspendedTasks();
        /*
        */
    }
    else
    {
    	  if( Firmware_Upgrade == SCI2_UPGRADE_YES)
    	  {
    		  ftimeSciOut++;
    		  if( ftimeSciOut > 500)  /// About 50 seconds
    		  {
                    bRequestNeed = MON_FROM_USER;
                    rprintf("[NOTICE]Firmware SCI2PC Update Waiting Time is over!\r\n");
                    Firmware_Upgrade = USER_UPGRADE_NONE;
 //         OS_ResumeAllSuspendedTasks();
    		  }
		  }
	}

	TaskRunCnt(ENUM_SCI2RS485_TASK);          // Notice Activated.

#ifdef DEBUG_MODE
        n485TaskCnt++;
        if( n485TaskCnt > 50000)
            n485TaskCnt = 0;

        if ((n485TaskCnt % 500) == 0) {  // 100 mS * 500 = 50 Sec
                rprintf(" SCI2 485 Task is Alive.\r\n");
    #ifdef HISTORY_TEST
              SetCardDataReceived();
    #endif
		}
#else
 //      IWDG_ReloadCounter();
#endif
}
/******************************************************************************
**   Console RS485 Port Control
******************************************************************************/
void Console485Enable(void)
{
//#if defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000 에서만 위갠드, lcd 작동...
	HAL_GPIO_WritePin( RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET);   // G7
}

void Console485Disable(void)
{
//#if defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000 에서만 위갠드, lcd 작동...
	HAL_GPIO_WritePin( RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);// Clear bit on port
}

void ConsoleReset485Timer(void)
{
}

/******************************************************************************
**   Console Communication Process
******************************************************************************/
void ConsoleCommProcess(void)
{
    gGoVect = 0;

	switch(ConsoleCommvect)
	{
    case 0:
      _RS485_LED_OFF();// LED Off
      bGot485Data = 0;
	  nLoopDelay = 0;
      if ( ConsolePollingConfig() == TRUE )    //송신
    	  ConsoleCommvect++;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 1:
      Console485Enable();
      ConsoleCommvect++;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 2:
      ConsoleDataSendStart();
      ConsoleCommvect++;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 3:
      if ( DMA_IsTxComplete() )   // 전송이 끝나면
    	ConsoleCommvect++;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 4:
      Console485Disable();
      ConsoleCommvect++;
      Rs485Tx2Enable = 0;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 5:
      if( ConsoleDataReceive() == TRUE )    //수신
      {
    	  _RS485_LED_ON();// LED On
    	  bGot485Data = 1;
    	  ConsoleCommvect++;
      }
      else
      {
		nLoopDelay++;
		if( nLoopDelay > 100)     // 수신 타임 아웃 일 경우 300ms
		{
			ConsoleErrorCounter[ConsolePollIndex]++;
			if(ConsoleErrorCounter[ConsolePollIndex] >= 20)
			{
				if(ConsoleTrouble[ConsolePollIndex] == FALSE)
				{
					ConsoleTrouble[ConsolePollIndex] = TRUE;
					gU8ConsoleAlive &= 0xFF^(0x80>>ConsolePollIndex);  //1234 5678 해당비트를 0으로..
																		//xor 다르면 true, 같으면 false
					//            rprintf( "SCI2 485 No Res %d", gReader900Alive);
				}
				ConsoleErrorCounter[ConsolePollIndex] = 0;
			}
			ConsoleCommvect = 0;
         }
      }
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
    case 6:    //처리
      ConsoleCommvect = 0;  // HAL_Delay를 없애려 메인에서 처리 되어 이동
      break;
    default:
      ConsoleCommvect = 0;
      gGoVect = 1;			// 타이머가 다시 호출 해도 됨
      break;
  }
}


/******************************************************************************
**   Console Data Send Start
******************************************************************************/
void ConsoleDataSendStart(void)
{
	static uint32_t nRs485ComCnt;
	while( Rs485Tx2Ctr > 0 )
		MyMsDelay(1);

    Rs485Tx2Ctr = (INT8U)(Rs485CommTx2Buf[1] + 3);
    Rs485Tx2Ptr = &Rs485CommTx2Buf[0];

#if 0
		uint8_t nLength;
		if( Rs485Tx2Ctr > 70) nLength = 70;
		else     nLength = Rs485Tx2Ctr;

		uint8_t strMsg[300], strHex[15], ni;
		sprintf((char *)strMsg, "485 T1 [");
		 for( ni = 0; ni < nLength; ni++)
		 {
			 sprintf((char *)strHex, "%02x ", Rs485CommTx2Buf[ni]);
			 strcat((char *)strMsg, (char const *)strHex);
		 }
		 sprintf((char *)strHex, "...]%d, %x\r\n", ni, nRs485ComCnt);
		 strcat((char *)strMsg, (char const *)strHex);

		 rprintf((char const *)strMsg);
#else
//                trace("send");
//                dumpd(CommTx4Rs485Buf, nLen);
#endif

    Tx2DmaIng = 1;
//	HAL_UART_Transmit_DMA(&huart1, Rs485Tx2Ptr, Rs485Tx2Ctr);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);

	 while(HAL_IS_BIT_CLR(huart1.Instance->ISR, UART_FLAG_TXE))
		 MyMsDelay(1);

//      if(( CopyRx2Rs485Buf[CS_INDEX_CONTROL] != CONTROL_CS_ACK) ||
//	      ( Rs485CommTx2Buf[CS_INDEX_CONTROL] != CONTROL_CS_ACK) )
//       {
//         if( (CommTx4Rs485Buf[INDEX_BT_STX] == IRDA_STX) || (CommTx4Rs485Buf[INDEX_CS_ID] == 0x03)) {
//       }

////	while(HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);
//	while(HAL_DMA_GetState(&hdma_usart1_tx) != HAL_DMA_STATE_READY);

   	nRs485ComCnt++;
}

/******************************************************************************
**   Console Polling Config
******************************************************************************/

INT8U ConsolePollingConfig(void)
{
  INT8U LastCommStat;

  if(ConsolePollIndexSelect() == ERROR)
  {
          return FALSE;
  }

  LastCommStat = ConsoleCommStat[ConsolePollIndex];

  switch(LastCommStat)
  {
    case CommOK:        //#define CommOK              0x0    //normal
            ConsoleTxDataLoad();  //송신 버퍼 복사
            break;
    case CommNak:
            ConsoleAckPacking();  //Nack Receive일 시, ACK 복사
            break;
    case CommChkErr:
            ConsoleCommStat[ConsolePollIndex] = CommOK;
            ConsoleNackPacking();  //수신 데이터가 Error 일 시, Nack 복사
            if(ConsoleTrouble[ConsolePollIndex] == TRUE)
            {
                    ConsoleRestoreCounter[ConsolePollIndex] = 0;
            }
            break;
    case CommNoRes:
            if(ConsoleTrouble[ConsolePollIndex] == TRUE)  // 응답이 없는 경우 && 콘솔 트러블이 True이면 버퍼 클리어 후 ACK를 보낸다.
            {
                    ConsoleRestoreCounter[ConsolePollIndex] = 0;
                    ConsoleAckPacking();
            }
            else                // 응답이 없는 경우 && 콘솔 트러블이 아닌 상태이면 송신 한 것으로 가정하여 버퍼 아웃 카운터 추가 하고 ACK를 보냄
            {
                    ConsoleTxDataLoad();
            }
            break;
    default:
            ConsoleAckPacking();	// ACK 복사
            break;
    }
    return TRUE;
}

/******************************************************************************
**
**   Polling Index Select Method
**
******************************************************************************/
INT8U ConsolePollIndexSelect(void)
{

	  INT8U i;

	  ConsolePollIndex++;

	  for (i=ConsolePollIndex; i < MAX_NUM_485CS; i++ )
	  {
	    if( ConsoleParameter.Type[i] != FALSE)
	    {
	        ConsolePollIndex = i;
	        return SUCCESS;
	    }
	  }
	  ConsolePollIndex = 0;
	  for (i=ConsolePollIndex; i < MAX_NUM_485CS; i++ )
	  {
	    if(ConsoleParameter.Type[i] != FALSE)
	    {
	        ConsolePollIndex = i;
	        return SUCCESS;
	    }
	  }
	  return ERROR;

}


/******************************************************************************
**
**   Load Data to Console
**
******************************************************************************/

void ConsoleTxDataLoad(void)
{
  INT8U LastCommLoad;

  LastCommLoad = ConsoleLoadCtrlType[ConsolePollIndex];
  
  switch(LastCommLoad)
  {
    case LoadNormal://#define LoadNormal              0x0    // normal
      ConsoleCopyPacking();//일반적으로 통신이 되고 있을 때는 여기로 와서 tx버퍼의 내용을 복사해서 전송함..
      break;
      
    case LoadPrepairBuffer:     //#define LoadPrepairBuffer        0x1    // flash data
      ConsoleCopyPacking();
      ConsoleLoadCtrlType[ConsolePollIndex] = LoadDirectBuffer;
      break;
      
    case LoadDirectBuffer:      //#define LoadDirectBuffer        0x2    // flash data
      if( ConsoleLoadCtrlCount[ConsolePollIndex] > 0)
      {
          ConsoleGetCardBufferDirect();
      }
      else 
      {
          ConsoleCopyPacking();
          ConsoleLoadCtrlType[ConsolePollIndex] = LoadNormal;
      }
      break;
      
    default:    //#define LoadConsoleParam        0x3    // Eeprom parameter
                //#define LoadNoResource           0x4   //Error
      ConsoleAckPacking();
      break;
  }
}

void ConsoleCopyPacking(void)
{
  INT8U *Dest,*Source;
  INT16U length = 0;
    
  Dest = &Rs485CommTx2Buf[0];
  Source = &ConsoleTxBuffer[ConsolePollIndex][0];

  if(*(Source) == StxStart485_TX)   //0xBB          StxStart485)     //0x7e
  {
    length = *(Source+1);
    memcpy(Dest, Source, length+3);
    *(Source) = 0x0;//복사 후 버퍼를 0로.. 없는것처럼..
  }
  else 
  {
    if(toDayTime.MIN % 10 == 0) { //10분마다..
      if(toDayTime.MIN != gU16RefreshConsoleTimeFlag[ConsolePollIndex]) {//저장값과 다르면 //시간바꾸고..
    	  gU16RefreshConsoleTimeFlag[ConsolePollIndex] = toDayTime.MIN;
    	  ConsolePowerOnMethod();
      }
      else {
        ConsoleAckPacking();
      }
    }
    else {
      ConsoleAckPacking();
    }
//      gU16ReceiveWaitTime = 2000;       //      OS_Delay(2000); // 2000 mS
  }
}

/******************************************************************************
**
**   ACK
**
******************************************************************************/

void ConsoleAckPacking()
{
  INT8U *Temp;
  Temp = &Rs485CommTx2Buf[0];
  
  if(ConsoleNeedSetTime[ConsolePollIndex] == 1) {
	  ConsoleTimeSetCsType2(ConsolePollIndex);
	  ConsoleNeedSetTime[ConsolePollIndex] = 0;
  }
  else {
	  *Temp++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e   //CS_INDEX_STX         0
	  *Temp++ = 0x02;//CS_INDEX_LENGTH         1
	  *Temp++ = guCsNormalCommand;//CONTROL_CS_ACK         2
	  *Temp++ = ConsolePollIndex;//CS_INDEX_ID        3
	  *Temp++ = CheckSum485(&Rs485CommTx2Buf[1]);
  }
}

/******************************************************************************
**
**   NACK
**
******************************************************************************/

void ConsoleNackPacking()
{
  INT8U *Temp;
  Temp = &Rs485CommTx2Buf[0];
  *Temp++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
  *Temp++ = 0x02;//CS_INDEX_LENGTH         1
  *Temp++ = CS_CMD_NACK;
  *Temp++ = ConsolePollIndex;
  *Temp++ = CheckSum485(&Rs485CommTx2Buf[1]);
}
/******************************************************************************
**
**   RF Channel Setting
**
******************************************************************************/
void ConsoleRfChannelPacking()
{
	INT8U *Temp;
	Temp = &Rs485CommTx2Buf[0];
	*Temp++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
	*Temp++ = 0x03;//CS_INDEX_LENGTH         1
	*Temp++ = CS_CMD_SET_RF;
	*Temp++ = ConsolePollIndex;
   	*Temp++ = gst900Channel.nCurrZone;
	*Temp++ = CheckSum485(&Rs485CommTx2Buf[1]);
}

/******************************************************************************
**
**   Make Checksum
**
******************************************************************************/

INT8U CheckSum485( INT8U *SumPtr )
{
  INT8U sumbuf=0,len,i;

  len = *SumPtr;
  for( i=0 ; i<len+1 ; i++ )  {
    sumbuf += *(SumPtr)++;
  }
  return sumbuf;
}

/******************************************************************************
**
**   Console Data Receive
**
******************************************************************************/

INT8U ConsoleDataReceive(void)
{
  if( Rs485Tx2Enable == 1)
  {
            if( (CheckSumVerify() == ChkGood) && (CopyRx2Rs485Buf[3] == ConsolePollIndex) )        //checksum ok.. and if target console ID..
            {
              ConsoleCommStat[ConsolePollIndex] = CommOK;
              ConsoleErrorCounter[ConsolePollIndex] = 0;

              if(ConsoleTrouble[ConsolePollIndex] == TRUE)
              {
                ConsoleRestoreCounter[ConsolePollIndex]++;
                if(ConsoleRestoreCounter[ConsolePollIndex] >= 5)
                {
                  ConsoleTrouble[ConsolePollIndex] = FALSE;
                  gU8ConsoleAlive |= (0x80>>ConsolePollIndex);  //1234 5678 해당비트를 1로..
                  ConsoleRestoreCounter[ConsolePollIndex] = 0;
                }
              }
#ifdef LOG_DEBUG
              INT16U i = 0;
              INT16U length = 0;
              length = (INT16U)CopyRx2Rs485Buf[1];
              if(CopyRx2Rs485Buf[2] != 0x02)
              {
                  rprintf( "[Console Recv]: ");
                  for(i = 0; i<length+3; i++)
                  {
                      rprintf( "%02X ", CopyRx2Rs485Buf[i]);
                  }
                  rprintf( "\r\n");
              }
#endif
              return TRUE;
            }
            else
            {
              rprintf( "Console Data CRC Error\r\n");
              return TRUE;
            }
  }
  return FALSE;
}

/******************************************************************************
**
******************************************************************************/
INT8U CheckSumVerify( void )
{
  INT8U  sumbuf=0,len,*SumPtr,i,RxCs;

  len = CopyRx2Rs485Buf[1];
  SumPtr = &CopyRx2Rs485Buf[1];
  for( i=0 ; i<len+1 ; i++ ){
      sumbuf += *(SumPtr)++;
  }
  RxCs = CopyRx2Rs485Buf[len+2];
  if( RxCs == sumbuf ) return ChkGood;
  else return ChkErr;
}

/******************************************************************************
**
******************************************************************************/

uint16_t u16CardResult;

void AnalysisConsoleRxData(void)
{
	INT8U u8Port, u8Result;//Ctrl, nPort;
	uint16_t u16CardResult;
	INT8U ReceiveTime[7];
	COMM_CS_CONTROL_TYPE Control;

	Control = (COMM_CS_CONTROL_TYPE) CopyRx2Rs485Buf[CS_INDEX_CONTROL];//#define CS_INDEX_CONTROL         2
	if( (Control !=  CS_CMD_ACK ) && (Control !=  CS_CMD_NACK ) )
	{
	//	  guCsGotCommand = true;
	}

	switch(Control)
	{

/************************************  cr747 방식  ***********************************************************/
    case CONTROL_CS_POR:// 0xb0
		ConsolePowerOnMethod();   //현재 암꺼두 안함..
		break;

    case CONTROL_CS_AWAYSTAY:   //CONTROL_CS_AWAYSTAY           = 0xB1
//      ConsoleTamperStatusSave();
    case CONTROL_CS_ACCREQ:     //CONTROL_CS_ACCREQ    	        = 0xB2
//cr747 방식은 CONTROL_CS_AWAYSTAY 0xb1의 데이터는 Time(6Byte) + Card ID(8Byte) + Away/Stay Data 근데.. 앞쪽부터 카드값임.. 12345678 00000000
//cr747 방식은 CONTROL_CS_ACCREQ   0xb1의 데이터는 Time(6Byte) + Card ID(8Byte) + Access Type 
    	if( g_isSetReader == 0)
    	{
    		// CopyRx2Rs485Buf[CS_INDEX_ID] 에 의한 설정 도어 번호, 사용자 설정에서 출입문 사용 권한을 다르게 주면 안 됨.
    		uint8_t n485Door = ConsoleParameter.DoorNumber[CopyRx2Rs485Buf[CS_INDEX_ID]];
    		u16CardResult = CheckRfCardDataInFlash(CopyRx2Rs485Buf + CS_INDEX_DATA + 6);
			u8Result = CheckEventCardAndLog( n485Door, CopyRx2Rs485Buf +CS_INDEX_DATA+ 6, u16CardResult);
			if(u8Result == 0)
			{   //  #define  Result_OK  0
				ConsoleResultPack(1); //  {  IDX_AUDIOMENT_DOOR_OPEN, IDX_AUDIOMENT_MUTE_100MS}, // 0x01

//				memcpy(&SdpParameter.CardData[0], &CopyRx2Rs485Buf[CS_INDEX_DATA+6], 8);
				//		RESTClient_SetPacketReady( UHS_Type_AccessInfo102);
			}
			else
			{
				ConsoleResultPack(2); // {  IDX_AUDIOMENT_NO_REGISTERD_CARD, IDX_AUDIOMENT_CHECK_USE_AGAIN}, // 0x02
			}
			ConsoleAccessMethod();
    	}
    	else
    	{   // 모니터 프로그램에서 카드 등록 몯로 설정한 경우
    		memcpy(CardParameter.stParam.CardId, &CopyRx2Rs485Buf[CS_INDEX_DATA+6], 4);
//    	    INT8U CardId[4];			// ID 4
//    	    INT8U DoorAuth_CardKind;                 // 1 : 상시 ID.	2 : 일시 ID.	3 : 당일카드 ID.
//    	    INT8U PassCode[2];      // 카드+비밀번호 사용시) 현재는 0x00, 0x00 으로 전송
    		memcpy(CardParameter.stParam.Pin, "48500485", 8);
    		CardParameter.stParam.DoorAuth_CardKind  = 0x1;//■ 카드종류/도어/안티패스 형식 : 1111 도어, 1 안티패스, 111 종류
	        CardParameter.stParam.PassCode[0] = 0x99;//여기에 사용기한이 julian date 로 2바이트가 들어간다..
	        CardParameter.stParam.PassCode[1] = 0x31;//여기에 사용기한이 julian date 로 2바이트가 들어간다..
	        CardParameter.stParam.Location = 0;
    	    uint8_t nResult = WriteRfCardDataToSerialFlash(CardParameter.u8strParam);
    		MakeReaderToReportCard(&CopyRx2Rs485Buf[CS_INDEX_DATA+6], nResult);
    	}

		break;
    case CONTROL_CS_CURR_DISPREQ://0xb3
//      ConsoleReportDevStatus();
    case CONTROL_CS_DETD_DISPREQ://0xb4
    case CONTROL_CS_TAMPER:              //0xB6
    case CONTROL_CS_MKEY_DOWNLOADREQ:    //0xBF  //Mkey 보내랏..
    case CONTROL_CS_MKEY_UPLOAD:         ////Mkey 업로드 한다!
		break;
/************************************  cr747 방식  ***********************************************************/
      
    case ConSetTime://#define ConSetTime            0xc0
		ConsoleTimeGetPc();
		break;

    case 0xC1:  //날짜,시간설정 결과 YY MM DD WD hh mm ss
		ReceiveTime[0] =  (CopyRx2Rs485Buf[4]  >99)?99:CopyRx2Rs485Buf[4];  // 년(1)   : 0x09 -> 09년
		ReceiveTime[1] = ((CopyRx2Rs485Buf[5] == 0)||(CopyRx2Rs485Buf[5] >12))? 1:CopyRx2Rs485Buf[5];  // 월(1)   : 0x0b -> 11월
		ReceiveTime[2] = ((CopyRx2Rs485Buf[6] == 0)||(CopyRx2Rs485Buf[6] >31))? 1:CopyRx2Rs485Buf[6];  // 일(1)   : 0x0b -> 11일
		ReceiveTime[3] =  (CopyRx2Rs485Buf[8]  >23)? 0:CopyRx2Rs485Buf[8];  // 시(1)    : 0x0b -> 11시
		ReceiveTime[4] =  (CopyRx2Rs485Buf[9]  >59)? 0:CopyRx2Rs485Buf[9];  // 분(1)    : 0x0b -> 11분
		ReceiveTime[5] =  (CopyRx2Rs485Buf[10] >59)? 0:CopyRx2Rs485Buf[10];  // 초(1)    : 0x0b -> 11초
		ReceiveTime[6] =  (CopyRx2Rs485Buf[7]  > 6)? 0:CopyRx2Rs485Buf[6];  // 요일(1) : 일(0x00) 월(0x01) 화(0x02) 수(0x03) 목(0x04) 금(0x05) 토(0x06).

		//     SetToDay(ReceiveTime);
		RTC_SetTimeAndDate( &toDayTime, &ReceiveTime[0] );
		EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime,(INT8U*)ReceiveTime,7);

		break;
    case CONTROL_CS_OPEN_RESULT:  //?? 초간 도어오픈 결과 "- Relay Number(1) : 0 - 1 사이의 수. -time(1) :  1~250
		TATreatControl(CopyRx2Rs485Buf[5], CopyRx2Rs485Buf[4]%2);
		break;
    case CONTROL_CS_CHANGE_LOCK_MODE:  // Lock mode 변경 - Relay Number(1) : 0 - 1 사이의 수. -Lock mode(1) :  0.수동풀림  1.수동잠금  2.일반모드(스케줄에따름)
        if( CopyRx2Rs485Buf[5] == 0) {//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          // SetLockOn( InputId);
          ChangeLockMode( LockMode_Manual_Unlock, CopyRx2Rs485Buf[4]%2);
        }
        else if( CopyRx2Rs485Buf[5] == 1)	{//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          // SetLockOff( InputId);
          ChangeLockMode( LockMode_Manual_Lock, CopyRx2Rs485Buf[4]%2);
        }
        else if( CopyRx2Rs485Buf[5] == 2)	{//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드
          if((CMSParameter.DoorTimeCode[0][CopyRx2Rs485Buf[4]%2] > 0)||(CMSParameter.DoorTimeCode[1][CopyRx2Rs485Buf[4]%2] > 0)||(CMSParameter.DoorTimeCode[2][CopyRx2Rs485Buf[4]%2] > 0)){//타임코드가 있으면..
            ChangeLockMode( LockMode_Schedule_Lock, CopyRx2Rs485Buf[4]%2);//일단 스케줄 Lock 상태로 바꿔버림..
          }
          else {
            ChangeLockMode( LockMode_Normal, CopyRx2Rs485Buf[4]%2);
          }
        }
        else {  // if( CommSCI2485RxBuf[5] == 3)	{//0 수동풀림, 1 수동잠금, 2 스케줄, 3 일반모드  
            ChangeLockMode( LockMode_Normal, CopyRx2Rs485Buf[4]%2);
        }
        break;
    case CONTROL_CS_CHANGE_OPENTIME:  // door open time 변경  - Relay Number(1) : 0 - 1 사이의 수. -door open time 1~250
        break;
    case CONTROL_CS_REGIST_TIMECODE:  // time code 저장 등록0/삭제1/전부삭제2(1), Time Code Number(1):1~255,  (Start Time(2) End Time(2))*8
        break;
    case CONTROL_CS_CHANGE_SCHEDULE:  // 스케줄 번호 변경 0은 스케쥴 없음..  LockNumber(1) Time Code Number(1)
        break;
    case CONTROL_CS_REGIST_HOLIDAY:  // 휴일 등록,삭제 결과  등록0/삭제1/전부삭제2 (1), MM DD
        break;
    case CONTROL_CS_RESULT_REBOOT:  // 리부트 명령 수행 결과..
      break;
      
    case ConFirmwareStart:     // 0x0d0
      ConsoleTreatFirmwareStart();
      break;

    case ConFirmwareHeader:        // 0x0d1
      ConsoleTreatFirmwareHeader();
      break;

    case ConFirmwareData:       //       0x0d2
      ConsoleTreatFirmwareData();
      break;
      
    case ConFirmwareTail:       //      0x0d3
      ConsoleTreatFirmwareTail();
      break;

    case ConFirmwareStop:       //      0x0d4
      ConsoleTreatFirmwareStop();
      break;

    case ConSetCardData:       //      0x0d5
      ConsoleSetCardData();
      break;

    case ConCheckCardData:       //      0x0d6
      ConsoleCheckCardData();
      break;

    case ConDeleteCardData:       //      0x0d7
      ConsoleDeleteCardData();
      break;

    case ConClearCardBuffer:       //      0x0d8
      ConsoleClearCardBuffer();
      break;

    case ConGetCardBuffer:       //      0x0d9
      ConsoleSendCardBuffer();
      break;

    case ConSetConfig:     //      0x0da
      ConsoleSetConfiguration();
      break;

    case ConGetConfig:     //      0x0db
      ConsoleGetConfiguration();
      break;

    case 0xEB : 
// Door Auth(8), antiPass(1), cardType(3), resultvalue(4) == 검색결과(0있음, 1기간지났음, 2이미존재?(쓸 때), 3없음)
      u16CardResult = CheckRfCardDataInFlash( &CopyRx2Rs485Buf[16]) & 0x00ff; //카드데이터 검색!!
      u8Port = CopyRx2Rs485Buf[3];//콘솔아이디..
      if(u16CardResult == 0x0010 && CopyRx2Rs485Buf[7] == 'N')
      { //ACU에는 등록되어 있는데 비등록카드로 오면
        ControlDoor(CMSParameter.DoorOpenTime[u8Port], u8Port);//문열어주고..
        CopyRx2Rs485Buf[7] = 'C';//상시카드로 바꾸고..
        
        //ECU에 등록하라고 해야 함..
        gU8ConsoleCardID[gU8ConsoleCardIDindexTCP][0] = 'A';    //'A' 등록 'D' 삭제..
        memcpy(gU8ConsoleCardID[gU8ConsoleCardIDindexTCP]+1, &CopyRx2Rs485Buf[16], 4);
        gU8ConsoleCardIDindexTCP++;
        gU8ConsoleCardIDindexTCP %= 20;
      }
      else if(u16CardResult == 0x0003 && CopyRx2Rs485Buf[7] == 'C')
      { //ACU에는 없는데 등록카드로 오면
        //이미 등록카드로 문열어줬으므로 그대로 상시카드로 보내고..
        //ECU에 삭제하라고 해야 함..
        gU8ConsoleCardID[gU8ConsoleCardIDindexTCP][0] = 'D';    //'A' 등록 'D' 삭제..
        memcpy(gU8ConsoleCardID[gU8ConsoleCardIDindexTCP]+1, &CopyRx2Rs485Buf[16], 4);
        gU8ConsoleCardIDindexTCP++;
        gU8ConsoleCardIDindexTCP %= 20;
      }
      
      memcpy(GVTAEventData.pU8EventLog + GVTAEventData.u8LogCount*16, &CopyRx2Rs485Buf[4], 16);//데이터 복사하고..
      CopyYYMMDDhhmmss(GVTAEventData.pU8EventLog + GVTAEventData.u8LogCount*16 + 6);//acu 의 현재 시간 복사하고..
//      memset(GVTAEventData.pU8EventLog+GVTAEventData.u8LogCount*16+12,0,4);//카드자리 4바이트 클리어
      SaveEventLog2EEprom_sFlash();//2. log count는 이 함수에서 올림..
      break;

    default:    //  0x02
//            ConsoleAckPacking();	// ACK 복사 여기선 굳이 안해도 됨..
      break;
  }

  memset(CopyRx2Rs485Buf, 0, Rs485Rx2Length);
  Rs485Rx2Length = 0;
}



/******************************************************************************
**  Console Access / Door Control Process
******************************************************************************/
void ConsoleAccessMethod(void)
{
  INT8U AccessType, n485Id, u8RS485CardData[4];
//    uint8_t uCheckResult;
//    uint16_t u16CardResult = 0;
   AccessType = CopyRx2Rs485Buf[CS_INDEX_DATA+16];//체크필요 cp710은.. [CS_INDEX_DATA+8]
  n485Id = CopyRx2Rs485Buf[CS_INDEX_ID];

#ifdef LOG_DEBUG
	printf( "ConsoleAccessMethod AccessType=%02X, n485Id=%02X\r\n",	AccessType,	n485Id);
#endif
//  if((CopyRx2Rs485Buf[10] != 'T')||(CopyRx2Rs485Buf[11] != 'S'))
//  {
//    AccessType = Access_Wrong_Zone;
//  }

  switch(AccessType)
  {
    case Access_No_Key: 
    {
 /*     u8RS485CardData[3] = CopyRx2Rs485Buf[CS_INDEX_DATA+10];      //14
      u8RS485CardData[2] = CopyRx2Rs485Buf[CS_INDEX_DATA+11];
      u8RS485CardData[1] = CopyRx2Rs485Buf[CS_INDEX_DATA+12];
      u8RS485CardData[0] = CopyRx2Rs485Buf[CS_INDEX_DATA+13];
//      memcpy(u8RS485CardData, CopyRx2Rs485Buf + 16, 4); // &CopyRx2Rs485Buf[CS_INDEX_DATA+12] 카드데이터 자체는 16개가 오게 되어 있음..
      CheckEventCardAndLog(n485Id, u8RS485CardData, CheckRfCardDataInFlash(u8RS485CardData));
      */
      break;
    }
//이 이하는 711 등에서 쓰이던 코드인 듯.. 현재로썬 안씀..    
    case Access_Yes_Key:
    {    
//        if( ConsoleSetDoorOpen() > 0)
//        {           
//            ConsoleResultPack( Result_OK);
//        }
//        else
//        {
//            ConsoleResultPack( Result_Wrong_Zone); // 권한 없는 카드로 음성 출력 
//        }
//        break;
    }
    case Access_Wrong_Time:
    {
        ConsoleResultPack( Result_Wrong_Time); //  
        break;
    }
    case Access_Wrong_Zone:
    {
        ConsoleResultPack( Result_Wrong_Zone); //  
        break;
     }
     default:
        break;
  }
}


/******************************************************************************
**	Console Report Reader Status
******************************************************************************/
void ConsoleReportDevStatus(void)
{    // 0 : OK, 3 : Rfid 2200 NoRes, 4 : 485 convertor NoRes
//  gU8ConsoleAlive  = Rs485CommTx2Buf[CS_INDEX_DATA+0];   // 2 : Reader NoRes, 0: Reader OK
}

/******************************************************************************
**	SetDoor Control Process
******************************************************************************/
//INT8U ConsoleSetDoorOpen(void)
//{
//   INT8U ErrorLevel = 0;
//    if( (ConsoleParameter.DoorNumber[ConsolePollIndex] & 0x01) == 0x01 )
//    {
//        ControlDoor(ConsoleParameter.AccessOpenTime[ConsolePollIndex], 0);
//        ErrorLevel++;
//    }
//
//    if( (ConsoleParameter.DoorNumber[ConsolePollIndex] & 0x02) == 0x02 )
//    {
//        ControlDoor(ConsoleParameter.AccessOpenTime[ConsolePollIndex], 1);
//        ErrorLevel++;
//    }
//
//    return ErrorLevel;
//}

/******************************************************************************
**
******************************************************************************/
void ConsolePowerOnMethod(void)
{
 //인식기의 파워온시간을 저장함.. 근데 acu에서는 안필요할 듯..
      INT16U SaveAddress = 0;
      ConsoleTimeSetCsType2(ConsolePollIndex);
////
////  SaveAddress = (INT16U)(EEPROM_ADDRESS_ConsoleInfoSave + (ConsolePollIndex * 6));
////
////  EEPROMSetDataBlock(SaveAddress, (uint8_t *)"\0\0\0\0\0\0",6);

}

void ConsoleTamperStatusSave(void)
{
  if( (CopyRx2Rs485Buf[10] == TRUE) || (CopyRx2Rs485Buf[10] == FALSE) )
  {
    ConsoleTamper[ConsolePollIndex] = CopyRx2Rs485Buf[10];
  }
}

/******************************************************************************
**
******************************************************************************/
void ConsoleTimeSetCsType2(INT8U PollIndex)
{
    byte *TempPtr;
    ////  시간 보내기
      TempPtr = &ConsoleTxBuffer[PollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB  StxStart485)
      *(TempPtr)++ = 0x08;     //  YYMMDDWWhhmmss(7)
      *(TempPtr)++ = CONTROL_CS_TIME_DATA_2;   // 0xA6  Time2 (YMDHMSW) //장비 시간 변경ConTimeSet;
      *(TempPtr)++ = PollIndex;

      *(TempPtr)++ = toDayTime.YEAR;        //year
      *(TempPtr)++ = toDayTime.MON;         //month
      *(TempPtr)++ = toDayTime.DATE;        //date
      *(TempPtr)++ = toDayTime.HOUR;       //hour
      *(TempPtr)++ = toDayTime.MIN;          //minute
      *(TempPtr)++ = toDayTime.SEC;         //sec
//      *(TempPtr)++ = toDayTime.WeekDAY;  //Week..
      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[PollIndex][1]);
}

void ConsoleTimeSetCsType1(INT8U PollIndex)
{
    byte *TempPtr;

      TempPtr = &ConsoleTxBuffer[PollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 0x08;     //  YYMMDDWWhhmmss(7)
      *(TempPtr)++ = CONTROL_CS_TIME_DATA_1;   // 0xA0    //장비 시간 변경ConTimeSet;
      *(TempPtr)++ = PollIndex;

      *(TempPtr)++ = toDayTime.YEAR;        //year
      *(TempPtr)++ = toDayTime.MON;         //month
      *(TempPtr)++ = toDayTime.DATE;        //date
      *(TempPtr)++ = toDayTime.WeekDAY;  //Week..
      *(TempPtr)++ = toDayTime.HOUR;       //hour
      *(TempPtr)++ = toDayTime.MIN;          //minute
      *(TempPtr)++ = toDayTime.SEC;         //sec
      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[PollIndex][1]);
}

void ConsoleTimeGetPc(void)
{
   INT8U ReceiveTime[7];
    
    ReceiveTime[0] = (CopyRx2Rs485Buf[CS_INDEX_DATA+0]  >99)?99:CopyRx2Rs485Buf[CS_INDEX_DATA+0];  // 년(1)   : 0x09 -> 09년
    ReceiveTime[1] = ((CopyRx2Rs485Buf[CS_INDEX_DATA+1] == 0)||(CopyRx2Rs485Buf[CS_INDEX_DATA+1] >12))? 1:CopyRx2Rs485Buf[CS_INDEX_DATA+1];  // 월(1)   : 0x0b -> 11월
    ReceiveTime[2] = ((CopyRx2Rs485Buf[CS_INDEX_DATA+2] == 0)||(CopyRx2Rs485Buf[CS_INDEX_DATA+2] >31))? 1:CopyRx2Rs485Buf[CS_INDEX_DATA+2];  // 일(1)   : 0x0b -> 11일
    ReceiveTime[3] = (CopyRx2Rs485Buf[CS_INDEX_DATA+4]  >23)? 0:CopyRx2Rs485Buf[CS_INDEX_DATA+4];  // 시(1)    : 0x0b -> 11시
    ReceiveTime[4] = (CopyRx2Rs485Buf[CS_INDEX_DATA+5]  >59)? 0:CopyRx2Rs485Buf[CS_INDEX_DATA+5];  // 분(1)    : 0x0b -> 11분
    ReceiveTime[5] = (CopyRx2Rs485Buf[CS_INDEX_DATA+6]  >59)? 0:CopyRx2Rs485Buf[CS_INDEX_DATA+6];  // 초(1)    : 0x0b -> 11초
    ReceiveTime[6] = (CopyRx2Rs485Buf[CS_INDEX_DATA+3]  > 6)? 0:CopyRx2Rs485Buf[CS_INDEX_DATA+3];  // 요일(1) : 일(0x00) 월(0x01) 화(0x02) 수(0x03) 목(0x04) 금(0x05) 토(0x06).
    
//    SetToDay(ReceiveTime);
	 RTC_SetTimeAndDate( &toDayTime, &ReceiveTime[0] );
    EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime, (INT8U*)ReceiveTime, 7);

    rprintf( "[TA1]->Time 20%02x.%02x.%02x %02x:%02x:%02x(%x)\r\n",
           toDayTime.YEAR, toDayTime.MON, toDayTime.DATE, 
           toDayTime.HOUR, toDayTime.MIN, toDayTime.SEC, toDayTime.WeekDAY);
}

/******************************************************************************
**
******************************************************************************/
void ConsoleSetTimeSend(void)
{
  INT8U i;
  for(i=0; i<MAX_NUM_485CS; i++)
     {
    if(ConsoleParameter.Type[i] != FALSE)
    {
    	ConsoleNeedSetTime[i] = 1;
    }
  }
}

/******************************************************************************
**
******************************************************************************/
void ConsoleResultPack( INT8U result)
{
    byte *TempPtr;

#ifdef LOG_DEBUG
	printf( "ConsoleResultPack result=%02X\r\n",	result);
#endif
      TempPtr = &ConsoleTxBuffer[ConsolePollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 4;//length[control, id, data)
      *(TempPtr)++ = CONTROL_CS_ACESS_RESULT;//     	= 0xA5  //ConAccResult; //pCsMsg.CsMsg[0] =  gStCsCmd.cs_st.control;
      *(TempPtr)++ = ConsolePollIndex;
      *(TempPtr)++ = 0; // pCsMsg.CsMsg[1] =  gStCsCmd.cs_st.data[0];
      *(TempPtr)++ = result; // pCsMsg.CsMsg[2] =  gStCsCmd.cs_st.data[1];   gUcFlash4k[FLASH_ADDR_VOICE_DATA] = CsData->CsMsg[2];
//      *(TempPtr)++ = result;    //pCsMsg.CsMsg[3] =  gStCsCmd.cs_st.data[2];

      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[ConsolePollIndex][1]);

#ifdef LOG_DEBUG
	printf( "ConsoleResultPack Buffer=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",
			ConsoleTxBuffer[ConsolePollIndex][0], ConsoleTxBuffer[ConsolePollIndex][1], ConsoleTxBuffer[ConsolePollIndex][2], ConsoleTxBuffer[ConsolePollIndex][3],
			ConsoleTxBuffer[ConsolePollIndex][4], ConsoleTxBuffer[ConsolePollIndex][5], ConsoleTxBuffer[ConsolePollIndex][6], ConsoleTxBuffer[ConsolePollIndex][7]);
#endif

}


/******************************************************************************
**
******************************************************************************/
void ConsoleFwResultPack( INT8U result)
{
    byte *TempPtr;

      TempPtr = &ConsoleTxBuffer[ConsolePollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 0x04;
      *(TempPtr)++ = ConFirmwareResult;
      *(TempPtr)++ = ConsolePollIndex;
      *(TempPtr)++ = CopyRx2Rs485Buf[CS_INDEX_CONTROL];
      *(TempPtr)++ = result;

      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[ConsolePollIndex][1]);
}


void ResetConsoleStatus(void)
{
  uint8_t i;
  rprintf( "\n\r Set all console true to check whether console is."); 
  for (i=0; i < MAX_NUM_485CS; i++ ) 
  {
      if( i == 0)
        ConsoleParameter.Type[i] = TRUE;    // Need using Configuration 
      else
        ConsoleParameter.Type[i] = FALSE;    // Need using Configuration 
      ConsoleCommStat[i] = CommOK;
      ConsoleLoadCtrlType[i] = LoadNormal;
  }      
//  ChangeLockMode(LockMode_Normal, 0);
//  ChangeLockMode(LockMode_Normal, 1);
        
}

/******************************************************************************
**
******************************************************************************/

void  ConsoleTreatFirmwareStart(void)
{  
/*
    OS_Suspend(&TCBSCI1FwdTask);
    OS_Suspend(&TCBSPI1NetTask);
#if defined (MODEL_NUM) && (MODEL_NUM == 1)//1:TA1000 cp710������ debug, cp711������ cdma.. ta4000 ������ LCD�� �����?..
    OS_Suspend(&TCBSCI4DebugCdcTask);
#elif defined (MODEL_NUM) && (MODEL_NUM == 8)        //8: aru8
    OS_Suspend(&TCBFacTask);
#else //#elif defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000
    OS_Suspend(&TCBWiegandTask);
    OS_Suspend(&TCBFacTask);
#endif	//MODEL_NUM
    OS_Suspend(&TCBInTask);
    //  FacTask --> In order to check task running.
*/
        
    g_DebugMode = FALSE;
    bRequestNeed = MON_FROM_USER;  
    // 
    Firmware_Upgrade = SCI2_UPGRADE_YES;

    ConsoleFwResultPack(  UPDATE_OK);
   
    ftimeSciOut = 0;
}

void  ConsoleTreatFirmwareHeader(void)
{
  PrepareUpdatesFlash(UPDATE_FROM_NEW); // Should be prior to send response
  
  ulBinLen  = (CopyRx2Rs485Buf[CS_INDEX_DATA+0] << 24);
  ulBinLen += (CopyRx2Rs485Buf[CS_INDEX_DATA+1] << 16);
  ulBinLen += (CopyRx2Rs485Buf[CS_INDEX_DATA+2] << 8);
  ulBinLen += (CopyRx2Rs485Buf[CS_INDEX_DATA+3] << 0);
    
  ulBinCrc  = (CopyRx2Rs485Buf[CS_INDEX_DATA+4] << 24);
  ulBinCrc += (CopyRx2Rs485Buf[CS_INDEX_DATA+5] << 16);
  ulBinCrc += (CopyRx2Rs485Buf[CS_INDEX_DATA+6] << 8);
  ulBinCrc += (CopyRx2Rs485Buf[CS_INDEX_DATA+7] << 0);
  
  ConsoleFwResultPack(  UPDATE_OK);

   ftimeSciOut = 0;
}

void  ConsoleTreatFirmwareData(void)
{
  uint8_t rtError = 0; 
  uint16_t nIndex = CopyRx2Rs485Buf[CS_INDEX_LENGTH];

  xWRITE_DataToExNorFlash(ulFlashAddr, CopyRx2Rs485Buf+CS_INDEX_DATA, nIndex);

     ulFlashAddr += nIndex;
     ulDataLen += nIndex;
 
   ConsoleFwResultPack(  rtError);

   ftimeSciOut = 0;
}

void  ConsoleTreatFirmwareTail(void)
{
  ulFlashAddr = FIRM_START_NEW_ADDR;
  nAddedFf = CopyRx2Rs485Buf[CS_INDEX_DATA];
  ulDataLen -= nAddedFf;

  if( ulBinLen == ulDataLen) {
    ConsoleFwResultPack(  FERROR_CLEAR);
  }
  else {
    ConsoleFwResultPack(  FERROR_SIZE_2);
  }
  
  ftimeSciOut = 0;
}
            
void  ConsoleTreatFirmwareStop(void)
{
    ConsoleFwResultPack(  FERROR_CLEAR);

    ftimeSciOut = 0;
    bRequestNeed = MON_NEED_SCI2_FWUD;
}
            
void  ConsoleSetCardData(void)
{

    uint8_t   nReturn;
  uint8_t *pCard = &CopyRx2Rs485Buf[CS_INDEX_DATA];

  nReturn = WriteRfCardDataToSerialFlash(pCard);
    
    ConsoleResultPack( nReturn);
}
            
void  ConsoleCheckCardData(void)
{
  uint8_t   nReturn;
  uint8_t *pCard = &CopyRx2Rs485Buf[CS_INDEX_DATA];

  nReturn = CheckRfCardDataInFlash(pCard);
  
  ConsoleResultPack( nReturn);
}
            
void  ConsoleDeleteCardData(void)
{
    uint8_t   nReturn;
  uint8_t *pCard = &CopyRx2Rs485Buf[CS_INDEX_DATA];

  nReturn = DeleteRfCardDataOnSerialFlash(pCard, 0);
    
    ConsoleResultPack( nReturn);
}
            
void  ConsoleClearCardBuffer(void)
{//체크필요 cp710은.. 아래 코멘트가 모두 해제된 상태임..
    uint8_t   nReturn;//, nCards;
//  uint32_t pCardAddr, aFlash4kAddr;
    
//    pCardAddr = ( (CopyRx2Rs485Buf[CS_INDEX_DATA]*0x100) + CopyRx2Rs485Buf[CS_INDEX_DATA+1]) ;
//    nCards = CopyRx2Rs485Buf[CS_INDEX_DATA+2];
    
//    aFlash4kAddr = ((pCardAddr*NUM_FLASH_CARD_DATA_BYTE) + \
                              FLASH_CardData_Start) & 0xFF000L; // 43FB2 => 43000  0x34440
//    if( nCards == 0xFC)
//        nReturn = ClearCardDataOfSerialFlashAll();
//    else
//        nReturn = ClearCardNumOfSerialFlash(aFlash4kAddr);

  nReturn = EraseSFlash(FLASH_RfCardData_Start, FLASH_RfCardData_End);//== ClearCardDataOfSerialFlashAll();
  ConsoleResultPack( nReturn);
}

void  Send2ConsoleCardData(void)
{
    byte *TempPtr;
//    byte nNc, ni, nCards;
//    uint32_t nnDir;
//    uint32_t cardAddr; // cardData  0x34567892 -> 0x3456
//    uint32_t sFlashRealAddr; // cardData*11+FLASH_CardData_Start -> 0x3456*0xB+0x20000 = 43FB2
    
      TempPtr = &ConsoleTxBuffer[ConsolePollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 9;//타입(1) 구분(1) 명령(1) 카드값(4)
      *(TempPtr)++ = 0xC8;//카드값 등록/삭제 전송 명령
      *(TempPtr)++ = ConsolePollIndex;
      *(TempPtr)++ = 0;//타입(1) : 카드 0, 번호키 1, 바코드 2, 지문 3, 얼굴 4, , 버튼입력 100 등 
      *(TempPtr)++ = 0;//구분(1) : 출입, 출근, 퇴근, 외출 복귀 조식 (코드는 별도로 협의 필요)
      for(int i = 0; i < 5; i++){//명령(1) : 등록 'A', 삭제 'D', 수정 'C'
        *(TempPtr)++ = gU8ConsoleCardID[gU8ConsoleCardIDindex485][i];//1~4 : 카드값 (4)
      }
      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[ConsolePollIndex][1]);
}
            
void  ConsoleSendCardBuffer(void)
{
    byte *TempPtr;
    byte nNc, ni, nCards;
    uint32_t nnDir;
    uint32_t cardAddr; // cardData  0x34567892 -> 0x3456
    uint32_t sFlashRealAddr; // cardData*11+FLASH_CardData_Start -> 0x3456*0xB+0x20000 = 43FB2
    
      TempPtr = &ConsoleTxBuffer[ConsolePollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 12;//length
      *(TempPtr)++ = ConSendCardBuffer; //#define ConSendCardBuffer          0xe3
      *(TempPtr)++ = ConsolePollIndex;

      cardAddr = (CopyRx2Rs485Buf[CS_INDEX_DATA]*0x100+(CopyRx2Rs485Buf[CS_INDEX_DATA+1]));
      nCards = CopyRx2Rs485Buf[CS_INDEX_DATA+2];
      if( nCards == 0xFC)
      {
          ConsoleLoadCtrlType[ConsolePollIndex] = LoadPrepairBuffer;
          nnDir = ReadNumCardFromEeprom();      //  0~1023 (1024 
          ConsoleLoadCtrlCount[ConsolePollIndex] = (byte)((nnDir/47)+1);
          *(TempPtr)++ = (byte)(nnDir/256);
          *(TempPtr)++ = (byte)(nnDir%256);
      }
      else
      {
           *(TempPtr)++ = CopyRx2Rs485Buf[CS_INDEX_DATA+0];
           *(TempPtr)++ = CopyRx2Rs485Buf[CS_INDEX_DATA+1];
      }
          

      *(TempPtr)++ = nNc;
      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[ConsolePollIndex][1]);
      
}
 
void ConsoleGetCardBufferDirect(void)
{
    byte *TempPtr;
    byte nNc, ni, nnDir;
//    uint32_t cardAddr; // cardData  0x34567892 -> 0x3456
    uint32_t sFlashRealAddr; // cardData*11+FLASH_CardData_Start -> 0x3456*0xB+0x20000 = 43FB2
    
    TempPtr = &Rs485CommTx2Buf[0];
    *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
    *(TempPtr)++ = 202;//length
    *(TempPtr)++ = ConSendDirectBuffer;//command        #define ConSendDirectBuffer        0xe6
    *(TempPtr)++ = ConsolePollIndex;

    *(TempPtr)++ = 0;
    nnDir = ConsoleLoadCtrlCount[ConsolePollIndex];
    nnDir -= 1;
    ConsoleLoadCtrlCount[ConsolePollIndex] = nnDir;
    *(TempPtr)++ = nnDir;
     *(TempPtr)++ = CheckSum485(&Rs485CommTx2Buf[1]);
}
            
void  ConsoleSetConfiguration(void)
{
    byte nNc, nTf;
    uint8_t CsNum;
 
    nTf = ((CopyRx2Rs485Buf[CS_INDEX_DATA] & 0x0F) >> 0);
    nNc = ((CopyRx2Rs485Buf[CS_INDEX_DATA] & 0xF0) >> 4);
    if( nNc > 0)
              CsNum = nNc-1;
    else      CsNum = 0;
    
    if( nTf > 0) 
    {
        ConsoleParameter.Type[CsNum] = TRUE;
        ConsoleCommStat[CsNum] = CommOK;
        ConsoleLoadCtrlType[CsNum] = LoadNormal;
    }
    else  {
        ConsoleParameter.Type[CsNum] = FALSE;
    }
    ConsoleParameter.DoorNumber[CsNum] = CopyRx2Rs485Buf[CS_INDEX_DATA+1];
    ConsoleParameter.AccessOpenTime[CsNum] = CopyRx2Rs485Buf[CS_INDEX_DATA+2];
 
    EEPROMSetDataBlock(E2P_Para_CONSOLE , (INT8U*)&ConsoleParameter.AccessOpenTime[0], LEN_E2ROM_CONSOLE);
}

void  ConsoleGetConfiguration(void)
{
    byte *TempPtr;
    byte nNc;
    uint8_t CsNum, CsId;
    
      TempPtr = &ConsoleTxBuffer[ConsolePollIndex][0];
      *(TempPtr)++ = StxStart485_TX;   //0xBB          StxStart485)     //0x7e
      *(TempPtr)++ = 5;
      *(TempPtr)++ = ConSendConfiguration;
      *(TempPtr)++ = ConsolePollIndex;
      
      if( CopyRx2Rs485Buf[CS_INDEX_DATA] > 0)
                CsNum = (CopyRx2Rs485Buf[CS_INDEX_DATA]-1);
      else      CsNum = 0;
      
      CsId = CsNum+1;
      if(   ConsoleParameter.Type[CsNum] == TRUE)
           nNc = (CsId << 4) + CsId;
      else nNc = (CsId << 4);    // Need using Configuration 
      *(TempPtr)++ = nNc;
      *(TempPtr)++ = ConsoleParameter.DoorNumber[CsNum];
      *(TempPtr)++ = ConsoleParameter.AccessOpenTime[CsNum];

      *(TempPtr)++ = CheckSum485(&ConsoleTxBuffer[ConsolePollIndex][1]);
      
}

void SetCardDataReceived(void)
{
  Rs485CommTx2Buf[CS_INDEX_DATA+10] = toDayTime.DATE;      //14
  Rs485CommTx2Buf[CS_INDEX_DATA+11] = toDayTime.HOUR;
  Rs485CommTx2Buf[CS_INDEX_DATA+12] = toDayTime.MIN;
  Rs485CommTx2Buf[CS_INDEX_DATA+13] = toDayTime.SEC;
  Rs485CommTx2Buf[CS_INDEX_DATA+14] = 0;
  
  ConsoleAccessMethod();
}
void CHAFON_RU5106_Inventory(void){
  ConsoleTxBuffer[ConsolePollIndex][0] = 0x04;
  ConsoleTxBuffer[ConsolePollIndex][1] = ConsolePollIndex;
  ConsoleTxBuffer[ConsolePollIndex][2] = GetRFdata;     //0x01  command;
  ConsoleTxBuffer[ConsolePollIndex][4] = StxStart485_TX;   //0xBB          StxStart485)     //0x7e header 가 없으므로 구분자로서 header를 여기(length가 pointer가 됨)다 해야...
}
void CHAFON_RU5106_ChangeMode(byte workmode)//0 answer mode, 1 active mode
{ 
  memcpy(ConsoleTxBuffer[ConsolePollIndex],"\x0a\x00\x35\x00\x06\x01\x04\x04\x01",9);// ba 34
  ConsoleTxBuffer[ConsolePollIndex][1] = ConsolePollIndex;
  ConsoleTxBuffer[ConsolePollIndex][3] = workmode;
  ConsoleTxBuffer[ConsolePollIndex][10] = StxStart485_TX;   //0xBB          StxStart485)     //0x7e header 가 없으므로 구분자로서 header를 여기(length가 pointer가 됨)다 해야...
}
 
#define PRESET_VALUE 0xFFFF
#define POLYNOMIAL  0x8408
unsigned int uiCrc16Cal(unsigned char const  * pucY, unsigned char ucX)
{
  unsigned char ucI,ucJ;
  unsigned short int  uiCrcValue = PRESET_VALUE;

  for(ucI = 0; ucI < ucX; ucI++)
  {
    uiCrcValue = uiCrcValue ^ *(pucY + ucI);
    for(ucJ = 0; ucJ < 8; ucJ++)
    {
      if(uiCrcValue & 0x0001)
      {
        uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL;
      }
      else
      {
        uiCrcValue = (uiCrcValue >> 1);
      }
    }
  }
  return uiCrcValue;
}


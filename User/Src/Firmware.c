/* Define to prevent recursive inclusion -------------------------------------*/
#define    FIRMWARE_GLOBALS 
#include "Firmware.h"

#define OS_Delay	MyMsDelay
// static uint8_t CommFwRespRetryBuf[] = { 0xff, 0x02, 0x01, 0x00, 0x00,  0x01, 0x03}; // 
static uint8_t CommFwRespErrorBuf[] = { 0xff, 0x02, 0x01, 0x00, 0xFF,  0x00, 0x03}; // 
static uint8_t CommFwRespStartBuf[] = { 0xff, 0x02, 0x01, 0x01, 0x01,  0x03, 0x03}; // 
static uint8_t CommFwRespHeaderBuf[] ={ 0xff, 0x02, 0x01, 0x02, 0x01,  0x04, 0x03}; // 
static uint8_t CommFwRespDataBuf[] =  { 0xff, 0x02, 0x01, 0x03, 0x01,  0x05, 0x03}; // 
static uint8_t CommFwRespTailBuf[] =  { 0xff, 0x02, 0x01, 0x04, 0x01,  0x06, 0x03}; // 
//  CommFwRespUploadBuf[134] //  =    { 0xff, 0x02, 0x80, 0x05, 0x01, ....., 0x80, 0xXX, 0x03}; // 
static uint8_t CommFwRespStopBuf[]  = { 0xff, 0x02, 0x01, 0x07, 0x01,  0x09, 0x03}; // 
INT16U g_logMaxPollCnt;
INT16U g_logCurrPollIndex;

pFunction JumpToApplication;
uint32_t JumpAddress;

uint32_t BlockNbr = 0, UserMemoryMask = 0;

uint32_t EraseCounter;
uint32_t NbrOfPage;
// FLASH_Status FLASHStatus;

extern uint8_t WriteHttpIPToEeprom(uint8_t* pData);
extern void MakeReaderToReportCard(uint8_t * cardId, uint8_t nRes);
extern void RTC_SetTimeAndDate( TIME *myDayTime, INT8U *pdBuffer);


/*
 *
 OS_RSEMA stringPutSem1;
 OS_RSEMA stringPutSem2;
 OS_RSEMA stringPutSem3;
 OS_RSEMA stringPutSem4;
 *
 */
void SerialProcInit(void)
{
	  uint16_t nRunning = 0;

	  BlockNbr = 0;
	  UserMemoryMask = 0;
	  FlashProtection = 0;
	  bRequestNeed = MON_FROM_USER;
	  Firmware_Upgrade = USER_UPGRADE_NONE;

	  makeCrcGlobalTable();
	  ReadGlobalUpgradeInfo();

	  if( ChecksFlashFlagOk( UPDATE_FROM_NEW ) == RESULT_OK)
	  {       // SPI TA영역 업그레이드 UPDATE_OK 플래그 확인
	    rprintf("[NOTICE]System Ready for Upgrade!\n");
	    if( ReadChecksumAndLen(UPDATE_FROM_NEW) == RESULT_OK)
	    {    // NEW  ulBinLen and ulBinCrc
	      PreparesFlash4Write(UPDATE_FROM_BAK);
	      rprintf("[NOTICE]Write program to Backup area!\n");
	      rprintf("[NOTICE]Wait until Backup is completed!\n");
	      WriteNewData2Bak();

	  	_STATE_LED_OFF();
	  	_RUN_LED_OFF();
	  	_CAN_LED_OFF();

	      WriteUpdateInfoFlag( UPDATE_FROM_NEW, UPDATE_OK_NEW );
	      WriteUpdateInfoFlag( UPDATE_FROM_BAK, UPDATE_OK );
	      rprintf("[NOTICE]Backup is Finished!\n");
	    }
	  }
	  else
	  {
	    if( strncmp( (const char *)&gUpgradeInfo[INDEX_FLASH_UPGRADE_STATUS_INFO], (const char *)fwCompileTime, 8) == 0)
	    {
	      nRunning = gUpgradeInfo[8] * 256 + gUpgradeInfo[9];
	      nRunning++;
	      gUpgradeInfo[8] = nRunning / 256;
	      gUpgradeInfo[9] = nRunning % 256;
	      WriteGlobalUpgradeInfo();
	    }
	    else
	    {
	      strncpy( (char *)&gUpgradeInfo[INDEX_FLASH_UPGRADE_STATUS_INFO], (const char *)fwCompileTime, 8);
	      gUpgradeInfo[8] = 0;
	      gUpgradeInfo[9] = 1;
	      WriteGlobalUpgradeInfo();
	    }
	  }
}

void FW_SetReceivedFwd2Buffer(uint8_t cFwReceivedChar)
{
     if (FwdTx1Enable == 0)
     {

		 FwdCommRx1Buf[nFwdPacketIndex] = cFwReceivedChar;

		 if( nFwdPacketIndex == FWD_STATE_LEN )
		 {
			 nFwdByteToReceive = (uint16_t)(cFwReceivedChar + 6);  // max 255 + 6 = 261
		 }
		 else if( nFwdPacketIndex == FWD_STATE_CMD )
		 {
			 nFwdPacketKind = cFwReceivedChar;
		 }
		 else if( (nFwdPacketIndex > FWD_STATE_CMD) && (nFwdPacketIndex == (nFwdByteToReceive-1)) )
		 {
		   FwdTx1Enable = 1;
		 }
		 nFwdPacketIndex++;
     }
}


uint8_t SerialCommandProcess(void)            // Firmware Update or moniter  Process
{
  uint32_t checkCrc = 0;

  static uint16_t nFwdTaskCycle = 0;


    if( FwdTx1Enable == 1) 
    {   // complete
//      OS_Delay(10);
      switch( nFwdPacketKind)
      {
        case FWD_TYP_START:               // 0x1
          //OS_SuspendAllTasks();

 /*         OS_Suspend(&TCBSCI2Rs485Task);
          OS_Suspend(&TCBSPI1NetTask);
  #if defined (MODEL_NUM) && (MODEL_NUM == 1)//1:TA1000 cp710������ debug, cp711������ cdma.. ta4000 ������ LCD�� �����..
          OS_Suspend(&TCBSCI4DebugCdcTask);
  #elif defined (MODEL_NUM) && (MODEL_NUM == 8)        //8: aru8
          OS_Suspend(&TCBFacTask);
  #else //#elif defined (MODEL_NUM) && (MODEL_NUM == 4)   //4:TA000
          OS_Suspend(&TCBWiegandTask);
          OS_Suspend(&TCBFacTask);
  #endif	//MODEL_NUM
          OS_Suspend(&TCBInTask);*/

          g_DebugMode = FALSE;
          bRequestNeed = MON_FROM_USER;  
          // 
          Firmware_Upgrade = SCI1_UPGRADE_YES;

          memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespStartBuf, 7);
          FirmwareTreatStartPacket();
          ftimeSciOut = 0;
          break;
        case FWD_TYP_HEADER:             // 0x2
          memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespHeaderBuf, 7);
          FirmwareTreatHeaderPacket();
          ftimeSciOut = 0;
          break;
        case FWD_TYP_DATA:               // 0x3
          memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespDataBuf, 7);
          FirmwareTreatDataPacket();
          ftimeSciOut = 0;
          break;
        case FWD_TYP_TAIL:               // 0x4
          memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespTailBuf, 7);
          FirmwareTreatTailPacket();
          ftimeSciOut = 0;
          break;
        case FWD_TYP_UPLOAD:             // 0x5   For Test
          FirmwareTreatUploadPacket();
          ftimeSciOut = 0;
          break; 
        case FWD_TYP_END:               // 0x7
          memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespStopBuf, 7);
          FirmwareTreatEndPacket();
          ftimeSciOut = 0;
          bRequestNeed = MON_NEED_SCI1_FWUD;
          break;
        case FWD_TYP_SET_READER:               // 0x10
           AteTreatSetReaderMode();
           break;
        case FWD_TYP_SET_CARD:               // 0x11
           AteTreatSetCardMode();
           break;
        case FWD_TYP_SET_FIRE:               // 0x21
           AteTreatSetFire();
           break;
        case FWD_TYP_CANCEL_FIRE:               // 0x22
        	AteTreatCancelFire();
           break;
       case FWD_TYP_SET_TIME:               // 0x45
          AteTreatSetTimeType();
          break;
        case FWD_TYP_SET_PSN:               // 0x46
          AteTreatSetPsnType();
          break;
        case ATE_TYP_SET_FACTORY:               // 0x57
          AteTreatFactory();
          break;
        case FWD_TYP_HISTORY_ON:               // 0x80
          bRequestNeed = MON_FROM_USER;    // 
          g_DebugMode = TRUE;
          break;
        case FWD_TYP_HISTORY_OFF:               // 0xC0
          g_DebugMode = FALSE;
          break;
        case FWD_TYP_SET_IP:          // 0x8A
        	FirmwareTreatSetipPacket();
        break;
        case FWD_TYP_GET_IP:          // 0x8B
        	FirmwareTreatGetipPacket();
        break;
        case FWD_TYP_SET_HTTP_IP:          // 0x8C
        	FirmwareTreatSetHttpipPacket();
        break;
        case FWD_TYP_GET_HTTP_IP:          // 0x8D
        	FirmwareTreatGetHttpipPacket();
        break;
        case FWD_TYP_SET_NETMODE:  //	(0x75)
        	FirmwareTreatNetModePacket();
        break;
        case FWD_TYP_SET_DEFAULT:          // 0x77
          {
//            OS_SuspendAllTasks();
            TestLockOut();
			FireOccurrence = FALSE;
            EEPROMSetData((uint8_t) EEPROM_ADDRESS_FireFlag,0);//eeprom ȭ���ȣ �����..
            EEPROMSetDataBlock(E2P_Para_SENSOR , (uint8_t*)&DefaultSensorParameter[0], LEN_E2ROM_SENSOR);
            EEPROMSetDataBlock(E2P_Para_LOCK , (uint8_t*)&DefaultLockParameter[0], LEN_E2ROM_LOCK);
            EEPROMSetDataBlock(E2P_Para_BUTTON , (uint8_t*)&DefaultButtonParameter[0], LEN_E2ROM_BUTTON);
            EEPROMSetDataBlock(E2P_Para_CONSOLE , (uint8_t*)&DefaultConsoleParameter[0], LEN_E2ROM_CONSOLE);
            EEPROMSetDataBlock(E2P_Para_WIEGAND , (uint8_t*)&DefaultWiegandParameter[0], LEN_E2ROM_WIEGAND);
            EEPROMSetDataBlock(E2P_Para_HTTPC , (uint8_t*)&DefaultHttpcParameter[0], LEN_E2ROM_HTTPC);
            EEPROMSetDataBlock(E2P_Para_FIRE , (uint8_t*)&DefaultFireParameter[0], LEN_E2ROM_FIRE);
            ReloadParameter(); 
//            OS_ResumeAllSuspendedTasks();
          }
          break;
        case FWD_TYP_CLEAR_CARD:          // 0x47
          AteTreatSclearType();
          break;
        case ATE_TYP_SFLASH_READ:          // 0x5C
          AteTreatSreadType();
          break;
        case ATE_TYP_SFLASH_WRITE:          // 0x5D
          AteTreatSwriteType();
          break;
        case ATE_TYP_EEPROM_READ :			// 0x5E
            AteTreatReadEepromType();
        	break;
        case ATE_TYP_EEPROM_WRITE:			// 0x5F
            AteTreatWriteEepromType();
        	break;
        default:
 /*           memcpy( (char *)FwdCommTx1Buf, (const char *)CommFwRespErrorBuf, 7);
            FirmwareRequestSendData();
 */   		nAtePacketIndex = nFwdPacketIndex;
 	 	 	nAteByteToReceive = 0;
 	 	 	nAtePacketType = nFwdPacketKind;
  	 	 	memcpy( (char *)AteCommTx3Buf, (const char *)FwdCommRx1Buf, nFwdPacketIndex);
 	 	 	AteAppCommandProcess();
        	break;
      }

      nFwdPacketIndex = 0;
      FwdTx1Enable = 0;
      
      if( bRequestNeed == MON_NEED_SCI1_FWUD)
      {
        checkCrc = getExtFlashCRC( FIRM_START_NEW_ADDR, ulBinLen );
        if( checkCrc == ulBinCrc)
        {
          WriteUpdateInfoFlag( UPDATE_FROM_NEW, UPDATE_NEED );
          rprintf("[NOTICE]System Reset for Upgrade!\r\n");
          NVIC_SystemReset();	// System Reset
        }
        else
        {
          bRequestNeed = MON_FROM_USER;
          rprintf("[NOTICE]Checksum Error!\r\n");
        }
        Firmware_Upgrade = USER_UPGRADE_NONE;
 //       OS_ResumeAllSuspendedTasks();
      }
      else
      {
        if (nFwdTaskCycle == 1000)
        {  // 100 mS * 1000 = 100 Sec
          SendCP_Status();
          nFwdTaskCycle = 0;
        }
        else
        {
          nFwdTaskCycle++;
        }

        if( Firmware_Upgrade == SCI1_UPGRADE_YES)
        {
          ftimeSciOut++;
          if( ftimeSciOut > 200)  /// About 20 seconds
          {
            bRequestNeed = MON_FROM_USER;
            rprintf("[NOTICE]Firmware SCI1 Update Waiting Time is over!\r\n");
            Firmware_Upgrade = USER_UPGRADE_NONE;
  //          OS_ResumeAllSuspendedTasks();
          }
        }
      }

      if( Firmware_Upgrade != SCI1_UPGRADE_YES)
        TaskRunCnt(ENUM_SCI1FWD_TASK);

  }
//  return TRUE;
}


/* START Auto Factory Test  Function  */
void FirmwareTreatSetipPacket(void)
{
      uint8_t BufData[6];
      TACommTaskStep = TAComm_End;
      OS_Delay(10);

      memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+0], 6);
      WriteMacAddress((uint8_t *)BufData);

      memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+6], 4);
      WriteIPToEeprom((uint8_t *)BufData);

//		  memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+10], 2);
//		  WritePortToEeprom((uint8_t *)BufData);

      memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+12], 4);
      WriteSnToEeprom((uint8_t *)BufData);

      memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+16], 4);
      WriteGwToEeprom((uint8_t *)BufData);

      ReloadParameter();
      OPEN_Ethernet();
 }


/* START Auto Factory Test  Function  */
void FirmwareTreatGetipPacket(void)
{
	uint8_t nChecksum = 0;

	FwdCommTx1Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
	FwdCommTx1Buf[ATE_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[ATE_STATE_LEN] = 1;      			nChecksum += FwdCommTx1Buf[ATE_STATE_LEN];
	FwdCommTx1Buf[ATE_STATE_CMD] = FWD_TYP_GET_IP;  nChecksum += FwdCommTx1Buf[ATE_STATE_CMD];//

	FwdCommTx1Buf[4] = CMSParameter.MacAddress[0];	nChecksum += FwdCommTx1Buf[ATE_STATE_DAT];
	FwdCommTx1Buf[5] = CMSParameter.MacAddress[1];	nChecksum += FwdCommTx1Buf[5];
	FwdCommTx1Buf[6] = CMSParameter.MacAddress[2];	nChecksum += FwdCommTx1Buf[6];
	FwdCommTx1Buf[7] = CMSParameter.MacAddress[3];  nChecksum += FwdCommTx1Buf[7];
	FwdCommTx1Buf[8] = CMSParameter.MacAddress[4];  nChecksum += FwdCommTx1Buf[8];
	FwdCommTx1Buf[9] = CMSParameter.MacAddress[5];  nChecksum += FwdCommTx1Buf[9];

	FwdCommTx1Buf[10] = CMSParameter.IPAddress[0];  nChecksum += FwdCommTx1Buf[10];
	FwdCommTx1Buf[11] = CMSParameter.IPAddress[1];  nChecksum += FwdCommTx1Buf[11];
	FwdCommTx1Buf[12] = CMSParameter.IPAddress[2];  nChecksum += FwdCommTx1Buf[12];
	FwdCommTx1Buf[13] = CMSParameter.IPAddress[3];  nChecksum += FwdCommTx1Buf[13];

	FwdCommTx1Buf[14] = 0x38;	 nChecksum += FwdCommTx1Buf[14]; //TACS_TCP_PORT 14443    //CMSParameter.TAPort[0];
	FwdCommTx1Buf[15] = 0x6b;	 nChecksum += FwdCommTx1Buf[15]; //TACS_TCP_PORT 14443    //CMSParameter.TAPort[1];

	FwdCommTx1Buf[16] = CMSParameter.SubnetMask[0];  nChecksum += FwdCommTx1Buf[16];
	FwdCommTx1Buf[17] = CMSParameter.SubnetMask[1];  nChecksum += FwdCommTx1Buf[17];
	FwdCommTx1Buf[18] = CMSParameter.SubnetMask[2];  nChecksum += FwdCommTx1Buf[18];
	FwdCommTx1Buf[19] = CMSParameter.SubnetMask[3];  nChecksum += FwdCommTx1Buf[19];

	FwdCommTx1Buf[20] = CMSParameter.GatewayAddress[0];  nChecksum += FwdCommTx1Buf[20];
	FwdCommTx1Buf[21] = CMSParameter.GatewayAddress[1];  nChecksum += FwdCommTx1Buf[21];
	FwdCommTx1Buf[22] = CMSParameter.GatewayAddress[2];  nChecksum += FwdCommTx1Buf[22];
	FwdCommTx1Buf[23] = CMSParameter.GatewayAddress[3];  nChecksum += FwdCommTx1Buf[23];

    FwdCommTx1Buf[24] = nChecksum;
	FwdCommTx1Buf[25] = ATE_CHAR_ETX;

	FwdTx1Ctr = 26;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}


/* START Auto Factory Test  Function  */
void FirmwareTreatSetHttpipPacket(void)
{
      uint8_t BufData[6];
      TACommTaskStep = TAComm_End;
      OS_Delay(10);

      memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+0], 4);
      WriteHttpIPToEeprom((uint8_t *)BufData);

	  memcpy( &BufData[0], &FwdCommRx1Buf[FWD_STATE_DAT+4], 2);
	  WriteHttpPortToEeprom((uint8_t *)BufData);

      ReloadParameter();
      OPEN_Ethernet();
}

void FirmwareTreatNetModePacket( void)
{
	uint8_t nChecksum = 0;

	FwdCommTx1Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
	FwdCommTx1Buf[ATE_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[ATE_STATE_LEN] = 1;      				nChecksum += FwdCommTx1Buf[ATE_STATE_LEN];
	FwdCommTx1Buf[ATE_STATE_CMD] = FWD_TYP_SET_NETMODE; nChecksum += FwdCommTx1Buf[ATE_STATE_CMD];//

/*	if( FwdCommRx1Buf[4] == 1)
		nNetPartMode = TA_SERVER;
	else if( FwdCommRx1Buf[4] == 2)
		nNetPartMode = PUSH_CLIENT;*/
//	else  // read mode
    OPEN_Ethernet();

	FwdCommTx1Buf[4] = 1;  nChecksum += FwdCommTx1Buf[4];

    FwdCommTx1Buf[5] = nChecksum;
	FwdCommTx1Buf[6] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}
void FirmwareTreatGetHttpipPacket(void)
{
	uint8_t nChecksum = 0;

	FwdCommTx1Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
	FwdCommTx1Buf[ATE_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[ATE_STATE_LEN] = 6;      				nChecksum += FwdCommTx1Buf[ATE_STATE_LEN];
	FwdCommTx1Buf[ATE_STATE_CMD] = FWD_TYP_GET_HTTP_IP; nChecksum += FwdCommTx1Buf[ATE_STATE_CMD];//

	FwdCommTx1Buf[4] = HttpcParameter.TAIP1[0];  nChecksum += FwdCommTx1Buf[4];
	FwdCommTx1Buf[5] = HttpcParameter.TAIP1[1];  nChecksum += FwdCommTx1Buf[5];
	FwdCommTx1Buf[6] = HttpcParameter.TAIP1[2];  nChecksum += FwdCommTx1Buf[6];
	FwdCommTx1Buf[7] = HttpcParameter.TAIP1[3];  nChecksum += FwdCommTx1Buf[7];

	FwdCommTx1Buf[8] = HttpcParameter.TAPort[0];	 nChecksum += FwdCommTx1Buf[8]; //HTTP_TCP_PORT 14443    //CMSParameter.TAPort[0];
	FwdCommTx1Buf[9] = HttpcParameter.TAPort[1];	 nChecksum += FwdCommTx1Buf[9]; //HTTP_TCP_PORT 14443    //CMSParameter.TAPort[1];

    FwdCommTx1Buf[10] = nChecksum;
	FwdCommTx1Buf[11] = ATE_CHAR_ETX;

	FwdTx1Ctr = 12;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}
/* START Auto Factory Test  Function  */
void AteTreatFactory(void)
{
	uint8_t nChecksum = 0;

	FwdCommTx1Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
	FwdCommTx1Buf[ATE_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[ATE_STATE_LEN] = 1;      					nChecksum += FwdCommTx1Buf[ATE_STATE_LEN];
	FwdCommTx1Buf[ATE_STATE_CMD] = ATE_TYP_SET_FACTORY;  	nChecksum += FwdCommTx1Buf[ATE_STATE_CMD];//
	FwdCommTx1Buf[ATE_STATE_DAT] = 1;     					nChecksum += FwdCommTx1Buf[ATE_STATE_DAT];
	FwdCommTx1Buf[5] = nChecksum;
	FwdCommTx1Buf[6] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();

	EEPROMSetDataBlock(EEPROM_ADDRESS_SiteInfo, (INT8U*)&FwdCommRx1Buf[4],    8);//"VA001234" 쓰기..

	EEPROMSetDataBlock(EEPROM_ADDRESS_SystemFlag, (INT8U*)&FwdCommTx1Buf[0],  8);//"BruceIvx" 지우기..

	OS_Delay(100);

	NVIC_SystemReset();	// System Reset
}

void AteTreatSetReaderMode(void)
{
    uint8_t ReceivedCard[8] = { 0,0,0,0, 0,0,0,0 };
    g_isSetReader = 1;
    /*   Response   */
    MakeReaderToReportCard(ReceivedCard, 0);
}

void AteTreatSetFire(void)
{
	uint8_t nChecksum = 0;
     g_isSetReader = 0;

    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;					nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_SET_FIRE;	nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = 0;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	FwdCommTx1Buf[FWD_STATE_CHE] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_ETX] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FireOccurrence = TRUE; //화재시..
	FireLockFlag = 0;
}

void AteTreatCancelFire(void)
{
	uint8_t nChecksum = 0;
     g_isSetReader = 0;

    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;					nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_CANCEL_FIRE;	nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = 0;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	FwdCommTx1Buf[FWD_STATE_CHE] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_ETX] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

    isFireAlertOccur = FALSE;
    FireOccurrence = FALSE;
    if(FireLockFlag != 99)
    {//EEPROMGetData((INT16U) EEPROM_ADDRESS_FireFlag) != 99){//화재작동안함(99)이 아닐 경우
      EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag, 0);//eeprom 화재신호 지우기..
    }
}


void AteTreatSetCardMode(void)
{
	uint8_t nChecksum = 0;
     g_isSetReader = 0;

    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;					nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_SET_CARD;	nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = 0;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	FwdCommTx1Buf[FWD_STATE_CHE] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_ETX] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void MakeReaderToReportCard(uint8_t * cardId, uint8_t nRes)
{
	uint8_t nChecksum = 0;
    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 9;						nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_SET_READER;		nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = nRes;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	for( int i=0; i < 8; i++)
	{
		FwdCommTx1Buf[FWD_STATE_DAT+i+1] = *(cardId+i);		nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+i+1];//
	}
	FwdCommTx1Buf[13] = nChecksum;
	FwdCommTx1Buf[14] = ATE_CHAR_ETX;

	FwdTx1Ctr = 15;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatSetPsnType(void)
{
    int8_t nChecksum = 0;
    EEPROMSetDataBlock(EEPROM_ADDRESS_SiteInfo,(uint8_t*)&FwdCommRx1Buf[4],8);

    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;					nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_SET_PSN;		nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = 0;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	FwdCommTx1Buf[FWD_STATE_CHE] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_ETX] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatSetTimeType(void)
{
    int8_t nChecksum = 0;
    uint8_t ReceiveTime[7];

    ReceiveTime[0] =  FwdCommRx1Buf[4];  // 년(1)   : 0x09 -> 09년
    ReceiveTime[1] =  FwdCommRx1Buf[5];  // 월(1)   : 0x0b -> 11월
    ReceiveTime[2] =  FwdCommRx1Buf[6];  // 일(1)   : 0x0b -> 11일

    ReceiveTime[3] =  FwdCommRx1Buf[7];  // 시(1)    : 0x0b -> 11시
    ReceiveTime[4] =  FwdCommRx1Buf[8];  // 분(1)    : 0x0b -> 11분
    ReceiveTime[5] =  FwdCommRx1Buf[9];  // 초(1)    : 0x0b -> 11초

//    SetToDay(ReceiveTime);
	RTC_SetTimeAndDate( &toDayTime, &ReceiveTime[0] );
    EEPROMSetDataBlock(EEPROM_ADDRESS_DateTime,(uint8_t*)ReceiveTime,7);

    /*   Response   */
	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;					nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = FWD_TYP_SET_TIME;	nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT] = 0;					nChecksum += FwdCommTx1Buf[FWD_STATE_DAT];//
	FwdCommTx1Buf[FWD_STATE_CHE] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_ETX] = ATE_CHAR_ETX;

	FwdTx1Ctr = 7;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatSreadType(void)
{
    int32_t StartAddr = 0, readLen = 250;
    ErrState eSts = _Fail;
    uint8_t nChecksum = 0;
    uint16_t writeCount = 0, ni;

	StartAddr  = (FwdCommRx1Buf[FWD_STATE_DAT+0] << 24);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+1] << 16);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+2] << 8);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+3] << 0);

	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;

    eSts = xREAD_DataFromExNorFlash(StartAddr, FwdCommTx1Buf+FWD_STATE_DAT+5, readLen);
    if( eSts == _Ok) {
	  FwdCommTx1Buf[FWD_STATE_DAT+0] = 0;
	  FwdCommTx1Buf[FWD_STATE_DAT+1] = FwdCommRx1Buf[FWD_STATE_DAT+0];
	  FwdCommTx1Buf[FWD_STATE_DAT+2] = FwdCommRx1Buf[FWD_STATE_DAT+1];
	  FwdCommTx1Buf[FWD_STATE_DAT+3] = FwdCommRx1Buf[FWD_STATE_DAT+2];
	  FwdCommTx1Buf[FWD_STATE_DAT+4] = FwdCommRx1Buf[FWD_STATE_DAT+3];

	  writeCount = readLen+5;
	  for ( ni = 0; ni < writeCount; ni++)
		  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+ni];
    }
    else
    {
  	  FwdCommTx1Buf[FWD_STATE_DAT+0] = 1;
	  writeCount = 1;
	  for ( ni = 0; ni < writeCount; ni++)
		  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+ni];
    }
	FwdCommTx1Buf[FWD_STATE_LEN] = writeCount;            nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = ATE_TYP_SFLASH_READ;   nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//

	FwdCommTx1Buf[FWD_STATE_DAT+writeCount+0] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_DAT+writeCount+1] = ATE_CHAR_ETX;

	FwdTx1Ctr = FWD_STATE_DAT+writeCount+2;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatSclearType(void)
{
    int32_t StartAddr = 0, writeAddr = 0, ni = 0;
    int8_t nChecksum = 0, wData = 0;

    ClearRfCardIndexOnSerialFlash();

	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
	FwdCommTx1Buf[FWD_STATE_DAT+0] = 0;
	FwdCommTx1Buf[FWD_STATE_LEN] = 1;                      nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = ATE_TYP_SFLASH_WRITE;   nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
	FwdCommTx1Buf[FWD_STATE_DAT+1] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_DAT+2] = ATE_CHAR_ETX;

	FwdTx1Ctr = FWD_STATE_DAT+3;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatSwriteType(void)
{
    int32_t StartAddr = 0, writeAddr = 0, ni = 0;
    int16_t writeCount = 0;
    int8_t nChecksum = 0, wData = 0;
   
  writeCount = FwdCommRx1Buf[FWD_STATE_LEN] - 4;
  
  StartAddr  = (FwdCommRx1Buf[FWD_STATE_DAT+0] << 24);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+1] << 16);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+2] << 8);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+3] << 0);
  
  FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
  FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;
  
  FwdCommTx1Buf[FWD_STATE_DAT+0] = FwdCommRx1Buf[FWD_STATE_DAT+0];
  FwdCommTx1Buf[FWD_STATE_DAT+1] = FwdCommRx1Buf[FWD_STATE_DAT+1];
  FwdCommTx1Buf[FWD_STATE_DAT+2] = FwdCommRx1Buf[FWD_STATE_DAT+2];
  FwdCommTx1Buf[FWD_STATE_DAT+3] = FwdCommRx1Buf[FWD_STATE_DAT+3];
  
  for( ni=0; ni<writeCount; ni++)
  {
    wData = FwdCommRx1Buf[FWD_STATE_DAT+4+ni];
    writeAddr = StartAddr + ni;
    
    if( (writeAddr % 0x1000) == 0)
    	xERASE_ExFlash4kSector(writeAddr);
    xWRITE_DataToExNorFlash(writeAddr, &wData, 1);
  }
  FwdCommTx1Buf[FWD_STATE_DAT+4] = (int8_t)((writeCount >> 8) & 0xFF);
  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+4];
  FwdCommTx1Buf[FWD_STATE_DAT+5] = (int8_t)((writeCount >> 0) & 0xFF);
  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+5];
  
  writeCount = 6;
  FwdCommTx1Buf[FWD_STATE_LEN] = writeCount;
  nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
  FwdCommTx1Buf[FWD_STATE_CMD] = ATE_TYP_SFLASH_WRITE;
  nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
  FwdCommTx1Buf[FWD_STATE_DAT+6] = nChecksum;
  FwdCommTx1Buf[FWD_STATE_DAT+7] = ATE_CHAR_ETX;
  
  FwdTx1Ctr = FWD_STATE_DAT+8;  
  FwdTx1Ptr = &FwdCommTx1Buf[0];

  FirmwareRequestSendData();
}


void AteTreatReadEepromType(void)
{
    int32_t StartAddr = 0;
    uint16_t readLen = 250;
    ErrState eSts = _Fail;
    uint8_t nChecksum = 0;
    uint16_t writeCount = 0, ni;

	StartAddr  = (FwdCommRx1Buf[FWD_STATE_DAT+0] << 24);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+1] << 16);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+2] << 8);
	StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+3] << 0);

	FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
	FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;

    eSts = EEPROMGetDataBlock((uint16_t)StartAddr, &FwdCommTx1Buf[FWD_STATE_DAT+5], readLen);

    if( eSts == TRUE) {
	  FwdCommTx1Buf[FWD_STATE_DAT+0] = 0;
	  FwdCommTx1Buf[FWD_STATE_DAT+1] = FwdCommRx1Buf[FWD_STATE_DAT+0];
	  FwdCommTx1Buf[FWD_STATE_DAT+2] = FwdCommRx1Buf[FWD_STATE_DAT+1];
	  FwdCommTx1Buf[FWD_STATE_DAT+3] = FwdCommRx1Buf[FWD_STATE_DAT+2];
	  FwdCommTx1Buf[FWD_STATE_DAT+4] = FwdCommRx1Buf[FWD_STATE_DAT+3];

	  writeCount = readLen+5;
	  for ( ni = 0; ni < writeCount; ni++)
		  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+ni];
    }
    else
    {
  	  FwdCommTx1Buf[FWD_STATE_DAT+0] = 1;
	  writeCount = 1;
	  for ( ni = 0; ni < writeCount; ni++)
		  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+ni];
    }
	FwdCommTx1Buf[FWD_STATE_LEN] = writeCount;            nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
	FwdCommTx1Buf[FWD_STATE_CMD] = ATE_TYP_EEPROM_READ;   nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//

	FwdCommTx1Buf[FWD_STATE_DAT+writeCount+0] = nChecksum;
	FwdCommTx1Buf[FWD_STATE_DAT+writeCount+1] = ATE_CHAR_ETX;

	FwdTx1Ctr = FWD_STATE_DAT+writeCount+2;
	FwdTx1Ptr = &FwdCommTx1Buf[0];

	FirmwareRequestSendData();
}

void AteTreatWriteEepromType(void)
{
    int32_t StartAddr = 0, writeAddr = 0, ni = 0;
    int16_t writeCount = 0;
    int8_t nChecksum = 0, wData = 0;

  writeCount = FwdCommRx1Buf[FWD_STATE_LEN] - 4;

  StartAddr  = (FwdCommRx1Buf[FWD_STATE_DAT+0] << 24);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+1] << 16);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+2] << 8);
  StartAddr += (FwdCommRx1Buf[FWD_STATE_DAT+3] << 0);

  FwdCommTx1Buf[FWD_STATE_PRM] = FW_UPD_PROTOCOL_PRM;
  FwdCommTx1Buf[FWD_STATE_STX] = ATE_CHAR_STX;

  FwdCommTx1Buf[FWD_STATE_DAT+0] = FwdCommRx1Buf[FWD_STATE_DAT+0];
  FwdCommTx1Buf[FWD_STATE_DAT+1] = FwdCommRx1Buf[FWD_STATE_DAT+1];
  FwdCommTx1Buf[FWD_STATE_DAT+2] = FwdCommRx1Buf[FWD_STATE_DAT+2];
  FwdCommTx1Buf[FWD_STATE_DAT+3] = FwdCommRx1Buf[FWD_STATE_DAT+3];


  EEPROMSetDataBlock((uint16_t)StartAddr, &FwdCommRx1Buf[FWD_STATE_DAT+4], writeCount);

  FwdCommTx1Buf[FWD_STATE_DAT+4] = (int8_t)((writeCount >> 8) & 0xFF);
  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+4];
  FwdCommTx1Buf[FWD_STATE_DAT+5] = (int8_t)((writeCount >> 0) & 0xFF);
  nChecksum += FwdCommTx1Buf[FWD_STATE_DAT+5];

  writeCount = 6;
  FwdCommTx1Buf[FWD_STATE_LEN] = writeCount;
  nChecksum += FwdCommTx1Buf[FWD_STATE_LEN];
  FwdCommTx1Buf[FWD_STATE_CMD] = ATE_TYP_EEPROM_WRITE;
  nChecksum += FwdCommTx1Buf[FWD_STATE_CMD];//
  FwdCommTx1Buf[FWD_STATE_DAT+6] = nChecksum;
  FwdCommTx1Buf[FWD_STATE_DAT+7] = ATE_CHAR_ETX;

  FwdTx1Ctr = FWD_STATE_DAT+8;
  FwdTx1Ptr = &FwdCommTx1Buf[0];

  FirmwareRequestSendData();
}

void PrepareUpdatesFlash(uint8_t where2prep)
{
  ulBinLen = 0;
  ulBinCrc = 0;
  ulDataLen = 0;
  ulDataCrc = 0;
  nAddedFf = 0;
  nFwdPacketIndex  = 0;
  
  nFwdPacketKind = FWD_TYP_START;
  
   if( where2prep == UPDATE_FROM_NEW)   {
      ulFlashAddr = FIRM_START_NEW_ADDR;
  }
   else {
      ulFlashAddr = FIRM_START_BAK_ADDR;
  }
   PreparesFlash4Write( where2prep);
}

void PreparesFlash4Write(uint8_t where2prep)
{
  if( where2prep == UPDATE_FROM_NEW)
  {
	   xERASE_ExFlash64kBlock(FIRM_START_NEW_ADDR+0x00000L);
	   xERASE_ExFlash64kBlock(FIRM_START_NEW_ADDR+0x10000L);
	   xERASE_ExFlash64kBlock(FIRM_START_NEW_ADDR+0x20000L);
	   xERASE_ExFlash64kBlock(FIRM_START_NEW_ADDR+0x30000L);
  }
   else {//safe firmware
	   xERASE_ExFlash64kBlock(FIRM_START_BAK_ADDR+0x00000L);
	   xERASE_ExFlash64kBlock(FIRM_START_BAK_ADDR+0x10000L);
	   xERASE_ExFlash64kBlock(FIRM_START_BAK_ADDR+0x20000L);
	   xERASE_ExFlash64kBlock(FIRM_START_BAK_ADDR+0x30000L);
   }
}

void WriteNewData2Bak(void)
{
    uint32_t ulFor;
    uint32_t rdAddr, wrAddr;
    uint8_t  chRecv[MX25L51245G_PAGE_SIZE]; // 256 => 0x100
  uint16_t blinks = 0;

    rdAddr = FIRM_START_NEW_ADDR;
    wrAddr = FIRM_START_BAK_ADDR;
    
    for( ulFor = 0; ulFor < ulBinLen; ulFor+=MX25L51245G_PAGE_SIZE) {
    	xREAD_DataFromExNorFlash( rdAddr, chRecv, MX25L51245G_PAGE_SIZE);
    	xWRITE_DataToExNorFlash(wrAddr, chRecv, MX25L51245G_PAGE_SIZE);
      rdAddr += MX25L51245G_PAGE_SIZE;
      wrAddr += MX25L51245G_PAGE_SIZE;

      if( blinks == 500 ){
    	  _RUN_LED_TOGGLE();      // LED  E6
      }
      else if ( blinks == 1000 ){
    	  _RUN_LED_TOGGLE();      // LED   E6
        blinks++;
      }
      else if ( blinks > 3000 ){
    	  _RUN_LED_TOGGLE();      // LED    E6
        blinks = 0;      
      }
      else {
        blinks++;
      }
   }
}

uint8_t FirmwareWriteDataOne(uint32_t wrAddr, uint8_t chRecv)
{
	xWRITE_DataToExNorFlash(wrAddr, &chRecv, 1);
    return FERROR_CLEAR;
}

uint8_t FirmwareTreatStartPacket(void)
{
   uint8_t rtError = FERROR_CLEAR;
   FwdTx1Ctr = 7;  
   FwdTx1Ptr = &FwdCommTx1Buf[0];

   g_DebugMode = FALSE;

   FirmwareRequestSendData();

   return rtError;
}

uint8_t FirmwareTreatHeaderPacket(void)
{
  uint8_t rtError = FERROR_CLEAR;
  PrepareUpdatesFlash(UPDATE_FROM_NEW); // Should be prior to send response
  
  ulBinLen  = (FwdCommRx1Buf[FWD_STATE_DAT+0] << 24);
  ulBinLen += (FwdCommRx1Buf[FWD_STATE_DAT+1] << 16);
  ulBinLen += (FwdCommRx1Buf[FWD_STATE_DAT+2] << 8);
  ulBinLen += (FwdCommRx1Buf[FWD_STATE_DAT+3] << 0);
    
  ulBinCrc  = (FwdCommRx1Buf[FWD_STATE_DAT+4] << 24);
  ulBinCrc += (FwdCommRx1Buf[FWD_STATE_DAT+5] << 16);
  ulBinCrc += (FwdCommRx1Buf[FWD_STATE_DAT+6] << 8);
  ulBinCrc += (FwdCommRx1Buf[FWD_STATE_DAT+7] << 0);
  
   FwdTx1Ctr = 7;  
   FwdTx1Ptr = &FwdCommTx1Buf[0];

   FirmwareRequestSendData();

   return rtError;
}

uint8_t FirmwareTreatDataPacket(void)
{
	uint8_t rtError = FERROR_CLEAR;
	uint16_t nIndex = FwdCommRx1Buf[FWD_STATE_LEN];

    uint16_t n1st, n2nd;

    n1st = (uint16_t) (256 - (ulFlashAddr % 256));
    if( n1st == 256)
    {
    	xWRITE_DataToExNorFlash(ulFlashAddr, FwdCommRx1Buf+FWD_STATE_DAT, nIndex);
    }
    else
    {
        n2nd = (uint16_t) (nIndex - n1st);
        xWRITE_DataToExNorFlash(ulFlashAddr, FwdCommRx1Buf+FWD_STATE_DAT, n1st);
        if(n2nd > 0)
        xWRITE_DataToExNorFlash(ulFlashAddr+n1st, &FwdCommRx1Buf[FWD_STATE_DAT+n1st], n2nd);
    }

    ulFlashAddr += nIndex;
    ulDataLen += nIndex;

    FwdTx1Ctr = 7;
    FwdTx1Ptr = &FwdCommTx1Buf[0];

    FirmwareRequestSendData();

    return rtError;
}

uint8_t FirmwareTreatUploadPacket(void)
{
  uint8_t rtError = FERROR_CLEAR;
  uint8_t iFor, nSum = 0;

  if( ulDataLen > 0) {
      FwdCommTx1Buf[0] = 0xFF;
      FwdCommTx1Buf[1] = 0x02;
      FwdCommTx1Buf[2] = 0x80;      nSum  = FwdCommTx1Buf[2];
      FwdCommTx1Buf[3] = 0x05;      nSum += FwdCommTx1Buf[3];

      for( iFor = 0; iFor<PACKET_UPLOAD_DATA_SIZE; iFor++) {
        if( ulDataLen > 0) {
        	xREAD_DataFromExNorFlash( ulFlashAddr, &FwdCommTx1Buf[FWD_STATE_DAT+iFor], 1);
            nSum += FwdCommTx1Buf[FWD_STATE_DAT+iFor];
            ulDataLen--;
            ulFlashAddr++;
        }
        else {
            FwdCommTx1Buf[FWD_STATE_DAT+iFor] = 0xFF;
            nSum += FwdCommTx1Buf[FWD_STATE_DAT+iFor];
        }      
      }
      FwdCommTx1Buf[FWD_STATE_DAT + PACKET_UPLOAD_DATA_SIZE] = nSum;
      FwdCommTx1Buf[FWD_STATE_DAT + PACKET_UPLOAD_DATA_SIZE +1] = 0x03;

      FwdTx1Ctr = FWD_STATE_DAT + PACKET_UPLOAD_DATA_SIZE +2;
      FwdTx1Ptr = &FwdCommTx1Buf[0];

      FirmwareRequestSendData();
  }
  else {
      FwdCommTx1Buf[0] = 0xFF;
      FwdCommTx1Buf[1] = 0x02;
      FwdCommTx1Buf[2] = 0x01;      nSum  = FwdCommTx1Buf[2];
      FwdCommTx1Buf[3] = 0x06;      nSum += FwdCommTx1Buf[3];
      FwdCommTx1Buf[4] = nAddedFf;  nSum += FwdCommTx1Buf[4];
      FwdCommTx1Buf[5] = nSum;
      FwdCommTx1Buf[6] = 0x03;

      FwdTx1Ctr = 7;  
      FwdTx1Ptr = &FwdCommTx1Buf[0];

      FirmwareRequestSendData();
  }
  return rtError;
}

uint8_t FirmwareTreatTailPacket(void)
{
  uint8_t rtError = FERROR_CLEAR;

  ulFlashAddr = FIRM_START_NEW_ADDR;
  nAddedFf = FwdCommRx1Buf[FWD_STATE_DAT];
  ulDataLen -= nAddedFf;

  if( ulBinLen == ulDataLen) {
       FwdTx1Ctr = 7;  
       FwdTx1Ptr = &FwdCommTx1Buf[0];

       FirmwareRequestSendData();
 //    for( iFor = 0; iFor<7; iFor++) 
 //       SerialPutChar(CommFwRespTailBuf[iFor]);
  }
  else {
       FwdTx1Ctr = 7;  
       FwdTx1Ptr = &FwdCommTx1Buf[0];

       FirmwareRequestSendData();
//     for( iFor = 0; iFor<7; iFor++) 
//        SerialPutChar(CommFwRespErrorBuf[iFor]);
  }
  
  return rtError;
}

uint8_t FirmwareTreatEndPacket(void)
{
  uint8_t rtError = FERROR_CLEAR;

    FwdTx1Ctr = 7;
    FwdTx1Ptr = &FwdCommTx1Buf[0];
   FirmwareRequestSendData();
// for( iFor = 0; iFor<7; iFor++) 
//  SerialPutChar(CommFwRespStopBuf[iFor]);

  return rtError;
}

uint8_t FirmwareRequestSendData(void)
{
  uint8_t rtError = FERROR_CLEAR;

/*    OS_Use( &stringPutSem1 );  */
//	while( FwdTx1Ctr > 0 )
//		MyMsDelay(1);

	__HAL_UART_ENABLE_IT(&huart4, UART_IT_TXE);

	 while(HAL_IS_BIT_CLR(huart4.Instance->ISR, UART_FLAG_TXE))
		 MyMsDelay(1);

/*    OS_Unuse( &stringPutSem1 );*/
    
}

void ReadGlobalUpgradeInfo(void)     // ������Ʈ Flag Read
{
    uint8_t i;
    xREAD_DataFromExNorFlash(ADDR_FLASH_UPGRADE_STATUS_INFO, gUpgradeInfo, NUM_FLASH_UPGRADE_INFO_TOTAL);
  /// For Information
    rprintf("[CP StatusF]sFlash Info.\r\n");
    rprintf( "gUpgradeInfo: ");

    for(i = 0;i<NUM_FLASH_UPGRADE_INFO_TOTAL;i++)
    {
        rprintf( "%02X ", gUpgradeInfo[i]);
    }
    rprintf( "\r\n");  
}

void WriteGlobalUpgradeInfo(void) 
{
  uint8_t nFor;
    rprintf("[CP StatusF] Sector Erase sFlash Info.\r\n");
    xERASE_ExFlash4kSector(FLASH_FIRMWARE_INFO);
    rprintf("[CP StatusF] Write sFlash Info.\r\n");
  xWRITE_DataToExNorFlash(FLASH_FIRMWARE_INFO, gUpgradeInfo, NUM_FLASH_UPGRADE_INFO_TOTAL);
}

uint8_t ChecksFlashFlagOk(uint8_t whereFrom)     // ������Ʈ Flag  UPDATE_OK Ȯ��
{
    uint8_t rResult = RESULT_NULL;
    //  if( strncmp( &gUpgradeInfo[0], "UpdateNeed", NUM_FLASH_UPGRADE_STATUS_INFO) == 0) {
    if ( whereFrom == UPDATE_FROM_NEW) 
    {
          if( gUpgradeInfo[INDEX_FLASH_UPGRADE_NEW_RESULT] == UPDATE_OK ) {
                rResult = RESULT_OK;
          }
          else {
                rResult = RESULT_FAIL;
          }
    }
    else {
          if( gUpgradeInfo[INDEX_FLASH_UPGRADE_BAK_RESULT] == UPDATE_OK ) {
                rResult = RESULT_OK;
          }
          else {
                rResult = RESULT_FAIL;
          }
    }
  return rResult;
}

uint8_t ReadChecksumAndLen(uint8_t where2Check)   // New NEW
{
    uint8_t rResult = RESULT_FAIL;
    uint8_t nIndexLen, nIndexCrc;
    
    if( where2Check == UPDATE_FROM_NEW) {
        nIndexLen = INDEX_FLASH_UPGRADE_NEW_LEN;
        nIndexCrc = INDEX_FLASH_UPGRADE_NEW_CRC;
    }
    else {
        nIndexLen = INDEX_FLASH_UPGRADE_BAK_LEN;
        nIndexCrc = INDEX_FLASH_UPGRADE_BAK_CRC;
    }

    ulBinLen = gUpgradeInfo[nIndexLen+0] <<24;  // FLASH_UPGRADE_NEW_XXX_LEN
    ulBinLen +=gUpgradeInfo[nIndexLen+1] <<16;
    ulBinLen +=gUpgradeInfo[nIndexLen+2] << 8;
    ulBinLen +=gUpgradeInfo[nIndexLen+3] << 0;

    ulBinCrc = gUpgradeInfo[nIndexCrc+0] <<24;  // FLASH_UPGRADE_NEW_XXX_CRC
    ulBinCrc +=gUpgradeInfo[nIndexCrc+1] <<16;
    ulBinCrc +=gUpgradeInfo[nIndexCrc+2] << 8;
    ulBinCrc +=gUpgradeInfo[nIndexCrc+3] << 0;

    if( ( ulBinLen > FIRM_MIN_LENGTH ) && ( ulBinLen < FIRM_MAX_LENGTH ) )   // Check Size
          rResult = RESULT_OK;
    else 
          rResult = RESULT_FAIL;
    return rResult;
}

void WriteUpdateInfoFlag(uint8_t whereTo, uint8_t whatInfo)
{
    uint8_t nIndexInfo, nIndexLen, nIndexCrc;
   
    if( whereTo == UPDATE_FROM_NEW) {
        nIndexInfo = INDEX_FLASH_UPGRADE_NEW_RESULT;
        nIndexLen = INDEX_FLASH_UPGRADE_NEW_LEN;
        nIndexCrc = INDEX_FLASH_UPGRADE_NEW_CRC;
    }
    else {
        nIndexInfo = INDEX_FLASH_UPGRADE_BAK_RESULT;
        nIndexLen = INDEX_FLASH_UPGRADE_BAK_LEN;
        nIndexCrc = INDEX_FLASH_UPGRADE_BAK_CRC;
    }
 ///*  
   if( whatInfo == UPDATE_OK) 
  {
    strncpy( (char *)&gUpgradeInfo[INDEX_FLASH_UPGRADE_STATUS_INFO], (const char *)fwCompileTime, 8); 
    gUpgradeInfo[8] = 0;    
    gUpgradeInfo[9] = 3;  
  }
 //*/
    gUpgradeInfo[nIndexInfo] = whatInfo;    
           
    gUpgradeInfo[nIndexLen+ 0] = ((ulBinLen >>24) & 0xff);  // ADDR_FLASH_UPGRADE_NEW_LEN
    gUpgradeInfo[nIndexLen+ 1] = ((ulBinLen >>16) & 0xff);
    gUpgradeInfo[nIndexLen+ 2] = ((ulBinLen >> 8) & 0xff);
    gUpgradeInfo[nIndexLen+ 3] = ((ulBinLen >> 0) & 0xff);

    gUpgradeInfo[nIndexCrc+ 0] = ((ulBinCrc >>24) & 0xff);  // ADDR_FLASH_UPGRADE_NEW_CRC
    gUpgradeInfo[nIndexCrc+ 1] = ((ulBinCrc >>16) & 0xff);
    gUpgradeInfo[nIndexCrc+ 2] = ((ulBinCrc >> 8) & 0xff);
    gUpgradeInfo[nIndexCrc+ 3] = ((ulBinCrc >> 0) & 0xff);

    WriteGlobalUpgradeInfo();
}

#ifdef LOG_DEBUG

int rprintf( const char* format, ...)
{
  if(g_DebugMode == TRUE)
  {
    char buf[512];
    va_list args;
    int i;
        
 //     OS_Use(&stringPutSem1);
      
    va_start(args, format);
        
        if( bRequestNeed == MON_FROM_USER)    // firmware update interface uses this com port.
        {
            i = vsprintf(buf, format, args);
            va_end(args);
            printf(buf);
        }
  
 //   OS_Unuse(&stringPutSem1);
    return i;
  }
  else
    return 0;
}
#endif

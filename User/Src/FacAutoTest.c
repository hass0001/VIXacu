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
#define    FACAUTOTEST_GLOBALS
#include   "FacAutoTest.h"

#define OS_Delay	MyMsDelay

/******************************************************************************
**
**  FacAutoTest
**
******************************************************************************/

int8_t AteAppCommandProcess(void)            // ATE Set or moniter  Process
{
		if( IsChecksumOk() == true)
		{
			switch( nAtePacketType)
			{
			case ATE_TYP_FIRMWARE:
				AteTreatFirmware();
					break;
			case ATE_TYP_HARD:
				AteTreatHardware();
					break;
			case ATE_TYP_MEMORY:
				AteTreatMemoryType();
					break;
			case ATE_TYP_PARAMTER:
				AteTreatParameter();
					break;
			case ATE_TYP_COMMUNICATION:
				AteTreatCommType();
					break;
			case ATE_TYP_TERMINAL:
				AteTreatTerminalType();
					break;

			case ATE_TYP_PANEL:
				AteTreatPanelType();
					break;
			default:
				AteTreatErrorType(ATE_ERROR_TYPE);
					break;
			}
		}
		else // checksum As3Erroror
		{
			AteTreatErrorType(ATE_ERROR_CHECKSUM);
		}

		nPreAtePacketIndex = nAtePacketIndex;
		nPackNum = 6;
		nAtePacketIndex = 0;
	//  memset(AteCommRx3Buf,0,COMM_ATE_DATA_BUF_LEN);

	return true;
}

uint8_t funcHexChar2Byte(uint8_t u8Char){
  uint8_t u8Return;
  if((u8Char >= '0')&&(u8Char <= '9'))    u8Return = u8Char - '0';
  else if((u8Char >= 'A')&&(u8Char <= 'F'))    u8Return = u8Char - 55;//'A' 65 - '7' 55 == 10
  else if((u8Char >= 'a')&&(u8Char <= 'f'))    u8Return = u8Char - 87;//'a' 97 - 'W' 87 == 10
  else u8Return = 0;
  return u8Return;
}

uint8_t IsChecksumOk(void)
{
	return true;
}

void AteTreatFirmware(void)
{
    uint8_t vVersion[21];
	uint8_t nFor = 0, nChecksum = 0;
    VS_GetVersionStr((char *)vVersion);
    vVersion[20] = 0;

    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 20;      			nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_FIRMWARE;   nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
    memcpy( (char *)&AteCommTx3Buf[ATE_STATE_DAT], (const char *)vVersion, 20);
    for( nFor = ATE_STATE_DAT; nFor < 24; nFor++)
    {
      nChecksum += AteCommTx3Buf[nFor];
    }
    AteCommTx3Buf[24] = nChecksum;
    AteCommTx3Buf[25] = ATE_CHAR_ETX;

    AteTx3Ctr = 26;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}

void AteTreatHardware(void)
{
	uint8_t nChecksum = 0;

    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 1;      		nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_HARD;   nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
    AteCommTx3Buf[ATE_STATE_DAT] = 1;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT];
    AteCommTx3Buf[5] = nChecksum;
    AteCommTx3Buf[6] = ATE_CHAR_ETX;

    AteTx3Ctr = 7;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}

void AteTreatMemoryType(void)
{
	uint32_t eepCnt = 0;
	uint32_t sflCnt = 0;
	uint8_t nChecksum = 0;
	
    FAC_Test_Eeprom(&eepCnt);
    FAC_Test_sFlash(&sflCnt);
    
    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 8;      		nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_MEMORY; nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
	
    AteCommTx3Buf[ATE_STATE_DAT+0] =  (byte)(eepCnt >> 24) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+0];
    AteCommTx3Buf[ATE_STATE_DAT+1] =  (byte)(eepCnt >> 16) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+1];
    AteCommTx3Buf[ATE_STATE_DAT+2] =  (byte)(eepCnt >>  8) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+2];
    AteCommTx3Buf[ATE_STATE_DAT+3] =  (byte)(eepCnt >>  0) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+3];
    AteCommTx3Buf[ATE_STATE_DAT+4] =  (byte)(sflCnt >> 24) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+4];
    AteCommTx3Buf[ATE_STATE_DAT+5] =  (byte)(sflCnt >> 16) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+5];
    AteCommTx3Buf[ATE_STATE_DAT+6] =  (byte)(sflCnt >>  8) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+6];
    AteCommTx3Buf[ATE_STATE_DAT+7] =  (byte)(sflCnt >>  0) & 0xFF;     			nChecksum += AteCommTx3Buf[ATE_STATE_DAT+7];
	
    AteCommTx3Buf[12] = nChecksum;
    AteCommTx3Buf[13] = ATE_CHAR_ETX;

    AteTx3Ctr = 14;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}

void AteTreatTerminalType(void)
{
    uint8_t nChecksum = 0;
    uint8_t nLock;

    for(nLock=0; nLock < MAX_NUM_DOOR; nLock++)
    	SetLockOnOff(nLock, true);// Relay on == door is unlocked..
	OS_Delay(3000);
    for(nLock=0; nLock < MAX_NUM_DOOR; nLock++)
    	SetLockOnOff(nLock, false);// Relay off == door is locked..

 //   if(FireLockFlag == 99) {
		isFireAlertOccur = FALSE;
		FireOccurrence = FALSE;
	    EEPROMSetData((INT16U) EEPROM_ADDRESS_FireFlag, 0);//eeprom 화재신호 지우기..
 //   }

    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 12;					nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_TERMINAL;	nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 

    AteCommTx3Buf[16] = nChecksum;
    AteCommTx3Buf[17] = ATE_CHAR_ETX;
    
    AteTx3Ctr = 18;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}

void AteTreatParameter(void)
{
  uint8_t pidData[15]; // = "TSN15022300010";
  uint8_t HardwareMac[6];
  uint8_t nFor, nChecksum = 0;
  
//  memcpy( pidData, &AteCommRx3Buf[ATE_STATE_DAT+0], 14);
//  pidData[14] = 0;
//  WritePIDToEeprom((INT8U *)pidData);

  HardwareMac[0] = AteCommRx3Buf[ATE_STATE_DAT+14];   // 0x7C;
  HardwareMac[1] = AteCommRx3Buf[ATE_STATE_DAT+15];   // 0x0A;
  HardwareMac[2] = AteCommRx3Buf[ATE_STATE_DAT+16];   // 0x9C;
  HardwareMac[3] = AteCommRx3Buf[ATE_STATE_DAT+17];   // 0x10;
  HardwareMac[4] = AteCommRx3Buf[ATE_STATE_DAT+18];   // 0x10;
  HardwareMac[5] = AteCommRx3Buf[ATE_STATE_DAT+19];   // 0x26;
  WriteMacAddress((INT8U *)HardwareMac);

  // memclr((pidData, 0, 14);
  AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
  AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
  AteCommTx3Buf[ATE_STATE_LEN] = 20;						nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
  AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_PARAMTER;		nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
  memcpy( (char *)&AteCommTx3Buf[ATE_STATE_DAT], (const char *)pidData, 14);
  memcpy( (char *)&AteCommTx3Buf[ATE_STATE_DAT+14], (const char *)&CMSParameter.MacAddress[0], 6);
  for( nFor = ATE_STATE_DAT; nFor < 24; nFor++)
  {
    nChecksum += AteCommTx3Buf[nFor];
  }
  AteCommTx3Buf[24] = nChecksum;
  AteCommTx3Buf[25] = ATE_CHAR_ETX;
  
  AteTx3Ctr = 26;	
  AteTx3Ptr = &AteCommTx3Buf[0];
  RequestOsSendTx3Data();
}

void AteTreatPanelType(void)
{
    uint8_t nChecksum = 0, setData = 0;
	if( AteCommRx3Buf[ATE_STATE_DAT+0] == 1) {
		setData =1;
	}

    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 1;					nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_PANEL;		nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
    AteCommTx3Buf[ATE_STATE_DAT] = setData;				nChecksum += AteCommTx3Buf[ATE_STATE_DAT];//
    AteCommTx3Buf[ATE_STATE_CHE] = nChecksum;
    AteCommTx3Buf[ATE_STATE_ETX] = ATE_CHAR_ETX;
    
    AteTx3Ctr = 7;	
    AteTx3Ptr = &AteCommTx3Buf[0];
     RequestOsSendTx3Data();
}

void AteTreatCommType(void)
{	
    uint8_t nChecksum = 0;
    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 1;						nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_COMMUNICATION;	nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
#if 0
    //if(EthernetReset_W5200() == TRUE)	// nRST_W6100�� ��ü�� ��
	{					
	    AteCommTx3Buf[ATE_STATE_DAT] = 1;	nChecksum += AteCommTx3Buf[ATE_STATE_DAT];// 
	}
	else 
	{
	    AteCommTx3Buf[ATE_STATE_DAT] = 0;	nChecksum += AteCommTx3Buf[ATE_STATE_DAT];// 
	}
#endif
	AteCommTx3Buf[ATE_STATE_CHE] = nChecksum;
    AteCommTx3Buf[ATE_STATE_ETX] = ATE_CHAR_ETX;
    
    AteTx3Ctr = 7;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}


void AteTreatErrorType(uint8_t As3ErrororKind)
{
    uint8_t nChecksum = 0;
    AteCommTx3Buf[ATE_STATE_PRM] = ATE_CHAR_PRM;
    AteCommTx3Buf[ATE_STATE_STX] = ATE_CHAR_STX;
    AteCommTx3Buf[ATE_STATE_LEN] = 1;				nChecksum += AteCommTx3Buf[ATE_STATE_LEN];
    AteCommTx3Buf[ATE_STATE_CMD] = ATE_TYP_READY;	nChecksum += AteCommTx3Buf[ATE_STATE_CMD];// 
    AteCommTx3Buf[ATE_STATE_DAT] = As3ErrororKind;		nChecksum += AteCommTx3Buf[ATE_STATE_DAT];// 
	AteCommTx3Buf[ATE_STATE_CHE] = nChecksum;
    AteCommTx3Buf[ATE_STATE_ETX] = ATE_CHAR_ETX;
    
    AteTx3Ctr = 7;	
    AteTx3Ptr = &AteCommTx3Buf[0];
    RequestOsSendTx3Data();
}

void RequestOsSendTx3Data(void)
{
    FwdTx1Ctr = AteTx3Ctr;
    FwdTx1Ptr = AteTx3Ptr;

	FirmwareRequestSendData();
}

void FAC_Test_Eeprom( uint32_t * As3ErrorCnt)
{
  test_fe = 0;
  ate_addr = EEPROM_ADDRESS_AteStart;
  eep_oData = 0;
  *As3ErrorCnt = 0;
  
  while ( ate_addr < EEPROM_ADDRESS_AteEnd) 
  {
    if(!test_fe)
    {
      test_fe=1;           // eeprom write(0x1500 ~ 0x1580) - 
      EEPROMSetData(ate_addr, eep_oData);
    }
    else
    {
      test_fe=0;                  // eeprom read(0x1500 ~ 0x1580) - 
      eep_iData = EEPROMGetData(ate_addr);

      if(eep_oData != eep_iData)
      {      // fail
        (*As3ErrorCnt)++;
//               printf("\r\n11+++++++++     Mission-I2C Interface fail    +++++++++");   
//                GPIO_ResetBits( LED_IPM_PORT, LED_IPM_PIN);
      }
      ate_addr++;            // address and data increment
      eep_oData++;
    }
  }
}

void FAC_Test_sFlash(uint32_t * As3ErrorCnt)
{
  test_sf = 0;
  ate_addr = FLASH_ATE_TEST_START;
  sfl_oData = 0;
  *As3ErrorCnt = 0;

  xERASE_ExFlash4kSector(FLASH_ATE_TEST_START);
          
  while( ate_addr < FLASH_ATE_TEST_END)
  {
    if(!test_sf)
    {
        test_sf=1;           // sflash write(0xf7000 ~ 0xf77df) - 
        xWRITE_DataToExNorFlash(ate_addr, &sfl_oData, 1);
    }
    else
    {
        test_sf=0;                  // sflash read(0xf7000 ~ 0xf77df) - 
        xREAD_DataFromExNorFlash(ate_addr, &sfl_iData, 1);

        if(sfl_oData != sfl_iData)
        {      // fail
                            (*As3ErrorCnt)++;
        }
      ate_addr++;            // address and data increment
      sfl_oData++;
    }
  }
}

uint16_t gu16CardResult;

void ATE_Test_AddAuth(void)
{
	uint8_t tString[256];
	// 41123499, 41D1B599,  0356B509, 12345678
	sprintf( tString, "C:20250326105130643:DATA UPDATE user CardNo=12345678 Pin=KS100  Password=");
	Httpc_TreatDataUpdate (tString, strlen( tString) );

	sprintf( tString, "C:20250522145805123:DATA UPDATE userauthorize Pin=KS100   AuthorizeTimezoneId=1   AuthorizeDoorId=   CardNo=12345678");
	Httpc_TreatDataUpdate (tString, strlen( tString) );

	uint8_t nWieg = 0;
	uint8_t wiegand[4];
	wiegand[0] = 0x12; wiegand[1] = 0x34; wiegand[2] = 0x56; wiegand[3] = 0x78;
    gu16CardResult = CheckRfCardDataInFlash( wiegand); //카드데이터 검색!!
    CheckEventCardAndLog( nWieg, wiegand, gu16CardResult);


	sprintf( tString, "C:20250522145819194:DATA DELETE userauthorize Pin=KS100   CardNo=12345678");
	Httpc_TreatDataDelete (tString, strlen( tString) );

    gu16CardResult = CheckRfCardDataInFlash( wiegand); //카드데이터 검색!!
    CheckEventCardAndLog( nWieg, wiegand, gu16CardResult);

}


void ATE_Test_VixpassSchedule(void)
{
	uint8_t tString[256];
	// 41123499, 41D1B599,  0356B509, 12345678
	sprintf( tString, "C:20250522145805123:DATA UPDATE timezone TimezoneId=2	SunTime1= 0	MonTime1=52430759	TueTime1=52430759	WedTime1=52430759	ThuTime1=52430759	FriTime1=52430759	SatTime1=0	Hol1Time1=0");
	Httpc_TreatDataUpdate (tString, strlen( tString) );

    LoadScheduleData();

	ChangeLockMode( LockMode_Schedule_Unlock, 0);

    DoorTimerProcess(); //도어의 lock mode를 체크해서 time code 확인으로 door 상태 변경 진행..

 }


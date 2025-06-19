/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: SCI1FwdTask.c
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
#define    SCI1_FWD_GLOBALS
#include   "SCI1FwdTask.h"

//  #define    DEBUG_PRINT_DOOR_SCHEDULE

void SCI1FwdTaskInit(void)
{
  uint32_t checkCrc = 0;
  uint16_t nRunning = 0;
  uint16_t nFwdTaskCycle = 0;

  BlockNbr = 0;
  UserMemoryMask = 0;
  FlashProtection = 0;
  bRequestNeed = MON_FROM_USER;
  Firmware_Upgrade = USER_UPGRADE_NONE;

  makeCrcGlobalTable();
  ReadGlobalUpgradeInfo();

  if( ChecksFlashFlagOk( UPDATE_FROM_NEW ) == RESULT_OK)
  {       // SPI TA���� ���׷��̵� UPDATE_OK �÷��� Ȯ��
    rprintf("[NOTICE]System Ready for Upgrade!\n");
    if( ReadChecksumAndLen(UPDATE_FROM_NEW) == RESULT_OK)
    {    // NEW  ulBinLen and ulBinCrc
      PreparesFlash4Write(UPDATE_FROM_BAK);
      rprintf("[NOTICE]Write program to Backup area!\n");
      rprintf("[NOTICE]Wait until Backup is completed!\n");
      WriteNewData2Bak();
      _CAN_LED_TOGGLE(); // LED OnOff

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

void SendCP_Status()
{
    uint8_t vVersion[21];
    VS_GetVersionStr((char *)vVersion);

    vVersion[20] = 0;
//	rprintf("[CP Status] %s\r\n", vVersion);//%s %d.%d.%d.%d %d %02x:%02x:%02x:%02x:%02x:%02x %d-%d-%d(%d) %d:%d:%d F%d .\r\n",
//        vVersion,TempPid,
//        CMSParameter.IPAddress[0],CMSParameter.IPAddress[1],CMSParameter.IPAddress[2],CMSParameter.IPAddress[3], nport,
//        CMSParameter.MacAddress[0],CMSParameter.MacAddress[1],CMSParameter.MacAddress[2],
//		CMSParameter.MacAddress[3],CMSParameter.MacAddress[4],CMSParameter.MacAddress[5],
//		toDayTime.YEAR, toDayTime.MON, toDayTime.DATE, toDayTime.WeekDAY, toDayTime.HOUR, toDayTime.MIN, toDayTime.SEC, 
//		FireOccurrence);
	
#ifdef DEBUG_PRINT_DOOR_SCHEDULE	
        rprintf("[CP Time] %d%d%d%d%d%d%d%d.\r\n",
          CMSParameter.DoorTimeCode[0][0],CMSParameter.DoorTimeCode[1][0],CMSParameter.DoorTimeCode[2][0],CMSParameter.DoorTimeCode[3][0],
          CMSParameter.DoorTimeCode[4][0],CMSParameter.DoorTimeCode[5][0],CMSParameter.DoorTimeCode[6][0],CMSParameter.DoorTimeCode[7][0]);
        {
          uint8_t nLock, nCode, nWeek;			
          for ( nLock = 0; nLock < MAX_NUM_DOOR; nLock++)
          {
            for( nCode = 0; nCode < MAX_NUM_TIMECODE; nCode++)			
            {
              for( nWeek = 0; nWeek < MAX_NUM_WEEK_HOLIDAY; nWeek++)				
              {
                rprintf("L[%d]C[%d]D[%d] [%02d:%02d][%02d:%02d]/",
                nLock,nCode,nWeek,
                ScheduleParameter[nLock][nCode][nWeek].StartHh,ScheduleParameter[nLock][nCode][nWeek].StartMm,
                ScheduleParameter[nLock][nCode][nWeek].EndHh,  ScheduleParameter[nLock][nCode][nWeek].EndMm);
              }
            }
          }
          if( nWeek == MAX_NUM_WEEK_HOLIDAY)	rprintf("\r\n");
	}
#endif
}		


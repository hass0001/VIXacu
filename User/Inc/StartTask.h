/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: StartTask.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _STARTTASK_
#define  _STARTTASK_

/******************************************************************************
**    Include Files
******************************************************************************/
#ifdef   STA_GLOBALS
#define  STA_EXT
#else
#define  STA_EXT   extern
#endif

#include    "vixConfig.h"

/******************************************************************************
**    Macro Define 
******************************************************************************/

/******************************************************************************
**    Type Define
******************************************************************************/
/******************************************************************************
**    Local Values
******************************************************************************/

/******************************************************************************
**    Global Values
******************************************************************************/
STA_EXT INT32U SystemResetFlag;
STA_EXT INT8U Firmware_Version[4];

/******************************************************************************
**    Prototype
******************************************************************************/
STA_EXT void SystemTask(void);

STA_EXT void ReloadParameter(void);
STA_EXT void CheckSystemInitial(void);
STA_EXT void ResetStatus(void);

STA_EXT void InitCMSComm(void);
STA_EXT void FireStatusReload(void);
STA_EXT void InitRTC(void);

STA_EXT void ReadPIDFromEeprom(void);
STA_EXT void WritePIDToEeprom(INT8U* data);
STA_EXT void ReadTimeCodeFromEeprom(void);
STA_EXT void WriteTimeCodeToEeprom(INT8U nlock);//AddressOffset, INT8U* data);
//STA_EXT void WriteDoorOpenTimeToEeprom(INT8U nlock);

#endif

								   


		    

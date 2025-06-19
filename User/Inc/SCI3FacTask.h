/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: FACTask.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _FACTASK_
#define  _FACTASK_			  								  

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"
						    
/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   FAC_GLOBALS
#define  FAC_EXT
#else
#define  FAC_EXT   extern
#endif

/******************************************************************************
**    Type Define
******************************************************************************/

/******************************************************************************
**    Protocol Define
******************************************************************************/

/******************************************************************************
**    Local Values
******************************************************************************/

/******************************************************************************
**    Type Define
******************************************************************************/
// Comm Stat
#define COMM_OK              0x00    //normal
#define COMM_NAK             0x01    //nack received
#define COMM_CHK_ERR          0x02    //received data checksum err
#define COMM_NO_RES           0x03    //no response
#define COMM_FAIL        0x04

#define YESTAG  1
#define NOTAG   0
/******************************************************************************
**    Local Values
******************************************************************************/


typedef enum 
{
	ENUM_SCI2RS485_TASK=0, ENUM_SCI1FWD_TASK, ENUM_SPI1NET_TASK, ENUM_WIEGAND_TASK, ENUM_IN_TASK, ENUM_FAC_TASK, ENUM_DEBUGCDC_TASK
}ENUM_TASK_NAME;

/******************************************************************************
**    Type Define 
******************************************************************************/
typedef struct
{
  INT8U SCI2Rs485Task;
  INT8U SCI1FwdTask;
  INT8U SPI1NetTask;
  INT8U WiegandTask;
  INT8U INTask;
  INT8U FACTask;
  INT8U DEBUGCDCTask;
}  __attribute__((packed)) TaskRunStatus;

/******************************************************************************
**    Global Values
******************************************************************************/

/******************************************************************************
** External Global Variables
******************************************************************************/
FAC_EXT  TaskRunStatus TaskRunning;

FAC_EXT  uint8_t g_isSetReader;

/******************************************************************************
**    Prototype
******************************************************************************/

FAC_EXT void FacTask(void);
FAC_EXT void initTaskRunStatus();
FAC_EXT void TaskRunCnt(INT8U taskName);
FAC_EXT void facLEDControl(void);

FAC_EXT void Rs232_Nesslab_SendCommand(uint8_t *, uint8_t);

FAC_EXT void Rs232StartInventory(bool);
FAC_EXT void Rs232Check900ReaderOk(void);

FAC_EXT void Rs232TreatCardPacket(INT8U*);

FAC_EXT void Rs232TreatReaderOk(void);
FAC_EXT void Rs232Set900ReaderFail(void);
FAC_EXT void Rs232Set900BuzzerOnOff(void);
FAC_EXT void Rs232Set900Channel(void);

FAC_EXT bool CHAFON_Crc16Cal(unsigned char const  * pucY, unsigned char ucX);

#endif

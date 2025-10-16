/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: History.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  __FACAUTOTEST_H_
#define  __FACAUTOTEST_H_


/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"
#include    "MonitorCmd.h"

/******************************************************************************
**    Macro Define 
******************************************************************************/
#ifdef   FACAUTOTEST_GLOBALS
#define  FACAUTOTEST_EXT
#else
#define  FACAUTOTEST_EXT   extern
#endif


#define COMM_ATE_DATA_RX_BUF_LEN		261    //256 +  5      250     //uint8_t ���� ũ�� �ȵ�..
#define COMM_ATE_DATA_TX_BUF_LEN		96     //64 +  5      250     //uint8_t ���� ũ�� �ȵ�..
/******************************************************************************
**    Card Check Info
******************************************************************************/

/******************************************************************************
**    Global Value 
******************************************************************************/
FACAUTOTEST_EXT uchar test_fe;
FACAUTOTEST_EXT uchar eep_oData;
FACAUTOTEST_EXT uchar eep_iData;
FACAUTOTEST_EXT uint32_t cnte;

FACAUTOTEST_EXT uchar test_sf;
FACAUTOTEST_EXT uchar sfl_oData;
FACAUTOTEST_EXT uchar sfl_iData;
FACAUTOTEST_EXT uint32_t ate_addr;

FACAUTOTEST_EXT uint8_t AteCommTx3Buf[COMM_ATE_DATA_TX_BUF_LEN];
FACAUTOTEST_EXT uint8_t AteCommRx3Buf[COMM_ATE_DATA_RX_BUF_LEN];

FACAUTOTEST_EXT uint8_t *AteTx3Ptr,*AteRx3Ptr,AteTx3Enable;
FACAUTOTEST_EXT uint8_t  AteRx3Length;
FACAUTOTEST_EXT uint16_t AteTx3Ctr;

FACAUTOTEST_EXT uint16_t  nAtePacketIndex, nAtePacketType, nPreAtePacketIndex, nPackNum;
FACAUTOTEST_EXT uint16_t nAteByteToReceive;

FACAUTOTEST_EXT uint8_t gU8PacketType;

//FACAUTOTEST_EXT uint8_t g_rfid900Data[16];
//FACAUTOTEST_EXT uint8_t gU8SensorCount[MAX_NUM_SENSOR];

FACAUTOTEST_EXT volatile uint8_t g_isReaderOk;
//FACAUTOTEST_EXT volatile uint8_t g_isCardData;
FACAUTOTEST_EXT volatile uint8_t g_u900LastOkTime;
FACAUTOTEST_EXT volatile uint8_t g_nChannelValue;
FACAUTOTEST_EXT volatile uint8_t g_bBeepValue;

/******************************************************************************
**    Function Define 
******************************************************************************/
FACAUTOTEST_EXT int8_t AteAppCommandProcess(void);            // ATE Set or moniter  Process
FACAUTOTEST_EXT void FAC_Test_Adc(void);
FACAUTOTEST_EXT void FAC_Test_Eeprom( uint32_t * errCnt);
FACAUTOTEST_EXT void FAC_Test_sFlash( uint32_t * errCnt);
FACAUTOTEST_EXT void FAC_Test_Rtc(void);
FACAUTOTEST_EXT void AteTreatParameter(void);

FACAUTOTEST_EXT uint8_t funcHexChar2Byte(uint8_t u8Char);
FACAUTOTEST_EXT uint8_t IsChecksumOk(void);

FACAUTOTEST_EXT void AteTreatFirmware(void);
FACAUTOTEST_EXT void AteTreatHardware(void);
FACAUTOTEST_EXT void AteTreatMemoryType(void);
FACAUTOTEST_EXT void AteTreatCommType(void);
FACAUTOTEST_EXT void AteTreatTerminalType(void);
FACAUTOTEST_EXT void AteTreatFacType(void);
FACAUTOTEST_EXT void AteTreatErrorType(uint8_t errorKind);
FACAUTOTEST_EXT void AteTreatPanelType(void);
FACAUTOTEST_EXT void ATE_Test_ID(uint8_t * CardData);
FACAUTOTEST_EXT void ATE_Test_SetTime(void);
FACAUTOTEST_EXT void RequestOsSendTx3Data(void);
FACAUTOTEST_EXT void AteSendParamReady(void);

#endif

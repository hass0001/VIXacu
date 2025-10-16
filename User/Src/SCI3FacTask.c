/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: FACTask.c
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
#define    FAC_GLOBALS
#include   "SCI3FacTask.h"

//extern IWDG_HandleTypeDef hiwdg;

//>x O 2<CR><LF> Sub mode 2
//>x O 3<CR><LF> Sub mode 3

// >f<CR><LF>  inventory
// >x d<CR><LF>  All default
// >x p 200 0 0 0<CR><LF>  ù��° ��Ʈ�� �Ŀ��� 250����..
// >y v 9<CR><LF>  version  
// const �� static �̳� �߿��� ������ ���̰� ����..????
/******************************************************************************/
//static uint8_t test_f;
/******************************************************************************/
// #define HISTORY_TEST
#define OS_Delay	MyMsDelay

void initTaskRunStatus()
{
	g_isSetReader = 0;
	memset(&TaskRunning, 0, sizeof(TaskRunStatus)); 
}

void TaskRunCnt(INT8U taskName)
{
  switch(taskName)
  {
    case ENUM_SCI2RS485_TASK:
      TaskRunning.SCI2Rs485Task=1;
      break;
    case ENUM_SCI1FWD_TASK: 
      TaskRunning.SCI1FwdTask=1;
      break;
    case ENUM_SPI1NET_TASK: 
      TaskRunning.SPI1NetTask=1;
      break;
    case ENUM_WIEGAND_TASK:
      TaskRunning.WiegandTask=1;
      break;
    case ENUM_IN_TASK: 
      TaskRunning.INTask=1;
      break;
    default:
      break;
  }
}
/******************************************************************************
**	Power LED Controller
******************************************************************************/
void facLEDControl(void) {
  static int8_t facLEDCounter = 0;

  if( facLEDCounter == 5)     {
	  _RUN_LED_TOGGLE();
      facLEDCounter = 0;
  }
  else  {
    facLEDCounter++;
  }
}

void Rs232_Nesslab_SendCommand(uint8_t *u8ArrSendData, uint8_t u8Length){
  memcpy( (char *)AteCommTx3Buf, u8ArrSendData, u8Length);

  AteTx3Ctr = u8Length;	
  AteTx3Ptr = &AteCommTx3Buf[0];
  RequestOsSendTx3Data();
  OS_Delay(500);
}

void Rs232StartInventory(bool bStart)
{
  INT8U u8ArrData[10] = ">f\r\n";// >f<CR><LF> Start inventory
  uint8_t u8Length = 4;
  if(!bStart)   u8ArrData[1] = '3';// >3<CR><LF> Stop inventory
  Rs232_Nesslab_SendCommand(u8ArrData, u8Length);
//  memcpy( (char *)AteCommTx3Buf, ">f\r\n", 4);  // >f<CR><LF>  inventory
//  AteCommTx3Buf[4] = 0x0;
//
  //  AteTx3Ctr = 5;	
//  AteTx3Ptr = &AteCommTx3Buf[0];
//  RequestOsSendTx3Data();
//  OS_Delay(500);
}

void Rs232Set900Channel(void)
{
  Rs232StartInventory(false);
  INT8U u8ArrData[10] = ">x e   \r\n";
  uint8_t u8Length = 8;
  if( g_nChannelValue > 9 ) {
    u8Length++;
    u8ArrData[5] = (g_nChannelValue/10) + '0';//0x30;  // >x e 10<CR><LF> //���׳� ��Ʈ�� 4321 bit value
  }
  u8ArrData[u8Length-3] = (g_nChannelValue%10) + '0';//0x30;  // >x e 15<CR><LF> //���׳� ��Ʈ�� 4321 bit value

  Rs232_Nesslab_SendCommand(u8ArrData, u8Length);
  Rs232StartInventory(true);
//  int nArrPointer=5;
//  memcpy( (char *)AteCommTx3Buf, ">x e ", 5); 
//  if( g_nChannelValue > 9 ) {
//    AteCommTx3Buf[nArrPointer++] = (g_nChannelValue/10) + '0';//0x30;  // >x e 10<CR><LF> //���׳� ��Ʈ�� 4321 bit value
//  }
//  AteCommTx3Buf[nArrPointer++] = (g_nChannelValue%10) + '0';//0x30;  // >x e 15<CR><LF> //���׳� ��Ʈ�� 4321 bit value
//  AteCommTx3Buf[nArrPointer++] = 0x0d;  //cr
//  AteCommTx3Buf[nArrPointer++] = 0x0a;  //lf
//  AteCommTx3Buf[nArrPointer++] = 0x0;  // 8 or 9
//
//  AteTx3Ctr = nArrPointer;	
//  AteTx3Ptr = &AteCommTx3Buf[0];
//  AteTx3Enable = 1;
//  RequestOsSendTx3Data();
//  OS_Delay(500);
}

void Rs232Set900BuzzerOnOff(void)
{
  Rs232StartInventory(false);
  INT8U u8ArrData[10] = ">x b  \r\n";
  uint8_t u8Length = 8;

  u8ArrData[5] = g_bBeepValue +'0';  // >x b 0<CR><LF> buzzer off
  Rs232_Nesslab_SendCommand(u8ArrData, u8Length);
  Rs232StartInventory(true);
//  int nArrPointer = 5;
//  memcpy( (char *)AteCommTx3Buf, ">x b ", 5);
//  AteCommTx3Buf[nArrPointer++] = g_bBeepValue + 0x30;  // >x b 0<CR><LF> buzzer off
//  AteCommTx3Buf[nArrPointer++] = 0x0d;  //
//  AteCommTx3Buf[nArrPointer++] = 0x0a;  //
//  AteCommTx3Buf[nArrPointer++] = 0x0;  // 8 or 9
//
//  AteTx3Ctr = nArrPointer;	
//  AteTx3Ptr = &AteCommTx3Buf[0];
//  AteTx3Enable = 1;
//  RequestOsSendTx3Data();
//  OS_Delay(500);
}

void Rs232TreatCardPacket(INT8U *u8arrCardData)
{
  uint16_t u16CardResult;
#ifdef EMBEDING_TESTCARD
//��õ ����μ�������� �ÿ���.. �׽�Ʈī�带 �׳� �ھƳ���.. //>1T 3400 23349001 5453 1905 0134142C
const uint8_t u8arrRegistedCardData[32] = "\xA0\x10\x10\x50\xA1\x10\x10\x50\xA2\x10\x10\x50\xA3\x10\x10\x50\x80\x10\x10\x50\x81\x10\x10\x50\x8A\x10\x10\x50\x8B\x10\x10\x50";
const uint8_t u8arrRegistedCardData2[16] = "\x65\x14\x34\x01\x66\x14\x34\x01\x67\x14\x34\x01\x2C\x14\x34\x01";
  if(memcmp(u8arrCardData, u8arrRegistedCardData+u8arrCardData[4]*4, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else if(memcmp(u8arrCardData, u8arrRegistedCardData+u8arrCardData[4]*4 + 16, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else if(memcmp(u8arrCardData, u8arrRegistedCardData2, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else if(memcmp(u8arrCardData, u8arrRegistedCardData2+4, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else if(memcmp(u8arrCardData, u8arrRegistedCardData2+8, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else if(memcmp(u8arrCardData, u8arrRegistedCardData2+12, 4) == 0){
    u16CardResult = (0x8000 >> u8arrCardData[4]) + 0x10;//#define Card_Registerd_OK   0
  }
  else{
    u16CardResult = 3;//#define Card_Not_Registerd    3
  }
#else  //
  u16CardResult = CheckRfCardDataInFlash(u8arrCardData, 1);
#endif
  CheckEventCardAndLog(u8arrCardData[4], u8arrCardData, u16CardResult);
}

void Rs232TreatReaderOk(void)
{
    g_isReaderOk = COMM_OK;
    g_u900LastOkTime = 0;
//    gReader900Alive = 0;
}

void Rs232Set900ReaderFail(void)
{
    g_isReaderOk = COMM_NO_RES;
    g_u900LastOkTime = 0;
//    gReader900Alive = 2;
   
    Rs232StartInventory(true);
}

//#define PRESET_VALUE 0xFFFF
//#define POLYNOMIAL  0x8408
bool CHAFON_Crc16Cal(unsigned char const  * pucY, unsigned char ucX) {
  unsigned char ucI,ucJ;
  unsigned short int  uiCrcValue = 0xFFFF;
  for(ucI = 0; ucI < ucX; ucI++)  {
    uiCrcValue = uiCrcValue ^ *(pucY + ucI);
    for(ucJ = 0; ucJ < 8; ucJ++)    {
      if(uiCrcValue & 0x0001)      {
      uiCrcValue = (uiCrcValue >> 1) ^ 0x8408;
      }
      else      {
      uiCrcValue = (uiCrcValue >> 1);
      }
    }
  }
  if((pucY[ucX] == uiCrcValue%0x100)&&(pucY[ucX+1] == uiCrcValue>>8)){
    return true;
  }
  else {
    return false;
  }
}

/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: SCI2Rs485Task.h
**   Project: TA000
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _SCI2RS485TASK_
#define  _SCI2RS485TASK_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   SCI2_485_GLOBALS
#define  SCI2_485_EXT
#else
#define  SCI2_485_EXT   extern
#endif

#define COMM_CS_DATA_RX_BUF_LEN		261     //256 +  5      300
#define COMM_CS_DATA_TX_BUF_LEN		69      //64 +  5       300

/******************************************************************************
**    Type Define
******************************************************************************/
#define CardLevel_Access	0x1
#define CardLevel_Master	0x2
#define CardLevel_Arm		0x4

#define ChkErr              1
#define ChkGood             0

#define SUCCESS             0				  
#define ERROR               1

#define CURRENT             0
#define LAST                1

//#define StxStart485          0x7e
#define StxStart485_TX          0xBB
#define StxStart485_RX          0xDD

#define CS_CMD_ACK                 0x2
#define CS_CMD_NACK                0x3

#define On          1
#define Off         0

#define CS_INDEX_STX         0
#define CS_INDEX_LENGTH         1
#define CS_INDEX_CONTROL         2
#define CS_INDEX_ID        3
#define CS_INDEX_DATA         4

//LastCommStat
#define CommOK              0x0    //normal
#define CommNak             0x1    //nack received
#define CommChkErr          0x2    //received data checksum err
#define CommNoRes           0x3    //no response

// DataLoadCtrlType
#define LoadNormal              0x0    // normal
#define LoadPrepairBuffer        0x1    // flash data
#define LoadDirectBuffer        0x2    // flash data
#define LoadConsoleParam        0x3    // Eeprom parameter
#define LoadNoResource           0x4   //Error

#define  Access_No_Key  0
#define  Access_Yes_Key  1
#define  Access_Wrong_Time  2
#define  Access_Wrong_Zone  3

#define  Result_OK  0
#define  Result_ERR_CSN  1
#define  Result_Wrong_Time  2
#define  Result_Wrong_Zone  4

/******************************************************************************
** TACS <-> Console or PC
******************************************************************************/
#define SEND_DEVICEINFO_AE      0xc0    //������� ����
#define SET_DATETIME_AB         0xc1    //��� �ð� ����


/******************************************************************************
** TA000-->Console
******************************************************************************/
#define ConTimeSet              0xa0
#define ConSoftReset            0xa1
#define ConAccResult            0xa2
#define ConCurrentStatAnswer    0xa3
#define ConLastStatAnswer       0xa4

/******************************************************************************
**  Console-->TA000
******************************************************************************/
#define ConPowerOn              0xb0
#define ConTamperStat           0xb1
#define ConAccess               0xb2
#define ConCurrentStatReq       0xb3
#define ConLastStatReq          0xb4

/******************************************************************************
** TA000-->PC
******************************************************************************/
#define ConSendCardBuffer          0xe3
#define ConSendConfiguration       0xe4
#define ConFirmwareResult          0xe5
#define ConSendDirectBuffer        0xe6

/******************************************************************************
**  PC-->TA000
******************************************************************************/
#define ConSetTime            0xc0
#define CS_CMD_SET_RF         0xc4

#define ConFirmwareStart      0xd0
#define ConFirmwareHeader     0xd1
#define ConFirmwareData       0xd2
#define ConFirmwareTail       0xd3
#define ConFirmwareStop       0xd4
#define ConSetCardData        0xd5
#define ConCheckCardData      0xd6
#define ConDeleteCardData     0xd7
#define ConClearCardBuffer    0xd8
#define ConGetCardBuffer      0xd9
#define ConSetConfig          0xda
#define ConGetConfig          0xdb

#define GetRFdata	0x01    //inventory..
#define GetReaderInformation	0x21
#define SetRegion	0x22
#define SetAddress	0x24
#define SetScanTime	0x25
#define SetBaudRate	0x28
#define SetPower	0x2F
#define Acousto_opticControl  	0x33
#define SetWiegand 	0x34
#define SetWorkMode	0x35
#define GetWorkMode	0x36
#define SetEasAccuracy	0x37
#define SyrisResponseOffset	0x38
#define TriggerOffset	0x3b

typedef struct  {
  INT8U nCurrZone;  
  INT8U nPrevZone;
  INT8U bSetChannel;  
  INT8U nCheckCnt;
  INT8U nCheckStep;       // 0: read 10 times, 1: check previous, 3: set flag 
}  __attribute__((packed)) stRfChannelSet;

typedef enum    //CR747��� �켱 ����
{
  CONTROL_CS_UNKNOWN	       	= 0
, CONTROL_CS_POR	        = 0xB0
, CONTROL_CS_AWAYSTAY           = 0xB1
, CONTROL_CS_ACCREQ    	        = 0xB2
, CONTROL_CS_CURR_DISPREQ       = 0xB3
, CONTROL_CS_DETD_DISPREQ    	= 0xB4
, CONTROL_CS_TAMPER           	= 0xB6
, CONTROL_CS_ARM_AUTODOOR      	= 0xBC
, CONTROL_CS_MKEY_DOWNLOADREQ       = 0xBF  //Mkey ������..
, CONTROL_CS_MKEY_UPLOAD   	= 0xBE  //Mkey ���ε� �Ѵ�!

, CONTROL_CS_ACK     		= 0x02
, CONTROL_CS_NACK         	= 0x03

, CONTROL_CS_KEY_PANIC          = 0xB7
, CONTROL_CS_PASS	        = 0x10
, CONTROL_CS_ARM_DISARM         = 0x11
, CONTROL_CS_BYPASS     	= 0x12
, CONTROL_CS_CURR_ZONE_REQ      = 0x13
, CONTROL_CS_DETD_ZONE_REQ    	= 0x14
, CONTROL_CS_BYPASS_ZONE_REQ  	= 0x15
, CONTROL_CS_CONSOL_INFO     	= 0x16
, CONTROL_CS_COMM_INFO     	= 0x17
, CONTROL_CS_EXP_INFO     	= 0x18
, CONTROL_CS_POWER_INFO     	= 0x19
, CONTROL_CS_CONSOLE_COMM_INFO	= 0x1a
, CONTROL_CS_VERSION_INFO	= 0x1b
, CONTROL_CS_MENU_END         	= 0x1c

, CONTROL_CS_CONSOLE_INFO_RPT 	= 0x1d
, CONTROL_CS_PATROL         	= 0x1e

, CONTROL_CS_DLS_CARD           = 0x30
, CONTROL_CS_DLS_CONN           = 0x31
, CONTROL_CS_DLS_PID_RX         = 0x32
, CONTROL_CS_DLS_GU_RX          = 0x33
, CONTROL_CS_DLS_DP_RX          = 0x34
, CONTROL_CS_DLS_CALLOUT        = 0x35

, CONTROL_CS_ZONE_NAME          = 0x20
, CONTROL_CS_ZONE_NAME_END      = 0x21

, CONTROL_CS_TIME_DATA_1     	= 0xA0
, CONTROL_CS_TIME_DATA_2     	= 0xA6
, CONTROL_CS_ICON_BEEP_VOICE   	= 0xA1
, CONTROL_CS_RESET         	= 0xA2
, CONTROL_CS_CURR_ZONE_DISP     = 0xA3
, CONTROL_CS_DETD_ZONE_DISP    	= 0xA4
, CONTROL_CS_ACESS_RESULT     	= 0xA5

, CONTROL_CS_MKEY_DOWNLOAD      = 0xAF  //Mkey �ٿ�ε� �޾���..
, CONTROL_CS_MKEY_UPLOADREQ    	= 0xAE  //Mkey ���ε� ����..

, CONTROL_CS_TTS_DATA     	= 0xC0

, CONTROL_CS_OPEN_RESULT 	  = 0xC2  //?? 초간 도어오픈 결과 "- Relay Number(1) : 0 - 1 사이의 수. -time(1) :  1~250
, CONTROL_CS_CHANGE_LOCK_MODE = 0xC3  // Lock mode 변경 - Relay Number(1) : 0 - 1 사이의 수. -Lock mode(1) :  0.수동풀림  1.수동잠금  2.일반모드(스케줄에따름)
, CONTROL_CS_CHANGE_OPENTIME  = 0xC4  // door open time 변경  - Relay Number(1) : 0 - 1 사이의 수. -door open time 1~250
, CONTROL_CS_REGIST_TIMECODE  = 0xC5  // time code 저장 등록0/삭제1/전부삭제2(1), Time Code Number(1):1~255,  (Start Time(2) End Time(2))*8
, CONTROL_CS_CHANGE_SCHEDULE  = 0xC6  // 스케줄 번호 변경 0은 스케쥴 없음..  LockNumber(1) Time Code Number(1)
, CONTROL_CS_REGIST_HOLIDAY   = 0xC7  // 휴일 등록,삭제 결과  등록0/삭제1/전부삭제2 (1), MM DD
, CONTROL_CS_RESULT_REBOOT    = 0xCF  // 리부트 명령 수행 결과..

} COMM_CS_CONTROL_TYPE;

/******************************************************************************
**    Local Values
******************************************************************************/


/******************************************************************************
**    Global Valuse
******************************************************************************/

// SCI2_485_EXT OS_RSEMA SCI2485Sem;
SCI2_485_EXT INT8U ConsolePollIndex;
SCI2_485_EXT INT16U nRs485PacketIndex;

SCI2_485_EXT INT8U Rs485CommTx2Buf[COMM_CS_DATA_TX_BUF_LEN];
SCI2_485_EXT INT8U Rs485CommRx2Buf[COMM_CS_DATA_RX_BUF_LEN];
SCI2_485_EXT INT8U CopyRx2Rs485Buf[COMM_CS_DATA_RX_BUF_LEN];

SCI2_485_EXT INT8U ConsoleTxBuffer[MAX_NUM_485CS][COMM_CS_DATA_TX_BUF_LEN];
//SCI2_485_EXT INT8U ConsoleTxIndex_IN[MAX_NUM_485CS];
//SCI2_485_EXT INT8U ConsoleTxIndex_OUT[MAX_NUM_485CS];

SCI2_485_EXT INT8U *Rs485Tx2Ptr, Rs485Tx2Enable;
SCI2_485_EXT INT8U Rs485Tx2Ctr;

//SCI2_485_EXT INT8U *Rs485Rx2Ptr;
SCI2_485_EXT INT16U  Rs485Rx2Length;

SCI2_485_EXT INT8U ConsoleCommStat[MAX_NUM_485CS];
SCI2_485_EXT INT8U ConsoleErrorCounter[MAX_NUM_485CS];
SCI2_485_EXT INT8U ConsoleRestoreCounter[MAX_NUM_485CS];
// SCI2_485_EXT INT8U ConsolePollCounter_Tx[MAX_NUM_485CS];
// SCI2_485_EXT INT8U ConsolePollCounter_Rx[MAX_NUM_485CS];
SCI2_485_EXT INT8U ConsoleNeedSetTime[MAX_NUM_485CS];
SCI2_485_EXT INT8U gF485SentChannel;

SCI2_485_EXT INT8U ConsoleLoadCtrlCount[MAX_NUM_485CS];
SCI2_485_EXT INT8U ConsoleLoadCtrlType[MAX_NUM_485CS];

SCI2_485_EXT stRfChannelSet gst900Channel;
SCI2_485_EXT volatile uint8_t Tx2DmaIng;
//SCI2_485_EXT INT8U ReceivedCardSerial[8];

/******************************************************************************
**    Prototype
******************************************************************************/
SCI2_485_EXT void  RS485_Init(void);
SCI2_485_EXT void SCI2Rs485Task(void);
SCI2_485_EXT void SCI2Rs485Process(void);
SCI2_485_EXT void ConsoleLEDControl(void);
SCI2_485_EXT void   ConsoleSetAddr(void);
SCI2_485_EXT void   ConsoleTestCard(void);

SCI2_485_EXT void Console485Enable(void);
SCI2_485_EXT void Console485Disable(void);
SCI2_485_EXT void ConsoleCommProcess(void);

SCI2_485_EXT void ConsoleDataSendStart(void);
SCI2_485_EXT INT8U ConsolePollingConfig(void);
SCI2_485_EXT INT8U ConsolePollIndexSelect(void);

SCI2_485_EXT void ConsoleTxDataLoad(void);
SCI2_485_EXT void ConsoleAckPacking(void);
SCI2_485_EXT void ConsoleCopyPacking(void);
SCI2_485_EXT void ConsoleRfChannelPacking();

SCI2_485_EXT void ConsoleNackPacking(void);
SCI2_485_EXT INT8U CheckSum485(INT8U *);

SCI2_485_EXT INT8U ConsoleDataReceive(void);
SCI2_485_EXT INT8U CheckSumVerify(void);

SCI2_485_EXT void makeLock900Parameter(void);

SCI2_485_EXT void AnalysisConsoleRxData(void);
SCI2_485_EXT void  ConsolePowerOnMethod(void);
SCI2_485_EXT void ConsoleTamperStatusSave(void);
SCI2_485_EXT void ConsoleTimeSetCsType1(INT8U PollIndex);
SCI2_485_EXT void ConsoleTimeSetCsType2(INT8U PollIndex);
SCI2_485_EXT void ConsoleSetTimeSend(void);
SCI2_485_EXT void ConsoleTimeGetPc(void);

SCI2_485_EXT void ConsoleAccessMethod(void);
SCI2_485_EXT void ConsoleReportDevStatus(void);

SCI2_485_EXT void ConsoleResultPack( INT8U voice);
SCI2_485_EXT void ConsoleFwResultPack( INT8U result);
//SCI2_485_EXT INT8U ConsoleSetDoorOpen(void);
SCI2_485_EXT void ResetConsoleStatus(void);

SCI2_485_EXT void	ConsoleTreatFirmwareStart(void);
SCI2_485_EXT void	ConsoleTreatFirmwareHeader(void);
SCI2_485_EXT void	ConsoleTreatFirmwareData(void);
SCI2_485_EXT void	ConsoleTreatFirmwareTail(void);
SCI2_485_EXT void	ConsoleTreatFirmwareStop(void);

SCI2_485_EXT void	Send2ConsoleCardData(void);

SCI2_485_EXT void	ConsoleSetCardData(void);
SCI2_485_EXT void	ConsoleCheckCardData(void);
SCI2_485_EXT void	ConsoleDeleteCardData(void);
SCI2_485_EXT void	ConsoleClearCardBuffer(void);
SCI2_485_EXT void	ConsoleSendCardBuffer(void);
SCI2_485_EXT void  ConsoleGetCardBufferDirect(void);
SCI2_485_EXT void	ConsoleSetConfiguration(void);
SCI2_485_EXT void	ConsoleGetConfiguration(void);

SCI2_485_EXT unsigned int uiCrc16Cal(unsigned char const  * pucY, unsigned char ucX);
SCI2_485_EXT void CHAFON_RU5106_ChangeMode(byte workmode);
SCI2_485_EXT void CHAFON_RU5106_Inventory(void);

SCI2_485_EXT void SetCardDataReceived(void);

#endif

/******************************************************************************
**
**   CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: SPI1Task.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _SPI1NETTASK_
#define  _SPI1NETTASK_                          

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"
                
/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   SPI1_GLOBALS
#define  SPI1_NET_EXT
#else
#define  SPI1_NET_EXT   extern
#endif

/******************************************************************************
**    Type Define
******************************************************************************/
#define TA_SERVER	1
#define PUSH_CLIENT	2
/******************************************************************************
**    ARU Values
******************************************************************************/
#define ARU_STX    0xA7
#define ARU_ETX    0x0C

#define ARU_STATE_STX  (0)
#define ARU_STATE_LEN  (1)
#define ARU_STATE_CMD  (2)
#define ARU_STATE_DAT  (3)
#define ARU_STATE_CRCH (4)
#define ARU_STATE_CRCL (5)

/******************************************************************************
**    TA Values
******************************************************************************/
#define TA_INDEX_HDR  (0)      // 0xCC
#define TA_INDEX_STX  (1)      // 0x02
#define TA_INDEX_ADDR (2)      // ACU Address
#define TA_INDEX_LENH (3)      // STX ~ ETX
#define TA_INDEX_LENL (4)      // 
#define TA_INDEX_CMD1 (5)
#define TA_INDEX_CMD2 (6)
#define TA_INDEX_DAT  (7)
#define TA_INDEX_ETX  (8)      // 0x03
#define TA_INDEX_SUM  (9)      // STX �������� ETX �������� Byte�� XOR�� ��

#define TA_INDEX_EVENT_CMD1   (8)
#define TA_INDEX_EVENT_CMD2   (9)
#define TA_INDEX_EVENT_DAT1  (10)
#define TA_INDEX_EVENT_DAT2  (11)
#define TA_INDEX_EVENT_TIME  (12)   //length 6
#define TA_INDEX_EVENT_CARD  (18)   //length 16
#define TA_INDEX_EVENT_INPUT (34)   //length 8
#define TA_INDEX_EVENT_LOGCOUNT (42)   //length 4

#define TA_HEADER_PC_MSG  (0xCC)
#define TA_HEADER_TA_MSG  (0xAA)

#define TA_MSGE_STX  (0x02)
#define TA_MSGE_ETX  (0x03)
/******************************************************************************
**    Fail Code Define
******************************************************************************/
#define Response_Success      1
#define Response_Fail        0

#define Msg_StatusNormal      '0'
#define Msg_StatusAbNormal    '1'

#define Msg_RegIDNew    '0'     // ���� / �űԵ��
#define Msg_RegIDSame   '1'     // ���� / ���� (������ ID�� ������ ��� ������)
#define Msg_RegIDOver   '2'     // ID�ڷ� �� �� �ʰ� ����. 

#define Error_Fail        0
#define Error_Write        1
#define  Error_Busy        2
#define Error_Comm        3
#define  Error_CRC        4     
#define  Error_NoErr        0xFF

/******************************************************************************
**    TA Values
******************************************************************************/
#define TA_Initial            0
#define TA_CheckSendData          1
#define TA_MakeFirstConnectData      2
#define TA_MakeKeyRequestData        3
#define TA_MakeEventData          4
#define TA_MakeAliveCheckData        5
#define TA_TryConnect            6
#define TA_SendPacket            7
#define TA_ReceiveData          8
#define TA_Disconnect              9
#define TA_CloseDelay             10  
#define TA_EventDelay             11  
#define TA_SendDelay             12

//define for Network Fail Alert
#define NetworkFail_SIP        0x04
#define NetworkFail_Internet    0x02
#define NetworkFail_Link      0x01


//#define TA_MonControl_Time    0x50
#define TA_MonControl_Access    0x51
#define TA_MonControl_Reset    0x52
#define TA_MonControl_Check    0x53

#define TA_MonControl_Fire    0x55
#define TA_MonControl_SetSchedule    0x56
#define TA_MonControl_SetTimeCode    0x57

#define TA_MonControl_Result    0x32

#define TA_Control_Power    0x60
#define TA_Control_Access    0x61
#define TA_Control_Stat    0x62
#define TA_Control_Port    0x63

#define TA_Control_Fire    0x65
#define TA_MonControl_GetSchedule    0x66
#define TA_MonControl_GetTimeCode    0x67

#define TA_Control_Result    0x42

#define TA_Access_ReportSchedule    0xF0
#define TA_Access_ReportCode     0xF1

/******************************************************************************
**    TA Command Values
******************************************************************************/
#define TA_LenResultAck  8  

#define TA_StrCmdCheckAcuStatus        0x4141  //  ("AA")  //  ACU�� ��� ���� Ȯ��
#define TA_StrCmdSetAcuTime            0x4142  //  ("AB")  //  ACU�� �ð�����.
#define TA_StrCmdCheckAcuTime          0x4143  //  ("AC")  //  ACU�� �ð����� Ȯ��..
#define TA_StrCmdResetAcu              0x4144  //  ("AD")  //  ACU�� �ʱ�ȭ.
#define TA_StrCmdGetAcuInfo            0x4145  //  ("AE")  //  ACU�� ���� Ȯ��.
#define TA_StrCmdAcuReboot             0x4146  //  ("AF")  //  ACU�� reboot
#define TA_StrCmdAcuInstallSite        0x4147  //  ("AG")  //  ACU�� ����Ʈ�� ����
#define TA_StrCmdAcuSetting            0x4153  //  ("AS")  //  ACU Setting

#define TA_StrCmdSetIDsg               0x4941  //  ("IA")  //  ID�ڷ� ���� ����. 
#define TA_StrCmdSetIDblocksg          0x4942  //  ("IB")  //  ID�ڷ� �� ���� (���� ��ä�� ��ü�ϴ� ���
#define TA_StrCmdCheckIDsg             0x4943  //  ("IC")  //  ID�ڷ� ����Ȯ��. ���X
#define TA_StrCmdDeleteIDsg            0x4944  //  ("ID")  //  ID�ڷ� ���� ����. 
#define TA_StrCmdDeleteIDAll           0x4945  //  ("IE")  //  ID�ڷ� ��ü ����. 
#define TA_StrCmdCheckIDCnt            0x4946  //  ("IF")  //  ID�ڷ� ��� �Ǽ� Ȯ��.
#define TA_StrCmdSetApbID              0x4947  //  ("IG")  //  ID�ڷ� Anti-passback ����.
#define TA_StrCmdCheckApbID            0x4948  //  ("IH")  //  ID�ڷ� Anti-passback ���� Ȯ��.
#define TA_StrCmdInitApbIDAll          0x4949  //  ("II")   //  ID�ڷ� Anti-passback ��ü �ʱ�ȭ.
#define TA_StrCmdDeleteIDBlock 	       0x494A  //  ("IJ")  //  ID�ڷ� Ư������ ����.
#define TA_StrCmdGetIDContent 	       0x494C  //  ("IL")  //  ID의 내용을 전달한다.
#define TA_StrCmdSetDoorZone           0x4441  //  ("DA")  //  DOOR ZONE ���� ����.
#define TA_StrCmdChkDoorzone           0x4442  //  ("DB")  //  DOOR ZONE ���� Ȯ��. 
#define TA_StrCmdDelDoorZone           0x4443  //  ("DC")  //  DOOR ZONE ���� ����.
#define TA_StrCmdDelDoorZoneAll        0x4444  //  ("DD")  //  DOOR ZONE ��ü ����.
#define TA_StrCmdSetDoorGroup          0x4445  //  ("DE")  //  DOOR GROUP ���� ����. 
#define TA_StrCmdChkDoorGroup          0x4446  //  ("DF")  //  DOOR GROUP ���� Ȯ��. 
#define TA_StrCmdDelDoorGroup          0x4447  //  ("DG")  //  DOOR GROUP ���� ����. 
#define TA_StrCmdDelDoorGroupAll       0x4448  //  ("DH")  //  DOOR GROUP ��ü ����.
#define TA_StrCmdSetTimeCode           0x5441  //  ("TA")  //  TIME CODE ���� ����.
#define TA_StrCmdChangeTimeCode        0x5442  //  ("TB")  //  ���Թ� TIME CODE ����
#define TA_StrCmdDelTimeCode           0x5443  //  ("TC")  //  TIME CODE ���� ����.
#define TA_StrCmdDelTimeCodeAll        0x5444  //  ("TD")  //  TIME CODE ��ü ����. 
#define TA_StrCmdSetTimeZone           0x5445  //  ("TE")  //  TIME ZONE ���� ����. 
#define TA_StrCmdChkTimeZone           0x5446  //  ("TF")  //  TIME CODE Ȯ��. 
#define TA_StrCmdDelTimeZone           0x5447  //  ("TG")  //  TIME ZONE ���� ����. 
#define TA_StrCmdDelTimeZoneAll        0x5448  //  ("TH")  //  TIME ZONE ��ü ����.
#define TA_StrCmdSetHoliday            0x4841  //  ("HA")  //  HOLIDAY ���� ����.
#define TA_StrCmdChkHoliday            0x4842  //  ("HB")  //  HOLIDAY ���� Ȯ��.
#define TA_StrCmdDelHoliday            0x4843  //  ("HC")  //  HOLIDAY ���� ����.
#define TA_StrCmdDelHolidayAll         0x4844  //  ("HD")  //  HOLIDAY ��ü ����. 
#define TA_StrCmdSetRelay              0x5941  //  ("YA")  //  RELAY ���� ����.
#define TA_StrCmdChkRelay              0x5942  //  ("YB")  //  RELAY ���� Ȯ��.
#define TA_StrCmdDelRelay              0x5943  //  ("YC")  //  RELAY ���� ����.
#define TA_StrCmdDelRelayAll           0x5944  //  ("YD")  //  RELAY ��ü ����. 
#define TA_StrCmdSetRelayGroup         0x5945  //  ("YE")  //  RELAY GROUP ���� ����.
#define TA_StrCmdChkRelayGroup         0x5946  //  ("YF")  //  RELAY GROUP ���� Ȯ��. 
#define TA_StrCmdDelRelayGroup         0x5947  //  ("YG")  //  RELAY GROUP ���� ����. 
#define TA_StrCmdDelRelayGroupAll      0x5948  //  ("YH")  //  RELAY GROUP ��ü ����. 
#define TA_StrCmdRelayUserSet          0x5949  //  ("YI")   //  RELAY USER ���. 
#define TA_StrCmdSetInput              0x5541  //  ("UA")  //  INPUT ���� ����. 
#define TA_StrCmdChkInput              0x5542  //  ("UB")  //  INPUT ���� ���� Ȯ��. 
#define TA_StrCmdDelInput              0x5543  //  ("UC")  //  INPUT ���� ����.
#define TA_StrCmdDelInputAll           0x5544  //  ("UD")  //  INPUT ��ü ����. 
#define TA_StrCmdSetReader             0x5241  //  ("RA")  //  READER ���� ����. 
#define TA_StrCmdChkReader             0x5242  //  ("RB")  //  READER ���� ���� Ȯ��. 
#define TA_StrCmdDelReader             0x5243  //  ("RC")  //  READER ���� ����. 
#define TA_StrCmdDelReaderAll          0x5244  //  ("RD")  //  READER ���� ����. 
#define TA_StrCmd485Reader             0x5253  //  ("RS")  //  RS485 ������ ����
#define TA_StrCmd485Status             0x5254  //  ("RT")  //  RS485 설정 리턴

#define TA_StrCmdAckEvent              0x4541  //  ("EA")  //  EVENT ping Ack.. EB �޽����� ���������..
#define TA_StrCmdGetEvent              0x4542  //  ("EB")  //  EVENT UPLOAD (EVENT ���� ���� �ʿ� ���� / �ڵ� ����).
#define TA_StrCmdDelEvent              0x4543  //  ("EC")  //  EVENT ���� ����. 
#define TA_StrCmdDelEventAll           0x4544  //  ("ED")  //  EVENT ��ü ����. 
#define TA_StrCmdChkEventCnt           0x4545  //  ("EE")  //  EVENT �Ǽ� Ȯ��. 
#define TA_StrCmdSetSubCnt             0x5341  //  ("SA")  //  SUB_BOARD �� ����. 
#define TA_StrCmdChkSubCnt             0x5342  //  ("SB")  //  SUB_BOARD �� ����Ȯ��.
#define TA_StrCmdSetSubType            0x5343  //  ("SC")  //  SUB_BOARD ���� ����.
#define TA_StrCmdChkSubVersion         0x5344  //  ("SD")  //  SUB_BOARD ���� Ȯ��. 
#define TA_StrCmdSetFire               0x4641  //  ("FA")  //  ȭ�� ���� ����.
#define TA_StrCmdChkFire               0x4642  //  ("FB")  //  ȭ�� ���� ����Ȯ��.
#define TA_StrCmdFireOccur             0x4643  //  ("FC")  //  ȭ�� �߻� / ����
#define TA_StrCmdSetAntiPassRule       0x5041  //  ("PA")  //  Anti-passback rule ��������. 
#define TA_StrCmdChkAntiPassRule       0x5042  //  ("PB")  //  Anti-passback rule ��������Ȯ��. 
#define TA_StrCmdDelAntiPassRule       0x5043  //  ("PC")  //  Anti-passback rule ��������.
#define TA_StrCmdDelAntiPassAll        0x5044  //  ("PD")  //  Anti-passback rule ��ü����. 

#define TA_FWUPDATE_START              0x4653      //"FS"
#define TA_FWUPDATE_HEADER             0x4648      //"FH"
#define TA_FWUPDATE_DATA               0x4644      //"FD"
#define TA_FWUPDATE_TAIL               0x4654      //"FT"
#define TA_FWUPDATE_END                0x4645      //"FE"

#define TA_StrCmdLogDisp       0x4C47  //  ("LG")  //  Log display 하기 위함

// u-id Data Type
#define UHS_Type_ReqDevConnection	    0       // 202
#define UHS_Type_Ping		        	1	// 100
#define UHS_Type_Registry		        2	// 101
#define UHS_Type_ExchangePublic			3      // 204
#define UHS_Type_ExchangeFactors		4      // 301
#define UHS_Type_GetRequest				5   // 201
#define UHS_Type_Push					7       // 203
#define UHS_Type_GetFirmware		    8      // 302
#define UHS_Type_SendCmdReturn			9      //	303
#define UHS_Type_CdataTable 		    10      // 401
#define UHS_Type_RtLog					11     // 402
#define UHS_Type_SetConsolePeriod403	    12      // 403	
#define UHS_Type_ChangeNodeSerial404	13      // 404
#define UHS_Type_ResetNode405			    14      // 405
#define UHS_Type_ChangeConsole406    	    15     // 406
#define UHS_Type_ChangeBattery407   	    16     // 407
#define UHS_Type_SetFireAlarm408           17     // 408
///////////////////////////////////////////////////////

#define TAErrorOK        0
#define TAError01        1
#define TAError02        2
#define TAError03        3
#define TAError04        4
#define TAError05        5
#define TAError06        6
#define TAError07        7

#define TAComm_Initial      0
#define TAComm_Server_Listen   1
#define TAComm_Abnormal_Delay  2
#define TAComm_End        3


#define UHSComm_Initial			    0
#define UHSComm_Client_Send   1
#define UHSComm_Receive     	2
#define UHSComm_End				3

/******************************************************************************
**    UDP Status Values
******************************************************************************/
#define UDP_Start        0
#define UDP_Init        1
#define UDP_Open			2
#define UDP_Send			3
#define UDP_Receive			4

#define UDP_SEND_BUFFER_SIZE	(256)	/**< Send Buffer Size */
#define UDP_RECV_BUFFER_SIZE		(256)	/**< Receive Buffer Size */

#define TACS_TCP_SERVER_PORT        14443      //������..
#define TACS_TCP_CLIENT_PORT        21327   //ȣ�� ��Ʈ..
#define UDP_SRV_PORT_FIRE		15693		/**< Fire Port Number */


#define TCP_CLIENT_IP0       106
#define TCP_CLIENT_IP1       245
#define TCP_CLIENT_IP2       226
#define TCP_CLIENT_IP3       42

/*
#define TCP_CLIENT_IP0       192  
#define TCP_CLIENT_IP1       168  
#define TCP_CLIENT_IP2       0  
#define TCP_CLIENT_IP3       114
*/
/*
#define TCP_CLIENT_IP0       192  
#define TCP_CLIENT_IP1       168  
#define TCP_CLIENT_IP2       0  
#define TCP_CLIENT_IP3       3
*/

typedef struct  {
  UWord8 *pData; 
  UWord16 length;
  BOOLEAN SetPacket;
  BOOLEAN GetPacket;
  BOOLEAN OverPacket;
}  __attribute__((packed)) TAComData;

typedef struct  {
  uint8_t u8SFlashSectorCount;  //sFlash Sector Size 0x1000 4096
  uint8_t u8EEPromPageCount;    //eeprom Page size   0x100  256
  uint8_t u8LogCount;
  uint8_t pU8EventLog[256];//16bytes * 16 log..
} __attribute__((packed)) TA_EventData;
/******************************************************************************
**    Global Values
******************************************************************************/
SPI1_NET_EXT TAComData     GVTAComData;
SPI1_NET_EXT TA_EventData  GVTAEventData;

SPI1_NET_EXT INT8U gU8LogCountFlag;

//#ifdef TACS_V244//FW_TACS_STM_V1.4.04_SC_210302.bin       ���������� CS10S ��ű�� �ο�.. 
SPI1_NET_EXT INT8U gU8ConsoleCardIDindexTCP;
SPI1_NET_EXT INT8U gU8ConsoleCardIDindex485;
SPI1_NET_EXT INT8U gU8ConsoleCardID[20][5];
//��� 'A', ���� 'D' ó���� 0 ����..  & ī��ID 4����Ʈ = 5����Ʈ
//#endif
//SPI1_NET_EXT uint8_t nNetPartMode;
//SPI1_NET_EXT INT8U TALEDIndicator;

SPI1_NET_EXT INT8U HttpcSubModeStep;
SPI1_NET_EXT INT8U TACommTaskStep;
//SPI1_NET_EXT INT8U UHSCommTaskStep;
SPI1_NET_EXT INT8U bSetEventUhsSend;
SPI1_NET_EXT INT8U bSetEventTa4Send;

//SPI1_NET_EXT INT8U Crc16Low;            
//SPI1_NET_EXT INT8U Crc16High;

SPI1_NET_EXT INT8U TASocketConnected;
SPI1_NET_EXT INT8U UHSSocketConnected;
SPI1_NET_EXT INT8U fOpenEthernet;

SPI1_NET_EXT INT16U TANeedUdpCheck;
//SPI1_NET_EXT INT16U TAAliveCheckTimer;

SPI1_NET_EXT INT8U UHSAliveCheckTimer;
SPI1_NET_EXT INT8U UHSAliveFailCounter;


SPI1_NET_EXT INT8U UdpSendReadyFlag;
SPI1_NET_EXT INT8U SSDPSendReadyFlag;


SPI1_NET_EXT uint8_t gU8ConsoleAlive;
//SPI1_NET_EXT uint8_t gReaderMsgSeq[4];

SPI1_NET_EXT INT8U NetworkFailAlertFlag;
//SPI1_NET_EXT INT8U NetworkDNSCheckTimer;
//SPI1_NET_EXT INT8U TACommKey;
//SPI1_NET_EXT INT8U IPM_TF_SendFlag;
//SPI1_NET_EXT INT8U IPM_TF_Timer;
//SPI1_NET_EXT INT8U IPM_TF_Restrore_Send;

SPI1_NET_EXT INT8U UDP_Step;    /**< UDP Step Check */
SPI1_NET_EXT uint32_t ftimeSpiOut;

SPI1_NET_EXT INT16U gU16EEpromFailCount;		//EEPROM sFalsh �����б� ���� ī��Ʈ... �ð����������� �ְ����� �߰��ؼ� ����...// 0x4142  //	("AB")
SPI1_NET_EXT INT16U gU16sFlashFailCount;		//EEPROM sFalsh �����б� ���� ī��Ʈ... �ð����������� �ְ����� �߰��ؼ� ����...// 0x4142  //	("AB")
//SPI1_NET_EXT   BOOLEAN EventSendFlag;

SPI1_NET_EXT INT8U nTa4EventSendType;

//SPI1_NET_EXT INT8U CMSSelect;
/******************************************************************************
**    Prototype
******************************************************************************/
SPI1_NET_EXT void SPI1NetTask(void);
SPI1_NET_EXT void TALEDControl(void);

SPI1_NET_EXT void EthernetCommunication(void);

SPI1_NET_EXT uint8_t convertNid2Index( uint16_t n16Id);

SPI1_NET_EXT void NetworkAlertProcess(void);
SPI1_NET_EXT BOOLEAN NetModuleCheck(void);
SPI1_NET_EXT BOOLEAN NetworkLinkCheck(void);
SPI1_NET_EXT void NetworkDNSCheck(void);
SPI1_NET_EXT BOOLEAN NetRJ45Check(void);

SPI1_NET_EXT INT8U TAPacketCheck(void);
SPI1_NET_EXT INT16U NetMessageCheck(byte *RxBuffer );

SPI1_NET_EXT INT8U TAConnectionPacketCheck(void);
SPI1_NET_EXT void TAErrorPacketSend(INT8U ErrorCode);
SPI1_NET_EXT void TAMakeServerPacking(void);

SPI1_NET_EXT void TAPacketAnalysis(void);
SPI1_NET_EXT void TAPacketSend(INT16U Length);
SPI1_NET_EXT void TATreatControl(INT8U Action, INT8U InputId);

SPI1_NET_EXT INT8U TACheckJumperSetting( void);

SPI1_NET_EXT void TAMakeResponsePacket( INT16U SendCommand);
SPI1_NET_EXT INT16U TAGetPacketLength( INT16U SendCommand);

SPI1_NET_EXT void SSDPReportMyMAC(void);
SPI1_NET_EXT void SSDPReportMyIp(void);
SPI1_NET_EXT void SSDPSetMyIp(void);
SPI1_NET_EXT void UdpSetDNSServer(void);

SPI1_NET_EXT void SetSSDPSendFireAlarm(void);
SPI1_NET_EXT void UdpServerProcess(void);
SPI1_NET_EXT INT8U UdpProcess(INT8U s, INT8U Action);

SPI1_NET_EXT void UHSMakeHeader(INT16U Length, INT16U EndIndex, INT16U CurrentIndex, INT8U DataType, INT8U ControlCode);
SPI1_NET_EXT void UHSConnectPacking(void);
SPI1_NET_EXT void UHSAlivePacking(void);
SPI1_NET_EXT void UHSEventPacking(void);
SPI1_NET_EXT INT8U UHSPacketCheck(void);
SPI1_NET_EXT void UHSMethod(void);
SPI1_NET_EXT void UHSConnectionProcess(void);
SPI1_NET_EXT void UHSParameterProcess(void);
SPI1_NET_EXT void UHSEventProcess(void);
SPI1_NET_EXT void UHSRemoteProcess(void);
SPI1_NET_EXT void UHSStatusProcess(void);


#endif

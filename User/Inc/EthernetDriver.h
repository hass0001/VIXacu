/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: EthernetDriver.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _ETHERNET_
#define  _ETHERNET_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Define
******************************************************************************/
#ifdef   ETHERNET_GLOBALS
#define  ETHERNET_EXT
#else
#define  ETHERNET_EXT   extern
#endif

typedef struct
{
  INT8U IPAddress[4];
  INT16U PortNumber;
} __attribute__((packed)) EthernetDestinationInfo;

ETHERNET_EXT EthernetDestinationInfo EthernetDest[2];

ETHERNET_EXT INT8U UHSRxPacket[Internet_UHSBuf_Size];   // 1024
ETHERNET_EXT INT8U UHSTxPacket[Internet_UHSBuf_Size];

ETHERNET_EXT INT8U TABuf_Rx[Internet_TABuf_Size];
ETHERNET_EXT INT8U TABuf_Tx[Internet_TABuf_Size];//Internet_TABuf_Size];

ETHERNET_EXT INT8U DNSPacket[600];

ETHERNET_EXT const INT8U MemSize_Tx[8];
ETHERNET_EXT const INT8U MemSize_Rx[8];

/******************************************************************************
**
**    Prototype
**
******************************************************************************/
ETHERNET_EXT uint8_t EthernetLinkCheck(void);
ETHERNET_EXT uint8_t EthernetSocketOpen(uint8_t SockNum, uint8_t Protocol, uint16_t Port);
ETHERNET_EXT uint8_t EthernetSocketClose(uint8_t SockNum);
ETHERNET_EXT uint8_t EthernetSocketConnect(uint8_t SockNum, uint8_t* pIpAddr, uint16_t Port);
ETHERNET_EXT uint8_t EthernetSocketDisconnect(uint8_t SockNum);
ETHERNET_EXT uint8_t EthernetSocketListen(uint8_t SockNum);
ETHERNET_EXT uint8_t EthernetNetworkSetting(void);

ETHERNET_EXT void Ethernet_Write_Data(INT8U s, INT8U *src, INT16U dst, INT16U len);
ETHERNET_EXT void Ethernet_Read_Data(INT8U s, INT16U src, INT8U *dst, INT16U len);
ETHERNET_EXT uint8_t EthernetReset_W6100(void);
ETHERNET_EXT void EthernetSetNetwork(void);
ETHERNET_EXT uint8_t EthernetRJ45Check(void);

ETHERNET_EXT uint16_t EthernetSendPacketTo(uint8_t SockNum, uint8_t* pData, uint16_t Length, uint8_t* pDestIp, uint16_t DestPort);
ETHERNET_EXT uint16_t TA_ServerSocket_Handling(uint8_t SockNum, uint16_t Port);
ETHERNET_EXT uint16_t TA_ClientSocket_Handling(uint8_t SockNum, uint16_t MyPort, uint8_t* pDestIP, uint16_t pDestPort);

ETHERNET_EXT uint8_t WriteMacAddress(uint8_t* pData);
ETHERNET_EXT uint8_t ReadMacAddress(void);
ETHERNET_EXT uint8_t WriteIPToEeprom(uint8_t* pData);
ETHERNET_EXT uint8_t WriteSnToEeprom(uint8_t* pData);
ETHERNET_EXT uint8_t WriteGwToEeprom(uint8_t* pData);
ETHERNET_EXT uint8_t WriteHttpPortToEeprom(uint8_t* pData);

#endif
 

				  		 

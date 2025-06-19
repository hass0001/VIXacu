/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: EthernetDriver.c
**   Project: TA000
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
/******************************************************************************
**   Include Files
******************************************************************************/
#define    ETHERNET_GLOBALS 

#include   "EthernetDriver.h"

/******************************************************************************
**   Local Function
******************************************************************************/

/******************************************************************************
**   Ethernet Configuration Values
******************************************************************************/
const INT16U Ethernet_Max_Packet[MAX_SOCK_NUM] = {1050, 512, 600, 4096, 4096, 4096, 1024, 256};

const INT16U EthernetRetryTimeout = 10000;	//6000
const INT8U EthernetRetryCount = 5;	//3 

const INT8U MemSize_Tx[8] = {2,1,1,4,4,2,1,1};
const INT8U MemSize_Rx[8] = {2,1,1,4,4,2,1,1};

INT16U EthernetTXBufSize[MAX_SOCK_NUM];
INT16U EthernetRXBufSize[MAX_SOCK_NUM];
INT16U EthernetTXBufMask[MAX_SOCK_NUM];
INT16U EthernetRXBufMask[MAX_SOCK_NUM];
INT16U TXBUF_BaseAddress[MAX_SOCK_NUM];
INT16U RXBUF_BaseAddress[MAX_SOCK_NUM];


uint8_t EthernetSocketOpen(uint8_t SockNum, uint8_t Protocol, uint16_t Port)
{
	if(SOCK_OK == socket(SockNum, Protocol, Port, 0x00))
		return TRUE;
	else
		return FALSE;
}

uint8_t EthernetSocketClose(uint8_t SockNum)
{
	if(SOCK_OK == close(SockNum))
		return TRUE;
	else
		return FALSE;
}

uint8_t EthernetSocketConnect(uint8_t SockNum, uint8_t* pIpAddr, uint16_t Port)
{
	uint8_t ret;

	if(((pIpAddr[0] == 0xFF) && (pIpAddr[1] == 0xFF) && (pIpAddr[2] == 0xFF) && (pIpAddr[3] == 0xFF)) || \
	   	((pIpAddr[0] == 0x00) && (pIpAddr[1] == 0x00) && (pIpAddr[2] == 0x00) && (pIpAddr[3] == 0x00)) || \
	   	(Port == 0x00) ) 
 	{
 		ret = FALSE;
	}
	else
	{
		ret = connect(SockNum, pIpAddr, Port, _IPV4_IP_BYTE_LEN);
		if(ret == SOCK_OK)
			return TRUE;
		else
			return FALSE;
	}
	return 0;
}

uint8_t EthernetSocketDisconnect(uint8_t SockNum)
{
	if(SOCK_OK == disconnect(SockNum))
		return TRUE;
	else
		return FALSE;
}

uint8_t EthernetSocketListen(uint8_t SockNum)
{
	if(SOCK_OK == listen(SockNum))
		return TRUE;
	else
		return FALSE;
}
  
uint8_t EthernetNetworkSetting(void)
{
	rprintf("DHCP: Static IP Use\r\n");
	rprintf("IP %d,%d,%d,%d\r\n", CMSParameter.IPAddress[0], CMSParameter.IPAddress[1], CMSParameter.IPAddress[2],	CMSParameter.IPAddress[3]);

	EthernetSetNetwork();
	return TRUE;
}

void Ethernet_Write_Data(INT8U s, INT8U *src, INT16U dst, INT16U len)
{
	INT16U size;
	INT16U dst_mask;
	INT16U dst_ptr;

	dst_mask = dst & EthernetTXBufMask[s];
	dst_ptr = TXBUF_BaseAddress[s] + dst_mask;
	
	if (dst_mask + len > EthernetTXBufSize[s]) 
	{
		size = EthernetTXBufSize[s] - dst_mask;
		WIZCHIP_WRITE_BUF(dst_ptr, src, size);
		src += size;
		size = len - size;
		dst_ptr = TXBUF_BaseAddress[s];
		WIZCHIP_WRITE_BUF(dst_ptr, src, size);
	} 
	else
	{
		WIZCHIP_WRITE_BUF(dst_ptr, src, len);
	}
}

void Ethernet_Read_Data(INT8U s, INT16U src, INT8U *dst, INT16U len)
{
	INT16U size;
	INT16U src_mask;
	INT16U src_ptr;

	src_mask = src & EthernetRXBufMask[s];
	src_ptr = RXBUF_BaseAddress[s] + src_mask;
	
	if( (src_mask + len) > EthernetRXBufSize[s] ) 
	{
		size = EthernetRXBufSize[s] - src_mask;
		WIZCHIP_READ_BUF(src_ptr, dst, size);
		dst += size;
		size = len - size;
		src_ptr = RXBUF_BaseAddress[s];
		WIZCHIP_READ_BUF(src_ptr, dst, size);
	} 
	else
	{
		WIZCHIP_READ_BUF(src_ptr, dst, len);
	}
}

uint16_t EthernetSendPacketTo(uint8_t SockNum, uint8_t* pData, uint16_t Length, uint8_t* pDestIp, uint16_t DestPort)
{
	uint16_t ret = 0;
		
	if (Length > Ethernet_Max_Packet[SockNum])
		ret = Ethernet_Max_Packet[SockNum]; 		// check size not to exceed MAX size.
    	else
		ret = Length;

	if(((pDestIp[0] == 0x00) && (pDestIp[1] == 0x00) && (pDestIp[2] == 0x00) && (pDestIp[3] == 0x00)) || \
		((DestPort == 0x00)) ||(ret == 0))
 	{
 		ret = 0;
	}
	else
	{
		ret = sendto(SockNum, pData, Length, pDestIp, DestPort, 4);
	}

	return ret;
}

uint8_t EthernetLinkCheck(void)
{
	int8_t ret;
	intr_kind temp;

	ret = ctlwizchip(CW_GET_PHYLINK,(void*)&temp);

	if(ret == 	-1 || temp == PHY_LINK_OFF)
		return FALSE;
	else
		return TRUE;
}

uint8_t EthernetRJ45Check(void)
{
	int8_t ret;
	intr_kind temp;

	ret = ctlwizchip(CW_GET_PHYSTATUS,(void*)&temp);

	if(temp & 0x10100)
		return TRUE;
	else
		return FALSE;
}

uint16_t TA_ServerSocket_Handling(uint8_t SockNum, uint16_t Port)
{
	int32_t ret;
    	int8_t status,inter;
   	uint16_t received_size;
    	uint8_t arg_tmp8;

	uint16_t ReturnStatus = FALSE;
	uint8_t* data_buf;

	//if(SockNum == Socket_TA)	
	//	return FALSE;

	getsockopt(SockNum, SO_STATUS, &status);

	switch(status)
	{
      		case SOCK_ESTABLISHED :

			ReturnStatus = SocketIdle;
			ctlsocket(SockNum,CS_GET_INTERRUPT,&inter);

			if(inter & Sn_IR_CON)
	            	{
	             	arg_tmp8 = Sn_IR_CON;
	            		ctlsocket(SockNum,CS_CLR_INTERRUPT,&arg_tmp8);
	             }

	            getsockopt(SockNum,SO_RECVBUF,&received_size);

			if(received_size > 0)
			{
				data_buf = TABuf_Rx;
				memset(TABuf_Rx, 0, sizeof(TABuf_Rx));
				if(received_size > Ethernet_Max_Packet[SockNum]) received_size = Ethernet_Max_Packet[SockNum];

				if(received_size > DATA_BUF_SIZE)
					received_size = Ethernet_Max_Packet[SockNum];
				else
					return FALSE;

	                	ret = recv(SockNum, data_buf, received_size);
				if(ret > 0)
					ReturnStatus = ret;
	            	}

			break;

		case SOCK_CLOSE_WAIT :

            		getsockopt(SockNum, SO_RECVBUF, &received_size);

			if(received_size > 0)
            		{
				data_buf = TABuf_Rx;
				memset(TABuf_Rx, 0, sizeof(TABuf_Rx));

				if(received_size > Ethernet_Max_Packet[SockNum])
					received_size = Ethernet_Max_Packet[SockNum];
				else
					return FALSE;
	
                		ret = recv(SockNum, data_buf, received_size);
                		if(ret > 0)
					ReturnStatus = ret;
            		}

			EthernetSocketDisconnect(SockNum);
			break;

 	case SOCK_INIT :

		EthernetSocketListen(SockNum);
      		break;

	case SOCK_CLOSED:

		EthernetSocketOpen(SockNum, Sn_MR_TCP4, Port);
		break;

	case SOCK_LISTEN :

		ReturnStatus = SocketListen;
		break;

	default:
      		break;
	}

	return ReturnStatus;
}

uint16_t TA_ClientSocket_Handling(uint8_t SockNum, uint16_t MyPort, uint8_t* pDestIP, uint16_t pDestPort)
{							
//	datasize_t sentsize=0;
	uint8_t status,inter;
// 	,addr_len;
//	uint8_t tmp = 0;
	uint8_t arg_tmp8;
//	wiz_IPAddress destinfo;

	uint16_t ret;
	uint16_t received_size;
	uint8_t* data_buf;
	uint16_t ReturnStatus = FALSE;


	getsockopt(SockNum,SO_STATUS,&status);

	switch(status)
	{
		case SOCK_ESTABLISHED:

			ReturnStatus = SocketIdle;

			ctlsocket(SockNum,CS_GET_INTERRUPT,&inter);
			if(inter & Sn_IR_CON)	// Socket n interrupt register mask; TCP CON interrupt = connection with peer is successful
        		{
            			arg_tmp8 = Sn_IR_CON;
            			ctlsocket(SockNum,CS_CLR_INTERRUPT,&arg_tmp8);// this interrupt should be write the bit cleared to '1'
        		}

			getsockopt(SockNum, SO_RECVBUF, &received_size);

       		if(received_size > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
       		{
				data_buf = TABuf_Rx;
				memset(TABuf_Rx, 0, sizeof(TABuf_Rx));
       		}
			else
				data_buf = TABuf_Rx;

			if(received_size > Ethernet_Max_Packet[SockNum])
				received_size = Ethernet_Max_Packet[SockNum];

			ret = recv(SockNum, data_buf, received_size); // Data Receive process (H/W Rx socket buffer -> User's buffer)
			if(ret <= 0)
				return ret; // If the received data length <= 0, receive failed and process end

			ReturnStatus = ret;

			break;

		case SOCK_CLOSE_WAIT:
			
			getsockopt(SockNum, SO_RECVBUF, &received_size);

			if((received_size = getSn_RX_RSR(SockNum)) > 0) // Sn_RX_RSR: Socket n Received Size Register, Receiving data length
        		{
				if(SockNum == Socket_TA)
				{
					data_buf = TABuf_Rx;
					memset(TABuf_Rx, 0, sizeof(TABuf_Rx));
				}
				else
					data_buf = TABuf_Rx;
				
            			if(received_size > Ethernet_Max_Packet[SockNum])
					received_size = Ethernet_Max_Packet[SockNum]; // DATA_BUF_SIZE means user defined buffer size (array)

				ret = recv(SockNum, data_buf, received_size); // Data Receive process (H/W Rx socket buffer -> User's buffer)
            			if(ret <= 0)
					return ret; // If the received data length <= 0, receive failed and process end

				received_size = ret;
       		}

			if(SOCK_OK != disconnect(SockNum))
				ReturnStatus = FALSE;

			break;

		case SOCK_CLOSED:

			socket(SockNum, Sn_MR_TCP4, MyPort, SOCK_IO_NONBLOCK);
    		break;

		case SOCK_INIT:

			//if(loopback_mode == AS_IPV4)
          		if(SOCK_OK != connect(SockNum, pDestIP, pDestPort, 4))
          			ReturnStatus = FALSE;	
          		//else if(loopback_mode == AS_IPV6)
          		//ret = connect(sn, destip, destport, 16); /* Try to connect to TCP server(Socket, DestIP, DestPort) */

			break;  

		default:
            		break;   
	}

	return ReturnStatus;
}

uint8_t WriteMacAddress(uint8_t* pData)
{
	uint16_t writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_MAC);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _MAC_BYTE_LEN))
		return FALSE;
	
	return FALSE;
}

uint8_t ReadMacAddress(void)
{
	uint8_t* ReadPtr = NULL;
 	uint16_t ReadAddr = 0;

  	ReadAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_MAC);
  	ReadPtr = &CMSParameter.MacAddress[0];

	if(_Ok != xREAD_DataFromEeprom(ReadAddr, ReadPtr, _MAC_BYTE_LEN))
		return FALSE;

  	rprintf("Read Mac Address from Module <%02x:%02x:%02x:%02x:%02x:%02x>\r\n",
    				CMSParameter.MacAddress[0],CMSParameter.MacAddress[1],CMSParameter.MacAddress[2],
    				CMSParameter.MacAddress[3],CMSParameter.MacAddress[4],CMSParameter.MacAddress[5]);

	return TRUE;
}

uint8_t WriteHttpIPToEeprom(uint8_t* pData)
{
	uint16_t writeAddr = (uint16_t)(E2P_Para_HTTPC+0);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_IP_BYTE_LEN))
		return FALSE;
/*
    	pData[3] = 1;
    	writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_GW);
	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_GATEWAY_BYTE_LEN))
		return FALSE;
*/
	else
		return TRUE;
}


uint8_t WriteHttpPortToEeprom(uint8_t* pData)
{
	uint16_t writeAddr = (uint16_t)(E2P_Para_HTTPC+4);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, 2))
		return FALSE;
/*
    	pData[3] = 1;
    	writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_GW);
	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_GATEWAY_BYTE_LEN))
		return FALSE;
*/
	else
		return TRUE;
}

uint8_t WriteIPToEeprom(uint8_t* pData)
{
	uint16_t writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_IP);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_IP_BYTE_LEN))
		return FALSE;
/*
    	pData[3] = 1;
    	writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_GW);
	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_GATEWAY_BYTE_LEN))
		return FALSE;
*/
	else
		return TRUE;
}

uint8_t WriteSnToEeprom(uint8_t* pData)
{
    	uint16_t writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_SNMASK);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_SUBNET_BYTE_LEN))
		return FALSE;
	else	
		return TRUE;
}

uint8_t WriteGwToEeprom(uint8_t* pData)
{
	uint16_t writeAddr = (uint16_t)(E2P_Para_CMS+EEPROM_PARAM_INDEX_GW);

	if(_Ok != xWRITE_DataToEeprom(writeAddr, pData, _IPV4_GATEWAY_BYTE_LEN))
		return FALSE;
	else
		return TRUE;
}

uint8_t EthernetReset_W6100(void)
{
#if 1
	NetworkFailAlertFlag = 0;
	return TRUE;
#else
	INT8U Status = 0;
	static INT8U ResetW5200Counter = 0;

	if ( ResetW5200Counter >= 15 ) {
		rprintf("Reset Network Module\r\n");	
		ResetW5200Counter = 0;

		nRST_w6100();
	}
	else 
	{
		ResetW5200Counter++;
	}

	//Status = getSn_MR(MR);
	//Status = EthernetRegisterRead(MR);
	//if((Status & 0x80) == 0x00)
	if(1)
	{
		if(TRUE == EthernetLinkCheck())
			return TRUE;
	}
	else
	{
		NetworkFailAlertFlag |= NetworkFail_SIP;
		return FALSE;
	}
	NetworkFailAlertFlag |= NetworkFail_Link;

	return FALSE;
#endif
}

void EthernetSetNetwork(void)
{
	OPEN_Ethernet();

	return;
}




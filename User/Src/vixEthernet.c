
#include "vixConfig.h"

/*

static OS_STACKPTR							int OS_STACK_SIZE_HTTPC_TASK[4096];			// Task Stack Size
static OS_TASK								OS_CTRL_BLOCK_HTTPC_TASK;					// Task Control Block
#define OS_PRIORITY_HTTPC_TASK				100
*/


// wiz_NetInfo ETH_IPv4NetInfo;

#define DATA_BUF_SIZE			4096

uint8_t socknumlist[] = { _HTTPC_SOCK };
//uint8_t socknumlist[] = {0};
unsigned int URX_BUF_cnt = 0;
unsigned int URX_BUF_Flag = 0;

uint8_t Domain_IP[16] = { 0, };

// dhcp
uint8_t my_dhcp_retry = 0;
uint8_t g_msec_cnt = 0;

ETH_IPv4NetInfoTypeDef ETH_IPv4NetInfo;
HTTP_ClientCommTypeDef HTTP_ClientComm;


extern INT8U udp_send_buffer[UDP_SEND_BUFFER_SIZE];
extern INT8U udp_recv_buffer[UDP_RECV_BUFFER_SIZE];

wiz_NetInfo gWIZNETINFO;


/*
@Name: nRST_w6100
@Brief: 遺����� W6100 Ethernet Chip Reset
@Param: 
@Return: 泥�由� 寃곌낵
@Note: Reset �댄�� Chip �ㅼ��
*/
void nRST_w6100(void)
{
	_W6100_NRST_HIGH();
	HAL_Delay(100);
	_W6100_NRST_LOW();
	HAL_Delay(500);
	//(Flag == _RTOS) ? OS_Delay(5) : MyMsDelay(5);			// Trst Max 1.0us
	_W6100_NRST_HIGH();
	HAL_Delay(100);
	//(Flag == _RTOS) ? OS_Delay(70) : MyMsDelay(70);			// Tsta Max 60.3ms
	return;
}


void wep_define_func(void)
{
	// Index page and netinfo / base64 image demo
	reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)index_page);				// index.html 		: Main page example
	reg_httpServer_webContent((uint8_t *)"netinfo.html", (uint8_t *)netinfo_page);			// netinfo.html 	: Network information example page
	reg_httpServer_webContent((uint8_t *)"netinfo.js", (uint8_t *)wiz6100web_netinfo_js);	// netinfo.js 		: JavaScript for Read Network configuration 	(+ ajax.js)
//	reg_httpServer_webContent((uint8_t *)"img.html", (uint8_t *)img_page);					// img.html 		: Base64 Image data example page

	// Example #1
	reg_httpServer_webContent((uint8_t *)"dio.html", (uint8_t *)dio_page);					// dio.html 		: Digital I/O control example page
	reg_httpServer_webContent((uint8_t *)"dio.js", (uint8_t *)wiz6100web_dio_js);			// dio.js 			: JavaScript for digital I/O control 	(+ ajax.js)

	// AJAX JavaScript functions
	reg_httpServer_webContent((uint8_t *)"ajax.js", (uint8_t *)wiz6100web_ajax_js);			// ajax.js			: JavaScript for AJAX request transfer

	return;
}


/*
@Name: OPEN_Ethernet
@Brief: W6100�� IP, Subnet, Gateway, MAC �ㅼ��
@Param: None
@Return: None
@Note: ���� IPV4留� 吏���
*/
void OPEN_Ethernet(void)
{
	uint8_t syslock = SYS_NET_LOCK;

	SET_EthernetToDefaults(_IP_V4);

	nRST_w6100();

	W6100Initialze();

	if(!ctlwizchip(CW_SYS_UNLOCK,& syslock)) {
		NETUNLOCK();
	}

	ctlnetwork(CN_SET_NETINFO,&gWIZNETINFO);

	ctlnetwork(CN_GET_NETINFO, &gWIZNETINFO);

	printf("\r\n");
	printf("<OPEN_Ethernet> Network Configuration\r\n");
	printf("MAC:%02x.%02x.%02x.%02x.%02x.%02x\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("Gateway:%d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], ETH_IPv4NetInfo.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("Subnetmask:%d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("MyIP:%d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("Http IP:%d.%d.%d.%d\r\n", ETH_IPv4NetInfo.HTTPC_DomainIP[0], ETH_IPv4NetInfo.HTTPC_DomainIP[1],
										ETH_IPv4NetInfo.HTTPC_DomainIP[2], ETH_IPv4NetInfo.HTTPC_DomainIP[3]);
	printf("\r\n");
	fOpenEthernet = 1;
	//DHCPv4_init(0, databuf);
	//reg_dhcpv4_cbfunc(dhcpv4_assign, dhcpv4_update, dhcpv4_conflict);
	httpc_init(_HTTPC_SOCK, ETH_IPv4NetInfo.HTTPC_DomainIP, ETH_IPv4NetInfo.HTTPC_DomainPort, HTTP_ClientComm.TxBuf, HTTP_ClientComm.RxBuf, HTTP_ClientComm.HdBuf, HTTP_ClientComm.BdBuf);
	HAL_TIM_Base_Start_IT(&htim4);

	return;
}

uint8_t DHCP_Thread(void)
{
	if(DHCPV4_FAILED == DHCPv4_run())
	{
		my_dhcp_retry++;
		if(my_dhcp_retry > 5)
		{
			my_dhcp_retry = 0;
			DHCPv4_stop();
			//network_init();	// chip reset ?
		}
	}
	else
	{
		my_dhcp_retry = 0;
	}

	return 0;
}


void dhcpv4_assign(void)
{
	getIPfromDHCPv4(ETH_IPv4NetInfo.ip);
	getGWfromDHCPv4(ETH_IPv4NetInfo.gw);
	getSNfromDHCPv4(ETH_IPv4NetInfo.sn);
//	getDNSfromDHCPv4(ETH_IPv4NetInfo.dns);
//	ETH_IPv4NetInfo.ipmode = NETINFO_DHCP_V4;
}

// static
void repeating_timer_callback(void)
{
	g_msec_cnt++;
	if(g_msec_cnt >= (1000-1))
	{
		g_msec_cnt = 0;
		DHCPv4_time_handler();
	}

	return;
}

/*
@Name: W6100Initialze
@Brief: W6100 Socket(0, 1, 2) 諛� PHY �ㅼ��
@Param: None
@Return: None
@Note: W6100 Socket buffer size瑜� 4K濡� �ㅼ��
*/
void W6100Initialze(void)
{
	uint8_t u8Cnt, ret;
	intr_kind temp;
	unsigned char W6100_AdrSet[2][8] = {{2, 2, 2, 2, 2, 2, 2, 2}, \
										{2, 2, 2, 2, 2, 2, 2, 2}};	// Up to 16K-bytes buffer Tx/Rx each


	for(u8Cnt=0; u8Cnt < 5; u8Cnt++)
	{
		ret = ctlwizchip(CW_GET_PHYLINK,(void*)&temp);
		if(ret != -1 && temp != PHY_LINK_OFF)
		{
			printf("PHY OK.\r\n");
			break;
		}
	}

	temp = IK_DEST_UNREACH;

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W6100_AdrSet) == -1)
	{
		//Dprintf("W6100 initialized fail.\r\n");
		printf("W6100 initialized fail.\r\n");
	}
	printf("W6100 initialized.\r\n");

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
	{
		//Dprintf("W6100 interrupt\r\n");
		printf("W6100 interrupt\r\n");
	}
	//Dprintf("interrupt mask: %02x\r\n",getIMR());
	printf("interrupt mask: %02x\r\n",getIMR());

	TANeedUdpCheck = FALSE;
	 // jsmn_init(&jParser);
	UHSAliveCheckTimer = HttpcParameter.ScheduleCode[0];

	gU16EEpromFailCount = 0;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...
	gU16sFlashFailCount = 0;//EEPROM sFalsh 쓰기읽기 실패 카운트... 시간정보보낼때 주간값에 추가해서 보냄...

#ifdef TACS_V244 //FW_TACS_STM_V1.4.04_SC_210302.bin       성능인증용 CS10S 통신기능 부여..
	  gU8ConsoleCardIDindexTCP = 0;
	  gU8ConsoleCardIDindex485 = 0;
#endif

	  gU8ConsoleAlive = 0;
	  for(int i= 0; i < MAX_NUM_485CS; i++)
	  {
		  if(ConsoleParameter.Type[i] != FALSE)//운영되는 콘솔에 대해서만..
			  gU8ConsoleAlive |= 0x80 >> i;//처음엔 연결상태로.. 안그러면 연결되어 있어도 연결값으로 안바뀜..
	  }

}

/*
@Name: UDP_Handler
@Brief:
@Param: None
@Return:
@Note:
*/
int32_t UDP_Handler(void)
{
	uint8_t status;
	static uint8_t destip[16] = {0,};
	static uint16_t destport;
	uint8_t addr_len;
	datasize_t ret;
	datasize_t received_size;
	uint16_t sentsize;

	static uint8_t sn = _UDP_SOCK;
	static uint16_t port = _UDP_PORT;


	getsockopt(sn, SO_STATUS,&status);
	switch(status)
	{
	case SOCK_UDP:
		getsockopt(sn, SO_RECVBUF, &received_size);
		if(received_size > DATA_BUF_SIZE)
		   received_size = DATA_BUF_SIZE;

		if(received_size>0)
		{
			ret = recvfrom(sn, &udp_recv_buffer[0], received_size, (uint8_t*)&destip, (uint16_t*)&destport, &addr_len);
			if(ret <= 0)
				return ret;

			received_size = (uint16_t) ret;
			sentsize = 0;
			while(sentsize != received_size)
			{
					ret = sendto(sn, udp_recv_buffer+sentsize, received_size-sentsize, destip, destport, addr_len);

					if(ret < 0)	return ret;

					sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
			}
			received_size = 0;
		}
		break;

	case SOCK_CLOSED:
		socket(sn,Sn_MR_UDP4, port, SOCK_IO_NONBLOCK);
		break;
	}

	return 0;
}

/*
@Name: TCPS_Handler
@Brief:
@Param:
@Return:
@Note:
*/
int32_t TCPS_Handler(uint8_t SockNum)
{
	int32_t ret;
    datasize_t sentsize=0;
    int8_t status,inter;
    uint8_t tmp = 0;
    datasize_t received_size;
    uint8_t arg_tmp8;

    static uint16_t port = 14443;

	getsockopt(SockNum, SO_STATUS, &status);

	switch(status)
    {
    	case SOCK_ESTABLISHED :
    		ctlsocket(SockNum,CS_GET_INTERRUPT,&inter);
    		if(inter & Sn_IR_CON)
    		{
    			arg_tmp8 = Sn_IR_CON;
    			ctlsocket(SockNum,CS_CLR_INTERRUPT,&arg_tmp8);
    		}
    		TASocketConnected = TRUE;

    		getsockopt(SockNum,SO_RECVBUF,&received_size);

    		if(received_size > 0)
    		{
				TreatTCPServerData(SockNum, received_size);
     		}
    		break;
        case SOCK_CLOSE_WAIT :
			getsockopt(SockNum, SO_RECVBUF, &received_size);

			if(received_size > 0) // Don't need to check SOCKERR_BUSY because it doesn't not occur.
            {
				TreatTCPServerData(SockNum, received_size);
            }
            if((ret = disconnect(SockNum)) != SOCK_OK)
            	return ret;
            break;
        case SOCK_INIT :
        	if((ret = listen(SockNum)) != SOCK_OK)
        		return ret;
        	break;
		case SOCK_CLOSED:
    		TASocketConnected = FALSE;
			tmp = socket(SockNum, Sn_MR_TCP4, port, SOCK_IO_NONBLOCK);
            break;
            if(tmp != SockNum)    /* reinitialize the socket */
            {
            	return SOCKERR_SOCKNUM;
            }
            break;
        default:
        	break;
    }

	return 1;
}

void TreatTCPServerData(uint8_t SockNum,  datasize_t received_size)
{
		int32_t ret;
		datasize_t sentsize=0;
		fOpenEthernet = 0;

		if( received_size > Internet_TABuf_Size)
			received_size = Internet_TABuf_Size;

		ret = recv(SockNum, TABuf_Rx, received_size);

        if(ret <= 0)
			return;      // check SOCKERR_BUSY & SOCKERR_XXX. For showing the occurrence of SOCKERR_BUSY.

	    received_size = (uint16_t)ret;
        if(TAPacketCheck() == TRUE)  // PC Commmunication
        {
        	int i;
            rprintf("SVR: Packet Received From PC\r\n");
            rprintf( "[PC]Set -> ");
            for(i = 0; i<received_size;i++)
            {
                rprintf( "%02X ", TABuf_Rx[i]);
            }
            rprintf( "\r\n");
            TAPacketAnalysis();
        }
        else    // Server communication
        {
             nTa4EventSendType = 100;
             bSetEventTa4Send = TRUE;
             rprintf("TcpSVR: Packet Received From Server nId=%d\r\n", nTa4EventSendType);
             TAMakeServerPacking();
        }

        sentsize = 0;
        if(GVTAComData.SetPacket == TRUE)
        {
           while(GVTAComData.length != sentsize)
           {
           	ret = send(SockNum, TABuf_Tx+sentsize, GVTAComData.length-sentsize);
           	if(ret < 0)
           	{
           		close(SockNum);
 //          		return ret;
           	}
           	else
           	{
           		sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
           	}
           }
           GVTAComData.SetPacket = FALSE;
           GVTAComData.length = 0;

           rprintf("TcpSVR: Packet Send To PC\r\n");
        }
}

void SET_EthernetToDefaults(uint8_t Type)
{
	if(Type == _IP_V4)
	{
		ETH_IPv4NetInfo.mac[0] = CMSParameter.MacAddress[0];
		ETH_IPv4NetInfo.mac[1] = CMSParameter.MacAddress[1];
		ETH_IPv4NetInfo.mac[2] = CMSParameter.MacAddress[2];
		ETH_IPv4NetInfo.mac[3] = CMSParameter.MacAddress[3];
		ETH_IPv4NetInfo.mac[4] = CMSParameter.MacAddress[4];
		ETH_IPv4NetInfo.mac[5] = CMSParameter.MacAddress[5];

		ETH_IPv4NetInfo.gw[0] = CMSParameter.GatewayAddress[0];
		ETH_IPv4NetInfo.gw[1] = CMSParameter.GatewayAddress[1];
		ETH_IPv4NetInfo.gw[2] = CMSParameter.GatewayAddress[2];
		ETH_IPv4NetInfo.gw[3] = CMSParameter.GatewayAddress[3];

		ETH_IPv4NetInfo.sn[0] = CMSParameter.SubnetMask[0];
		ETH_IPv4NetInfo.sn[1] = CMSParameter.SubnetMask[1];
		ETH_IPv4NetInfo.sn[2] = CMSParameter.SubnetMask[2];
		ETH_IPv4NetInfo.sn[3] = CMSParameter.SubnetMask[3];

		ETH_IPv4NetInfo.ip[0] = CMSParameter.IPAddress[0];
		ETH_IPv4NetInfo.ip[1] = CMSParameter.IPAddress[1];
		ETH_IPv4NetInfo.ip[2] = CMSParameter.IPAddress[2];
		ETH_IPv4NetInfo.ip[3] = CMSParameter.IPAddress[3];

		ETH_IPv4NetInfo.HTTPC_MyPort = _HTTPC_MY_DEF_PORT;
		ETH_IPv4NetInfo.HTTPC_DomainPort = (HttpcParameter.TAPort[0] * 256) + HttpcParameter.TAPort[1];

		ETH_IPv4NetInfo.HTTPC_DomainIP[0] = HttpcParameter.TAIP1[0];
		ETH_IPv4NetInfo.HTTPC_DomainIP[1] = HttpcParameter.TAIP1[1];
		ETH_IPv4NetInfo.HTTPC_DomainIP[2] = HttpcParameter.TAIP1[2];
		ETH_IPv4NetInfo.HTTPC_DomainIP[3] = HttpcParameter.TAIP1[3];

		memcpy(&gWIZNETINFO.mac, &ETH_IPv4NetInfo.mac, _MAC_BYTE_LEN);
		memcpy(&gWIZNETINFO.gw, &ETH_IPv4NetInfo.gw, _IPV4_GATEWAY_BYTE_LEN);
		memcpy(&gWIZNETINFO.sn, &ETH_IPv4NetInfo.sn, _IPV4_SUBNET_BYTE_LEN);
		memcpy(&gWIZNETINFO.ip, &ETH_IPv4NetInfo.ip, _IPV4_IP_BYTE_LEN);

		gWIZNETINFO.dns[0] = CMSParameter.DNSAddress[0];
		gWIZNETINFO.dns[1] = CMSParameter.DNSAddress[1];
		gWIZNETINFO.dns[2] = CMSParameter.DNSAddress[2];
		gWIZNETINFO.dns[3] = CMSParameter.DNSAddress[3];

		gWIZNETINFO.lla[0] = 0xfe;
		gWIZNETINFO.lla[1] = 0x80;
		gWIZNETINFO.lla[2] = 0x00;
		gWIZNETINFO.lla[3] = 0x00;
		gWIZNETINFO.lla[4] = 0x00;
		gWIZNETINFO.lla[5] = 0x00;
		gWIZNETINFO.lla[6] = 0x00;
		gWIZNETINFO.lla[7] = 0x00;
		gWIZNETINFO.lla[8] = 0x02;
		gWIZNETINFO.lla[9] = 0x00;
		gWIZNETINFO.lla[10] = 0xdc;
		gWIZNETINFO.lla[11] = 0xff;
		gWIZNETINFO.lla[12] = 0xfe;
		gWIZNETINFO.lla[13] = 0x57;
		gWIZNETINFO.lla[14] = 0x57;
		gWIZNETINFO.lla[15] = 0x62;

		gWIZNETINFO.ipmode = NETINFO_DHCP_V4;

	}

	return;
}


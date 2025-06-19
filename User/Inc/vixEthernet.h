/*
 * vixEthernet.h
 *
 *  Created on: Sep 25, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXETHERNET_H_
#define INC_VIXETHERNET_H_

#include "stm32l4xx_hal.h"
#include "wizchip_conf.h"

#define _IP_V4								0
#define _IP_V6								1

#define MAX_SOCK_NUM		 				8

#define _HTTPC_SOCK							0
#define _HTTPS_SOCK							1
#define _UDP_SOCK							2
#define Socket_FIRE							3		// UDP Server
#define Socket_MANG							4		// TCP
#define Socket_UPNP_EVENTING				5		// TCP
#define Socket_TA							6		// TCP Client
#define Socket_NetCheck						7

#define _UDP_PORT							15693

#define MAX_HTTPSOCK						1


#define Internet_TABuf_Size					1024  	//512 + data������ 9
#define Internet_UHSBuf_Size				1024   	//
#define Internet_TA1Buf_Size				1024   	// 0xF4

#define SocketIdle			    				0xFFFF
#define SocketListen							0xFFFE

// Addfess Family v4
#define AF_INET								2
// Addfess Family v6
#define AF_INET6								23
// Addfess Family DUAL
#define AF_INET_DUAL						11


// u-lifecare Data Type
#define UHS_Type_ConsoleInfo101		        	1	// 101
#define UHS_Type_AccessInfo102		        	2	// 102
#define UHS_Type_RequestCpInfo201			3   	// 201
#define UHS_Type_ReportFireAlarm202	    		4   	// 202
#define UHS_Type_RequestEnvInfo203			5   	// 203
#define UHS_Type_RequestDayInfo204			6   	// 204
#define UHS_Type_ResetCp301			    	7   	// 301
#define UHS_Type_SetSecurity302		        	8   	// 302
#define UHS_Type_ChangePassword303		9   	// 303
#define UHS_Type_StartService401		    		10  	// 401
#define UHS_Type_TerminateService402		11  	// 402
#define UHS_Type_SetConsolePeriod403	    	12  	// 403
#define UHS_Type_ChangeNodeSerial404		13  	// 404
#define UHS_Type_ResetNode405			    	14  	// 405
#define UHS_Type_ChangeConsole406    	    	15  	// 406
#define UHS_Type_ChangeBattery407   	    	16  	// 407
#define UHS_Type_SetFireAlarm408           		17  	// 408


#define _MAC_BYTE_LEN						6
#define _IPV4_IP_BYTE_LEN					4
#define _IPV4_GATEWAY_BYTE_LEN				4
#define _IPV4_SUBNET_BYTE_LEN				4
#define _IPV4_DNS_BYTE_LEN					4

#define _HTTPC_MY_DEF_PORT					8080

#define _PUBLIC_KEY							1
#define _PRIVATE_KEY						2
#define _SHARED_KEY							3
#define _FACTOR_KEY							4

/*
typedef struct _SYSTEM_SettingsTypeDef {
	uint8_t Token[ ];

} __attribute__((packed)) ETH_IPv4NetInfoTypeDef;
*/

typedef struct _ETH_IPv4NetInfoTypeDef {
	uint8_t mac[_MAC_BYTE_LEN];
	uint8_t ip[_IPV4_IP_BYTE_LEN];
	uint8_t gw[_IPV4_GATEWAY_BYTE_LEN];
	uint8_t sn[_IPV4_SUBNET_BYTE_LEN];
	uint16_t HTTPC_MyPort;
	uint16_t HTTPC_DomainPort;
	uint8_t HTTPC_DomainIP[16];
} __attribute__((packed)) ETH_IPv4NetInfoTypeDef;

typedef struct _HTTP_ClientCommTypeDef {
	uint8_t HeaderBuf[512];
	uint8_t BodyBuf[1024];
	uint8_t TxBuf[4096];
	uint8_t RxBuf[4096];
} __attribute__((packed)) HTTP_ClientCommTypeDef;

void nRST_w6100(void);
void OPEN_Ethernet(void);
//void wep_define_func(void);
//static void SET_NetInfoToW6100(void);
void Ethernet_Handler(void);
int8_t process_dhcp(void);
uint8_t DHCP_Thread(void);
void dhcpv4_assign(void);
void dhcpv4_update(void);
void dhcpv4_conflict(void);
void repeating_timer_callback(void);
void W6100Initialze(void);

void SET_EthernetToDefaults(uint8_t Type);
void TreatTCPServerData(uint8_t SockNum,  datasize_t received_size);
void CREATE_EthernetTask(void);
int32_t UDP_Handler(void);
void HTTPClient_Thread(void);

//static void HTTPC_Task(void* pContext);
void HTTPC_Task(void);

/*static void Task_httpClient(void);
static void Task_httpServer(void);
static void Task_UDP(void);*/

#endif /* INC_VIXETHERNET_H_ */

/*#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>

#include "w6100.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "httpClient.h"*/

/* Private define ------------------------------------------------------------*/
/*#include "stm32l4xx.h"
#include "RTOS.h"*/
#include "vixConfig.h"
#include "httpUtil.h"

// #define _HTTPCLIENT_DEBUG_
extern uint16_t t50msTim6_Net;
/* Private variables ---------------------------------------------------------*/
extern unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
extern char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

extern char *RTC_GetGmtDate(void);
//OS_TIMER						OS_SW_TIMER_HTTPC_POLLING;

extern ETH_IPv4NetInfoTypeDef ETH_IPv4NetInfo;
extern HTTP_ClientCommTypeDef HTTP_ClientComm;
//HTTPC_FlowCtrlTypeDef HTTPC_FlowCtrl;
_HTTPC_GetPostTypeDef HTTP_RestCmd[MAX_NUM_DOOR];

uint8_t * httpc_Header_buf;
uint8_t * httpc_Body_buf;
uint8_t * httpc_send_buf;
uint8_t * httpc_recv_buf;

static int8_t httpsock = 0;
static uint8_t dest_ip[16] = {0, };
static uint16_t dest_port = 0;
static uint16_t httpc_any_port = 0;

uint8_t httpc_isSockOpen = HTTPC_FALSE;
uint8_t httpc_isConnected = HTTPC_FALSE;
uint16_t httpc_isReceived = HTTPC_FALSE;

uint8_t flag_sent_http_request = ENABLE;
uint8_t flag_recv_http_request = ENABLE;

stRtLog stRealtimeLog;
/* Private functions prototypes ----------------------------------------------*/
uint16_t get_httpc_any_port(void);
extern wiz_NetInfo gWIZNETINFO;
/* Public & Private functions ------------------------------------------------*/

uint8_t httpc_init(uint8_t sock, uint8_t * ip, uint16_t port, uint8_t * sbuf, uint8_t * rbuf)
{
	uint8_t ret = HTTPC_FALSE;
	
	stRealtimeLog.index = 110;
	if(sock <= _WIZCHIP_SOCK_NUM_)
	{
		// Hardware socket number for HTTP client (0 ~ 7)
		httpsock = sock;
		
		// Shared buffers: HTTP Send / Recevice
		httpc_send_buf = sbuf;
		httpc_recv_buf = rbuf;
		
		// Destination IP address and Port number
		// (Destination = HTTP server)
		dest_ip[0] = ip[0];
		dest_ip[1] = ip[1];
		dest_ip[2] = ip[2];
		dest_ip[3] = ip[3];
		dest_ip[4] = ip[4];
		dest_ip[5] = ip[5];
		dest_ip[6] = ip[6];
		dest_ip[7] = ip[7];
		dest_ip[8] = ip[8];
		dest_ip[9] = ip[9];
		dest_ip[10] = ip[10];
		dest_ip[11] = ip[11];
		dest_ip[12] = ip[12];
		dest_ip[13] = ip[13];
		dest_ip[14] = ip[14];
		dest_ip[15] = ip[15];

		dest_port = port;
		
		ret = HTTPC_TRUE;
	}
	
	return ret;
}

// return: true / false
uint8_t httpc_connection_handler(uint8_t addr_len)
{
	uint8_t ret = HTTPC_FALSE;
	
	uint16_t source_port;

#ifdef _HTTPCLIENT_DEBUG_
	uint8_t destip[4] = {0, };
	uint16_t destport = 0;
#endif
	
	uint8_t state = getSn_SR(httpsock);
	switch(state)
	{
		case SOCK_INIT:
			// state: connect
			ret = HTTPC_TRUE;
			break;
		
		case SOCK_ESTABLISHED:
			if(getSn_IR(httpsock) & Sn_IR_CON)
			{
#ifdef _HTTPCLIENT_DEBUG_
				// Serial debug message printout
				getsockopt(httpsock, SO_DESTIP, &destip);
				getsockopt(httpsock, SO_DESTPORT, &destport);
				printf(" > HTTP CLIENT: CONNECTED TO - %d.%d.%d.%d : %d\r\n",destip[0], destip[1], destip[2], destip[3], destport);
#endif
				httpc_isConnected = HTTPC_TRUE;
				setSn_IR(httpsock, Sn_IR_CON);
			}
			
			httpc_isReceived = getSn_RX_RSR(httpsock);
			ret = HTTPC_CONNECTED;
			break;
		
		case SOCK_CLOSE_WAIT:
			disconnect(httpsock);
			httpc_isConnected = HTTPC_FALSE;
			break;
		
		case SOCK_FIN_WAIT:
		case SOCK_CLOSED:
			httpc_isSockOpen = HTTPC_FALSE;
			httpc_isConnected = HTTPC_FALSE;
			
			source_port = get_httpc_any_port();
#ifdef _HTTPCLIENT_DEBUG_
			printf(" > HTTP CLIENT: source_port = %d\r\n", source_port);
#endif
			if(addr_len == 4)
			{
				if(socket(httpsock, Sn_MR_TCP, source_port, Sn_MR_ND) == httpsock)
				{
					if(httpc_isSockOpen == HTTPC_FALSE)
					{
	#ifdef _HTTPCLIENT_DEBUG_
						printf(" > HTTP CLIENT: SOCKOPEN\r\n");
	#endif
						httpc_isSockOpen = HTTPC_TRUE;
					}
				}
			}
			else{
				if(socket(httpsock, Sn_MR_TCP6, source_port, Sn_MR_ND) == httpsock)
				{
					if(httpc_isSockOpen == HTTPC_FALSE)
					{
	#ifdef _HTTPCLIENT_DEBUG_
						printf(" > HTTP CLIENT: SOCKOPEN\r\n");
	#endif
						httpc_isSockOpen = HTTPC_TRUE;
					}
				}
			}
			break;
			
		default:
			break;
	}
	
	return ret;
}

// return: socket status
uint8_t httpc_connect(uint8_t addrlen)
{
	uint8_t ret = HTTPC_FALSE;
	
	if(httpsock >= 0)
	{
		if(httpc_isSockOpen == HTTPC_TRUE)
		{
			// TCP connect
			ret = connect(httpsock, dest_ip, dest_port, addrlen);
			if(ret == SOCK_OK) ret = HTTPC_TRUE;
		}
	}
	
	return ret;
}

uint16_t httpc_add_customHeader_field(uint8_t * customHeader_buf, const char * name, const char * value)
{
	uint16_t len = 0;
	
	if((strlen((char *)name) + strlen((char *)value)) > DATA_BUF_SIZE) return 0;
	
	if(strlen((char *)customHeader_buf) == 0)
	{
		len = sprintf((char *)customHeader_buf, "%s: %s\r\n", name, value);
	}
	else
	{
		len = sprintf((char *)customHeader_buf, "%s%s: %s\r\n", customHeader_buf, name, value);
	}
	
	return len;
}

// return: sent header length
uint16_t httpc_send_header(uint8_t * buf, uint16_t len, uint8_t * customHeader_buf, uint16_t content_len)
{
	if(httpc_isConnected == HTTPC_TRUE)
	{
		// HTTP content type: POST / PUT
		if(content_len > 0)
		{
			len += sprintf((char *)buf+len, "Content-Length: %d\r\n", content_len);
	/*
			if(strcmp((char *)req->content_type, HTTP_CTYPE_MULTIPART_FORM) == 0)
			{
				// HTTP content type: multipart/form-data
				len += sprintf((char *)buf+len, "Content-Type: %s; boundary=%s\r\n", req->content_type, formDataBoundary);
			}
			else
			{
				// HTTP content type: others
				len += sprintf((char *)buf+len, "Content-Type: %s\r\n", req->content_type); // HTTP content type: others
			}
	*/
		}
		
		// for adding custom header fields
		if(customHeader_buf != NULL)
		{
			if((strlen((char *)customHeader_buf) + len + 2) <= DATA_BUF_SIZE)
			{
				len += sprintf((char *)buf+len, "%s", customHeader_buf);
			}
		}
		
		len += sprintf((char *)buf+len, "\r\n");

#ifdef _HTTPCLIENT_DEBUG_
		printf(" >> HTTP Request header - Method: %s, URI: %s, Content-Length: %d\r\n", req->method, req->uri, content_len);
		printf("%s", buf);
#endif
		send(httpsock, buf, len);
	}
	else
	{
		len = HTTPC_FAILED;
	}
	
	return len;
}


// return: sent body length
uint16_t httpc_send_body(uint8_t * buf, uint16_t len)
{
	uint16_t sentlen = 0;

#ifdef _HTTPCLIENT_DEBUG_
	uint16_t i;
#endif
	
	if(( httpc_isConnected == HTTPC_TRUE) && (len > 0)){
		do{
			sentlen += send(httpsock, buf, len);
		} while(sentlen < len);

	}
//	else
//	{
//		sentlen = 0;
//	}
	
	return sentlen;
}

// return: received data length
uint16_t httpc_recv(uint8_t * buf, uint16_t len)
{
	uint16_t recvlen;
	
	if(httpc_isConnected == HTTPC_TRUE)
	{
		if(len > DATA_BUF_SIZE) len = DATA_BUF_SIZE;
		recvlen = recv(httpsock, buf, len);
	}
	else
	{
		recvlen = HTTPC_FAILED;
	}
	
	return recvlen;
}

// return: true / false
uint8_t httpc_disconnect(void)
{
	uint8_t ret = HTTPC_FALSE;
	
	if(httpc_isConnected == HTTPC_TRUE)
	{
#ifdef _HTTPCLIENT_DEBUG_
		//printf(" > HTTP CLIENT: Try to disconnect\r\n");
#endif
		ret = disconnect(httpsock);
		if(ret == SOCK_OK)
		{
			ret = HTTPC_TRUE;
#ifdef _HTTPCLIENT_DEBUG_
			//printf(" > HTTP CLIENT: Disconnected\r\n");
#endif
		}
	}
	
	return ret;
}

// return: source port number for tcp client 
uint16_t get_httpc_any_port(void)
{
	if(httpc_any_port)
	{
		if((httpc_any_port >= HTTP_CLIENT_PORT_MIN) && (httpc_any_port < HTTP_CLIENT_PORT_MAX)) {
			httpc_any_port++;
		} else {
			httpc_any_port = 0;
		}
	}
	
	if(httpc_any_port < HTTP_CLIENT_PORT_MIN)
	{
		// todo: get random seed value
		httpc_any_port = (rand() % 10000) + 46000; // 46000 ~ 55999
	}
	
	return httpc_any_port;
}

static uint16_t remainingTime = 0;
void HTTPClient_Handler(void)
{

	uint16_t headerSize = 0;
	uint16_t bodySize = 0;
	
	httpc_connection_handler(4);

	if(httpc_isSockOpen)
	{
		httpc_connect(4);
	}

	// HTTP client example
	if(httpc_isConnected)
	{
		if( (flag_recv_http_request == ENABLE) && (flag_sent_http_request == ENABLE) )
		{
			flag_recv_http_request = DISABLE;

			xPROT_HTTPC_PostDevice(httpc_recv_buf, &headerSize, httpc_send_buf, &bodySize);
			{
				httpc_send_header( httpc_recv_buf, headerSize, NULL, bodySize);
				httpc_send_body(httpc_send_buf, bodySize); // Send HTTP requset message body
			}

			flag_sent_http_request = DISABLE;
			remainingTime = 0;
		}
		else
		{
			remainingTime++;
			if( remainingTime > 30000) /// 30 초 경과 ?
			{
				remainingTime = 0;
				flag_recv_http_request = ENABLE;
			}
//			printf("idle httpc.\r\n");
		}

		// Recv: HTTP response
		if(httpc_isReceived > 0)
		{
			headerSize = httpc_recv(httpc_recv_buf, httpc_isReceived);
			xPROT_HTTPC_CheckRecv(httpc_recv_buf, httpc_isReceived);

			HttpcSubModeStep++;
			if( HttpcSubModeStep >= MAX_NUM_DOOR)
				HttpcSubModeStep = 0;
			flag_recv_http_request = ENABLE;
		}
		else
		{
//			printf("No data received.\r\n");
		}
	}

	return;
}

void xPROT_HTTPC_MakeHeader(uint8_t *restCmd, uint8_t *headerName, uint8_t* pData, uint16_t* Length)
{
	uint16_t len = 0;

	len  = sprintf((char*)pData, "%s %s%s HTTP/1.1\r\n", (uint8_t*)restCmd, (uint8_t*)_ACLOCK, headerName);
	len += sprintf((char*)pData+len, "Host: %d.%d.%d.%d:%d\r\n", ETH_IPv4NetInfo.HTTPC_DomainIP[0], \
		ETH_IPv4NetInfo.HTTPC_DomainIP[1], ETH_IPv4NetInfo.HTTPC_DomainIP[2], ETH_IPv4NetInfo.HTTPC_DomainIP[3], ETH_IPv4NetInfo.HTTPC_DomainPort);
	len += sprintf((char*)pData+len, "User-Agent: aClock Proxy/1.09\r\n");
	len += sprintf((char*)pData+len, "Connection: %s\r\n", (uint8_t*)_CONNECTION_KEEPALIVE);
	len += sprintf((char*)pData+len, "Content-Type: application/push;charset=UTF-8\r\n");
	len += sprintf((char*)pData+len, "Accept: application/push\r\n");
	len += sprintf((char*)pData+len, "Accept-Charset: %s\r\n",  (uint8_t*)_UNICODE_UTF8);
	len += sprintf((char*)pData+len, "Accept-Language: %s\r\n", (uint8_t*)_LANGUAGE_KR);
	*Length = len;
}

// return: received data length
ErrState xPROT_HTTPC_CheckRecv(uint8_t * recvBuf, uint16_t recvLen)
{
	uint8_t* ptr;
	uint8_t * ptrId;
 	uint8_t * ptrCmd;
	uint8_t * ptrDevice;
	uint16_t nPtrSize;

	uint8_t nMainType = GET_HTTPC_HeartType();
	switch( nMainType)
	{
	case UHS_Type_RtLog:
		SET_HTTPC_HeartType(UHS_Type_GetRequest);
		break;
	case UHS_Type_Ping:
		if ( UHSSocketConnected == 1)
		{
			SET_HTTPC_HeartType(UHS_Type_GetRequest);
		}
		else
		{
			SET_HTTPC_HeartType(UHS_Type_Push);
		}
    	break;
    case UHS_Type_Push:
   		SET_HTTPC_HeartType(UHS_Type_ExchangePublic);
    	break;
    case UHS_Type_ReqDevConnection:
   		SET_HTTPC_HeartType(UHS_Type_ExchangePublic);
		break;
    case UHS_Type_Registry:
    	if( strstr( (char const*)recvBuf, "RegistryCode") != NULL)
    	{
    		UHSSocketConnected = 1;
       		SET_HTTPC_HeartType(UHS_Type_GetRequest);
    	}
    	else
    	{
       		SET_HTTPC_HeartType(UHS_Type_Push);
    	}
    	break;
    case UHS_Type_ExchangePublic:
   		SET_HTTPC_HeartType(UHS_Type_ExchangeFactors);
    	break;
    case UHS_Type_ExchangeFactors:
   		SET_HTTPC_HeartType(UHS_Type_Registry);
    	break;
    case UHS_Type_GetRequest:
    	if((ptr = strstr( (char const*)recvBuf, "SET OPTIONS")) != NULL)
    	{
    		printf("SET OPTIONS_%d:%s\r\n", HttpcSubModeStep, ptr);
    		Httpc_TreatSetOption( recvBuf, recvLen);
    	}  // C:384:UPGRADE
    	else if((ptr = strstr( (char const*)recvBuf, "DATA DELETE")) != NULL)
    	{
    		printf("DATA DELETE_%d:%s\r\n", HttpcSubModeStep, ptr);
    		Httpc_TreatDataDelete( recvBuf, recvLen);
    	}
    	else if((ptr = strstr( (char const*)recvBuf, "DATA UPDATE")) != NULL)
    	{
    		printf("DATA UPDATE_%d:%s\r\n", HttpcSubModeStep, ptr);
    		Httpc_TreatDataUpdate( recvBuf, recvLen);
    	}
    	else if((ptr = strstr( (char const*)recvBuf, "CONTROL DEVICE")) != NULL)
    	{
    		printf("CONTROL DEVICE_%d:%s\r\n", HttpcSubModeStep, ptr);
    		Httpc_TreatControlDevice(  recvBuf,  recvLen);
    	}
    	else if((ptr = strstr( (char const*)recvBuf, "UPGRADE")) != NULL)
    	{
    		printf("UPGRADE_%d:%s\r\n", HttpcSubModeStep, ptr);
    		Httpc_TreatUpgrade( recvBuf, recvLen);
    	}
    	else    //  getRequest 의 응답에 CMD 없는 상태
    	{
//    		printf("4C else\r\n");
    		SET_HTTPC_HeartType(UHS_Type_GetRequest);
    	}
    	break;
    case UHS_Type_SendCmdReturn:
    {
//		printf("4R %s\r\n", recvBuf);
   		SET_HTTPC_HeartType(UHS_Type_GetRequest);
    	break;
    }
    case UHS_Type_GetFirmware:
    {
        if(Firmware_Upgrade != SPI1_UPGRADE_YES)
        {  // 앞에 커맨드로 받지 않으면 안됨.
        }
        else
        { // 앞에 커맨드로 받았으면
  //        rprintf("[NOTICE] Fwd TA_FWUPDATE_HEADER Result!\r\n");
        	ftimeSpiOut = 0;
        	int outlen;
			uint8_t *outBuf;
			outBuf = base64_decode(recvBuf, recvLen, &outlen);

			free(outBuf);
        }
		SET_HTTPC_HeartType(UHS_Type_GetRequest);
		break;
    }
	default:
		SET_HTTPC_HeartType(UHS_Type_GetRequest);
		break;
	}

	memset( recvBuf, 0,  2048);
	return _Ok;
}

ErrState xPROT_HTTPC_PostDevice(uint8_t* pHeader, uint16_t* pHeaderSize, uint8_t* pBody, uint16_t* pBodySize)
{
	uint16_t headerSize=0, bodySize=0;
	uint8_t nMainType = GET_HTTPC_HeartType();

	switch( nMainType){
		case UHS_Type_RtLog:
			xPOST_RtLogTableHeader(pHeader, &headerSize);
			xPOST_RtLogTableBody(pBody, &bodySize);
			break;
		case UHS_Type_ReqDevConnection:
			xGET_CdataConnectionHeader(pHeader, &headerSize);
			xGET_CdataConnectionBody(pBody, &bodySize);
			break;
		case UHS_Type_CdataTable:
			xPOST_CdataTableHeader(pHeader, &headerSize);
			xPOST_CdataTableBody(pBody, &bodySize);
			break;
		case UHS_Type_Ping:
			xGET_PingHeader(pHeader, &headerSize);
			xGET_PingBody(pBody, &bodySize);
			break;
		case UHS_Type_ExchangePublic:
			xPOST_ExchangePublicKeyHeader(pHeader, &headerSize);
			xPOST_ExchangePublicKeyBody(pBody, &bodySize);
			break;
		case UHS_Type_ExchangeFactors:
			xPOST_ExchangeFactorsHeader(pHeader, &headerSize);
			xPOST_ExchangeFactorsBody(pBody, &bodySize);
			break;
		case UHS_Type_Registry:
			xPOST_RegistryHeader(pHeader, &headerSize);
			xPOST_RegistryBody(pBody, &bodySize);
			break;
		case UHS_Type_GetRequest:
			xGET_RequestHeader(pHeader, &headerSize);
			xGET_RequestBody(pBody, &bodySize);
			break;
		case UHS_Type_Push:
			xPOST_PushHeader(pHeader, &headerSize);
			xPOST_PushBody(pBody, &bodySize);
			break;
		case UHS_Type_GetFirmware:
			xGET_FwUpgradeHeader(pHeader, &headerSize);
			xGET_FwUpgradeBody(pBody, &bodySize);
			break;
		case UHS_Type_SendCmdReturn:
			xHTTP_CmdReturnHeader(pHeader, &headerSize);
			xHTTP_CmdReturnBody(pBody, &bodySize);
			break;
		default:
			break;
	}

	*pHeaderSize = headerSize;
	*pBodySize = bodySize;

	return _Ok;
}

void xPROT_CDATA_MakeTable(uint8_t *restCmd, uint8_t *headerName, uint8_t* pData, uint16_t* Length)
{
	uint16_t len = 0;

	len  = sprintf((char*)pData, "%s %s%s HTTP/1.1\r\n", (uint8_t*)restCmd, (uint8_t*)_ACLOCK, headerName);
	len += sprintf((char*)pData+len, "Host: %d.%d.%d.%d:%d\r\n", ETH_IPv4NetInfo.HTTPC_DomainIP[0], \
		ETH_IPv4NetInfo.HTTPC_DomainIP[1], ETH_IPv4NetInfo.HTTPC_DomainIP[2], ETH_IPv4NetInfo.HTTPC_DomainIP[3], ETH_IPv4NetInfo.HTTPC_DomainPort);
	len += sprintf((char*)pData+len, "User-Agent: iClock Proxy/1.09\r\n");
	len += sprintf((char*)pData+len, "Connection: %s\r\n", (uint8_t*)_CONNECTION_KEEPALIVE);
	len += sprintf((char*)pData+len, "Content-Type: application/push;charset=UTF-8\r\n");
	len += sprintf((char*)pData+len, "Accept: application/push\r\n");
	len += sprintf((char*)pData+len, "Accept-Charset: %s\r\n",  (uint8_t*)_UNICODE_UTF8);
	len += sprintf((char*)pData+len, "Accept-Language: %s\r\n", (uint8_t*)_LANGUAGE_KR);
	*Length = len;
}

//////////////////////////// REST GET ///////////////////////
ErrState xGET_RequestHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "getrequest?SN=%s", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_GET, headerType, pHeader, &len);

	len += sprintf((char *)pHeader+len, "Cookie: token=%s\r\n", httpc_getCookie());
	*Length = len;
	return _Ok;
}

ErrState xGET_RequestBody(uint8_t* pData, uint16_t* Length)
{
	*(pData+0) = '\0';
	*Length = 0;
	return _Ok;
}

ErrState xGET_CdataConnectionHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len = 0;
	uint8_t headerType[80];

	sprintf(headerType, "cdata?SN=%s&options=all&pushver=3.1.2&PushOptionsFlag=1", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_GET, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xGET_CdataConnectionBody(uint8_t* pData, uint16_t* Length)
{
	*(pData+0) = '\0';
	*Length = 0;
	return _Ok;
}

ErrState xGET_PingHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "ping?SN=%s", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_GET, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xGET_PingBody(uint8_t* pBody, uint16_t* Length)
{
	*(pBody+0) = '\0';
	*Length = 0;
	return _Ok;
}

ErrState xGET_FwUpgradeHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

//	sprintf(headerType, "file?SN=%s&url=http://192.168.213.17:8088/fireware/F20/admin/emfw.cfg", httpc_getSerial());
	sprintf(headerType, "file?SN=%s&url=%s", httpc_getSerial(), HTTP_RestCmd[HttpcSubModeStep].Httpc_Url);
	xPROT_HTTPC_MakeHeader( HTTP_GET, headerType, pHeader, &len);
	len += sprintf((char *)pHeader+len, "Cookie: token=%s\r\n", httpc_getCookie());
	*Length = len;
	return _Ok;
}

ErrState xGET_FwUpgradeBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len = 0;
	if (Firmware_Upgrade == SPI1_UPGRADE_YES)
	{
		len = sprintf((char*)pBody, "ID=%s&Return=%d&CMD=%s\r\n\r\n",
				HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn, HTTP_RestCmd[HttpcSubModeStep].Httpc_Command);
	}
	else
	{
		len = sprintf((char*)pBody, "\r\n\r\n");
	}
	*Length = len;
	return _Ok;
}

ErrState xHTTP_CmdReturnHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

//	sprintf(headerType, "devicecmd?SN=%s", httpc_getSerial());
	sprintf(headerType, "devicecmd?SN=%s", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);
//	len += sprintf((char *)pHeader+len, "Cookie: token=%s\r\n", httpc_getCookie());
//	len += sprintf((char *)pHeader+len, "\r\n");
	*Length = len;
	return _Ok;
}

ErrState xHTTP_CmdReturnBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len = 0;
	len = sprintf((char*)pBody, "ID=%s&Return=%d&CMD=%s\r\n\r\n",
			HTTP_RestCmd[HttpcSubModeStep].chrHttpcCmd, HTTP_RestCmd[HttpcSubModeStep].nHttpcReturn, HTTP_RestCmd[HttpcSubModeStep].Httpc_Command);
	*Length = len;
	return _Ok;
}

////////////////////////// REST  POST ////////////////////////////////
ErrState xPOST_PushHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "push?SN=%s", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xPOST_PushBody(uint8_t* pBody, uint16_t* Length)
{
	*(pBody+0) = '\0';
	*Length = 0;
	return _Ok;
}

ErrState xPOST_ExchangePublicKeyHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "exchange?SN=%s&type=publickey", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);

	*Length = len;
	return _Ok;
}

ErrState xPOST_ExchangePublicKeyBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len = 0;
	uint8_t inBuff[] = "JABC012345";
	int outLen = 0 ;

	uint8_t * outBuff = base64_encode(inBuff, sizeof(inBuff), &outLen);
	len = sprintf((char*)pBody, "PublicKey=%s,", outBuff);
	free(outBuff);
	*Length = len;
	return _Ok;
}

ErrState xPOST_ExchangeFactorsHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "exchange?SN=%s&type=factors", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);

	*Length = len;
	return _Ok;
}

uint8_t * cDataTable;
uint8_t * cTableName;

ErrState xPOST_RtLogTableHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len = 0;
	uint8_t headerType[80];

	sprintf(headerType, "cdata?SN=%s&table=rtlog", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xPOST_RtLogTableBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len;
	uint8_t tempBuf[50];
	uint16_t u16Temp[10];
	uint8_t tltBuffer[7];

	RTC_GetTimeAndDate( &toDayTime, tltBuffer);

	len = 0;
	VS_GetVersionStr((char*)tempBuf);

	len =  sprintf((char*)pBody+len, "time=20%02d-%02d-%02d %02d:%02d:%02d\t",
			tltBuffer[0], tltBuffer[1], tltBuffer[2], tltBuffer[3], tltBuffer[4], tltBuffer[5]);
	len += sprintf((char*)pBody+len, "pin=%s\t", (uint8_t*)stRealtimeLog.pin);
	len += sprintf((char*)pBody+len, "cardno=%s\t", (uint8_t*) stRealtimeLog.cardno);
	len += sprintf((char*)pBody+len, "sitecode=%d\t", stRealtimeLog.sitecode);
	len += sprintf((char*)pBody+len, "linkid=%d\t", stRealtimeLog.linkid);
	len += sprintf((char*)pBody+len, "eventaddr=%d\t", stRealtimeLog.eventaddr);
	len += sprintf((char*)pBody+len, "event=%d\t", stRealtimeLog.event);
	len += sprintf((char*)pBody+len, "inoutstatus=%d\t", 2);
	len += sprintf((char*)pBody+len, "verifytype=%d\t", stRealtimeLog.verifytype);
	len += sprintf((char*)pBody+len, "index=%d\t", stRealtimeLog.index);
	len += sprintf((char*)pBody+len, "maskflag=%d\t", stRealtimeLog.maskflag);
	len += sprintf((char*)pBody+len, "temperature=%d\t", 255);
	len += sprintf((char*)pBody+len, "convtemperature=%d\t", 255);
	len += sprintf((char*)pBody+len, "attstate=%d\t", stRealtimeLog.attstate);
	len += sprintf((char*)pBody+len, "bitCount=%d", 16);
	len += sprintf((char*)pBody+len, "\r\n");

	*Length = len;
	return _Ok;
}

ErrState xPOST_CdataTableHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len = 0;
	uint8_t headerType[80];

	sprintf(headerType, "cdata?SN=%s&table=tabledata&tablename=%s&count=1", httpc_getSerial(), cTableName);
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xPOST_CdataTableBody(uint8_t* pData, uint16_t* Length)
{
	*(pData+0) = '\0';
	*Length = 0;
	return _Ok;
}

ErrState xPOST_ExchangeFactorsBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len = 0;
	uint8_t inBuff[] = "JABC012345";
	int outLen = 0 ;

	uint8_t * outBuff = base64_encode(inBuff, sizeof(inBuff), &outLen);
	len = sprintf((char*)pBody, "Factors=%s,", outBuff);
	free(outBuff);
	*Length = len;

	return _Ok;
}

ErrState xPOST_RegistryHeader(uint8_t* pHeader, uint16_t* Length)
{
	uint16_t len=0;
	uint8_t headerType[64];

	sprintf(headerType, "registry?SN=%s", httpc_getSerial());
	xPROT_HTTPC_MakeHeader( HTTP_POST, headerType, pHeader, &len);
	*Length = len;
	return _Ok;
}

ErrState xPOST_RegistryBody(uint8_t* pBody, uint16_t* Length)
{
	uint16_t len;
	uint8_t tempBuf[50];
	uint16_t u16Temp[10];

	len = 0;
	VS_GetVersionStr((char*)tempBuf);

	len =  sprintf((char*)pBody+len, "DeviceType=%s,", (uint8_t*)_DEVICE_TYPE_STRING);
	len += sprintf((char*)pBody+len, "~DeviceName=%s,", (uint8_t*)_DEVICE_NAME_STRING);
	len += sprintf((char*)pBody+len, "FirmVer=%s,", (uint8_t*) tempBuf);
	len += sprintf((char*)pBody+len, "PushVersion=%s,", (uint8_t*)_PUSH_VERSION_STRING);
	len += sprintf((char*)pBody+len, "CommType=%s,", (uint8_t*)_COMMTYPE_ETHERNET_STRING);
	len += sprintf((char*)pBody+len, "MaxPackageSize=%d,", 4096000);
	len += sprintf((char*)pBody+len, "LockCount=%d,", 4);
	len += sprintf((char*)pBody+len, "ReaderCount=%d,", 2);
	len += sprintf((char*)pBody+len, "AuxInCount=%d,", 1);
	len += sprintf((char*)pBody+len, "AuxOutCount:%d,", 0);
	len += sprintf((char*)pBody+len, "MachineType=%d,", 101);
	len += sprintf((char*)pBody+len, "~IsOnlyRFMachine=%d,", 0);
	len += sprintf((char*)pBody+len, "~MaxUserCount=%d,", 50);
	len += sprintf((char*)pBody+len, "~MaxUserFinger=%d,", 0);
	len += sprintf((char*)pBody+len, "MThreshold=%d,", 35);
	len += sprintf((char*)pBody+len, "IPAddress=%d.%d.%d.%d,", ETH_IPv4NetInfo.ip[0], ETH_IPv4NetInfo.ip[1], ETH_IPv4NetInfo.ip[2], ETH_IPv4NetInfo.ip[3]);
	len += sprintf((char*)pBody+len, "NetMask=%d.%d.%d.%d,", 255, 255, 255, 1);
	len += sprintf((char*)pBody+len, "~ZKFPVersion= %d,", 10);
	len += sprintf((char*)pBody+len, "IclockSvrFun=%d,", 1);
	len += sprintf((char*)pBody+len, "OverallAntiFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~REXInputFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~CardFormatFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~SupAuthrizeFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~ReaderCFGFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~ReaderLinkageFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~RelayStateFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~Ext485ReaderFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~TimeAPBFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~CtlAllRelayFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "~LossCardFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "SimpleEventType=%d,", 0);
	len += sprintf((char*)pBody+len, "VerifyStyles=%d,", 0);
	len += sprintf((char*)pBody+len, "EventTypes=BF0FE03D30000100000000007000000000000000000000000077002001000000,");
	len += sprintf((char*)pBody+len, "NewNormalEventTypes=,");
	len += sprintf((char*)pBody+len, "NewErrorEventTypes=,");
	len += sprintf((char*)pBody+len, "NewWarningEventTypes=,");
	len += sprintf((char*)pBody+len, "DisableUserFunOn=%d,",0);
	len += sprintf((char*)pBody+len, "DeleteAndFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "LogIDFunOn==%d,", 0);
	len += sprintf((char*)pBody+len, "DateFmtFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "DelAllLossCardFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "AutoClearDay=%d,", 0);
	len += sprintf((char*)pBody+len, "FirstDelayDay=%d,", 0);
	len += sprintf((char*)pBody+len, "DelayDay=%d,", 0);
	len += sprintf((char*)pBody+len, "FvFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "FaceFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "FingerFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "CameraOpen=%d,", 0);
	len += sprintf((char*)pBody+len, "AccSupportFunList=%s,", "0010000010101000000000001000000000100011101000000000000000000000000000000000000000000000000000000");
	len += sprintf((char*)pBody+len, "AutoServerFunOn=%d,", 1);
	len += sprintf((char*)pBody+len, "SubcontractingUpgradeFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "UserPicURLFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "IsSupportNFC=%d,", 1);
	len += sprintf((char*)pBody+len, "~MaxFingerCount=%d,",0);
	len += sprintf((char*)pBody+len, "~DSTF=%d,", 0);
	len += sprintf((char*)pBody+len, "Reader1IOState=%d,", 1);
	len += sprintf((char*)pBody+len, "MultiCardInterTimeFunOn=%d,", 0);
	len += sprintf((char*)pBody+len, "MachineTZFunOn=%d,",0);
	len += sprintf((char*)pBody+len, "BioPhotofun=%d,", 0);
	len += sprintf((char*)pBody+len, "VisilightFun=%d,", 0);
	len += sprintf((char*)pBody+len, "~MaxFvcount=%d,", 0);
	len += sprintf((char*)pBody+len, "~MaxBioPhotoCount=%d,", 0);
	len += sprintf((char*)pBody+len, "MultiBioDataSupport=0:0:0:0:0:0:0:0:1:1,");
	len += sprintf((char*)pBody+len, "MultiBioVersion=0:0:0:0:0:0:0:0:12:0:39.3,");
	len += sprintf((char*)pBody+len, "MaxMultiBioPhotoCount=0:0:0:0:0:0:0:0:6000:6000,");
	len += sprintf((char*)pBody+len, "MaskDetectionFunOn=%d",0);
	len += sprintf((char*)pBody+len, "\r\n");

	*Length = len;
	return _Ok;
}

//////////////////////  utils//////////////////////////////////////////////
void SET_NewTimeByHTTPC(uint32_t NewTime)
{
////	OS_SetTimerPeriod(&OS_SW_TIMER_HTTPC_POLLING, NewTime);
	return;
}

void SET_HTTPC_HeartType(uint8_t NewType)
{
	HTTP_RestCmd[HttpcSubModeStep].HttpcMainModeStep = NewType;
	return;
}

uint8_t GET_HTTPC_HeartType(void)
{
	return HTTP_RestCmd[HttpcSubModeStep].HttpcMainModeStep;
}

uint8_t * httpc_getCookie( void)
{
	static uint8_t outBuff[64];
	uint8_t *freeBuff;
	uint8_t inBuff[64];
	int outLen = 0 ;

	sprintf( inBuff, "%s", RTC_GetGmtDate());
	freeBuff = base64_encode(inBuff, sizeof(inBuff), &outLen);
	if( outLen >= 60) outLen = 60;
	outBuff[outLen]= '\0';
	memcpy(outBuff, freeBuff, outLen);
	free(freeBuff);
	return outBuff;
}

uint8_t * httpc_getSerial( void)
{
     static uint8_t prodSn[16];
     uint8_t strSnDoorNum[5];
     EEPROMGetDataBlock( EEPROM_ADDRESS_SiteInfo, prodSn, 4);
     sprintf( prodSn+4, "%02X%02X", gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
     sprintf( strSnDoorNum, "_%d", HttpcSubModeStep);
   	 memcpy( prodSn+8, strSnDoorNum, 2);
     return prodSn;
}

/*
@Name: MakeHttpPacketRtLog
@Brief:
@Param: None
@Return: None
@Note: Test Sample
*/
void HTTP_MakeHttpPacketRtLog(uint8_t nPort, uint8_t nEvent, uint8_t nVerifyType, uint8_t nReason)
{
//	Event 221 // 화재발생
//	Event 220 // 화재복구
//	로 판단 하면 됩니다. 이때 verifyType은 200 other 입니다.

	HTTP_RestCmd[nPort].HttpcMainModeStep = UHS_Type_RtLog;
	stRealtimeLog.event = nEvent;
	stRealtimeLog.verifytype = nVerifyType;
	if ( nEvent == HTTP_EVENT_ACCESS_CODE)
	{
		if( nReason == Result_OK)
		{
			memcpy( stRealtimeLog.pin, CardParameter.stParam.Pin, 8);
			stRealtimeLog.attstate = 9;	// 출입
		}
		else if( nReason == Result_Wrong_Zone)
		{
			memcpy( stRealtimeLog.pin, CardParameter.stParam.Pin, 8);
			stRealtimeLog.event = HTTP_EVENT_NO_AUTHOR;
			stRealtimeLog.verifytype = HTTP_VERIFYTYPE_NO_AUTHOR;
			stRealtimeLog.attstate = 10;	// 권한없슴
		}
		else if( nReason == Card_No_Granted)
		{
			memcpy( stRealtimeLog.pin, CardParameter.stParam.Pin, 8);
			stRealtimeLog.event = HTTP_EVENT_NO_GRANT;
			stRealtimeLog.verifytype = HTTP_VERIFYTYPE_NO_GRANT;
			stRealtimeLog.attstate = 11;	// 시간외
		}
		else  //Card_Not_Registerd
		{
			sprintf(stRealtimeLog.pin, "%d", 0);
			stRealtimeLog.event = HTTP_EVENT_NOT_REGISTER;
			stRealtimeLog.verifytype = HTTP_VERIFYTYPE_NOT_REGISTER;
			stRealtimeLog.attstate = 12;   // 미등록
		}
		sprintf(stRealtimeLog.cardno, "%02X%02X%02X%02X", CardParameter.stParam.CardId[0],
				CardParameter.stParam.CardId[1], CardParameter.stParam.CardId[2], CardParameter.stParam.CardId[3] );
	}
	else
	{
		sprintf(stRealtimeLog.pin, "%d", 0);
		sprintf(stRealtimeLog.cardno, "%d", 0);
	}
	stRealtimeLog.sitecode = 0;
	stRealtimeLog.linkid = 0;
	stRealtimeLog.eventaddr = 1;
	stRealtimeLog.index++;
	stRealtimeLog.maskflag = 255;
	return;
}




#include <stdint.h>

#ifndef __HTTPCLIENT__
#define __HTTPCLIENT__


typedef struct _HTTPC_FlowCtrlTypeDef {
	uint8_t TxFlag;
	uint8_t RxFlag;
	uint8_t Trials;
	uint32_t Timer;
} __attribute__((packed)) HTTPC_FlowCtrlTypeDef;

typedef struct _HTTPC_GetPostTypeDef {
	uint8_t HttpcMainModeStep;
	int8_t  nHttpcReturn;
	uint8_t	chrHttpcCmd[32];
	uint8_t Httpc_Command[32];
	uint8_t Httpc_Url[128];
	uint8_t Httpc_Checksum[64];
} __attribute__((packed)) _HTTPC_GetPostTypeDef;


// HTTP client debug message enable


// Data buffer size
#ifndef DATA_BUF_SIZE
	#define DATA_BUF_SIZE           			4096
#endif

#define HTTP_CLIENT_PORT_MIN   				0xB000 // 45056
#define HTTP_CLIENT_PORT_MAX   				0xFFFF // 65535

#define HTTP_EVENT_FIRE_RECOVER	220
#define HTTP_EVENT_TAMPER_CODE	100
#define HTTP_EVENT_FIRE_CODE	221
#define HTTP_EVENT_ACCESS_CODE	3
#define HTTP_EVENT_NO_GRANT	22
#define HTTP_EVENT_NO_AUTHOR	23
#define HTTP_EVENT_NOT_REGISTER	27
#define HTTP_EVENT_EXIT_CODE	102
#define HTTP_EVENT_LONG_OPEN_CODE	28

#define HTTP_VERIFYTYPE_EVENT	200
#define HTTP_VERIFYTYPE_TAMPER	0
#define HTTP_VERIFYTYPE_CARD	4
#define HTTP_VERIFYTYPE_NO_GRANT	15
#define HTTP_VERIFYTYPE_NO_AUTHOR	15
#define HTTP_VERIFYTYPE_NOT_REGISTER	4
#define HTTP_VERIFYTYPE_RS485	20
#define HTTP_VERIFYTYPE_WIEGAND	21
/*********************************************
* HTTP Method String
*********************************************/
#define HTTP_GET                    				"GET"
#define HTTP_HEAD                   				"HEAD"
#define HTTP_POST                   				"POST"
#define HTTP_PUT                    				"PUT"
#define HTTP_DELETE                 				"DELETE"

/*********************************************
* HTTP Content-Type String
*********************************************/
#define HTTP_CTYPE_MULTIPART_FORM			"multipart/form-data"
#define HTTP_CTYPE_APP_HTTP_FORM    		"Application/x-www-form-urlencoded"
#define HTTP_CTYPE_APP_JS           			"Application/javascript"
#define HTTP_CTYPE_APP_JSON         			"Application/json"
#define HTTP_CTYPE_APP_XML          			"Application/xml"
#define HTTP_CTYPE_TEXT_PLAIN       			"text/plain"
#define HTTP_CTYPE_TEXT_HTML        			"text/html"
#define HTTP_CTYPE_TEXT_CSS         			"text/css"
#define HTTP_CTYPE_TEXT_JS          			"text/javascript"
#define HTTP_CTYPE_TEXT_XML         			"text/xml"


/*********************************************
* HTTP Simple Return Value
*********************************************/
// Return value
#define HTTPC_FAILED                				0
#define HTTPC_SUCCESS               				1
#define HTTPC_CONNECTED             				2
// Return value: boolean type
#define HTTPC_FALSE                 				0
#define HTTPC_TRUE                  				1

/* Private define ------------------------------------------------------------*/
#define _ICLOCK								"/iclock/"
#define _ACLOCK								"/aclock/"
#define _HTTP_VERSION						"HTTP/1.1"
#define _USER_AGENT							"iClock Proxy/1.09"
#define _CONNECTION_KEEPALIVE				"Keep-alive"
#define _CONNECTION_STARTING				"starting"
#define _UNICODE_UTF8						"UTF-8"
#define _LANGUAGE_KR						"ko-KR"
#define _LANGUAGE_CN						"zh-CN"

#define _DEVICE_TYPE_STRING				"acc"
#define _DEVICE_NAME_STRING				"VIXacu4.0"
#define _FIRM_VER_STRING					"ZAM180-NF5-VF-3.3.3-0CM-1928-Ver1.0.1"
#define _PUSH_VERSION_STRING				"Ver 2.0.33S-20220623"
#define _COMMTYPE_ETHERNET_STRING		"ethernet"

#define _SN_BYTE_LEN						6
#define _COOKIE_BYTE_LEN					16

typedef union {
	struct __rtLog {
		uint8_t pin[9];
		uint8_t cardno[8];  // hexdecimal 은 8 자리 까지
		uint8_t sitecode;
		uint8_t linkid;
		uint8_t eventaddr;
		uint8_t event;
		uint8_t verifytype;
		uint8_t index;
		uint8_t maskflag;
		uint8_t attstate;
	} __attribute__((packed)) stRtLog;
	 uint8_t u8rtLogParam[25];
}unionRtLog_Parameter;

// cdata table tablename ////////
#define CTABLE_RTLOG	1
#define CTABLE_ERRLOG	2
#define CTABLE_IRSTATE	3
#define CTABLE_TESTDATA	4
#define CTABLE_BIODATA	5
#define CTABLE_USERPIC	6
#define CTABLE_ATTPHOTO	7
#define CTABLE_BIOPHOTO	8
#define CTABLE_TEMPEV10	9
#define CTABLE_IDCARD	10
#define CTABLE_USER		11
#define CTABLE_RTSTATE	12
#define CTABLE_DEVAUTH	13

// cdata type typename  //////////

// HTTP client status flags
extern uint8_t  httpc_isSockOpen;
extern uint8_t  httpc_isConnected;
extern uint16_t httpc_isReceived;

// extern: HTTP request structure
//extern HttpRequest request;

/*********************************************
* HTTP Client Functions
*********************************************/
uint8_t  httpc_connection_handler(uint8_t addr_len); // HTTP client socket handler - for main loop, implemented in polling

uint8_t  httpc_init(uint8_t sock, uint8_t * ip, uint16_t port, uint8_t * sbuf, uint8_t * rbuf,  uint8_t * hbuf, uint8_t * bbuf); // HTTP client initialize
uint8_t  httpc_connect(uint8_t addrlen); // HTTP client connect (after HTTP socket opened)
uint8_t  httpc_disconnect(void);
uint16_t httpc_recv(uint8_t * buf, uint16_t len); // Receive the HTTP response header and body, User have to parse the received messages depending on needs
uint8_t * httpc_getCookie( void);

uint8_t * httpc_getSerial( void);
void HTTPClient_Handler(void);

ErrState xSEND_HTTPC_SendPacket(uint8_t* pHeader, uint16_t HeaderSize, uint8_t* pBody, uint16_t BodySize);
ErrState xPROT_HTTPC_PostDevice(uint8_t* pHeader, uint16_t* pHeaderSize, uint8_t* pBody, uint16_t* pBodySize);
ErrState xPROT_HTTPC_GetHeartBeat(uint8_t* pHeader, uint16_t* pHeaderSize);

ErrState xGET_PingHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xGET_PingBody(uint8_t* pBody, uint16_t* Length);
ErrState xGET_RequestBody(uint8_t* pData, uint16_t* Length);
ErrState xGET_RequestHeader(uint8_t* pHeader,  uint16_t* Length);

ErrState xPOST_ExchangeFactorsBody(uint8_t* pBody, uint16_t* Length);
ErrState xPOST_ExchangeFactorsHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xPOST_ExchangeFactorsBody(uint8_t* pBody, uint16_t* Length);
ErrState xPOST_PushHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xPOST_PushBody(uint8_t* pBody, uint16_t* Length);
ErrState xPOST_ExchangePublicKeyHeader(uint8_t* pHeader,  uint16_t* Length);
ErrState xPOST_ExchangePublicKeyBody(uint8_t* pBody, uint16_t* Length);
ErrState xPOST_RegistryHeader(uint8_t* pHeader,  uint16_t* Length);
ErrState xPOST_RegistryBody(uint8_t* pBody, uint16_t* Length);
ErrState xPOST_CdataTableHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xPOST_CdataTableBody(uint8_t* pData, uint16_t* Length);

ErrState xPOST_RtLogTableHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xPOST_RtLogTableBody(uint8_t* pData, uint16_t* Length);

ErrState xGET_CdataConnectionBody(uint8_t* pData, uint16_t* Length);
ErrState xGET_CdataConnectionHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xGET_FwUpgradeBody(uint8_t* pBody, uint16_t* Length);
ErrState xGET_FwUpgradeHeader(uint8_t* pHeader, uint16_t* Length);

ErrState xPROT_HTTPC_CheckRecv(uint8_t * buf, uint16_t len);
ErrState xPROT_HTTPC_SendPacket(uint8_t* pHeader, uint16_t HeaderSize, uint8_t* pBody, uint16_t BodySize);

ErrState xHTTP_CmdReturnHeader(uint8_t* pHeader, uint16_t* Length);
ErrState xHTTP_CmdReturnBody(uint8_t* pBody, uint16_t* Length);

uint32_t bGET_RemainingTimeByHTTPC(void);
void SET_NewTimeByHTTPC(uint32_t NewTime);
void SET_HTTPC_HeartType(uint8_t NewType);
uint8_t GET_HTTPC_HeartType(void);
void HTTP_MakeHttpPacketRtLog(uint8_t nPort, uint8_t nEvent, uint8_t nVerifyType, uint8_t nReason);

#endif



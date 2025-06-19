/**
 * @file	userHandler.c
 * @brief	User Control Example
 * @version 1.0
 * @date	2014/07/15
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "board.h"
#include "httpUtil.h"
#include "userHandler.h"
//#include "adcHandler.h"
#include "wizchip_conf.h"
#include "main.h"

extern wiz_NetInfo gWIZNETINFO;

// Data IO Status
typedef enum
{
	Off	= 0,
	On 	= 1
} IO_Status_Type;

uint16_t LED_pin[3]={GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9};

// Pre-defined Get CGI functions
void make_json_netinfo(uint8_t * buf, uint16_t * len);

// Pre-defined Set CGI functions
int8_t set_diodir(uint8_t * uri);
int8_t set_diostate(uint8_t * uri);

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = 1 means 'uri_name' matched
	//uint8_t cgibuf[14] = {0, };
	int8_t cgi_dio = -1;
	int8_t cgi_ain = -1;

	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//make_json_todo(buf, len);
	}
	else if(strcmp((const char *)uri_name, "get_netinfo.cgi") == 0)
	{
		make_json_netinfo(buf, len);
	}
	else
	{
		if((cgi_dio < 0) && (cgi_ain < 0)) ret = 0;
	}

	return ret;
}


uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len)
{
	uint8_t ret = 1;	// ret = '1' means 'uri_name' matched
	uint8_t val = 0;

	if(strcmp((const char *)uri_name, "todo.cgi") == 0)
	{
		// to do
		;//val = todo(uri);
		//*len = sprintf((char *)buf, "%d", val);
	}
	else if(strcmp((const char *)uri_name, "set_diostate.cgi") == 0)
	{
		val = set_diostate(uri);
		*len = sprintf((char *)buf, "%d", val);
	}
	else
	{
		ret = 0;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Get CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void make_json_netinfo(uint8_t * buf, uint16_t * len)
{
	//ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);
    wizchip_getnetinfo((void*) &gWIZNETINFO);
	// DHCP: 1 - Static, 2 - DHCP
	*len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\", \
											\"ip\":\"%d.%d.%d.%d\", \
											\"gw\":\"%d.%d.%d.%d\", \
											\"sn\":\"%d.%d.%d.%d\", \
											\"dns\":\"%d.%d.%d.%d\" });",
											gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5],
											gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3],
											gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3],
											gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3],
											gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]
											);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pre-defined Set CGI functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int8_t set_diostate(uint8_t * uri)
{
	uint8_t * param;
	uint8_t pin = 0, val = 0;

	if((param = get_http_param_value((char *)uri, "pin"))) // GPIO; D0 ~ D15
	{
		pin = (uint8_t)ATOI(param, 10);
		if(pin > 15) return -1;

		if((param = get_http_param_value((char *)uri, "val")))  // State; high(on)/low(off)
		{
			val = (uint8_t)ATOI(param, 10);
			if(val > On) val = On;
		}
		printf("set_diostate.cgi LED out pin[%d] \r\n", pin );
		if(val == On) 		HAL_GPIO_WritePin(GPIOC, LED_pin[pin], GPIO_PIN_SET);	// High
		else				HAL_GPIO_WritePin(GPIOC, LED_pin[pin], GPIO_PIN_RESET);	// Low
	}

	return pin;
}


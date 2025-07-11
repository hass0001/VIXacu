/**
 * @file	httpUtil.h
 * @brief	Header File for HTTP Server Utilities
 * @version 1.0
 * @date	2014/07/15
 * @par Revision
 *			2014/07/15 - 1.0 Release
 * @author	
 * \n\n @par Copyright (C) 1998 - 2014 WIZnet. All rights reserved.
 */

#ifndef	__HTTPUTIL_H__
#define	__HTTPUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "httpServer.h"
#include "httpParser.h"

uint8_t http_get_cgi_handler(uint8_t * uri_name, uint8_t * buf, uint32_t * file_len);
uint8_t http_post_cgi_handler(uint8_t * uri_name, st_http_request * p_http_request, uint8_t * buf, uint32_t * file_len);

uint8_t predefined_get_cgi_processor(uint8_t * uri_name, uint8_t * buf, uint16_t * len);
uint8_t predefined_set_cgi_processor(uint8_t * uri_name, uint8_t * uri, uint8_t * buf, uint16_t * len);
void HTTPC_OldDecodeTime(uint32_t tt, uint8_t *tBuffyear);
uint32_t HTTPC_GetSecFromDateTime( uint8_t *chrTime);
uint8_t A2H( uint8_t c);
void parseDurString(uint8_t * encodedStr, uint8_t * startHour, uint8_t * startMinute, uint8_t * endHour, uint8_t * endMinute);

void Httpc_TreatControlDevice( uint8_t * recvBuf, uint16_t recvLen);
void Httpc_TreatUpgrade( uint8_t * recvBuf, uint16_t recvLen);
void Httpc_TreatDataDelete( uint8_t * recvBuf, uint16_t recvLen);
void Httpc_TreatSetOption( uint8_t * recvBuf, uint16_t recvLen);
void Httpc_TreatDataUpdate( uint8_t * recvBuf, uint16_t recvLen);

#ifdef __cplusplus
}
#endif

#endif

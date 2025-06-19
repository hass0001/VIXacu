/*
 * vixExRTC.h
 *
 *  Created on: Nov 11, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXEXRTC_H_
#define INC_VIXEXRTC_H_


#define _MCP7940N_DEV_ADDR						(0b01101111 << 1)	// 0x6F => 0xDE

#define _MCP7940N_REG_CMD_SIZE					1
#define _MCP7940N_REG_TIME_SIZE					3
#define _MCP7940N_REG_DATE_SIZE					4
#define _MCP7940N_REG_TIME_DATE_SIZE				(_MCP7940N_REG_TIME_SIZE + _MCP7940N_REG_DATE_SIZE)
#define _MCP7940N_REG_TIME_DATE_CMD_SIZE			(_MCP7940N_REG_TIME_SIZE + _MCP7940N_REG_DATE_SIZE + _MCP7940N_REG_CMD_SIZE)

#define _MCP7940N_TRANSMIT_MAX_DELAY				500
#define _MCP7940N_RECEIVE_MAX_DELAY				500



/* Timekkeeping */
#define _MCP7940N_REG_RTCSEC						0x00
#define _MCP7940N_REG_RTCMIN						0x01
#define _MCP7940N_REG_RTCHOUR						0x02
#define _MCP7940N_REG_RTCWKDAY					0x03
#define _MCP7940N_REG_RTCDATE						0x04
#define _MCP7940N_REG_RTCMTH						0x05
#define _MCP7940N_REG_RTCYEAR						0x06
#define _MCP7940N_REG_CONTROL						0x07
#define _MCP7940N_REG_OSCTRIM						0x08

/* Alarms */
#define _MCP7940N_REG_ALM0SEC						0x0A
#define _MCP7940N_REG_ALM0MIN						0x0B
#define _MCP7940N_REG_ALM0HOUR					0x0C
#define _MCP7940N_REG_ALM0WKDAY					0x0D
#define _MCP7940N_REG_ALM0DATE					0x0E
#define _MCP7940N_REG_ALM0MTH					0x10

#define _MCP7940N_REG_ALM1SEC						0x11
#define _MCP7940N_REG_ALM1MIN						0x12
#define _MCP7940N_REG_ALM1HOUR					0x13
#define _MCP7940N_REG_ALM1WKDAY					0x14
#define _MCP7940N_REG_ALM1DATE					0x15
#define _MCP7940N_REG_ALM1MTH					0x16

/* Power-Fail Time-Stamp */
#define _MCP7940N_REG_PWRDNMIN					0x18
#define _MCP7940N_REG_PWRDNHOUR					0x19
#define _MCP7940N_REG_PWRDNMTH					0x1B

/* Power-Fail Time-Stamp */
#define _MCP7940N_REG_PWRUPMIN					0x1C
#define _MCP7940N_REG_PWRUPHOUR					0x1D
#define _MCP7940N_REG_PWRUPDATE					0x1E
#define _MCP7940N_REG_PWRUPMTH					0x1F


#define _VBATEN_OF_RTCWKDAY_MASK_SET			(0x01 << 3)
#define _EXTOSC_OF_CONTROL_MASK_SET				(0x01 << 3)

#define _TIMEKEEPING_SEC_ONLY_MASK				0x7F
#define _TIMEKEEPING_MIN_ONLY_MASK				0x7F
#define _TIMEKEEPING_HOUR_ONLY_MASK				0x1F
#define _TIMEKEEPING_DAYOFWEEK_ONLY_MASK		0x07
#define _TIMEKEEPING_DAY_ONLY_MASK				0x3F
#define _TIMEKEEPING_MONTH_ONLY_MASK				0x1F
#define _TIMEKEEPING_YEAR_ONLY_MASK				0xFF


#define _SUNDAY										1
#define _MONDAY										2
#define _TUESDAY									3
#define _WEDNESDAY									4
#define _THURSDAY									5
#define _FRIDAY										6
#define _SATURDAY									7

#define _MONTH_MIN									1
#define _MONTH_MAX									12
#define _DAY_MIN									1
#define _DAY_MAX									31
#define _DAYOFWEEK_MIN							1
#define _DAYOFWEEK_MAX							7
#define _HOUR_MIN									0
#define _HOUR_MAX									24
#define _MINUTE_MIN									0
#define _MINUTE_MAX								60
#define _SECOND_MIN								0
#define _SECOND_MAX								60


typedef struct _EXRTC_TimeInfoTypeDef {
	RTC_TimeTypeDef RTC_Time;
	RTC_DateTypeDef RTC_Date;	
} __attribute__((packed)) EXRTC_TimeInfoTypeDef;

extern EXRTC_TimeInfoTypeDef EXRTC_TimeInfo;


typedef struct _MCP7940N_TimeKeepingRegTypeDef {
	uint8_t StartAddr;
	uint8_t RTCSEC_00h;
	uint8_t RTCMIN_01h;
	uint8_t RTCHOUR_02h;
	uint8_t RTCWKDAY_03h;
	uint8_t RTCDATE_04h;
	uint8_t RTCMTH_05h;
	uint8_t RTCYEAR_06h;
	uint8_t CONTROL_07h;
} __attribute__((packed)) MCP7940N_TimeKeepingRegTypeDef;




uint8_t bBcdToDec(uint8_t ByteBCD);
uint8_t bDecToBcd(uint8_t ByteDecimal);
ErrState xWRITE_TimeInfoToExRTC(EXRTC_TimeInfoTypeDef* EXRTC_TimeInfo);
ErrState xREAD_TimeInfoFromExRTC(EXRTC_TimeInfoTypeDef* EXRTC_TimeInfo);
ErrState xREAD_DateFromExRTC(RTC_DateTypeDef* pRTC_Date);
ErrState xREAD_TimeFromExRTC(RTC_TimeTypeDef* pRTC_Time);

uint8_t CopyToDayTime(void);

void TEST_ExRTC(void);


#endif /* INC_VIXEXRTC_H_ */

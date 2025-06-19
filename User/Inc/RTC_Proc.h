/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: RTC.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _RTC_
#define  _RTC_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   RTC_GLOBALS
#define  RTC_EXT
#else
#define  RTC_EXT   extern
#endif

/******************************************************************************
**    RTC Type Define
******************************************************************************/
typedef struct{
    INT8U YEAR;
    INT8U MON;
    INT8U DATE;				 
    INT8U WeekDAY;
    INT8U HOUR;
    INT8U MIN;
    INT8U SEC;
    INT16U totalDate;
}  __attribute__((packed)) TIME;

/******************************************************************************
**    Local Values
******************************************************************************/

/******************************************************************************
**    Global Values
******************************************************************************/
RTC_EXT TIME toDayTime;
RTC_EXT uint32_t TimeDisplay;

/******************************************************************************
**
**    Prototype
**
******************************************************************************/
RTC_EXT void RTC_Initial(void);
RTC_EXT uint32_t Time_Regulate(void);
RTC_EXT void Time_Adjust(void);
RTC_EXT void Time_Show(void);
RTC_EXT void Time_Display(uint32_t TimeVar);

RTC_EXT uint8_t GetDaysOfLeapMonth( void);
RTC_EXT void RTC_AdjustSetSequence( TIME *myDayTime);
RTC_EXT void RTC_SetTimeAndDate( TIME *myDayTime, INT8U *pdBuffer);
RTC_EXT void RTC_GetTimeAndDate( TIME *myDayTime, INT8U *ptdBuffer);
RTC_EXT void ApplyToDay(void);
RTC_EXT void SetToDay(INT8U *pBuffer);
RTC_EXT INT8U BIN_BCD(INT8U);
RTC_EXT INT8U BCD_BIN(INT8U);
INT8U INT_ASCII_LO(INT8U val);
INT8U INT_ASCII_HI(INT8U val);
#endif

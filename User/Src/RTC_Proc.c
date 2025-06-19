/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: RTC.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
/******************************************************************************
**   Include Files
******************************************************************************/
#define    RTC_GLOBALS

#include   "RTC_Proc.h"

/******************************************************************************
**    Global Values*/

uint8_t DaysOfMonth[12] =
{ 31,28,31,30,31,30,31,31,30,31,30,31};

char  StrOfMonth[12][3] =
{ {"Jan"}, {"Feb"}, {"Mar"}, {"Apl"}, {"May"}, {"Jun"}, {"Jly"}, {"Aug"}, {"Sep"}, {"Oct"}, {"Nov"}, {"Dec"} };

char StrOfWeek[7][3] =
{ {"Sun"}, {"Mon"}, {"Tus"}, {"Wed"}, {"Thr"}, {"Fri"}, {"Sat"} };


/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Initial(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}


uint8_t GetDaysOfLeapMonth(void)
{
    uint16_t theYear;
    theYear = 2000 + toDayTime.YEAR;
    
    if( ( ( ((theYear%4) == 0) && ((theYear%100) != 0) ) || ( (theYear%400) == 0) ) ) 
    {
        DaysOfMonth[1] = 29;
    }
    else
    {
        DaysOfMonth[1] = 28;
    }
    
    return DaysOfMonth[toDayTime.MON-1];
}
/******************************************************************************

((( 21 * a ) / 4) + ( 5 * b / 4 ) + ( 26 * ( c + 1 ) / 10 ) + d  - 1) % 7
 
a : 년도의 앞 두자리
b : 년도의 뒤 두자리
c : 달
d : 날짜
 
입력 날짜가 1월이나 2월이라면 년도를 - 1 하고 달에 12를 더함
13월 , 14 월

if ( mm < 3 )
{
            a = (( year - 1 ) / 100);
            b = ( year - 1 ) % ( a * 100 );
            c = 12 + month;
            d = day;
}
else
{
            a = (( year ) / 100);
            b = ( year ) % ( a * 100 );
            c = month;
            d = day;
}       
 
0 ~ 6 : 일 ~ 토
******************************************************************************/

uint8_t GetWeekDay(void)
{
    uint8_t zCentry, zDecades, zMonth, zDay, zWeek;
    uint16_t theYear;
    theYear = 2000 + toDayTime.YEAR;

    if ( toDayTime.MON < 3 )
    {
                zCentry = (( theYear - 1 ) / 100);
                zDecades = ( theYear - 1 ) % ( zCentry * 100 );
                zMonth = 12 + toDayTime.MON;
                zDay = toDayTime.DATE;
    }
    else
    {
                zCentry = (( theYear ) / 100);
                zDecades = ( theYear ) % ( zCentry * 100 );
                zMonth = toDayTime.MON;
                zDay = toDayTime.DATE;
    }       
    zWeek = (((21*zCentry) / 4) + ((5*zDecades) / 4 ) + ( (26*(zMonth+1)) / 10 ) + zDay  - 1) % 7;
//    zWeek += 1;     //  To apply to Console System
    return zWeek;
}

void ApplyToDay(void)
{
  toDayTime.SEC++;
  if( toDayTime.SEC > 59 ) {
    toDayTime.MIN++;
    toDayTime.SEC = 0;
  }
  if( toDayTime.MIN > 59 ) {
    toDayTime.HOUR++;
    toDayTime.MIN = 0;
  }  
  if( toDayTime.HOUR > 23 ) {
    toDayTime.DATE++;
    toDayTime.HOUR = 0;
  }
  if( toDayTime.DATE > GetDaysOfLeapMonth() ) {
    toDayTime.MON++;
    toDayTime.DATE = 1;
  }
  if( toDayTime.MON > 12) {
    toDayTime.YEAR++;
    toDayTime.MON = 1;
  }
  toDayTime.WeekDAY = GetWeekDay();
}


char *RTC_GetGmtDate(void)
{
    //     Tue, 14 Aug 2018 12:45:26 GMT\r\n");
    static char strGmt[64];
    char strIdMsg[32];

    strcpy(strGmt, (const char *)" ");

    sprintf(strIdMsg, "%c%c%c, ", StrOfWeek[toDayTime.WeekDAY][0], StrOfWeek[toDayTime.WeekDAY][1], StrOfWeek[toDayTime.WeekDAY][2]);
    strcat(strGmt, (const char *)strIdMsg);

    sprintf(strIdMsg, "%02d ", toDayTime.DATE);
    strcat(strGmt, (const char *)strIdMsg);

    sprintf(strIdMsg, "%c%c%c ", StrOfMonth[toDayTime.MON-1][0], StrOfMonth[toDayTime.MON-1][1], StrOfMonth[toDayTime.MON-1][2]);
    strcat(strGmt, (const char *)strIdMsg);

    sprintf(strIdMsg, "20%02d ", toDayTime.YEAR);
    strcat(strGmt, (const char *)strIdMsg);

    sprintf(strIdMsg, "%d:%d:%d GMT\r\n", toDayTime.HOUR, toDayTime.MIN, toDayTime.SEC);
    strcat(strGmt, (const char *)strIdMsg);

   return strGmt;
}

/******************************************************************************
// Set Today's Time.  Argument is required BCD data
******************************************************************************/
void SetToDay(uint8_t *pBuffer)
{
	toDayTime.YEAR = *pBuffer++;
	toDayTime.MON  = *pBuffer++;
	toDayTime.DATE = *pBuffer++;
	toDayTime.HOUR = *pBuffer++;
	toDayTime.MIN  = *pBuffer++;
	toDayTime.SEC  = *pBuffer++;
	toDayTime.WeekDAY = *pBuffer++;
}

uint8_t RTC_GetDaysOfLeapMonthF( uint8_t toYEAR, uint8_t toMON)
{
    uint16_t theYear;
    theYear = 2000 + toYEAR;

    if( ( ( ((theYear%4) == 0) && ((theYear%100) != 0) ) || ( (theYear%400) == 0) ) )
    {
        DaysOfMonth[1] = 29;
    }
    else
    {
        DaysOfMonth[1] = 28;
    }

    return DaysOfMonth[toMON-1];
}

uint8_t RTC_GetWeekDayF(uint8_t nYear, uint8_t nMonth, uint8_t nDate)
{
    uint8_t zCentry, zDecades, zMonth, zDay, zWeek;
    uint16_t theYear;
    theYear = 2000 + nYear;

    if ( nMonth < 3 )
    {
                zCentry = (( theYear - 1 ) / 100);
                zDecades = ( theYear - 1 ) % ( zCentry * 100 );
                zMonth = 12 + nMonth;
                zDay = nDate;
    }
    else
    {
                zCentry = (( theYear ) / 100);
                zDecades = ( theYear ) % ( zCentry * 100 );
                zMonth = nMonth;
                zDay = nDate;
    }
    zWeek = (((21*zCentry) / 4) + ((5*zDecades) / 4 ) + ( (26*(zMonth+1)) / 10 ) + zDay  - 1) % 7;
//    zWeek += 1;     //  To apply to Console System
    return zWeek;
}

void RTC_ApplyToDayF( TIME *myDayTime)
{
/*	nRtcScale++;  // 1.1 sec per 10 sec

	if( nRtcScale > 20)
	{
		nRtcScale = 0;
		  myDayTime->SEC++;
		  RTC_AdjustSetSequence(myDayTime);
	}
	*/
	  myDayTime->SEC++;
	  RTC_AdjustSetSequence(myDayTime);
}


void RTC_AdjustSetSequence( TIME *myDayTime)
{
#ifdef DEBUG_MODE
 //   if( (bComTimeMonitor == 1) && (WD_IsNotAllowedPrint() == YES_PRINT))
 //   	printf("RTC_ApplyToDayF Sec %d, min %d, hour %d , Date %d, mon %d\r\n",
//			 myDayTime->SEC,  myDayTime->MIN,  myDayTime->HOUR,  myDayTime->DATE,  myDayTime->MON);
#endif

  if( myDayTime->SEC > 59 ) {
    myDayTime->MIN++;
    myDayTime->SEC = 0;
  }
  if( myDayTime->MIN > 59 ) {
    myDayTime->HOUR++;
    myDayTime->MIN = 0;
  }
  if( myDayTime->HOUR > 23 ) {
    myDayTime->DATE++;
    myDayTime->HOUR = 0;
  }
  if( myDayTime->DATE > RTC_GetDaysOfLeapMonthF(myDayTime->YEAR, myDayTime->MON) ) {
    myDayTime->MON++;
    myDayTime->DATE = 1;
  }
  if( myDayTime->MON > 12) {
    myDayTime->YEAR++;
    myDayTime->MON = 1;
  }
  myDayTime->WeekDAY = RTC_GetWeekDayF(myDayTime->YEAR, myDayTime->MON, myDayTime->DATE);

}

/******************************************************************************
// Set Today's Time.  Argument is required
******************************************************************************/
void RTC_SetTimeAndDate( TIME *myDayTime, INT8U *pdBuffer)
{
	if( *(pdBuffer+0) < 80)
	{
	  myDayTime->YEAR = *(pdBuffer+0);
	  myDayTime->MON = *(pdBuffer+1);
	  myDayTime->DATE = *(pdBuffer+2);
	  myDayTime->HOUR = *(pdBuffer+3);
	  myDayTime->MIN = *(pdBuffer+4);
	  myDayTime->SEC = *(pdBuffer+5);
	  myDayTime->WeekDAY = RTC_GetWeekDayF(myDayTime->YEAR, myDayTime->MON, myDayTime->DATE);

#ifdef DEBUG_MODE
//	    if( (bComTimeMonitor == 1) && (WD_IsNotAllowedPrint() == YES_PRINT))
	    	printf("RTC_SetTimeAndDate Sec %d, min %d, hour %d , Date %d, mon %d\r\n",
					 myDayTime->SEC,  myDayTime->MIN,  myDayTime->HOUR,  myDayTime->DATE,  myDayTime->MON);
#endif
	    ConsoleSetTimeSend();
	}
}

void RTC_GetTimeAndDate( TIME *myDayTime, INT8U *ptdBuffer)
{
	 *(ptdBuffer+0) = myDayTime->YEAR;
	 *(ptdBuffer+1) = myDayTime->MON;
	 *(ptdBuffer+2) = myDayTime->DATE;
	 *(ptdBuffer+3) = myDayTime->HOUR;
	 *(ptdBuffer+4) = myDayTime->MIN;
	 *(ptdBuffer+5) = myDayTime->SEC;
	 *(ptdBuffer+6) = myDayTime->WeekDAY;
}

/******************************************************************************
******************************************************************************/
uint8_t BIN_BCD(uint8_t val){
   uint8_t  bcd10, bcd1, remain;
   remain = (uint8_t)(val%100);
   bcd10  = (uint8_t)(remain/10);
   bcd1   = (uint8_t)(remain%10);
// return (uint8_t)(bcd10<<4 + bcd1);
   return (uint8_t)(bcd10<<4 | bcd1);
}

uint8_t BCD_BIN(uint8_t val){
   uint8_t bcdBackup, binH, binL;
   bcdBackup = val;
   binH = (uint8_t)((val >> 4)*10);
   binL = (uint8_t)(bcdBackup & 0x0F);
   return (uint8_t)(binH + binL);
}

uint8_t INT_ASCII_LO(uint8_t val)
{
	return ((val%10)+'0');
}

uint8_t INT_ASCII_HI(uint8_t val)
{
	return (((val%100)/10)+'0');
}


/*
 * vixTypes.h
 *
 *  Created on: Oct 8, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXTYPES_H_
#define INC_VIXTYPES_H_


#define _RTOS							1
#define _NONE							0

#define _CLEAR							0
#define _SET								1
#define _ONGOING						2
#define _DONE							3
#define _TIMEOUT						4
#define _INVALID							5
#define _BUSY							6

#define _SUCCESS						0
#define _FAIL							1

#define _OFF								0
#define _ON								1

#define _LOW							0
#define _HIGH							1

#define _BYTE							1
#define _STRING							2

#define _FLOATING						0
#define _TX								1
#define _RX								2
#define _TX_RX							3

#define _1MS								1
#define _2MS								2
#define _3MS								3
#define _4MS								4
#define _5MS								5
#define _6MS								6
#define _7MS								7
#define _8MS								8
#define _9MS								9
#define _10MS							10
#define _20MS							20
#define _30MS							30
#define _40MS							40
#define _50MS							50
#define _100MS							100
#define _200MS							200
#define _300MS							300
#define _400MS							400
#define _500MS							500
#define _600MS							600
#define _700MS							700
#define _800MS							800
#define _1500MS							1500
	
#define _1SEC							1000
#define _2SEC							2000
#define _3SEC							3000
#define _4SEC							4000
#define _5SEC							5000
#define _6SEC							6000
#define _7SEC							7000
#define _8SEC							8000
#define _9SEC							9000
#define _10SEC							10000
#define _15SEC							15000
#define _20SEC							20000
#define _30SEC							30000
#define _1MIN							60000
#define _5MIN							300000

#define _TRIALS_1						1
#define _TRIALS_2						2
#define _TRIALS_3						3
#define _TRIALS_5						5
#define _TRIALS_10						10

#define _UPPER							1
#define _LOWER							2

#define _MSB								0
#define _LSB								1

#define _DO_NOT_CHANGE				0xFF

#define _INPUT_EVENT_TYPE				1
#define _WIEGAND_EVENT_TYPE			2




typedef enum _ErrState
{
	_Fail = 0,
	_Ok = 1,
	_Timeout = 2,
	_Invalid = 3,
	_LenErr = 4,
	_SumErr = 5,
	_WriteMemErr = 6,
	_ReadMemErr = 7,
	_OutOfMemErr = 8,
	_Unknown = 9,
	_Polling = 10,
	_Skip = 11,
	_NullErr = 12,
	_NotConnected = 13,
	_Busy = 14,

} ErrState;



#define MAX_NUM_SENSOR  4  //��?? 8???????? ?????????.. ta1000, ta4000, aru8 ��� ???????????..
#define MAX_NUM_RELAY  6  //��?? 8???????? ?????????.. ta1000, ta4000, aru8 ��� ???????????..
#define MAX_NUM_DOOR  4  //��?? 8???????? ?????????.. ta1000, ta4000, aru8 ��� ???????????..

#define MAX_NUM_BUTTON  4
#define MAX_NUM_WIEGAND 4


#ifdef  RS485_CLIENT_MODE  //????????? ????? PC?????? 485?? ????? ����?????? ���?? ����..
  #define MAX_NUM_485CS   10 //����?????????.. ????????? 10����?? ??????????? ??????.. ?????? 10?? �ֱܼ�?? ???????????? ??? ?? ����..
#else
  #define MAX_NUM_485CS   8     //16    //8?????? �ٲ�
#endif



#endif /* INC_VIXTYPES_H_ */

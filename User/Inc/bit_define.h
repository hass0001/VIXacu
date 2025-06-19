/*******************************(C) COPYRIGHT 2007 INSEM Inc.****************************************/
/* processor 	  : CORETEX-M3(STM32F10X)         		    				    */
/* compiler       : EWARM Compiler								    */
/* History:											    */
/* 04/13/2007     : Version 1.0									    */
/* copy right	  : Insem Inc.									    */
/****************************************************************************************************/
#ifndef  __BIT_DEFINE_H__
#define __BIT_DEFINE_H__

#include "stm32l4xx.h"

/************************* typedef define **********************************************************************/
typedef uint8_t 	bool;

typedef unsigned char 	uchar;
typedef unsigned char 	byte;

typedef unsigned short 	ushort;
typedef unsigned int  	uint;
typedef unsigned long  	ulong;

typedef unsigned long long u64;
typedef signed long long s64;


typedef int8_t 	Word8;
typedef uint16_t 	word;
typedef uint32_t 	dword;

typedef uint8_t        UWord8;
typedef uint16_t        UWord16;
typedef signed short	Word16;
typedef unsigned long  UWord32;
//typedef uint32_t        UWord32;
typedef unsigned long  UInt32;

typedef uint8_t 	INT8U;
typedef uint16_t 	INT16U;
typedef uint32_t 	INT32U;
typedef uint8_t 	BOOLEAN;

#define TRUE 1
#define FALSE  0

#define true           1
#define false          0

//2014-12-23, by ygkim          added
#define LINE_CUT		1
#define LINE_OK		0
#define YES			1
#define NO			0

#define BIT_FWD_SHIFT(bit) (1<<bit)
#define BYTE1(byte) (byte & 0xFF)
#define BYTE2(byte) ((byte >> 8) &0xFF)

/************************* 8,16,32 bit union **********************************************************************/

typedef union {//64��Ʈ
  uint64_t  u64Value;
  uint8_t   u8String[8];
}unionU64U8;

typedef union {//32��Ʈ
  uint32_t  u32Value;
  uint8_t   u8String[4];//little endian �̶� 1234 �� [4] [3] [2] [1] �� ��ġ��
}unionU32U8;

typedef union {//16��Ʈ
  uint16_t  u16Value;
  uint8_t   u8String[2];//little endian �̶� 12 �� [2] [1] �� ��ġ��
}unionU16U8;

typedef union {
  uint8_t u8Byte;
  struct {
    bool bit7 : 1;
    bool bit6 : 1;
    bool bit5 : 1;
    bool bit4 : 1;
    bool bit3 : 1;
    bool bit2 : 1;
    bool bit1 : 1;
    bool bit0 : 1;
  }bit;
} unionU8Bit8;

/************************* flag union **********************************************************************/


typedef union {
  unsigned char byte;
  struct {
          unsigned  b0	:1;	
          unsigned  b1	:1;	
          unsigned  b2	:1;	
          unsigned  b3	:1;	
          unsigned  b4	:1;	
          unsigned  b5	:1;	
          unsigned  b6	:1;	
          unsigned  b7	:1;
  } bit;
}__bits; 

typedef union {
  unsigned char slag;
  struct {
          unsigned  _tm0fg	:1;	
          unsigned  _tm1fg	:1;	
          unsigned  _tm2fg	:1;	
          unsigned  _tm3fg	:1;	
          unsigned  _tm4fg	:1;	
          unsigned  _tm5fg	:1;	
          unsigned  _tm6fg	:1;	
          unsigned  _tm7fg	:1;	
  } s;
}__tm;

/********************************global flag define **************************************************/
#define tm0fg	    tm.s._tm0fg
#define tm1fg	    tm.s._tm1fg
#define tm2fg	    tm.s._tm2fg
#define tm3fg	    tm.s._tm3fg
#define tm4fg	    tm.s._tm4fg
#define tm5fg	    tm.s._tm5fg
#define tm6fg	    tm.s._tm6fg
#define tm7fg	    tm.s._tm7fg


#endif
/******************* (C) COPYRIGHT 2007 INSEM Inc ***************************************END OF FILE****/


/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: WiegandTask.h
**   Project: ARU8
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  _WIEGAND_TASK_
#define  _WIEGAND_TASK_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   WIEGAND_GLOBALS
#define  WIEGAND_EXT
#else
#define  WIEGAND_EXT   extern
#endif
/******************************************************************************
**    Type Define		   		      
******************************************************************************/
#define  MAX_WIEGAND_PORT   4
#define  MAX_BYTE_CARD   4      //16 까지.. 이 값을 바꾸려면 카드값 조회 로직이 바뀐다는 전제하에 진행되어야 함..

#define  MIN_TIME_BIT_DURATION   150        // 150 us   이게 사실상 의미가 거의 없음..
#define  MAX_TIME_BYTE_DURATION    550    //기존은 10번.. 휴젠단말기처럼 느리게 보내는 경우 W130 받을 땐 계속 리셋됨


#define BIT_RESET   0
#define BIT_SET     1

/******************************************************************************
**    Global Values						    
******************************************************************************/
//WIEGAND_EXT OS_RSEMA ProgramSWSem;

WIEGAND_EXT uint64_t gU64WiegandValue1[MAX_WIEGAND_PORT];
WIEGAND_EXT uint64_t gU64WiegandValue2[MAX_WIEGAND_PORT];

WIEGAND_EXT uint8_t gUcWiegBitCnt[MAX_WIEGAND_PORT];
WIEGAND_EXT uint8_t gUcWiegBitCntSUM[MAX_WIEGAND_PORT];

WIEGAND_EXT uint16_t gUcPrevBitCnt[MAX_WIEGAND_PORT];
WIEGAND_EXT uint8_t gUcSetValue[MAX_WIEGAND_PORT];
WIEGAND_EXT uint8_t gUcWiegandType[MAX_WIEGAND_PORT];

WIEGAND_EXT uint32_t tWiegand1us[MAX_WIEGAND_PORT];
WIEGAND_EXT uint32_t  currGetus[MAX_WIEGAND_PORT];

WIEGAND_EXT uint8_t WiegandCard[MAX_WIEGAND_PORT][MAX_BYTE_CARD];

/******************************************************************************
**    Prototype        	   		    
******************************************************************************/
WIEGAND_EXT void WiegandTask(void);
WIEGAND_EXT void WiegandProcess(void);
WIEGAND_EXT void Wiegand_init_value(void);
WIEGAND_EXT void Check_Wiegand_Input(void);
//////////////////////////////////////////////////////////////////////

WIEGAND_EXT uint32_t funcConverAsciiArr2U32(uint8_t* gU8TargetData, int nLength);

//////////////////////////////////////////////////////////////////////
WIEGAND_EXT void Wiegand_reset_port(uint8_t nPort, uint8_t b34Type);
//////////////////////////////////////////////////////////////////////

WIEGAND_EXT void EXTI1_Wiegand_Init(void);
// void EXTI1_IRQHandler(void);
//////////////////////////////////////////////////////////////////////
WIEGAND_EXT void EXTI5_9_Wiegand_Init(void);
// void EXTI5_9_IRQHandler(void);
//////////////////////////////////////////////////////////////////////
WIEGAND_EXT void EXTI13_Wiegand_Init(void);
// void EXTI13_IRQHandler(void);
WIEGAND_EXT void WiegandSetInputData( uint8_t nPort, uint8_t bReset);


#endif

		   

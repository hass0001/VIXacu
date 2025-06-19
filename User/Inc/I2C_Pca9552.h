/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: PCA9552.h
**   Project: TACS
**
**   Copyright 2025 Intellivix
**
******************************************************************************/
#ifndef  _PCA9552_
#define  _PCA9552_

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   PCA9552_GLOBALS
#define  PCA9552_EXT
#else
#define  PCA9552_EXT   extern
#endif

#define  I2C_PCA9552_SLAVE_ADDR   		0xC0


PCA9552_EXT void PCALED_Init(void);
PCA9552_EXT void PCALED_Reset(void);
PCA9552_EXT void PCALED_TIM2HandlerProc(void);
PCA9552_EXT uint8_t PCALED_Check_Timer5_Alive(void);
PCA9552_EXT void PCALED_SetSensorLed(uint8_t nSensor, uint8_t nOn);

#endif

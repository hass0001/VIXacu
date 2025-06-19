/**
  ******************************************************************************
  * @file    Checksum.h 
  * @author  Tecsen CP Application Team
  * @version V3.3.0
  * @date    1/15/2015
  * @brief   This file provides all the software function headers of the Checksum.c 
  *          file.
  ******************************************************************************
  * @copy  Jylee Tecsen  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

/* Includes ------------------------------------------------------------------*/
/******************************************************************************
**    Include Files
******************************************************************************/
#include "vixConfig.h"
/******************************************************************************
**    Define
******************************************************************************/
#ifdef   CHECKSUM_GLOBALS
#define  CHECKSUM_EXT
#else
#define  CHECKSUM_EXT   extern
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
CHECKSUM_EXT uint32_t gCrcTable[256];
CHECKSUM_EXT uint32_t ulCRC;
/* Exported functions ------------------------------------------------------- */

CHECKSUM_EXT uint32_t getExtFlashCRC( uint32_t fAddr, uint32_t fLen );
CHECKSUM_EXT uint32_t getFileCRC(void);
CHECKSUM_EXT uint32_t calcCRC(uint8_t *mem, uint32_t size, uint32_t gCRC);
CHECKSUM_EXT void makeCRCtable(uint32_t *table, uint32_t id);

 
#endif  /* _CHECKSUM_H_ */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

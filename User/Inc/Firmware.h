/**
  ******************************************************************************
  * @file    OSDrivers/Firmware.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the software function headers of the ymodem.c 
  *          file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _FIRMWARE_H_
#define _FIRMWARE_H_

/* Includes ------------------------------------------------------------------*/

/******************************************************************************
**    Include Files
******************************************************************************/
#include    "vixConfig.h"
#include    "MonitorCmd.h"

/******************************************************************************
**    Define
******************************************************************************/
#ifdef   FIRMWARE_GLOBALS
#define  FIRMWARE_EXT
#else
#define  FIRMWARE_EXT   extern
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef LOG_DEBUG
#define rprintf(s...)	
#endif

#define PACKET_DOWNLOAD_DATA_SIZE (0xFF)
#define PACKET_UPLOAD_DATA_SIZE (60)//0x80)
    
#define FW_UPD_RECALL (0)
#define FW_UPD_FAIL (0xFF)


#define FIRM_START_NEW_ADDR     FLASH_FIRMWARE_NEW               // From FlashRom.h
#define FIRM_START_BAK_ADDR     FLASH_FIRMWARE_BAK               // From FlashRom.h

//#define FIRM_START_BAK_ADDR FLASH_FIRMWARE0               // From FlashRom.h
//#define FIRM_START_ARU_ADDR FLASH_FIRMWARE4               // From FlashRom.h

#define FIRM_MIN_LENGTH    (0x01C000)	// 49,152
#define FIRM_JTEG_LENGTH   (0x0245F8)	// 155,000
#define FIRM_MAX_LENGTH    (0x040000)	// 256K 로 바꿈.. 원래 아래줄임.. firmware update flag를 넣기 위함..

#define FIRM_READ_TIMEOUT    (100)
#define FIRM_CHECK_TIMEOUT    (50)

#define 	UPDATE_OK         (0)
#define 	UPDATE_ERROR      (3)
#define 	UPDATE_LEN_ERROR  (5)
#define 	UPDATE_CRC_ERROR  (7)
#define 	UPDATE_HW_ERROR   (9)
#define 	UPDATE_JTAG       (0xA)
#define 	UPDATE_OK_NEW     (0xB)
#define 	UPDATE_NEED       (0xD)

#define  INDEX_FLASH_UPGRADE_STATUS_INFO    (0)
#define  INDEX_FLASH_UPGRADE_BAK_RESULT     (INDEX_FLASH_UPGRADE_STATUS_INFO+NUM_FLASH_UPGRADE_STATUS_INFO)
#define  INDEX_FLASH_UPGRADE_BAK_LEN           (INDEX_FLASH_UPGRADE_BAK_RESULT+NUM_FLASH_UPGRADE_BAK_RESULT)
#define  INDEX_FLASH_UPGRADE_BAK_CRC           (INDEX_FLASH_UPGRADE_BAK_LEN    +NUM_FLASH_UPGRADE_BAK_LEN)
#define  INDEX_FLASH_UPGRADE_NEW_RESULT      (INDEX_FLASH_UPGRADE_BAK_CRC      +NUM_FLASH_UPGRADE_BAK_CRC)
#define  INDEX_FLASH_UPGRADE_NEW_LEN            (INDEX_FLASH_UPGRADE_NEW_RESULT +NUM_FLASH_UPGRADE_NEW_RESULT)
#define  INDEX_FLASH_UPGRADE_NEW_CRC            (INDEX_FLASH_UPGRADE_NEW_LEN      +NUM_FLASH_UPGRADE_NEW_LEN)

#define UPDATE_FROM_NEW 0 
#define UPDATE_FROM_BAK 1

typedef enum
{
  FERROR_CLEAR, //not error
  FERROR_STATE_0,      //not any state
  FERROR_LENGTH_1,     //not valid length
  FERROR_SIZE_2,     //not match data situation and data packet size
  FERROR_COMMAND_3,     //not valid command
  FERROR_ADDRESS_4,     //not match address
  FERROR_CHECKSUM_5,     //not match checksum
  FERROR_INDEX_6    //not match packet index
} FIRM_ERROR;


typedef enum
{
  MON_FROM_USER,  // Application Program Debugging
  MON_NEED_SCI1_FWUD,  // Sci1 Firmware Update 
  MON_NEED_SCI2_FWUD,  // Sci2 Firmware Update 
  MON_NEED_SPI1_FWUD,  // Spi1 Firmware Update 
  MON_END_DEF
}E_MON_MODE;

/* Exported types ------------------------------------------------------------*/
typedef  void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

/* Compute the FLASH upload image size */  
#define FLASH_IMAGE_SIZE     t32_t) (FLASH_SIZE - (APPLICATION_ADDRESS - 0x08000000))

#define RESULT_NULL -1 
#define RESULT_FAIL 0 
#define RESULT_OK    1


#define USER_UPGRADE_NONE 0
#define SCI1_UPGRADE_YES  1
#define SCI2_UPGRADE_YES  2
#define SPI1_UPGRADE_YES  3

FIRMWARE_EXT INT8U Firmware_Upgrade;

/******************************************************************************
**    Global Values
******************************************************************************/
											  
FIRMWARE_EXT uint32_t   ulFlashAddr, ulBinLen, ulBinCrc, ulDataLen,  ulDataCrc;
FIRMWARE_EXT uint16_t   nAddedFf;

FIRMWARE_EXT uint8_t    nFwdPacketKind;
FIRMWARE_EXT uint16_t   nFwdPacketIndex, nFwdByteToReceive;

FIRMWARE_EXT uint8_t	gUpgradeInfo[28];

//FIRMWARE_EXT pFunction Jump_To_Application;
//FIRMWARE_EXT uint32_t JumpAddress;
FIRMWARE_EXT uint32_t BlockNbr, UserMemoryMask;
FIRMWARE_EXT __IO uint32_t FlashProtection;

FIRMWARE_EXT uint32_t FlashDestination; /* Flash user program offset */

//FIRMWARE_EXT  uint16_t PageSize;
//FIRMWARE_EXT  uint32_t EraseCounter;
//FIRMWARE_EXT  uint32_t NbrOfPage;
//FIRMWARE_EXT  FLASH_Status FLASHStatus;

FIRMWARE_EXT INT8U g_DebugMode;
FIRMWARE_EXT E_MON_MODE bRequestNeed;

/* Exported functions ------------------------------------------------------- */

FIRMWARE_EXT void PrepareUpdatesFlash(uint8_t where2prep);
FIRMWARE_EXT void PreparesFlash4Write(uint8_t where2prep);
FIRMWARE_EXT void makeCrcGlobalTable(void);
FIRMWARE_EXT void SerialProcInit(void);
FIRMWARE_EXT void FW_SetReceivedFwd2Buffer(uint8_t cFwReceivedChar);
FIRMWARE_EXT uint8_t FirmwareWriteData(uint32_t wrAddr, uint8_t chRecv);
FIRMWARE_EXT uint8_t FirmwareReadData(uint32_t rdAddr,  uint8_t *chRecv);

FIRMWARE_EXT uint8_t FirmwareTreatStartPacket(void);
FIRMWARE_EXT uint8_t FirmwareTreatHeaderPacket(void);
FIRMWARE_EXT uint8_t FirmwareTreatDataPacket(void);
FIRMWARE_EXT uint8_t FirmwareTreatEndPacket(void);
FIRMWARE_EXT uint8_t FirmwareRequestSendData(void);
FIRMWARE_EXT uint8_t FirmwareTreatUploadPacket(void);
FIRMWARE_EXT uint8_t FirmwareTreatTailPacket(void);

FIRMWARE_EXT void FirmwareTreatSetipPacket(void);
FIRMWARE_EXT void FirmwareTreatGetipPacket(void);
FIRMWARE_EXT void FirmwareTreatSetHttpipPacket(void);
FIRMWARE_EXT void FirmwareTreatGetHttpipPacket(void);
FIRMWARE_EXT void FirmwareTreatNetModePacket( void);

FIRMWARE_EXT uint8_t FlashUploadDataPacket(void);
FIRMWARE_EXT uint8_t FlashUploadEndPacket(void);
FIRMWARE_EXT void WriteNewData2Bak(void);

FIRMWARE_EXT void AteTreatSetReaderMode(void);
FIRMWARE_EXT void AteTreatSetCardMode(void);
FIRMWARE_EXT void AteTreatSetTimeType(void);
FIRMWARE_EXT void AteTreatFactory(void);
FIRMWARE_EXT void AteTreatSreadType(void);
FIRMWARE_EXT void AteTreatReadEepromType(void);
FIRMWARE_EXT void AteTreatSetFire(void);
FIRMWARE_EXT void AteTreatCancelFire(void);
FIRMWARE_EXT void AteTreatSclearType(void);

FIRMWARE_EXT uint8_t SerialCommandProcess(void);        // external sFlash 영역에 업데이트
FIRMWARE_EXT void ReadGlobalUpgradeInfo(void);     // 업데이트 Flag Read
FIRMWARE_EXT void WriteUpdateInfoFlag(uint8_t whereTo, uint8_t whatInfo);
FIRMWARE_EXT void WriteGlobalUpgradeInfo(void);
                    
FIRMWARE_EXT uint8_t ChecksFlashFlagOk(uint8_t whereFrom);     // 업데이트 Flag  UPDATE_OK 확인
FIRMWARE_EXT uint8_t ReadChecksumAndLen(uint8_t where2Check);   // New TA
FIRMWARE_EXT void AteTreatWriteEepromType(void);
FIRMWARE_EXT void AteTreatSwriteType(void);
FIRMWARE_EXT void AteTreatSetPsnType(void);
FIRMWARE_EXT void rsprintf(const char *testbuf);
#ifdef LOG_DEBUG
FIRMWARE_EXT int rprintf( const char* format, ...);
#endif

#endif  /* _FIRMWARE_H_ */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

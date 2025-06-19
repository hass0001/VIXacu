/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: Flashrom.h
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
#ifndef  __FLASHROM_H_
#define  __FLASHROM_H_

/******************************************************************************
**    Macro Define
******************************************************************************/
#ifdef   FLASH_GLOBALS
#define  FLASH_EXT
#else
#define  FLASH_EXT   extern
#endif

/******************************************************************************
**    Include Files
******************************************************************************/
#include "vixConfig.h"
#include "vixTypes.h"
/******************************************************************************
**    Type Define
******************************************************************************/
#define Card_Registerd_OK   0
#define Card_No_Granted    1
#define Card_Deleted_OK   0
#define Card_No_Space       1
#define Card_Already_Exist    2
#define Card_Not_Registerd    3
#define Card_Error_Registerd  4

/******************************************************************************
**    Local Values
******************************************************************************/
//static uint8_t err;

/******************************************************************************
**    Global Values
******************************************************************************/

FLASH_EXT uint8_t FlashErasing;

#define	FLASH_ROM_BASE			0x00000000L

  // 1섹터내의 모바일 카드 영역
#define SIZE_OF_MPCARD_DATA 		 16 	// 16바이트의 카드정보 데이터.. cardData[7~2](6), Pin[7~0](8), Using Pass[1~0](2)
#define SIZE_OF_MPCARD_MSLOT	 	512 	//할당된 모바일 카드 슬롯의 바이트 수
#define NUM_OF_MPCARD_DATA_PER_MSLOT  31 	 // 한 슬롯당 할당된 모바일 카드 개수 512/16 - 1
#define NUM_OF_MSLOT_PER_SECTOR		  8	 // 1 섹터마다 저장되는 모바일 카드는 8*32

// 1섹터내의 모바일 카드 확장영역
#define SIZE_OF_DIR_MPCARD_DATA		 18 	// 18바이트의 카드정보 데이터.. cardData[7~0](8), Pin[7~0](8), Using Pass[1~0](2)
#define SIZE_OF_DIR_MPCARD_SLOT		4096	 // 1 Sector 4096 바이트의 확장공간 카드정보 데이터..
#define NUM_OF_DIR_MPCARD_PER_SECTOR	227 	 // 1섹터 확장공간에 저장되는 카드는 227개
#define NUM_OF_DIR_MPCARD_SECTOR		2048 	 // 모바일 카드 확장공간 섹터 슬롯 수는 2048개
#define IDX_OF_DIR_MPCARD_COUNT		4086 	 // 모바일 카드 확장공간 섹터 당 저장 모바일 카드 데이터 수가 있는 인덱스

// 1섹터내의 RF 카드 영역
#define SIZE_OF_RFCARD_DATA 		 16 	// 16바이트의 카드정보 데이터.. cardData[3~2](2), Pin[7~0](8), Using Pass[1~0](2), Last(4)
#define SIZE_OF_RFCARD_RSLOT	    256	 	//할당된 RF 카드 슬롯의 바이트 수
#define NUM_OF_RFCARD_DATA_PER_RSLOT  15 	 // 한 슬롯당 할당된 RF 카드 개수 256/16 - 1
#define NUM_OF_RSLOT_PER_SECTOR		  16	 // 1 섹터마다 저장되는 RF 카드는 8*32

// 1섹터내의 RF 카드 확장영역
#define SIZE_OF_DIR_RFCARD_DATA		 18 	// 18바이트의 카드정보 데이터.. cardData[7~0](8), Pin[7~0](8), Using Pass[1~0](2), Last(4)
#define SIZE_OF_DIR_RFCARD_SLOT		4096	 // 1 Sector 4096 바이트의 확장공간 카드정보 데이터..
#define NUM_OF_DIR_RFCARD_PER_SECTOR	227 	 // 1섹터 확장공간에 저장되는 카드는 227개
#define NUM_OF_DIR_RFCARD_SECTOR		1024 	 // RF카드 확장공간 섹터 수는 1024개
#define IDX_OF_DIR_RFCARD_COUNT		4086 	 // RF카드 확장공간 섹터 당 저장 RF 카드 데이터 수가 있는 인덱스


// pin 권한 Flash ㅇㅁㅅㅁ
#define NUM_BYTE_PIN_COUNT	2
#define NUM_BYTE_PIN_AUTHORITY	10
#define NUM_SECTOR_PIN_AUTHORITY	10


//  Firmware 업데이트 정보..
#define FLASH_FIRMWARE_NEW	0x00000000L	//  F/W 업데이트 영역
#define FLASH_FIRMWARE_BAK	0x00040000L	//  F/W 복구 safe 영역

#define FLASH_FIRMWARE_INFO	0x00080000L	//  F/W 업데이트 관련 정보, eeprom 으로 옮겨가기..

#define FLASH_ScheduleData	     0x00081000L  // 타임코드, Event History 영역
#define FLASH_EventHisory_Start  0x00082000L
#define FLASH_EventHisory_End	 0x00092000L

// CARD 데이터 영역
#define FLASH_MpCardData_Start	FLASH_EventHisory_End	//	0x00092000L
#define FLASH_MpCardData_End	0x02092000L

#define FLASH_RfCardData_Start	FLASH_EventHisory_End	//	0x00092000L
#define FLASH_RfCardData_End	0x01092000L
//#define FLASH_RfCardData_Start	FLASH_MpCardData_End	//	0x02092000L
//#define FLASH_RfCardData_End	0x03092000L

#define FLASH_MpCardData_Direct_Start	FLASH_RfCardData_End	//	0x03092000L
#define FLASH_MpCardData_Direct_End		0x03892000L

#define FLASH_RfCardData_Direct_Start	FLASH_RfCardData_End	// 0x01092000L
#define FLASH_RfCardData_Direct_End		0x01493000L
//#define FLASH_RfCardData_Direct_Start	FLASH_MpCardData_Direct_End	// 0x03892000L
//#define FLASH_RfCardData_Direct_End		0x03C92000L

#define FLASH_AUTHORITY_DATA  	0x03092000L
#define FLASH_ATE_TEST_START	0x03C93000L
#define FLASH_ATE_TEST_END		0x03FFF000L	// 테스트 끝인데 위에꺼 보다 길게..

//External Flash Control
#define  ADDR_FLASH_UPGRADE_STATUS_INFO    FLASH_FIRMWARE_INFO
#define  NUM_FLASH_UPGRADE_STATUS_INFO    10
#define  ADDR_FLASH_UPGRADE_BAK_RESULT    ADDR_FLASH_UPGRADE_STATUS_INFO+NUM_FLASH_UPGRADE_STATUS_INFO
#define  NUM_FLASH_UPGRADE_BAK_RESULT    1
#define  ADDR_FLASH_UPGRADE_BAK_LEN    ADDR_FLASH_UPGRADE_BAK_RESULT+NUM_FLASH_UPGRADE_BAK_RESULT
#define  NUM_FLASH_UPGRADE_BAK_LEN    4
#define  ADDR_FLASH_UPGRADE_BAK_CRC    ADDR_FLASH_UPGRADE_BAK_LEN+NUM_FLASH_UPGRADE_BAK_LEN
#define  NUM_FLASH_UPGRADE_BAK_CRC    4

#define  ADDR_FLASH_UPGRADE_NEW_RESULT    ADDR_FLASH_UPGRADE_BAK_CRC+NUM_FLASH_UPGRADE_BAK_CRC
#define  NUM_FLASH_UPGRADE_NEW_RESULT   1
#define  ADDR_FLASH_UPGRADE_NEW_LEN    ADDR_FLASH_UPGRADE_NEW_RESULT+NUM_FLASH_UPGRADE_NEW_RESULT
#define  NUM_FLASH_UPGRADE_NEW_LEN    4
#define  ADDR_FLASH_UPGRADE_NEW_CRC    ADDR_FLASH_UPGRADE_NEW_LEN+NUM_FLASH_UPGRADE_NEW_LEN
#define  NUM_FLASH_UPGRADE_NEW_CRC    4

#define  NUM_FLASH_UPGRADE_INFO_TOTAL    28

/*************************W25Q80 **************************************/
#define RDID            0x9F  /*  Figure 25. Read Identifcation (RDID) Sequence (Command 9F) */

#define WRITE           0x02  /* Write to Memory instruction */
#define WRSR            0x01  /* Write Status Register instruction */

#define WREN            0x06  /* Write enable instruction */
#define WRDI            0x04  /* Write disable instruction */

#define READ            0x03  /* Read from Memory instruction */
#define READ_HIGH_SPEED 0x0B  /* Read from Memory instruction. Read at Higher Speed (FAST_READ) Sequence (Command 0B)*/
#define RDSR            0x05  /* Read Status Register instruction  */

#define SE              0x20  /* Sector Erase instruction */
#define BE32            0x52  /* Block32 Erase instruction */
#define BE64            0xD8  /* Block64 Erase instruction */
#define CHIP_ERASE      0xC7  /* Chip Erase instruction */

#define Dummy_Byte      0xA5  /* dumy data */

#define WIP_Flag        0x01  /* Write In Progress (WIP) flag */


#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_W25Q80_ID         0x134014

void __no_operation(void);
void Delay(void);

#define NOP         __no_operation();

#define  Success                0
#define  Fail                   1

/******************************************************************************
**    Type Define
******************************************************************************/

typedef struct
{
//	INT8U EnWeek;			// From Sunday to Holiday... what..?
	INT8U StartHh;
	INT8U StartMm;
	INT8U EndHh;
	INT8U EndMm;
} __attribute__((packed)) Schedule_Parameter;

#define MAX_NUM_TIMECODE  32  //63개면 64로.. 30개면 31로.. 배열 0은 그냥 공란..
#define MAX_NUM_WEEK_HOLIDAY  8 //(일~월,휴)

#define  LEN_TIMECODE_ALL   (4*32*8*4)  // 4096 시작시분2byte종료시분2byte * 7개요일+공휴일 * 127개 4096, 8192, 30개 3968
#define  LEN_TIMECODE_LOCK	  (32*8*4) 	// 1024
#define  LEN_TIMECODE		     (8*4)  // 32

//FLASH_EXT Schedule_Parameter ScheduleParameter[MAX_NUM_TIMECODE][MAX_NUM_WEEK_HOLIDAY];
// MaxTimeCode*Week&holiday(일~월,휴)
FLASH_EXT Schedule_Parameter ScheduleParameter[MAX_NUM_DOOR][MAX_NUM_TIMECODE][MAX_NUM_WEEK_HOLIDAY];
// MaxLock*MaxTimeCode*Week&holiday(일~월,휴)   4096
      // ScheduleParameter[nLock][nCode][nWeek].EnWeek  // 출입문별 스케쥴 적용을 위해서 MAX_LOCk 도 넣어야 함.

typedef union {
  struct {
    INT8U CardId[4];			// ID 4
    INT8U Pin[8];				// pin 8
    INT8U DoorAuth_CardKind;    // 1 : 상시 ID.	2 : 일시 ID.	3 : 당일카드 ID.
    INT8U PassCode[2];      	// 카드+비밀번호 사용시) 현재는 0x00, 0x00 으로 전송
    INT8U Location;      		// 1 어드레스 영역 : 2 확장 영역.
//    INT8U DoorAuth;      // Door 별 출입권한. bit flag.. 0xf0 == 1111 0000 : 1~4 번 권한 있음..
//    INT8U AntiPass;         // 초기 값은 0x00이며 모든 문 통과. 0xFF는 모든 문 통과 못함 (기본 0x00으로 설정). 
//    INT8U StartDate[5];     // (일시카드에서 출입가능기간)  년(1) 월(1) 일(1) 시(1) 분(1).
//    INT8U EndDate[5];
  }  __attribute__((packed)) stParam;
  uint8_t u8strParam[16];
}unionCard_Parameter;

FLASH_EXT unionCard_Parameter CardParameter;        // 39

/******************************************************************************
**
**    Prototype
**
******************************************************************************/

FLASH_EXT void SPI2_sFlash_Init(void);

FLASH_EXT uint16_t CheckRfCardDataInFlash( uint8_t *cardData);
FLASH_EXT uint16_t CheckMpCardDataInFlash( uint8_t *cardData);
FLASH_EXT uint8_t WriteRfCardDataToSerialFlash( uint8_t *cardData);
FLASH_EXT uint8_t WriteRfCardDataBlockToSerialFlash(uint8_t *cardData); //address형 card data 블럭으로 쓰기..
FLASH_EXT uint8_t OverWriteRfCardDataToSerialFlash( uint8_t *stCardData);
FLASH_EXT uint8_t NewWriteRfCardDataToSerialFlash( uint8_t *stCardData);
FLASH_EXT uint8_t WriteMpCardDataToSerialFlash( uint8_t *cardData) ;
FLASH_EXT uint8_t DeleteRfCardDataOnSerialFlash( uint8_t *cardData, uint8_t u8Flag);
FLASH_EXT uint8_t DeleteMpCardDataOnSerialFlash( uint8_t *cardData) ;
//FLASH_EXT uint8_t ClearCardDataOfSerialFlashAll(void);
FLASH_EXT  uint8_t ClearRfCardIndexOnSerialFlash( void);
//FLASH_EXT uint8_t ClearEventDataOfSerialFlashAll(void);
FLASH_EXT uint8_t ClearCardNumOfSerialFlash(uint32_t sFlash4kAddr);
FLASH_EXT uint8_t ResetCardDataOfSerialFlash(void);

FLASH_EXT uint8_t EraseSFlash(uint32_t u32StartAddress, uint32_t u32EndAddress);
FLASH_EXT void WriteSFlash4kSectorData( uint32_t sectorAddress, uint8_t * buf4kData);

FLASH_EXT uint8_t AddAuthorityToSerialFlash( uint8_t *stPinData, uint8_t *stCardData, uint8_t nPort);
FLASH_EXT uint8_t DeleteAuthorityFromSerialFlash( uint8_t *stPinData, uint8_t *stCardData, uint8_t nPort);
FLASH_EXT uint8_t GetAuthorityFromSerialFlash( uint8_t *stPinData, uint8_t *stCardData);

#endif



  

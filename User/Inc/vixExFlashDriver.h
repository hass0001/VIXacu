/*
 * vixExFlashDriver.h
 *
 *  Created on: Oct 21, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXEXFLASHDRIVER_H_
#define INC_VIXEXFLASHDRIVER_H_


/* MX25L51245G Macronix memory */
/* Size of the flash */
#define QSPI_FLASH_SIZE                  							0x4000000 	/* 512 MBits => 64MBytes */
#define QSPI_SECTOR_SIZE                 							0x10000   	/* 1024 sectors of 64KBytes */
#define MX25LM51245G_SECTOR_SIZE							QSPI_SECTOR_SIZE
#define QSPI_SUBSECTOR_SIZE             	 					0x1000    	/* 16384 subsectors of 4kBytes */
#define QSPI_PAGE_SIZE                   							0x100     	/* 262144 pages of 256 bytes */

#define QSPI_DUMMY_CYCLES_READ_QUAD      					3
#define QSPI_DUMMY_CYCLES_READ           					8
#define QSPI_DUMMY_CYCLES_READ_QUAD_IO   				10
#define QSPI_DUMMY_CYCLES_READ_DTR       					6
#define QSPI_DUMMY_CYCLES_READ_QUAD_DTR  				8

/* Dummy Clock Cycle */
#define DUMMY_CYCLES_READ            							8U
#define DUMMY_CYCLES_READ_OCTAL      						6U
#define DUMMY_CYCLES_READ_OCTAL_DTR  					6U
#define DUMMY_CYCLES_REG_OCTAL       						4U
#define DUMMY_CYCLES_REG_OCTAL_DTR   						5U

#define DUMMY_CLOCK_CYCLES_READ     						6
#define DUMMY_CLOCK_CYCLES_READ_REG 					4

#define QSPI_BULK_ERASE_MAX_TIME         					600000
#define QSPI_SECTOR_ERASE_MAX_TIME       					2000
#define QSPI_SUBSECTOR_ERASE_MAX_TIME   	 				800


/* Flash commands */
#define OCTAL_IO_READ_CMD           							0xEC13
#define OCTAL_PAGE_PROG_CMD         							0x12ED
#define OCTAL_READ_STATUS_REG_CMD   						0x05FA
#define OCTAL_SECTOR_ERASE_CMD      						0x21DE
#define OCTAL_WRITE_ENABLE_CMD      						0x06F9

#define WRITE_STATUS_REG_CMD								0x01
#define READ_STATUS_REG_CMD         							0x05
#define WRITE_CFG_REG_2_CMD         							0x72
#define WRITE_ENABLE_CMD            							0x06
#define RESET_ENABLE_CMD									0x66
#define RESET_MEMORY_CMD									0x99
#define SUBSECTOR_ERASE_CMD								0x21
#define BULK_ERASE_CMD										0x60



/***** READ/WRITE MEMORY Operations with 3-Byte Address ****************************/
#define MX25LM51245G_READ_CMD                             0x03U   /*!< Normal Read 3 Byte Address                            */
#define MX25LM51245G_FAST_READ_CMD                        0x0BU   /*!< Fast Read 3 Byte Address                              */
#define MX25LM51245G_PAGE_PROG_CMD                        0x02U   /*!< Page Program 3 Byte Address                           */
#define MX25LM51245G_SUBSECTOR_ERASE_4K_CMD               0x20U   /*!< SubSector Erase 4KB 3 Byte Address                    */
#define MX25LM51245G_SECTOR_ERASE_64K_CMD                 0xD8U   /*!< Sector Erase 64KB 3 Byte Address                      */
#define MX25LM51245G_BULK_ERASE_CMD                       0x60U   /*!< Bulk Erase                                            */

/***** READ/WRITE MEMORY Operations with 4-Byte Address ****************************/
#define MX25LM51245G_4_BYTE_ADDR_READ_CMD                 0x13U   /*!< Normal Read 4 Byte address                            */
#define MX25LM51245G_4_BYTE_ADDR_FAST_READ_CMD            0x0CU   /*!< Fast Read 4 Byte address                              */
#define MX25LM51245G_4_BYTE_PAGE_PROG_CMD                 0x12U   /*!< Page Program 4 Byte Address                           */
#define MX25LM51245G_4_BYTE_SUBSECTOR_ERASE_4K_CMD        0x21U   /*!< SubSector Erase 4KB 4 Byte Address                    */
#define MX25LM51245G_4_BYTE_SECTOR_ERASE_64K_CMD          0xDCU   /*!< Sector Erase 64KB 4 Byte Address                      */

/***** Setting commands ************************************************************/
#define MX25LM51245G_WRITE_ENABLE_CMD                     0x06U   /*!< Write Enable                                          */
#define MX25LM51245G_WRITE_DISABLE_CMD                    0x04U   /*!< Write Disable                                         */
#define MX25LM51245G_PROG_ERASE_SUSPEND_CMD               0xB0U   /*!< Program/Erase suspend                                 */
#define MX25LM51245G_PROG_ERASE_RESUME_CMD                0x30U   /*!< Program/Erase resume                                  */
#define MX25LM51245G_ENTER_DEEP_POWER_DOWN_CMD            0xB9U   /*!< Enter deep power down                                 */
#define MX25LM51245G_SET_BURST_LENGTH_CMD                 0xC0U   /*!< Set burst length                                      */
#define MX25LM51245G_ENTER_SECURED_OTP_CMD                0xB1U   /*!< Enter secured OTP)                                    */
#define MX25LM51245G_EXIT_SECURED_OTP_CMD                 0xC1U   /*!< Exit secured OTP)                                     */

/***** RESET commands ************************************************************/
#define MX25LM51245G_NOP_CMD                              0x00U   /*!< No operation                                          */
#define MX25LM51245G_RESET_ENABLE_CMD                     0x66U   /*!< Reset Enable                                          */
#define MX25LM51245G_RESET_MEMORY_CMD                     0x99U   /*!< Reset Memory                                          */

/***** Register Commands (SPI) ****************************************************/
#define MX25LM51245G_READ_ID_CMD                          0x9FU   /*!< Read IDentification                                   */
#define MX25LM51245G_READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5AU   /*!< Read Serial Flash Discoverable Parameter              */
#define MX25LM51245G_READ_STATUS_REG_CMD                  0x05U   /*!< Read Status Register                                  */
#define MX25LM51245G_READ_CFG_REG_CMD                     0x15U   /*!< Read configuration Register                           */
#define MX25LM51245G_WRITE_STATUS_REG_CMD                 0x01U   /*!< Write Status Register                                 */
#define MX25LM51245G_READ_CFG_REG2_CMD                    0x71U   /*!< Read configuration Register2                          */
#define MX25LM51245G_WRITE_CFG_REG2_CMD                   0x72U   /*!< Write configuration Register2                         */
#define MX25LM51245G_READ_FAST_BOOT_REG_CMD               0x16U   /*!< Read fast boot Register                               */
#define MX25LM51245G_WRITE_FAST_BOOT_REG_CMD              0x17U   /*!< Write fast boot Register                              */
#define MX25LM51245G_ERASE_FAST_BOOT_REG_CMD              0x18U   /*!< Erase fast boot Register                              */
#define MX25LM51245G_READ_SECURITY_REG_CMD                0x2BU   /*!< Read security Register                                */
#define MX25LM51245G_WRITE_SECURITY_REG_CMD               0x2FU   /*!< Write security Register                               */
#define MX25LM51245G_READ_LOCK_REG_CMD                    0x2DU   /*!< Read lock Register                                    */
#define MX25LM51245G_WRITE_LOCK_REG_CMD                   0x2CU   /*!< Write lock Register                                   */

#define MX25LM51245G_READ_DPB_REG_CMD                     0xE0U   /*!< Read DPB register                                     */
#define MX25LM51245G_WRITE_DPB_REG_CMD                    0xE1U   /*!< Write DPB register                                    */
#define MX25LM51245G_READ_SPB_STATUS_CMD                  0xE2U   /*!< Read SPB status                                       */
#define MX25LM51245G_WRITE_SPB_BIT_CMD                    0xE3U   /*!< SPB bit program                                       */
#define MX25LM51245G_ERASE_ALL_SPB_CMD                    0xE4U   /*!< Erase all SPB bit                                     */
#define MX25LM51245G_WRITE_PROTECT_SEL_CMD                0x68U   /*!< Write Protect selection                               */
#define MX25LM51245G_GANG_BLOCK_LOCK_CMD                  0x7EU   /*!< Gang block lock: whole chip write protect             */
#define MX25LM51245G_GANG_BLOCK_UNLOCK_CMD                0x98U   /*!< Gang block unlock: whole chip write unprotect         */
#define MX25LM51245G_READ_PASSWORD_REGISTER_CMD           0x27U   /*!< Read Password                                         */
#define MX25LM51245G_WRITE_PASSWORD_REGISTER_CMD          0x28U   /*!< Write Password                                        */
#define MX25LM51245G_PASSWORD_UNLOCK_CMD                  0x29U   /*!< Unlock Password                                       */


/*******************************************************************/
/********************************* OPI  ****************************/
/*******************************************************************/

/***** READ/WRITE MEMORY Operations  ****************************/
#define MX25LM51245G_OCTA_READ_CMD                             0xEC13U  /*!< Octa IO Read                                     */
#define MX25LM51245G_OCTA_READ_DTR_CMD                         0xEE11U  /*!< Octa IO Read DTR                                 */
#define MX25LM51245G_OCTA_PAGE_PROG_CMD                        0x12EDU  /*!< Octa Page Program                                */
#define MX25LM51245G_OCTA_SUBSECTOR_ERASE_4K_CMD               0x21DEU  /*!< Octa SubSector Erase 4KB                         */
#define MX25LM51245G_OCTA_SECTOR_ERASE_64K_CMD                 0xDC23U  /*!< Octa Sector Erase 64KB 3                         */
#define MX25LM51245G_OCTA_BULK_ERASE_CMD                       0x609FU  /*!< Octa Bulk Erase                                  */

/***** Setting commands ************************************************************/
#define MX25LM51245G_OCTA_WRITE_ENABLE_CMD                     0x06F9U   /*!< Octa Write Enable                               */
#define MX25LM51245G_OCTA_WRITE_DISABLE_CMD                    0x04FBU   /*!< Octa Write Disable                              */
#define MX25LM51245G_OCTA_PROG_ERASE_SUSPEND_CMD               0xB04FU   /*!< Octa Program/Erase suspend                      */
#define MX25LM51245G_OCTA_PROG_ERASE_RESUME_CMD                0x30CFU   /*!< Octa Program/Erase resume                       */
#define MX25LM51245G_OCTA_ENTER_DEEP_POWER_DOWN_CMD            0xB946U   /*!< Octa Enter deep power down                      */
#define MX25LM51245G_OCTA_SET_BURST_LENGTH_CMD                 0xC03FU   /*!< Octa Set burst length                           */
#define MX25LM51245G_OCTA_ENTER_SECURED_OTP_CMD                0xB14EU   /*!< Octa Enter secured OTP)                         */
#define MX25LM51245G_OCTA_EXIT_SECURED_OTP_CMD                 0xC13EU   /*!< Octa Exit secured OTP)                          */

/***** RESET commands ************************************************************/
#define MX25LM51245G_OCTA_NOP_CMD                              0x00FFU   /*!< Octa No operation                               */
#define MX25LM51245G_OCTA_RESET_ENABLE_CMD                     0x6699U   /*!< Octa Reset Enable                               */
#define MX25LM51245G_OCTA_RESET_MEMORY_CMD                     0x9966U   /*!< Octa Reset Memory                               */

/***** Register Commands (OPI) ****************************************************/
#define MX25LM51245G_OCTA_READ_ID_CMD                          0x9F60U   /*!< Octa Read IDentification                        */
#define MX25LM51245G_OCTA_READ_SERIAL_FLASH_DISCO_PARAM_CMD    0x5AA5U   /*!< Octa Read Serial Flash Discoverable Parameter   */
#define MX25LM51245G_OCTA_READ_STATUS_REG_CMD                  0x05FAU   /*!< Octa Read Status Register                       */
#define MX25LM51245G_OCTA_READ_CFG_REG_CMD                     0x15EAU   /*!< Octa Read configuration Register                */
#define MX25LM51245G_OCTA_WRITE_STATUS_REG_CMD                 0x01FEU   /*!< Octa Write Status Register                      */
#define MX25LM51245G_OCTA_READ_CFG_REG2_CMD                    0x718EU   /*!< Octa Read configuration Register2               */
#define MX25LM51245G_OCTA_WRITE_CFG_REG2_CMD                   0x728DU   /*!< Octa Write configuration Register2                     */
#define MX25LM51245G_OCTA_READ_FAST_BOOT_REG_CMD               0x16E9U   /*!< Octa Read fast boot Register                    */
#define MX25LM51245G_OCTA_WRITE_FAST_BOOT_REG_CMD              0x17E8U   /*!< Octa Write fast boot Register                   */
#define MX25LM51245G_OCTA_ERASE_FAST_BOOT_REG_CMD              0x18E7U   /*!< Octa Erase fast boot Register                   */
#define MX25LM51245G_OCTA_READ_SECURITY_REG_CMD                0x2BD4U   /*!< Octa Read security Register                     */
#define MX25LM51245G_OCTA_WRITE_SECURITY_REG_CMD               0x2FD0U   /*!< Octa Write security Register                    */
#define MX25LM51245G_OCTA_READ_LOCK_REG_CMD                    0x2DD2U   /*!< Octa Read lock Register                         */
#define MX25LM51245G_OCTA_WRITE_LOCK_REG_CMD                   0x2CD3U   /*!< Octa Write lock Register                        */
#define MX25LM51245G_OCTA_READ_DPB_REG_CMD                     0xE01FU   /*!< Octa Read DPB register                          */
#define MX25LM51245G_OCTA_WRITE_DPB_REG_CMD                    0xE11EU   /*!< Octa Write DPB register                         */
#define MX25LM51245G_OCTA_READ_SPB_STATUS_CMD                  0xE21DU   /*!< Octa Read SPB status                            */
#define MX25LM51245G_OCTA_WRITE_SPB_BIT_CMD                    0xE31CU   /*!< Octa SPB bit program                            */
#define MX25LM51245G_OCTA_ERASE_ALL_SPB_CMD                    0xE41BU   /*!< Octa Erase all SPB bit                          */
#define MX25LM51245G_OCTA_WRITE_PROTECT_SEL_CMD                0x6897U   /*!< Octa Write Protect selection                    */
#define MX25LM51245G_OCTA_GANG_BLOCK_LOCK_CMD                  0x7E81U   /*!< Octa Gang block lock: whole chip write protect  */
#define MX25LM51245G_OCTA_GANG_BLOCK_UNLOCK_CMD                0x9867U   /*!< Octa Gang block unlock: whole chip write unprote*/
#define MX25LM51245G_OCTA_READ_PASSWORD_REGISTER_CMD           0x27D8U   /*!< Octa Read Password                              */
#define MX25LM51245G_OCTA_WRITE_PASSWORD_REGISTER_CMD          0x28D7U   /*!< Octa Write Password                             */
#define MX25LM51245G_OCTA_PASSWORD_UNLOCK_CMD                  0x29D6U   /*!< Octa Unlock Password                            */

/******************************************************************************
  * @brief  MX25LM51245G Registers
  ****************************************************************************/
/* Status Register */
#define MX25LM51245G_SR_WIP                               0x01U   /*!< Write in progress                                       */
#define MX25LM51245G_SR_WEL                               0x02U   /*!< Write enable latch                                      */
#define MX25LM51245G_SR_PB                                0x3CU   /*!< Block protected against program and erase operations    */

/* Configuration Register 1 */
#define MX25LM51245G_CR1_ODS                              0x07U   /*!< Output driver strength                                  */
#define MX25LM51245G_CR1_TB                               0x08U   /*!< Top / bottom  selected                                  */
#define MX25LM51245G_CR1_PBE                              0x10U   /*!< Preamble bit enable                                     */

/* Configuration Register 2 */
/* Address : 0x00000000 */
#define MX25LM51245G_CR2_REG1_ADDR                        0x00000000U  /*!< CR2 register address 0x00000000                    */
#define MX25LM51245G_CR2_SOPI                             0x01U        /*!< STR OPI Enable                                     */
#define MX25LM51245G_CR2_DOPI                             0x02U        /*!< DTR OPI Enable                                     */
/* Address : 0x00000200 */
#define MX25LM51245G_CR2_REG2_ADDR                        0x00000200U  /*!< CR2 register address 0x00000200                    */
#define MX25LM51245G_CR2_DQSPRC                           0x01U        /*!< DTR DQS pre-cycle                                  */
#define MX25LM51245G_CR2_DOS                              0x02U        /*!< DQS on STR mode                                    */
/* Address : 0x00000300 */
#define MX25LM51245G_CR2_REG3_ADDR                        0x00000300U  /*!< CR2 register address 0x00000300                    */
#define MX25LM51245G_CR2_DC                               0x07U        /*!< Dummy cycle                                        */
#define MX25LM51245G_CR2_DC_20_CYCLES                     0x00U        /*!< 20 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_18_CYCLES                     0x01U        /*!< 18 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_16_CYCLES                     0x02U        /*!< 16 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_14_CYCLES                     0x03U        /*!< 14 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_12_CYCLES                     0x04U        /*!< 12 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_10_CYCLES                     0x05U        /*!< 10 Dummy cycles                                    */
#define MX25LM51245G_CR2_DC_8_CYCLES                      0x06U        /*!< 8 Dummy cycles                                     */
#define MX25LM51245G_CR2_DC_6_CYCLES                      0x07U        /*!< 6 Dummy cycles                                     */
/* Address : 0x00000500 */
#define MX25LM51245G_CR2_REG4_ADDR                        0x00000500U  /*!< CR2 register address 0x00000500                    */
#define MX25LM51245G_CR2_PPTSEL                           0x01U        /*!< Preamble pattern selection                         */
/* Address : 0x40000000 */
#define MX25LM51245G_CR2_REG5_ADDR                        0x40000000U  /*!< CR2 register address 0x40000000                    */
#define MX25LM51245G_CR2_DEFSOPI                          0x01U        /*!< Enable SOPI after power on reset                   */
#define MX25LM51245G_CR2_DEFDOPI                          0x02U        /*!< Enable DOPI after power on reset                   */

/* Security Register */
#define MX25LM51245G_SECR_SOI                             0x01U        /*!< Secured OTP indicator                              */
#define MX25LM51245G_SECR_LDSO                            0x02U        /*!< Lock-down secured OTP                              */
#define MX25LM51245G_SECR_PSB                             0x04U        /*!< Program suspend bit                                */
#define MX25LM51245G_SECR_ESB                             0x08U        /*!< Erase suspend bit                                  */
#define MX25LM51245G_SECR_P_FAIL                          0x20U        /*!< Program fail flag                                  */
#define MX25LM51245G_SECR_E_FAIL                          0x40U        /*!< Erase fail flag                                    */
#define MX25LM51245G_SECR_WPSEL                           0x80U        /*!< Write protection selection                         */



/* Auto-polling values */
#define WRITE_ENABLE_MATCH_VALUE    						0x02
#define WRITE_ENABLE_MASK_VALUE     						0x02

#define MEMORY_READY_MATCH_VALUE    						0x00
#define MEMORY_READY_MASK_VALUE     						0x01

#define AUTO_POLLING_INTERVAL       							0x10

/* Memory registers address */
#define CONFIG_REG2_ADDR1           							0x0000000
#define CR2_STR_OPI_ENABLE         	 						0x01

#define CONFIG_REG2_ADDR3           							0x00000300
#define CR2_DUMMY_CYCLES_66MHZ      						0x07

/* Status Register */
#define MX25LM51245G_SR_WIP                               			0x01U   /*!< Write in progress                                       */
#define MX25LM51245G_SR_WEL                               			0x02U   /*!< Write enable latch                                      */
#define MX25LM51245G_SR_PB                                			0x3CU   /*!< Block protected against program and erase operations    */

/* Configuration Register 1 */
#define MX25LM51245G_CR1_ODS                              			0x07U   /*!< Output driver strength                                  */
#define MX25LM51245G_CR1_TB                               			0x08U   /*!< Top / bottom  selected                                  */
#define MX25LM51245G_CR1_PBE                              			0x10U   /*!< Preamble bit enable                                     */

/* Memory delay */
#define MEMORY_REG_WRITE_DELAY      						40
#define MEMORY_PAGE_PROG_DELAY							2

/* End address of the OSPI memory */
#define OSPI_END_ADDR              	 							(1 << OSPI_FLASH_SIZE)

/* Size of buffers */
//#define BUFFERSIZE                  								(COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)         							(sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))


/**
  * @brief  MX25LM51245G Size configuration
  */
#define MX25LM51245G_SECTOR_64K                   				(uint32_t)(64 * 1024)        		/* 1024 sectors of 64KBytes     */
#define MX25LM51245G_SUBSECTOR_4K                 				(uint32_t)(4  * 1024)        		/* 16384 subsectors of 4KBytes  */

#define MX25LM51245G_FLASH_SIZE                   				(uint32_t)(512*1024*1024/8)  	/* 512 Mbits => 64MBytes        */
#define MX25LM51245G_PAGE_SIZE                    				(uint32_t)256                		/* 262144 pages of 256 Bytes    */

/**
  * @brief  MX25LM51245G Timing configuration
  */

#define MX25LM51245G_BULK_ERASE_MAX_TIME          			460000U
#define MX25LM51245G_SECTOR_ERASE_MAX_TIME        			1000U
#define MX25LM51245G_SUBSECTOR_4K_ERASE_MAX_TIME  		400U
#define MX25LM51245G_WRITE_REG_MAX_TIME           			40U
#define MX25LM51245G_RESET_MAX_TIME               				100U                 				/* when SWreset during erase operation */
#define MX25LM51245G_AUTOPOLLING_INTERVAL_TIME    		0x10U

#define _BLOCK_ERASE_MAX_TIME								2000U


/** @defgroup MX25LM51245G_Exported_Types MX25LM51245G Exported Types
  * @{
  */
typedef struct
{
	uint32_t FlashSize;                        		/*!< Size of the flash                             */
	uint32_t EraseSectorSize;                  	/*!< Size of sectors for the erase operation       */
  	uint32_t EraseSectorsNumber;              /*!< Number of sectors for the erase operation     */
  	uint32_t EraseSubSectorSize;               	/*!< Size of subsector for the erase operation     */
  	uint32_t EraseSubSectorNumber;          /*!< Number of subsector for the erase operation   */
  	uint32_t EraseSubSector1Size;              /*!< Size of subsector 1 for the erase operation   */
  	uint32_t EraseSubSector1Number;         /*!< Number of subsector 1 for the erase operation */
  	uint32_t ProgPageSize;                     	/*!< Size of pages for the program operation       */
  	uint32_t ProgPagesNumber;                  /*!< Number of pages for the program operation     */
}  __attribute__((packed)) MX25LM51245G_Info_t;

typedef enum
{
  	MX25LM51245G_SPI_MODE = 0,                 /*!< 1-1-1 commands, Power on H/W default setting  */
  	MX25LM51245G_OPI_MODE                      /*!< 8-8-8 commands                                */
}  __attribute__((packed)) MX25LM51245G_Interface_t;

typedef enum
{
  	MX25LM51245G_STR_TRANSFER = 0,            /*!< Single Transfer Rate                          */
  	MX25LM51245G_DTR_TRANSFER                  /*!< Double Transfer Rate                          */
}  __attribute__((packed)) MX25LM51245G_Transfer_t;

typedef enum
{
  	MX25LM51245G_ERASE_4K = 0,                 /*!< 4K size Sector erase                          */
  	MX25LM51245G_ERASE_64K,                     /*!< 64K size Block erase                          */
  	MX25LM51245G_ERASE_BULK                    /*!< Whole bulk erase                              */
}  __attribute__((packed)) MX25LM51245G_Erase_t;

typedef enum
{
  	MX25LM51245G_3BYTES_SIZE = 0,              /*!< 3 Bytes address mode                           */
  	MX25LM51245G_4BYTES_SIZE                   	 /*!< 4 Bytes address mode                           */
}  __attribute__((packed)) MX25LM51245G_AddressSize_t;




/* Private function prototypes -----------------------------------------------*/

ErrState xRESET_ExFlash(OSPI_HandleTypeDef *hospi);
ErrState xOSPI_WriteEnable(OSPI_HandleTypeDef *hospi);
ErrState xOSPI_AutoPollingMemReady(OSPI_HandleTypeDef *hospi);
ErrState xOSPI_EnableMemoryMappedMode(OSPI_HandleTypeDef *hospi);
ErrState xERASE_ExFlashChip(OSPI_HandleTypeDef *hospi);
ErrState xOSPI_Config(OSPI_HandleTypeDef *hospi);

void OPEN_ExFlash_MX25L(void);
ErrState xWRITE_BurstToExFlash(OSPI_HandleTypeDef *hospi, uint32_t WriteAddr, uint8_t* pData, uint16_t Len);
ErrState xREAD_BurstFromExFlash(OSPI_HandleTypeDef *hospi, uint32_t ReadAddr, uint8_t* pData, uint16_t Len);
ErrState xERASE_ExFlashBlock(OSPI_HandleTypeDef *hospi, uint32_t BlockAddr);
void MX25LM51245G_GetFlashInfo(MX25LM51245G_Info_t *pInfo);
ErrState xMX25LM51245G_ReadCfgRegister(OSPI_HandleTypeDef *hospi, uint8_t *Value);
ErrState xMX25LM51245G_ReadCfg2Register(OSPI_HandleTypeDef *hospi, uint32_t ReadAddr, uint8_t *Value);
ErrState xREAD_ExFlashChipID(OSPI_HandleTypeDef *hospi, uint8_t* Manufacture);

void TEST_ExFlash(void);


#endif /* INC_VIXEXFLASHDRIVER_H_ */

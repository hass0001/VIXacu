/*
 * vixI2cDriver.h
 *
 *  Created on: Oct 8, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXEEPROM_H_
#define INC_VIXEEPROM_H_


#include "vixTypes.h"



#define _I2C_MASTER_ADDR						0x00	// STM32
#define _24LC128_DEV_ADDR						0x00	// 0b00000000
#define _23LC128_CTRL_CODE						0b10100000

#define _LEDDRIVER_I2C_ADDR					(_PCA9552_DEV_ADDR << 1)
#define _EEPROM_I2C_ADDR						(_23LC128_CTRL_CODE | (_24LC128_DEV_ADDR << 1) )
#define _RTC_I2C_ADDR							(_MCP7940N_DEV_ADDR << 1)

#define _I2C_MEMADD_SIZE_8BIT					0x01
#define _I2C_MEMADD_SIZE_16BIT					0x02
#define _24LC128_PAGE_SIZE						64
#define _24LC128_PAGE_WRITETIME_MAX			5
#define _EEPROM_CAPACITY_BYTE_SIZE			1024		// 1K
#define _EEPROM_ADDR_MASK						(_EEPROM_CAPACITY_BYTE_SIZE-1)
#define _EEPROM_PAGE_MASK						(_24LC128_PAGE_SIZE-1)
#define _EEPROM_ALL_ERASE						0xFF
#define _EEPROM_FIRST_PAGE_NUM				0
#define _EEPROM_LAST_PAGE_NUM					15




void SEARCH_I2cSlavesAll(void);
void RECOVER_I2cError(I2C_HandleTypeDef *hi2c);

ErrState xWRITE_DataToEeprom(uint16_t WriteAddr, uint8_t* pData, uint16_t Length);
ErrState xERASE_EepromPage(uint8_t PageNum);
ErrState xREAD_DataFromEeprom(uint16_t WriteAddr, uint8_t* pData, uint16_t Length);
void TEST_Eeprom(void);

void WriteNumCardToEeprom(INT32U nCardData);
uint32_t ReadNumCardFromEeprom(void);



#endif /* INC_VIXEEPROM_H_ */

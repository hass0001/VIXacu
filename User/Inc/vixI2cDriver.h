/*
 * vixI2cDriver.h
 *
 *  Created on: Oct 8, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXI2CDRIVER_H_
#define INC_VIXI2CDRIVER_H_


#include "vixTypes.h"


#define _I2C_MASTER_ADDR						0x00	// STM32
#define _PCA9552_DEV_ADDR						0x60	// 0b01100000
#define _24LC128_DEV_ADDR						0x00	// 0b00000000
#define _MCP7940N_DEV_ADDR					0x6F	// ob01101111

#define _23LC128_CTRL_CODE						0b10100000

#define _LEDDRIVER_I2C_ADDR					(_PCA9552_DEV_ADDR << 1)
#define _EEPROM_I2C_ADDR						(_23LC128_CTRL_CODE | (_24LC128_DEV_ADDR << 1) )
#define _RTC_I2C_ADDR							(_MCP7940N_DEV_ADDR << 1)

/* PCA9552 Control Register */
#define LEDDRIVER12_REG_INPUT0					0x00		// input register 0
#define LEDDRIVER12_REG_INPUT1                    		0x01		// input register 1
#define LEDDRIVER12_REG_PSC0                       		0x02		// frequency prescaler 0
#define LEDDRIVER12_REG_PWM0					0x03		// PWM register 0
#define LEDDRIVER12_REG_PSC1					0x04		// frequency prescaler 1
#define LEDDRIVER12_REG_PWM1					0x05		// PWM register 1
#define LEDDRIVER12_REG_LS0					0x06		// LED0 to LED3 selector
#define LEDDRIVER12_REG_LS1                         		0x07		// LED4 to LED7 selector
#define LEDDRIVER12_REG_LS2                         		0x08		// LED8 to LED11 selector
#define LEDDRIVER12_REG_LS3                         		0x09		// LED12 to LED15 selector

#define _I2C_MEMADD_SIZE_8BIT					0x01
#define _I2C_MEMADD_SIZE_16BIT					0x02
#define _24LC128_PAGE_SIZE						64
#define _24LC128_PAGE_WRITETIME_MAX			5
#define _EEPROM_CAPACITY_BYTE_SIZE			1024		// 1K
#define _EEPROM_ADDR_MASK						(_EEPROM_CAPACITY_BYTE_SIZE-1)
#define _EEPROM_PAGE_MASK						(_24LC128_PAGE_SIZE-1)



void SEARCH_I2cSlaves(uint8_t Flag);
void RESET_LedDriver_PCA9552(uint8_t Flag);
void RECOVER_I2cError(I2C_HandleTypeDef *hi2c);
ErrState CONTROL_InputLeds(uint16_t RegAddr, uint8_t* Buf, uint16_t Len) ;

ErrState xWRITE_BurstToEeprom(uint16_t WriteAddr, uint8_t* pData, uint16_t Length);
ErrState xREAD_BurstFromEeprom(uint16_t WriteAddr, uint8_t* pData, uint16_t Length);
void TEST_Eeprom(void);


#endif /* INC_VIXI2CDRIVER_H_ */

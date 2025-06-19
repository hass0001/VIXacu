/*
 * vixIndicator.h
 *
 *  Created on: Oct 7, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXINDICATOR_H_
#define INC_VIXINDICATOR_H_

#include "vixTypes.h"



#define _STATE_LED_ON()						HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_SET)
#define _RUN_LED_ON()						HAL_GPIO_WritePin(RUN_LED_GPIO_Port, RUN_LED_Pin, GPIO_PIN_SET)
#define _RS485_LED_ON()						HAL_GPIO_WritePin(RS485_LED_GPIO_Port, RS485_LED_Pin, GPIO_PIN_SET)
#define _CAN_LED_ON()						HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_SET)

#define _STATE_LED_OFF()					HAL_GPIO_WritePin(STATE_LED_GPIO_Port, STATE_LED_Pin, GPIO_PIN_RESET)
#define _RUN_LED_OFF()						HAL_GPIO_WritePin(RUN_LED_GPIO_Port, RUN_LED_Pin, GPIO_PIN_RESET)
#define _RS485_LED_OFF()					HAL_GPIO_WritePin(RS485_LED_GPIO_Port, RS485_LED_Pin, GPIO_PIN_RESET)
#define _CAN_LED_OFF()						HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_RESET)

#define _STATE_LED_TOGGLE()					HAL_GPIO_TogglePin(STATE_LED_GPIO_Port, STATE_LED_Pin)
#define _RUN_LED_TOGGLE()					HAL_GPIO_TogglePin(RUN_LED_GPIO_Port, RUN_LED_Pin)
#define _RS485_LED_TOGGLE()					HAL_GPIO_TogglePin(RS485_LED_GPIO_Port, RS485_LED_Pin)
#define _CAN_LED_TOGGLE()					HAL_GPIO_TogglePin(CAN_LED_GPIO_Port, CAN_LED_Pin);

#define _PCA9552_NRST_LOW()				HAL_GPIO_WritePin(LED_RESET_GPIO_Port, LED_RESET_Pin, GPIO_PIN_RESET);
#define _PCA9552_NRST_HIGH()				HAL_GPIO_WritePin(LED_RESET_GPIO_Port, LED_RESET_Pin, GPIO_PIN_SET);

// i2c
#define _PCA9552_DEV_ADDR								(0b01100000 << 1)	// 0x60 => 0xC0

#define _PCA9552_LED_CHANNEL_MAX						16
#define _PCA9552_LED_ADDR_BLOCK_SIZE					4
#define _INPUT_LED_NUM_MAX							12
#define _ARRAY_LED_NUM_MAX							16

#define _PCA9552_LED_0_3_BASE_ADDR					6
#define _PCA9552_LED_4_7_BASE_ADDR					7
#define _PCA9552_LED_8_11_BASE_ADDR					8
#define _PCA9552_LED_12_15_BASE_ADDR					9

#define _PCA9552_LED_0_3_BLOCK_SEL					0
#define _PCA9552_LED_4_7_BLOCK_SEL					1
#define _PCA9552_LED_8_11_BLOCK_SEL					2
#define _PCA9552_LED_12_15_BLOCK_SEL					3

#define _PCA9552_LED_BIT_MASK							0x01
#define _PCA9552_LED_BLOCK_ON							0x00
#define _PCA9552_LED_BLOCK_OFF						0x55

#define _PCA9552_NO_PWM_MASK							0x01
#define _PCA9552_AUTOINC_REG_SET						0b00010000			// 0x10
#define _PAC9552_AUTOINC_REG_CLEAR					0b00000000

#define _PCA9552_TRANSMIT_MAX_DELAY					200
#define _PCA9552_RECEIVE_MAX_DELAY					200




typedef struct _PCA9552_FullPacketTypeDef {
	uint8_t SubAddr;
	uint8_t Period0;
	uint8_t PWM0;
	uint8_t Period1;
	uint8_t PWM1;
	uint8_t LED_0_3;
	uint8_t LED_4_7;
	uint8_t LED_8_11;
	uint8_t LED_12_15;
} __attribute__((packed)) PCA9552_FullPacketTypeDef;


typedef struct _PCA9552_LedPacketTypeDef {
	uint8_t SubAddr;
	uint8_t LED_0_3;
	uint8_t LED_4_7;
	uint8_t LED_8_11;
	uint8_t LED_12_15;
} __attribute__((packed)) PCA9552_LedPacketTypeDef;





void INIT_StatusLeds(uint8_t Flag);

void nRST_PCA9552(void);
void SEARCH_I2cLedDriver(void);
void OPEN_ArrayLED(void);
void CONTROL_ArrayLED(uint8_t* pLedBuf);
ErrState xCONTROL_ArrayLED_PCA95(uint8_t* pControlBit);
ErrState xCONTROL_IndividualLED_PCA95(uint8_t ControlBit, uint8_t LedBlock);
void TEST_LedDriver(void);
void INIT_ArrayLeds(uint8_t Flag);



#endif /* INC_VIXINDICATOR_H_ */

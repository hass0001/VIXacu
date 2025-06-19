/*
 * vixAdcInput.h
 *
 *  Created on: Nov 6, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXINPUT_H_
#define INC_VIXINPUT_H_


#define _GPIO_INPUT
//#define _ADC_INPUT

#define _NORMAL_OPEN							0
#define _NORMAL_CLOSE							1

#define _GPIO_INPUT_PIN_MAX					12
#define _ADC_INPUT_CH_MAX						12
//#define _ADC_REF_VOLTAGE						3.3f

#define _ALARM_DETECT_PIN_MAX					4

#define _DOOR_SENSOR							1
#define _EXIT_BUTTON							2
#define _AUX_IN									3
#define _TAMPER_ALARM							4
#define _FIRE_ALARM								5
#define _LOW_BAT_ALARM						6
#define _PWR_FAIL_ALARM						7




typedef struct _INPUT_StatusTypeDef {
	uint8_t OldPinStatus[_GPIO_INPUT_PIN_MAX];
	uint8_t ChatteringFlag[_GPIO_INPUT_PIN_MAX];
	uint8_t ChatteringCount[_GPIO_INPUT_PIN_MAX];
} __attribute__((packed)) INPUT_StatusTypeDef;



typedef struct _ALARM_SenseStatusTypeDef {
	uint8_t OldPinStatus[_ALARM_DETECT_PIN_MAX];
	//uint8_t ChatteringFlag[_ALARM_DETECT_PIN_MAX];
	uint8_t ChatteringCount[_ALARM_DETECT_PIN_MAX];
} __attribute__((packed)) ALARM_SenseStatusTypeDef;



typedef struct _INPUT_ConfigTypeDef {
	uint8_t PinNum[_GPIO_INPUT_PIN_MAX];
} __attribute__((packed)) INPUT_ConfigTypeDef;





uint8_t hREAD_InputPin(uint8_t PinNum);
//uint8_t hREAD_AlarmPin(uint8_t PinNum);
void TEST_Input(void);
void INIT_Input(void);


#endif /* INC_VIXINPUT_H_ */

/*
 * vixOutput.h
 *
 *  Created on: Nov 15, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXOUTPUT_H_
#define INC_VIXOUTPUT_H_



#define _RELAY_OUTPUT_PIN_MAX				6

#define _RELAY_1_ON()						HAL_GPIO_WritePin(RELAY1_GPIO_Port, RELAY1_Pin, GPIO_PIN_SET)
#define _RELAY_2_ON()						HAL_GPIO_WritePin(RELAY2_GPIO_Port, RELAY2_Pin, GPIO_PIN_SET)
#define _RELAY_3_ON()						HAL_GPIO_WritePin(RELAY3_GPIO_Port, RELAY2_Pin, GPIO_PIN_SET)
#define _RELAY_4_ON()						HAL_GPIO_WritePin(RELAY4_GPIO_Port, RELAY3_Pin, GPIO_PIN_SET)
#define _RELAY_5_ON()						HAL_GPIO_WritePin(RELAY5_GPIO_Port, RELAY5_Pin, GPIO_PIN_SET)
#define _RELAY_6_ON()						HAL_GPIO_WritePin(RELAY6_GPIO_Port, RELAY6_Pin, GPIO_PIN_SET)

#define _RELAY_1_OFF()						HAL_GPIO_WritePin(RELAY1_GPIO_Port, RELAY1_Pin, GPIO_PIN_RESET)				
#define _RELAY_2_OFF()						HAL_GPIO_WritePin(RELAY2_GPIO_Port, RELAY2_Pin, GPIO_PIN_RESET)
#define _RELAY_3_OFF()						HAL_GPIO_WritePin(RELAY3_GPIO_Port, RELAY3_Pin, GPIO_PIN_RESET)
#define _RELAY_4_OFF()						HAL_GPIO_WritePin(RELAY4_GPIO_Port, RELAY4_Pin, GPIO_PIN_RESET)
#define _RELAY_5_OFF()						HAL_GPIO_WritePin(RELAY5_GPIO_Port, RELAY5_Pin, GPIO_PIN_RESET)
#define _RELAY_6_OFF()						HAL_GPIO_WritePin(RELAY6_GPIO_Port, RELAY6_Pin, GPIO_PIN_RESET)


#define _TTL_OUTPUT_PIN_MAX				2

#define _TTL_1								0
#define _TTL_2								1

#define _TTL_1_ON()							HAL_GPIO_WritePin(TTL_OUT1_GPIO_Port, TTL_OUT1_Pin, GPIO_PIN_SET)
#define _TTL_2_ON()							HAL_GPIO_WritePin(TTL_OUT2_GPIO_Port, TTL_OUT2_Pin, GPIO_PIN_SET)

#define _TTL_1_OFF()							HAL_GPIO_WritePin(TTL_OUT1_GPIO_Port, TTL_OUT1_Pin, GPIO_PIN_RESET)				
#define _TTL_2_OFF()							HAL_GPIO_WritePin(TTL_OUT2_GPIO_Port, TTL_OUT2_Pin, GPIO_PIN_RESET)

#define _TTL_OUT								0
#define _WIEGAND_OUT						1



void INIT_Relays(uint8_t Flag);
void CONTROL_aRelay(uint8_t PinNum, uint8_t Flag);
void CONTROL_ArrayRelay(uint8_t* pRlyBuf);
void INIT_TTLOut(uint8_t Flag);
void CONTROL_aTTLOut(uint8_t PinNum, uint8_t Flag);
void TEST_TTLOut(uint8_t Flag);
ErrState xFIND_MappedRelay(uint8_t Type, uint8_t Index, uint8_t* pRlyBuf);



#endif /* INC_VIXOUTPUT_H_ */

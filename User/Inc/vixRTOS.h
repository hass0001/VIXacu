/*
 * vixRTOS.h
 *
 *  Created on: Nov 12, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXRTOS_H_
#define INC_VIXRTOS_H_


void TEST_SoftwareTimSample(void);
void CREATE_SoftwareTimer(void);
void DELAY_OsHal(uint32_t Millisecond);

void LedIndicator_Task(void);
void CREATE_IndicatorTask(void);



#endif /* INC_VIXRTOS_H_ */

/*
 * vixConsole.h
 *
 *  Created on: Oct 2, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXDEBUGCONSOLE_H_
#define INC_VIXDEBUGCONSOLE_H_

#include "stm32l4xx_hal.h"


void CREATE_DebugConsoleMutex(void);
void Dprintf(const char *fmt, ...);
void OPEN_DebugConsole(void);


#endif /* INC_VIXDEBUGCONSOLE_H_ */

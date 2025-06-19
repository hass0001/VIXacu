/*
 * vixConfig.h
 *
 *  Created on: Oct 7, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXCONFIG_H_
#define INC_VIXCONFIG_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//#include "RTOS.h"
//#include "cmsis_os2.h"

#include "stm32l4xx.h"
#include "stm32l4xx_it.h"

#include "main.h"
#include "bit_define.h"
#include <RTC_Proc.h>

#include "FlashRom.h"
#include "vixEthernet.h"
#include "vixDebugConsole.h"
#include "vixIndicator.h"
#include "vixEEPROM.h"
#include "vixSpiDriver.h"
#include "vixExNorFlash.h"
//#include "vixRS485Reader.h"
#include "vixUartQueue.h"
#include "vixInput.h"
#include "vixOutput.h"
#include "vixCrypto.h"
#include "vixExRTC.h"
#include "vixLibrary.h"
#include "vixID.h"
#include "vixTypes.h"
#include "vixRTOS.h"

#include "httpServer.h"
#include "httpClient.h"

#include "webpage.h"
#include "wizchip_conf.h"
#include "dhcpv4.h"
#include "application.h"
#include "socket.h"
#include "loopback.h"
#include "w6100.h"
#include "httpParser.h"
#include "httpUtil.h"

//#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "octospi.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "hash.h"
#include "crc.h"
#include "rng.h"

#include "vixFactoryMode.h"
/* */

#include "Version_Plus.h"
#include "WiegandTask.h"
#include "SCI3FacTask.h"
#include "Firmware.h"
#include "FacAutoTest.h"
#include <I2C_Eeprom.h>
#include "I2C_Pca9552.h"
#include "CheckSum.h"

#include "SPI1NetTask.h"
#include "InTask.h"
#include "SCI1FwdTask.h"

#include "EthernetDriver.h"
#include "SCI2Rs485Task.h"
#include "StartTask.h"
#include "History.h"

#define _DEBUG_MODE
#define _USE_RTOS


#ifdef __cplusplus
}
#endif

#endif /* INC_VIXCONFIG_H_ */

/*
 * vixSpiDriver.h
 *
 *  Created on: Oct 10, 2024
 *      Author: intellivix
 */

#ifndef INC_VIXSPIDRIVER_H_
#define INC_VIXSPIDRIVER_H_


#define _SPI_TIMEOUT					1000


typedef struct _WIZ_DatapackTypeDef {
	uint8_t Address[3];
	void* vBuf;
} __attribute__((packed)) WIZ_DatapackTypeDef;



void ENABLE_Spi1Dma(void);
uint8_t READ_aByteFromW6100(void);
void WRITE_aByteToW6100(uint8_t Byte);
void READ_BurstFromW6100(uint8_t* pRxBuf, uint16_t Length);
void WRITE_BurstToW6100(uint8_t* pTxBuf, uint16_t Length);


#endif /* INC_VIXSPIDRIVER_H_ */

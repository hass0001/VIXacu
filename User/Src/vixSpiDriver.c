/*
 * vixSpiDriver.c
 *
 *  Created on: Oct 10, 2024
 *      Author: intellivix
 */


#include "vixConfig.h"


#define _W6100_SPI_HANDLE					hspi1
#define _W6100_DMA_TX_BASE				DMA1
#define _W6100_DMA_RX_BASE				DMA1
#define _W6100_DMA_TX_CH					LL_DMA_CHANNEL_3
#define _W6100_DMA_RX_CH					LL_DMA_CHANNEL_4
#define _W6100_SPI_SIZE						1



WIZ_DatapackTypeDef WIZ_Datapack;


uint8_t READ_aByteFromW6100(void)
{
	uint8_t rx = 0, tx = 0xFF;
	uint16_t cntWait = 0;

	while(HAL_SPI_GetState(&_W6100_SPI_HANDLE) != HAL_SPI_STATE_READY &&
		HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) != HAL_DMA_STATE_READY)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	HAL_SPI_TransmitReceive(&_W6100_SPI_HANDLE, &tx, &rx, _W6100_SPI_SIZE, _SPI_TIMEOUT);

	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	return rx;
}

void WRITE_aByteToW6100(uint8_t Tx)
{
	uint8_t rx = 0;
	uint16_t cntWait = 0;

	while(HAL_SPI_GetState(&_W6100_SPI_HANDLE) != HAL_SPI_STATE_READY &&
		HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) != HAL_DMA_STATE_READY)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	HAL_SPI_TransmitReceive(&_W6100_SPI_HANDLE, &Tx, &rx, _W6100_SPI_SIZE, _SPI_TIMEOUT);

	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	return;
}

void READ_BurstFromW6100(uint8_t* pRxBuf, uint16_t Length)
{
//	SPI1_Callback.TxStatus = _CLEAR;
//	SPI1_Callback.RxStatus = _CLEAR;
//	SPI1_Callback.TxTimeout = _1SEC;
	uint16_t cntWait = 0;

	while(HAL_SPI_GetState(&_W6100_SPI_HANDLE) != HAL_SPI_STATE_READY &&
		HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) != HAL_DMA_STATE_READY)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;

	HAL_SPI_Receive_DMA(&_W6100_SPI_HANDLE, pRxBuf, Length);

	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	return;
}

void WRITE_BurstToW6100(uint8_t* pTxBuf, uint16_t Length)
{
	uint16_t cntWait = 0;
	while(HAL_SPI_GetState(&_W6100_SPI_HANDLE) != HAL_SPI_STATE_READY &&
		HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) != HAL_DMA_STATE_READY && HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) != HAL_DMA_STATE_READY)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;

	HAL_SPI_Transmit_DMA(&_W6100_SPI_HANDLE, pTxBuf, Length);

	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmarx) == HAL_DMA_STATE_RESET)
	{
	 cntWait++;
	 if( cntWait > 5000)
		 break;
	}
cntWait = 0;
	while(HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_BUSY || HAL_DMA_GetState(_W6100_SPI_HANDLE.hdmatx) == HAL_DMA_STATE_RESET)
		{
		 cntWait++;
		 if( cntWait > 5000)
			 break;
		}
	cntWait = 0;
	return;
}



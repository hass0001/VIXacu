
/******************************************************************************
**
**	 CONFIDENTIAL PROPERTY OF TECSEN
**
**   File Name: EEPROM.c
**   Project: TACS
**
**   Copyright 2012 TECSEN
**
******************************************************************************/
/******************************************************************************
**   Include Files
******************************************************************************/
#define   PCA9552_GLOBALS
#include   <I2C_Pca9552.h>

extern 	I2C_HandleTypeDef hi2c1;
extern  TIM_HandleTypeDef htim5;

uint8_t gPcaLed[4];
void PCALED_Init(void)
{
	PCALED_Reset();
	HAL_TIM_Base_Start_IT(&htim5);
}

void PCALED_Reset(void)
{
	HAL_GPIO_WritePin( LED_RESET_GPIO_Port, LED_RESET_Pin ,GPIO_PIN_RESET);
	MyMsDelay(2);
	HAL_GPIO_WritePin( LED_RESET_GPIO_Port, LED_RESET_Pin ,GPIO_PIN_SET);
	memset( gPcaLed, 0x55, 4);
}

void PCALED_Set_LED(void)
{
	uint8_t pcsSetting[9] = { 0x12, 0x2B, 0x80, 0x0A, 0xC0, 0x55, 0x55, 0x55, 0x55 };

	HAL_I2C_Master_Transmit( &hi2c1,  I2C_PCA9552_SLAVE_ADDR, pcsSetting, 9, HAL_MAX_DELAY);
}

void PCALED_SetSensorLed(uint8_t nSensor, uint8_t nOn)
{
	uint8_t pcsSetting[9] = { 0x06, 0xFA };
	switch(nSensor)
	{
	case 0:  // ADC1 -> LED0
		{
			pcsSetting[0] = 0x06;
			if(nOn == 1)
			{
				gPcaLed[0] &= 0xFC;
	//			gPcaLed[0] |= 0x00;
			}
			else
			{
				gPcaLed[0] &= 0xFC;
				gPcaLed[0] |= 0x01;
			}
			pcsSetting[1] = gPcaLed[0];
			break;
		}
	case 1:  // ADC4 -> LED3
		{
			pcsSetting[0] = 0x06;
			if(nOn == 1)
			{
				gPcaLed[0] &= 0x3F;
	//			gPcaLed[0] |= 0x00;
			}
			else
			{
				gPcaLed[0] &= 0x3F;
				gPcaLed[0] |= 0x40;
			}
			pcsSetting[1] = gPcaLed[0];
			break;
		}
	case 2:	  // ADC12 -> LED8
		{
			pcsSetting[0] = 0x08;
			if(nOn == 1)
			{
				gPcaLed[2] &= 0xFC;
	//			gPcaLed[2] |= 0x00;
			}
			else
			{
				gPcaLed[2] &= 0xFC;
				gPcaLed[2] |= 0x01;
			}
			pcsSetting[1] = gPcaLed[2];
			break;
		}
	case 3:	  // ADC15 -> LED11
		{
			pcsSetting[0] = 0x08;
			if(nOn == 1)
			{
				gPcaLed[2] &= 0x3F;
	//			gPcaLed[2] |= 0x00;
			}
			else
			{
				gPcaLed[2] &= 0x3F;
				gPcaLed[2] |= 0x40;
			}
			pcsSetting[1] = gPcaLed[2];
			break;
		}
	default:
		{
			pcsSetting[0] = 0x09;
			pcsSetting[1] = 0x5A;
			break;
		}
	}

	HAL_I2C_Master_Transmit( &hi2c1,  I2C_PCA9552_SLAVE_ADDR, pcsSetting, 2, HAL_MAX_DELAY);
}

void PCALED_TIM2HandlerProc(void)
{
	static uint8_t nLedStep = 0;
	uint8_t pcsSetting[2] = { 0x09, 0x00 };  // Control LED12 to LED15 Not Increment
	// 01 01 01 01 // 00 01 01 01 // 00 00 01 01 // 00 00 00 01 // 01 00 00 00 // 01 01 00 00 // 01 01 01 00
	switch(nLedStep)
	{
	case 0:		pcsSetting[1] = 0x15;		break;
	case 1:		pcsSetting[1] = 0x05;		break;
	case 2:		pcsSetting[1] = 0x01;		break;
	case 3:		pcsSetting[1] = 0x40;		break;
	case 4:		pcsSetting[1] = 0x50;		break;
	case 5:		pcsSetting[1] = 0x54;		break;
	case 6:		pcsSetting[1] = 0x55;		break;
	default: 	pcsSetting[1] = 0x00;		break;
	}
	HAL_I2C_Master_Transmit( &hi2c1,  I2C_PCA9552_SLAVE_ADDR, pcsSetting, 2, HAL_MAX_DELAY);
	nLedStep++;
	if( nLedStep >= 7)
		nLedStep = 0;
}

uint8_t PCALED_Check_Timer5_Alive(void)
{
	if (TIM5->CR1 & TIM_CR1_CEN) {
	    // Timer is running
		return 1;
	} else {
	    // Timer is not running
		return 0;
	}
}

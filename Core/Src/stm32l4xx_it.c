/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vixConfig.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern void RS485_SetReceived485Buffer( uint8_t cRs485RecevedChar);
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern HASH_HandleTypeDef hhash;
extern I2C_HandleTypeDef hi2c1;
extern OSPI_HandleTypeDef hospi1;
extern RNG_HandleTypeDef hrng;
extern DMA_HandleTypeDef hdma_spi1_tx;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */



/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */

  /* USER CODE END EXTI2_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(IWG1_D0_D2_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */

  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line3 interrupt.
  */
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(IWG3_D1_D3_Pin);
  /* USER CODE BEGIN EXTI3_IRQn 1 */

  /* USER CODE END EXTI3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_rx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

  /* USER CODE END DMA1_Channel3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_spi1_tx);
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(IWG2_D0_B6_Pin);
  HAL_GPIO_EXTI_IRQHandler(IWG1_D1_B7_Pin);
  HAL_GPIO_EXTI_IRQHandler(IWG4_D0_B8_Pin);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */

  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */

  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_ER_IRQn 0 */

  /* USER CODE END I2C1_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
  * @brief This function handles SPI1 global interrupt.
  */
void SPI1_IRQHandler(void)
{
  /* USER CODE BEGIN SPI1_IRQn 0 */

  /* USER CODE END SPI1_IRQn 0 */
  HAL_SPI_IRQHandler(&hspi1);
  /* USER CODE BEGIN SPI1_IRQn 1 */

  /* USER CODE END SPI1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

	if( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
	{
		RS485_SetReceived485Buffer((uint8_t)(huart1.Instance->RDR & 0x00FF));
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

		__HAL_UART_CLEAR_OREFLAG(&huart1);
		__HAL_UART_CLEAR_PEFLAG(&huart1);

		return;
	}

	if( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) != RESET )
	{
        if(Rs485Tx2Ctr > 0)
  		{
			huart1.Instance->TDR = (uint8_t)(*Rs485Tx2Ptr++ & 0xFFU);
			Rs485Tx2Ctr--;
		}
		else { //  Disable the UART1 Transmit interrupt
			Tx2DmaIng = 0;
			__HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
			__HAL_UART_DISABLE_IT(&huart1, UART_IT_TC);
		}
	}

	__HAL_UART_CLEAR_OREFLAG(&huart1);
	__HAL_UART_CLEAR_PEFLAG(&huart1);

	return;

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(IWG3_D0_G11_Pin);
  HAL_GPIO_EXTI_IRQHandler(IWG4_D1_C12_Pin);
  HAL_GPIO_EXTI_IRQHandler(IWG2_D1_G15_Pin);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */

  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */

  /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	if( __HAL_UART_GET_FLAG(&huart4, UART_FLAG_RXNE) != RESET)
	{
		FW_SetReceivedFwd2Buffer((uint8_t)(huart4.Instance->RDR & 0x00FF));
		__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);

	}

	if( __HAL_UART_GET_FLAG(&huart4, UART_FLAG_TXE) != RESET )
	{
        if(FwdTx1Ctr > 0)
  		{
			huart4.Instance->TDR = (uint8_t)(*FwdTx1Ptr++ & 0xFFU);
			FwdTx1Ctr--;
		}
		else {  /* Disable the UART5 Transmit interrupt */
			 __HAL_UART_DISABLE_IT(&huart4, UART_IT_TXE);
			 __HAL_UART_DISABLE_IT(&huart4, UART_IT_TC);
		}
	}

	__HAL_UART_CLEAR_OREFLAG(&huart4);
	__HAL_UART_CLEAR_PEFLAG(&huart4);

	return;
  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC channel1 and channel2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles OCTOSPI1 global interrupt.
  */
void OCTOSPI1_IRQHandler(void)
{
  /* USER CODE BEGIN OCTOSPI1_IRQn 0 */

  /* USER CODE END OCTOSPI1_IRQn 0 */
  HAL_OSPI_IRQHandler(&hospi1);
  /* USER CODE BEGIN OCTOSPI1_IRQn 1 */

  /* USER CODE END OCTOSPI1_IRQn 1 */
}

/**
  * @brief This function handles RNG global interrupt.
  */
void RNG_IRQHandler(void)
{
  /* USER CODE BEGIN RNG_IRQn 0 */

  /* USER CODE END RNG_IRQn 0 */
  HAL_RNG_IRQHandler(&hrng);
  /* USER CODE BEGIN RNG_IRQn 1 */

  /* USER CODE END RNG_IRQn 1 */
}

/**
  * @brief This function handles HASH and CRS global interrupts.
  */
void HASH_CRS_IRQHandler(void)
{
  /* USER CODE BEGIN HASH_CRS_IRQn 0 */

  /* USER CODE END HASH_CRS_IRQn 0 */
  HAL_HASH_IRQHandler(&hhash);
  /* USER CODE BEGIN HASH_CRS_IRQn 1 */

  /* USER CODE END HASH_CRS_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void DMA1_Ch1_Spi1Rx_ErrorCallback(void)
{
#if 1
	//uint8_t buf;

	//while(READ_BIT(SPI1->SR, SPI_SR_RXNE)
	//{
	//	buf = SPI1->DR;
	//}

	//HAL_SPI_Receive_DMA(&  ,  , 5);
#else
	while(HAL_GPIO_ReadPin(  ,   ) != GPIO_PIN_SET);
	HAL_SPI_TransmitReceive_DMA( , , , );
#endif
}

void DMA1_Ch3_Spi1Tx_ErrorCallback(void)
{
	return;
}

void DMA1_Ch2_Uart1Rx_ErrorCallback(void)
{
	return;
}

void DMA1_Ch4_Uart1Tx_ErrorCallback(void)
{
	return;
}

#if 0
void DMA1_Channel3_Error_Callback(void)
{
//	LL_DMA_ClearFlag_GI3(DMA1);
//	LL_DMA_ClearFlag_TC3(DMA1);
//	LL_DMA_ClearFlag_HT3(DMA1);
//	LL_DMA_ClearFlag_TE3(DMA1);
	//LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
	//LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
	// LED Blink or Flag Set
	return;
}
void DMA1_Channel4_Error_Callback(void)
{
//	LL_DMA_ClearFlag_GI4(DMA1);
//	LL_DMA_ClearFlag_TC4(DMA1);
//	LL_DMA_ClearFlag_HT4(DMA1);
//	LL_DMA_ClearFlag_TE4(DMA1);
	return;
}
#endif

#if 0
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance == SPI1)
	{
		SPI1_Callback.RxStatus = _DONE;
		//osThreadFlagsSet( , );
		//osTrheadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
	}

	return;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
	if(hspi->Instance == SPI1)
	{
		SPI1_Callback.TxStatus = _DONE;

		osEventFlagsSet(Spi1RxCallback_EvtId, 0x00000001U);
		osThreadYield();
	}

	return;
}
#endif

#if 0
static void HAL_SPI_DMATransmitCplt(DMA_HandleTypeDef* hdma)
{
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1u)
	hspi->TxCpltCallback(hspi);
#else
	HAL_SPI_TxCpltCallback(hspi);
#endif
}
#endif

/* USER CODE END 1 */

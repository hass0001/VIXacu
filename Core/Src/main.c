/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "crc.h"
#include "dma.h"
#include "hash.h"
#include "i2c.h"
#include "iwdg.h"
#include "octospi.h"
#include "rng.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "vixConfig.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern uint32_t delay_ticks;

void MyMsDelay(uint32_t milisecons)
{
	delay_ticks = milisecons;
	while ( delay_ticks > 0) {
		// CPU ?��?�� ?��?�� ??�???
	}
}

void MyUsDelay(uint32_t microseconds)
{
  /* Go to number of cycles for system */
  microseconds *= (SystemCoreClock / 1000000);
  /* Delay till end */
  while (microseconds--);
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*
static OS_STACKPTR			int Stack0[128];
static OS_TASK				TCB0;
*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_OCTOSPI1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_CRC_Init();
  MX_HASH_Init();
  MX_RNG_Init();
  MX_TIM6_Init();
#ifndef DEBUG_MODE
  MX_IWDG_Init();
#endif
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  //MX_ADC1_Init();

	HAL_TIM_Base_Start_IT(&htim7);  // �????�� ?���??? ???���??? 초기 ?��?��

//	TEST_Eeprom();
//  INIT_Relays(_OFF);
//  INIT_TTLOut(_OFF);
//  INIT_StatusLeds(_OFF);
 // INIT_ArrayLeds(_OFF);
	fOpenEthernet = 0;
  //CREATE_DebugConsoleMutex();
#if 0
setvbuf(stdin, NULL, _IONBF, 0);
//setvbuf(stdout, NULL, _IONBF, 0);
#endif

  //printf("printf: VIXacu\r\n");
  //Dprintf("VIXacu\r\n");

  OPEN_DebugConsole();
  OPEN_ExNorFlash();
  //OPEN_ArrayLED();
  //xOpen_EepromMutex();

   // TEST_Eeprom();
 //  TEST_ExNorFlash();
   // TestLockOut();
   //TEST_RNG();
   //TEST_ExRTC();
   //TEST_LedDriver();
   //TEST_CRC32();
   //TEST_TTLOut(_TTL_OUT);
   RTC_Initial();
   RS485_Init();
   InputInit();
   SCI1FwdTaskInit();
   PCALED_Init();

   SystemTask();	// ?�� ?��?���??????? ?��?�� 값을 ?��?�� ?���??????? ?��?��?�� ?��?��.

   Wiegand_init_value();
   SerialProcInit();

   EthernetCommunication();
   OPEN_Ethernet();

   initTaskRunStatus();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//   xOSPI_EraseChip();
 //  TEST_ExNorFlash();
 //  ATE_Test_AddAuth();
#ifdef DEBUG_MODE
  ATE_Test_VixpassSchedule();
#endif
	while (1)
	{
		SerialCommandProcess();
		SCI2Rs485Process();
		WiegandProcess();

		SPI1NetTask();
		InProcess();

	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
		HAL_IWDG_Refresh(&hiwdg);
	//	  HAL_Delay(10);
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
//while(HAL_I2C_Master_Transmit(&hi2c1, addr, &data, 1, 100000) != HAL_OK);

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

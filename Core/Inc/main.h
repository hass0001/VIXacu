/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

void MyMsDelay(uint32_t milisecons);
void MyUsDelay(uint32_t microseconds);
void MyNsDelay(uint32_t nanoseconds);
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

//#define    FIRMWARE_GLOBALS 

/******************************** Peripheral define ***************************************/

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IWG1_D0_D2_Pin GPIO_PIN_2
#define IWG1_D0_D2_GPIO_Port GPIOE
#define IWG1_D0_D2_EXTI_IRQn EXTI2_IRQn
#define RUN_LED_Pin GPIO_PIN_6
#define RUN_LED_GPIO_Port GPIOE
#define FIRE_ALARM_Pin GPIO_PIN_13
#define FIRE_ALARM_GPIO_Port GPIOC
#define TTL_OUT1_Pin GPIO_PIN_14
#define TTL_OUT1_GPIO_Port GPIOC
#define TTL_OUT2_Pin GPIO_PIN_15
#define TTL_OUT2_GPIO_Port GPIOC
#define QSPI1_SO_SIO3_Pin GPIO_PIN_6
#define QSPI1_SO_SIO3_GPIO_Port GPIOF
#define QSPI1_NWP_SIO2_Pin GPIO_PIN_7
#define QSPI1_NWP_SIO2_GPIO_Port GPIOF
#define QSPI1_SI_SIO0_Pin GPIO_PIN_8
#define QSPI1_SI_SIO0_GPIO_Port GPIOF
#define QSPI1_SO_SIO1_Pin GPIO_PIN_9
#define QSPI1_SO_SIO1_GPIO_Port GPIOF
#define QSPI1_SCLK_Pin GPIO_PIN_10
#define QSPI1_SCLK_GPIO_Port GPIOF
#define ADC1_IN1_Pin GPIO_PIN_0
#define ADC1_IN1_GPIO_Port GPIOC
#define ADC1_IN2_Pin GPIO_PIN_1
#define ADC1_IN2_GPIO_Port GPIOC
#define ADC1_IN3_Pin GPIO_PIN_2
#define ADC1_IN3_GPIO_Port GPIOC
#define ADC1_IN4_Pin GPIO_PIN_3
#define ADC1_IN4_GPIO_Port GPIOC
#define ADC1_IN5_Pin GPIO_PIN_0
#define ADC1_IN5_GPIO_Port GPIOA
#define ADC1_IN6_Pin GPIO_PIN_1
#define ADC1_IN6_GPIO_Port GPIOA
#define ADC1_IN15_Pin GPIO_PIN_2
#define ADC1_IN15_GPIO_Port GPIOA
#define ADC1_IN14_Pin GPIO_PIN_3
#define ADC1_IN14_GPIO_Port GPIOA
#define ETH_NSS_Pin GPIO_PIN_4
#define ETH_NSS_GPIO_Port GPIOA
#define ETH_SCK_Pin GPIO_PIN_5
#define ETH_SCK_GPIO_Port GPIOA
#define ETH_MOSI_Pin GPIO_PIN_6
#define ETH_MOSI_GPIO_Port GPIOA
#define ADC1_IN13_Pin GPIO_PIN_7
#define ADC1_IN13_GPIO_Port GPIOA
#define ADC1_IN12_Pin GPIO_PIN_4
#define ADC1_IN12_GPIO_Port GPIOC
#define ADC1_IN8_Pin GPIO_PIN_5
#define ADC1_IN8_GPIO_Port GPIOC
#define ADC1_IN7_Pin GPIO_PIN_0
#define ADC1_IN7_GPIO_Port GPIOB
#define PWR_FAIL_Pin GPIO_PIN_1
#define PWR_FAIL_GPIO_Port GPIOB
#define LOW_BAT_Pin GPIO_PIN_2
#define LOW_BAT_GPIO_Port GPIOB
#define TAMPER_Pin GPIO_PIN_11
#define TAMPER_GPIO_Port GPIOF
#define STATE_LED_Pin GPIO_PIN_10
#define STATE_LED_GPIO_Port GPIOB
#define QSPI1_NCS_Pin GPIO_PIN_11
#define QSPI1_NCS_GPIO_Port GPIOB
#define SET_0_Pin GPIO_PIN_12
#define SET_0_GPIO_Port GPIOB
#define SET_1_Pin GPIO_PIN_13
#define SET_1_GPIO_Port GPIOB
#define SET_2_Pin GPIO_PIN_14
#define SET_2_GPIO_Port GPIOB
#define SET_3_Pin GPIO_PIN_15
#define SET_3_GPIO_Port GPIOB
#define CAN_LED_Pin GPIO_PIN_6
#define CAN_LED_GPIO_Port GPIOG
#define RS485_LED_Pin GPIO_PIN_7
#define RS485_LED_GPIO_Port GPIOG
#define ALARM_G8_Pin GPIO_PIN_8
#define ALARM_G8_GPIO_Port GPIOG
#define ALARM_C6_Pin GPIO_PIN_6
#define ALARM_C6_GPIO_Port GPIOC
#define RELAY4_Pin GPIO_PIN_7
#define RELAY4_GPIO_Port GPIOC
#define RELAY3_Pin GPIO_PIN_8
#define RELAY3_GPIO_Port GPIOC
#define RELAY2_Pin GPIO_PIN_9
#define RELAY2_GPIO_Port GPIOC
#define RELAY1_Pin GPIO_PIN_8
#define RELAY1_GPIO_Port GPIOA
#define ETH_NINT_Pin GPIO_PIN_9
#define ETH_NINT_GPIO_Port GPIOA
#define ETH_NRST_Pin GPIO_PIN_10
#define ETH_NRST_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_10
#define DBG_TX_GPIO_Port GPIOC
#define DBG_RX_Pin GPIO_PIN_11
#define DBG_RX_GPIO_Port GPIOC
#define IWG4_D1_C12_Pin GPIO_PIN_12
#define IWG4_D1_C12_GPIO_Port GPIOC
#define IWG4_D1_C12_EXTI_IRQn EXTI15_10_IRQn
#define IWG3_D1_D3_Pin GPIO_PIN_3
#define IWG3_D1_D3_GPIO_Port GPIOD
#define IWG3_D1_D3_EXTI_IRQn EXTI3_IRQn
#define EEPROM_WP_D6_Pin GPIO_PIN_6
#define EEPROM_WP_D6_GPIO_Port GPIOD
#define RS485_TX_Pin GPIO_PIN_9
#define RS485_TX_GPIO_Port GPIOG
#define RS485_RX_Pin GPIO_PIN_10
#define RS485_RX_GPIO_Port GPIOG
#define IWG3_D0_G11_Pin GPIO_PIN_11
#define IWG3_D0_G11_GPIO_Port GPIOG
#define IWG3_D0_G11_EXTI_IRQn EXTI15_10_IRQn
#define RS485_DE_Pin GPIO_PIN_12
#define RS485_DE_GPIO_Port GPIOG
#define I2C1_SDA_Pin GPIO_PIN_13
#define I2C1_SDA_GPIO_Port GPIOG
#define I2C1_SCL_Pin GPIO_PIN_14
#define I2C1_SCL_GPIO_Port GPIOG
#define IWG2_D1_G15_Pin GPIO_PIN_15
#define IWG2_D1_G15_GPIO_Port GPIOG
#define IWG2_D1_G15_EXTI_IRQn EXTI15_10_IRQn
#define ETH_MOSIB5_Pin GPIO_PIN_5
#define ETH_MOSIB5_GPIO_Port GPIOB
#define IWG2_D0_B6_Pin GPIO_PIN_6
#define IWG2_D0_B6_GPIO_Port GPIOB
#define IWG2_D0_B6_EXTI_IRQn EXTI9_5_IRQn
#define IWG1_D1_B7_Pin GPIO_PIN_7
#define IWG1_D1_B7_GPIO_Port GPIOB
#define IWG1_D1_B7_EXTI_IRQn EXTI9_5_IRQn
#define IWG4_D0_B8_Pin GPIO_PIN_8
#define IWG4_D0_B8_GPIO_Port GPIOB
#define IWG4_D0_B8_EXTI_IRQn EXTI9_5_IRQn
#define LED_RESET_Pin GPIO_PIN_9
#define LED_RESET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

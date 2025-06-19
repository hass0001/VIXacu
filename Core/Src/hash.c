/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hash.c
  * @brief   This file provides code for the configuration
  *          of the HASH instances.
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
#include "hash.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

HASH_HandleTypeDef hhash;

/* HASH init function */
void MX_HASH_Init(void)
{

  /* USER CODE BEGIN HASH_Init 0 */

  /* USER CODE END HASH_Init 0 */

  /* USER CODE BEGIN HASH_Init 1 */

  /* USER CODE END HASH_Init 1 */
  hhash.Init.DataType = HASH_DATATYPE_32B;
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN HASH_Init 2 */

  /* USER CODE END HASH_Init 2 */

}

void HAL_HASH_MspInit(HASH_HandleTypeDef* hashHandle)
{

  /* USER CODE BEGIN HASH_MspInit 0 */

  /* USER CODE END HASH_MspInit 0 */
    /* HASH clock enable */
    __HAL_RCC_HASH_CLK_ENABLE();

    /* HASH interrupt Init */
    HAL_NVIC_SetPriority(HASH_CRS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(HASH_CRS_IRQn);
  /* USER CODE BEGIN HASH_MspInit 1 */

  /* USER CODE END HASH_MspInit 1 */
}

void HAL_HASH_MspDeInit(HASH_HandleTypeDef* hashHandle)
{

  /* USER CODE BEGIN HASH_MspDeInit 0 */

  /* USER CODE END HASH_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_HASH_CLK_DISABLE();

    /* HASH interrupt Deinit */
  /* USER CODE BEGIN HASH:HASH_CRS_IRQn disable */
    /**
    * Uncomment the line below to disable the "HASH_CRS_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(HASH_CRS_IRQn); */
  /* USER CODE END HASH:HASH_CRS_IRQn disable */

  /* USER CODE BEGIN HASH_MspDeInit 1 */

  /* USER CODE END HASH_MspDeInit 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

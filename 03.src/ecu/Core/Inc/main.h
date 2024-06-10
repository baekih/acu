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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WDI_Pin GPIO_PIN_15
#define WDI_GPIO_Port GPIOA
#define LED1_ON_Pin GPIO_PIN_13
#define LED1_ON_GPIO_Port GPIOJ
#define LED2_ON_Pin GPIO_PIN_14
#define LED2_ON_GPIO_Port GPIOJ
#define DBG_RX_Pin GPIO_PIN_10
#define DBG_RX_GPIO_Port GPIOA
#define DBG_TX_Pin GPIO_PIN_9
#define DBG_TX_GPIO_Port GPIOA
#define MTR_P0_Pin GPIO_PIN_8
#define MTR_P0_GPIO_Port GPIOC
#define MTR_nBRK_Pin GPIO_PIN_6
#define MTR_nBRK_GPIO_Port GPIOC
#define MTR_PWM_Pin GPIO_PIN_7
#define MTR_PWM_GPIO_Port GPIOF
#define MTR_P2_Pin GPIO_PIN_6
#define MTR_P2_GPIO_Port GPIOF
#define MTR_CS_Pin GPIO_PIN_6
#define MTR_CS_GPIO_Port GPIOH
#define MTR_P1_Pin GPIO_PIN_0
#define MTR_P1_GPIO_Port GPIOJ
#define MTR_DIR_Pin GPIO_PIN_1
#define MTR_DIR_GPIO_Port GPIOJ

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

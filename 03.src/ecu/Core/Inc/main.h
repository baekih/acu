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
#define MTR_EN_Pin GPIO_PIN_8
#define MTR_EN_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOJ
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOJ
#define CAN1_STBY_Pin GPIO_PIN_10
#define CAN1_STBY_GPIO_Port GPIOA
#define MTR_PWM_Pin GPIO_PIN_7
#define MTR_PWM_GPIO_Port GPIOF
#define W_EN_Pin GPIO_PIN_4
#define W_EN_GPIO_Port GPIOJ
#define H3_Pin GPIO_PIN_12
#define H3_GPIO_Port GPIOD
#define V_EN_Pin GPIO_PIN_3
#define V_EN_GPIO_Port GPIOJ
#define H1_Pin GPIO_PIN_5
#define H1_GPIO_Port GPIOA
#define U_EN_Pin GPIO_PIN_2
#define U_EN_GPIO_Port GPIOJ
#define U_CTL_Pin GPIO_PIN_3
#define U_CTL_GPIO_Port GPIOA
#define H2_Pin GPIO_PIN_0
#define H2_GPIO_Port GPIOB
#define V_CTL_Pin GPIO_PIN_0
#define V_CTL_GPIO_Port GPIOJ
#define W_CTL_Pin GPIO_PIN_1
#define W_CTL_GPIO_Port GPIOJ
#define DBG_TX_Pin GPIO_PIN_14
#define DBG_TX_GPIO_Port GPIOB
#define DBG_RX_Pin GPIO_PIN_15
#define DBG_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

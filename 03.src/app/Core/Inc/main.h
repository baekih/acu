/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define BUZZER_ON_Pin GPIO_PIN_3
#define BUZZER_ON_GPIO_Port GPIOE
#define LED_ON_Pin GPIO_PIN_2
#define LED_ON_GPIO_Port GPIOE
#define MCU_LCD_SCL_Pin GPIO_PIN_8
#define MCU_LCD_SCL_GPIO_Port GPIOB
#define KEY_DOWN_Pin GPIO_PIN_12
#define KEY_DOWN_GPIO_Port GPIOC
#define WDI_Pin GPIO_PIN_15
#define WDI_GPIO_Port GPIOA
#define MCU_LCD_SDA_Pin GPIO_PIN_9
#define MCU_LCD_SDA_GPIO_Port GPIOB
#define LED_PWM_Pin GPIO_PIN_7
#define LED_PWM_GPIO_Port GPIOB
#define KEY_SEL_Pin GPIO_PIN_11
#define KEY_SEL_GPIO_Port GPIOC
#define KEY_UP_Pin GPIO_PIN_10
#define KEY_UP_GPIO_Port GPIOC
#define KEY_PWR_Pin GPIO_PIN_13
#define KEY_PWR_GPIO_Port GPIOC
#define CAN1_STBY_Pin GPIO_PIN_10
#define CAN1_STBY_GPIO_Port GPIOA
#define LCD_LR_Pin GPIO_PIN_9
#define LCD_LR_GPIO_Port GPIOA
#define LCD_UD_Pin GPIO_PIN_8
#define LCD_UD_GPIO_Port GPIOA
#define KEY_PREV_Pin GPIO_PIN_8
#define KEY_PREV_GPIO_Port GPIOC
#define PWR_HOLD_Pin GPIO_PIN_6
#define PWR_HOLD_GPIO_Port GPIOC
#define TS_INT_Pin GPIO_PIN_4
#define TS_INT_GPIO_Port GPIOA
#define LCD_STBY_Pin GPIO_PIN_2
#define LCD_STBY_GPIO_Port GPIOA
#define TS_RSTn_Pin GPIO_PIN_5
#define TS_RSTn_GPIO_Port GPIOA
#define LCD_RSTn_Pin GPIO_PIN_3
#define LCD_RSTn_GPIO_Port GPIOA
#define LCD_BKL_PWM_Pin GPIO_PIN_7
#define LCD_BKL_PWM_GPIO_Port GPIOA
#define BUZZER_PWM_Pin GPIO_PIN_0
#define BUZZER_PWM_GPIO_Port GPIOB
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

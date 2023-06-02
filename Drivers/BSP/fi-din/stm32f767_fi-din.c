/**
  ******************************************************************************
  * @file    stm32f769i_eval.c
  * @author  MCD Application Team
  * @brief   This file provides a set of firmware functions to manage LEDs, 
  *          push-buttons and COM ports available on STM32F769I-EVAL 
  *          evaluation board(MB1219) from STMicroelectronics.
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
            This driver requires the stm32f769i_eval_io.c/.h files to manage the 
            IO module resources mapped on the MFX IO expander.
            These resources are mainly LEDs, Joystick push buttons, SD detect pin, 
            USB OTG power switch/over current drive pins, Camera plug pin, Audio
            INT pin
  @endverbatim
  ******************************************************************************
  */ 

/* Dependencies
- stm32f769i_eval_io.c
- stm32f7xx_hal_cortex.c
- stm32f7xx_hal_gpio.c
- stm32f7xx_hal_uart.c
- stm32f7xx_hal_i2c.c
- stm32f7xx_hal_adc.c
EndDependencies */
    
/* Includes ------------------------------------------------------------------*/
#include <stm32f767_fi-din.h>

/**
 * @brief STM32F769I EVAL BSP Driver version number V2.1.1
   */
#define __STM32F769I_EVAL_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F769I_EVAL_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32F769I_EVAL_BSP_VERSION_SUB2   (0x01) /*!< [15:8]  sub2 version */
#define __STM32F769I_EVAL_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __STM32F769I_EVAL_BSP_VERSION         ((__STM32F769I_EVAL_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F769I_EVAL_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F769I_EVAL_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F769I_EVAL_BSP_VERSION_RC))

/**
  * @}
  */

/** @defgroup STM32F769I_EVAL_LOW_LEVEL_Private_FunctionPrototypes  STM32F769I_EVAL LOW LEVEL Private Function Prototypes
  * @{
  */

void OTM8009A_IO_Delay(uint32_t Delay);

uint32_t BSP_GetVersion(void)
{
  return __STM32F769I_EVAL_BSP_VERSION;
}

/**
  * @brief  Configures LED on GPIO and/or on MFX.
  * @param  Led: LED to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  LED1
  *            @arg  LED2
  *            @arg  LED3
  *            @arg  LED4
  * @retval None
  */

/*******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/


/*******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/


/**************************** LINK OTM8009A (Display driver) ******************/
/**
  * @brief  OTM8009A delay
  * @param  Delay: Delay in ms
  */
void OTM8009A_IO_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

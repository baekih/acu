/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : STM32TouchController.cpp
  ******************************************************************************
  * This file was created by TouchGFX Generator 4.23.0. This file is only
  * generated once! Delete this file from your project and re-generate code
  * using STM32CubeMX or change this file manually to update it.
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

/* USER CODE BEGIN STM32TouchController */

#include "eco.h"
#include <STM32TouchController.hpp>

void STM32TouchController::init()
{

}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    uint8_t xy1[TS_XY1_LEN] = {0};
    uint16_t x_cur = 0, y_cur = 0;
    static uint16_t x_prv = 0, y_prv = 0;

//    printf("%s\n",__PRETTY_FUNCTION__);

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_XY1_REG, 1, &xy1[0], TS_XY1_LEN, 1000))
    {
        printf("%d i2c read xy1 error\n",__LINE__);
        return false;
    }

    x_cur = ((((uint16_t)xy1[0])&0x70)<<4) + xy1[1];
    y_cur = ((((uint16_t)xy1[0])&0x07)<<8) + xy1[2];
    if((x_prv == x_cur) && (y_prv == y_cur)) return false;

    x_prv = x_cur;
    y_prv = y_cur;
    x = (int32_t)x_cur;
    y = (int32_t)y_cur;

    printf("ts x[%d] y[%d]\n", x_cur, y_cur);

    return true;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

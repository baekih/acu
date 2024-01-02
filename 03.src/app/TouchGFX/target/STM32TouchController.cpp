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

#include "main.h"
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include <STM32TouchController.hpp>
#include <printf.h>

#define TS2_I2C_ADR          0x55

#define TS2_CNT_REG          0x08
#define TS2_CNT_LEN          1

#define TS2_XY1_REG          0x12
#define TS2_XY1_LEN          3

extern "C" void printk(const char* pstr, ...);
extern I2C_HandleTypeDef hi2c1;

void STM32TouchController::init()
{
    printk("%s\r\n",__PRETTY_FUNCTION__);
}

bool STM32TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    static uint8_t cnt = 0, cnt_prev = 0;
    uint8_t xy1[TS2_XY1_LEN] = {0};
    uint16_t x_dat, y_dat;

//    printf("%s\n",__PRETTY_FUNCTION__);

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS2_I2C_ADR)<<1, TS2_CNT_REG, 1, &cnt, TS2_CNT_LEN, 1000))
    {
        printf("%d i2c read cnt err\n",__LINE__);
        return false;
    }

    if(cnt == cnt_prev)
    {
//        printf("%d no touch detected.\n",__LINE__);
        return false;
    }

    printf("cnt[%d:%d]\n",cnt, cnt_prev);
    cnt_prev = cnt;

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS2_I2C_ADR)<<1, TS2_XY1_REG, 1, &xy1[0], TS2_XY1_LEN, 1000))
    {
        printf("%d i2c read xy1 error\n",__LINE__);
        return false;
    }

    x_dat = ((((uint16_t)xy1[0])&0x70)<<4) + xy1[1];
    y_dat = ((((uint16_t)xy1[0])&0x07)<<8) + xy1[2];
    x = (int32_t)x_dat;
    y = (int32_t)y_dat;

    printf("xy1[%d:%d]\n", x_dat, y_dat);

    return true;
}

/* USER CODE END STM32TouchController */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

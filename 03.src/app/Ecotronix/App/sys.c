/*
 * sys.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */
#include "sys.h"
#include "nmea2k.h"
#include "images.h"
#include "printf.h"

#ifdef FI_DIN_1_0
#define  SYS_LCD_WIDTH   (480)
#else
#define  SYS_LCD_WIDTH   (800)
#endif

app_dat g_app_dat_org = {.lcd_bl = 50, .bzr_vol = 0, .rsv = 0, .chksum = 0};
app_dat g_app_dat;
#ifndef FI_DIN_1_0
ts_position pos_curr[5] ={0}, pos_prev[5]={0};
#endif

#ifndef FI_DIN_1_0
void initTS(void)
{
    uint8_t reg[4] = {0};

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(TS_RSTn_GPIO_Port, TS_RSTn_Pin, GPIO_PIN_SET);
    HAL_Delay(60);

    GPIO_InitStruct.Pin = TS_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TS_INT_GPIO_Port, &GPIO_InitStruct);

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_PID_REG, 2, &reg[0], TS_PID_LEN, 1000)) printf("%d error\n",__LINE__);
    else printf("PID[0x%02x%02x%02x%02x]\n", reg[3], reg[2],reg[1], reg[0]);

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_RES_REG, 2, &reg[0], TS_RES_LEN, 1000)) printf("%d error\n",__LINE__);
    else printf("RES[%d:%d]\n", (reg[1]<<8) + reg[0], (reg[3]<<8) + reg[2]);
}

void getTS(void)
{
    uint8_t reg[4] = {0};
    uint8_t stat = 0;

    if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_STAT_REG, 2, &stat, TS_STAT_LEN, 1000)) printf("%d error\n",__LINE__);
//    printf("[%06ld] status[0x%02x]\n", cnt, dat[0]);

    if(stat&TS_STAT_BUF_EN_MSK)
    {
        memset(&pos_curr[0], 0x00, sizeof(ts_position)*5);

        for(uint32_t i=0; i<(stat&TS_STAT_NUM_MSK); i++)
        {
            if(HAL_OK != HAL_I2C_Mem_Read(&hi2c1, (TS_I2C_ADR)<<1, TS_PTR1_REG+(i*8), 2, &reg[0], TS_PTR1_LEN, 1000)) printf("%d error\n",__LINE__);
            pos_curr[i].x = (reg[1]<<8) + reg[0];
            pos_curr[i].y = (reg[3]<<8) + reg[2];
        }

        if(memcmp(&pos_prev[0], &pos_curr[0], sizeof(ts_position)*5))
        {
            printf("TS STAT[0x%02x] XY1[%03d:%03d] XY2[%03d:%03d] XY3[%03d:%03d] XY4[%03d:%03d] XY5[%03d:%03d]\r", stat,
                   pos_curr[0].x, pos_curr[0].y, pos_curr[1].x, pos_curr[1].y, pos_curr[2].x, pos_curr[2].y,
                   pos_curr[3].x, pos_curr[3].y, pos_curr[4].x, pos_curr[4].y);
            memcpy(&pos_prev[0], &pos_curr[0], sizeof(ts_position)*5);
        }

        stat = 0;
        if(HAL_OK != HAL_I2C_Mem_Write(&hi2c1, (TS_I2C_ADR)<<1, TS_STAT_REG, 2, &stat, TS_STAT_LEN, 1000)) printf("%d error\n",__LINE__);
    }
}
#endif

uint32_t doFlashErase(void)
{
    uint32_t SectorError;
    FLASH_EraseInitTypeDef pEraseInit =
    {
        .TypeErase = TYPEERASE_SECTORS,
        .Sector = FLASH_SECTOR_3,
        .NbSectors = 1,
        .VoltageRange = VOLTAGE_RANGE_3
    };

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_ERSERR);

    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
       /* Error occurred while page erase */
       return (FLASHIF_ERASE_ERROR);
    }

    return (FLASHIF_OK);
}

uint32_t doFlashWrite(uint32_t addr, uint32_t* pdata, uint32_t len)
{
    uint32_t i = 0;

    for (i = 0; (i < len) && (addr <= (USER_FLASH_END_ADDRESS-4)); i++)
    {
      /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
         be done by word */
      if (HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *(uint32_t*)(pdata+i)) == HAL_OK)
      {
       /* Check the written value */
        if (*(uint32_t*)addr != *(uint32_t*)(pdata+i))
        {
          /* Flash content doesn't match SRAM content */
          return(FLASHIF_WRITINGCTRL_ERROR);
        }
        /* Increment FLASH destination address */
        addr += 4;
      }
      else
      {
        /* Error occurred while writing data in Flash memory */
        return (FLASHIF_WRITING_ERROR);
      }
    }

    return (FLASHIF_OK);
}

void setBuzzer(uint8_t bzr_vol)
{
    static uint8_t bzr_vol_prev = 0;

    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(100 < bzr_vol) bzr_vol = 100;
    if(bzr_vol_prev == bzr_vol) return;
    bzr_vol_prev = bzr_vol;

    sConfigOC.Pulse = (125*bzr_vol)/100;
    printf("set bzr_vol[%d] Pulse[%d]\n", bzr_vol, sConfigOC.Pulse);

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    return;
}

void setLCDBL(uint8_t lcd_bl)
{
    static uint8_t lcd_bl_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(100 < lcd_bl) lcd_bl = 100;
    if(lcd_bl_prev == lcd_bl) return;
    lcd_bl_prev = lcd_bl;

    sConfigOC.Pulse = (10000*lcd_bl)/100 - 1;
    printf("set lcd_bl[%d] Pulse[%d]\n", lcd_bl, sConfigOC.Pulse);

    HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    return;
}

void setFlashDAT(app_dat app_dat_local)
{
//    uint32_t *pval = (uint32_t*)APPLICATION_ADDRESS;
    app_dat *papp_dat = (app_dat*)APPLICATION_ADDRESS;
    if(memcmp(&app_dat_local, papp_dat, sizeof(app_dat)))
    {
        printf("Flash write run lcd_bl[%d] bzr_vol[%d]\n", app_dat_local.lcd_bl, app_dat_local.bzr_vol);
        doFlashErase();
        doFlashWrite(APPLICATION_ADDRESS, (uint32_t*)&g_app_dat, 1);
    }

    return;
}

void setLCDTestImage(uint8_t img_sel)
{
    uint16_t *pbuf = (uint16_t*)0xC0000000;

    switch(img_sel)
    {
    case LCD_TST_IMG_CHESS:
        for(uint32_t y=0; y<480; y++)
        {
            for(uint32_t x=0; x<SYS_LCD_WIDTH; x++)
            {
                if(0<=x && x<160)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
                else if(160<=x && x<320)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                }
                else if(320<=x && x<480)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
#ifndef FI_DIN_1_0
                else if(480<=x && x<640)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                }
                else if(640<=x && x<SYS_LCD_WIDTH)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*SYS_LCD_WIDTH) = 0xFFFF;
                    }
                }
#endif
            }
        }
        printf("chess\n");
        break;
    case LCD_TST_IMG_WHITE:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0xFFFF;
        printf("white\n");
        break;
    case LCD_TST_IMG_RED:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0x00F8;
        printf("red\n");
        break;
    case LCD_TST_IMG_GREEN:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0xE007;
        printf("green\n");
        break;
    case LCD_TST_IMG_BLUE:
        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++) *(pbuf+i) = 0x1F00;
        printf("blue\n");
        break;
    case LCD_TST_IMG_GRAY:
    {
        uint32_t x, y;

        for(uint32_t i=0; i<(SYS_LCD_WIDTH*480); i++)
        {
            x = i/SYS_LCD_WIDTH;
            y = i%SYS_LCD_WIDTH;

            if((x&0x00000001)==0)
            {
                if((y&0x00000001)==0) *(pbuf+i) = 0x0000;
                else                  *(pbuf+i) = 0xFFFF;
            }
            else
            {
                if((y&0x00000001)==0) *(pbuf+i) = 0xFFFF;
                else                  *(pbuf+i) = 0x0000;
            }
        }
        printf("gray\n");
        break;
    }
    case LCD_TST_IMG_DEF:
#ifdef FI_DIN_1_0
        memcpy((uint32_t*)0xC0000000, &image_compass_480x480[0], 480*480*2);
        printf("compass\n");
#else
        memcpy((uint32_t*)0xC0000000, &image_autopilot_800x480[0], 800*480*2);
        printf("autopilot\n");
#endif
        break;
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
//    printf("%s() called...\r\n",__FUNCTION__);
    CAN_RxHeaderTypeDef RxHeader;
    RxProtocol RxPacket;

    /* Get CAN1 RX message */
    if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxPacket.dat) != HAL_OK) Error_Handler();

    RxPacket.canid = RxHeader.ExtId;
    RxPacket.len = RxHeader.DLC;

    for(uint8_t i=0; i < sizeof(RxProtocol); i++)
    {
        if(osOK != osMessageQueuePut(EcoQueueNMEA2KRX1Handle, (uint8_t*)(&RxPacket) + i, 0, 0)) Error_Handler();
    }
}

#ifdef FI_DIN_1_0
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint8_t image_sel = 0;
//    printf("%s() Enter...\n",__FUNCTION__);
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY1_GPIO_Port, MCU_KEY1_Pin))
    {
        if(g_switch_bank[0] == 0) g_switch_bank[0] = 1;
        else                      g_switch_bank[0] = 0;
    }

    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY2_GPIO_Port, MCU_KEY2_Pin))
    {
        setLCDTestImage(image_sel++);
        if(LCD_TST_IMG_DEF < image_sel) image_sel = LCD_TST_IMG_CHESS;
    }
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY3_GPIO_Port, MCU_KEY3_Pin)) printf("MCU_KEY3 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY4_GPIO_Port, MCU_KEY4_Pin)) printf("MCU_KEY4 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_PWR_SW_GPIO_Port, MCU_PWR_SW_Pin)) printf("MCU_PWR pressed\n");
}
#endif

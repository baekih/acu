/*
 * sys.c
 *
 *  Created on: Nov 15, 2023
 *      Author: ihbaek
 */
#include "sys.h"
#include "images.h"
#include "printf.h"

static uint32_t getFlashSector(uint32_t Address)
{
    uint32_t sector = 0;

    if     ((Address < ADDR_FLASH_SECTOR_1)  && (Address >= ADDR_FLASH_SECTOR_0))  sector = FLASH_SECTOR_0;
    else if((Address < ADDR_FLASH_SECTOR_2)  && (Address >= ADDR_FLASH_SECTOR_1))  sector = FLASH_SECTOR_1;
    else if((Address < ADDR_FLASH_SECTOR_3)  && (Address >= ADDR_FLASH_SECTOR_2))  sector = FLASH_SECTOR_2;
    else if((Address < ADDR_FLASH_SECTOR_4)  && (Address >= ADDR_FLASH_SECTOR_3))  sector = FLASH_SECTOR_3;
    else if((Address < ADDR_FLASH_SECTOR_5)  && (Address >= ADDR_FLASH_SECTOR_4))  sector = FLASH_SECTOR_4;
    else if((Address < ADDR_FLASH_SECTOR_6)  && (Address >= ADDR_FLASH_SECTOR_5))  sector = FLASH_SECTOR_5;
    else if((Address < ADDR_FLASH_SECTOR_7)  && (Address >= ADDR_FLASH_SECTOR_6))  sector = FLASH_SECTOR_6;
    else if((Address < ADDR_FLASH_SECTOR_8)  && (Address >= ADDR_FLASH_SECTOR_7))  sector = FLASH_SECTOR_7;
    else if((Address < ADDR_FLASH_SECTOR_9)  && (Address >= ADDR_FLASH_SECTOR_8))  sector = FLASH_SECTOR_8;
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))  sector = FLASH_SECTOR_9;
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)) sector = FLASH_SECTOR_10;
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/      sector = FLASH_SECTOR_11;

    return sector;
}

uint32_t doFlashErase(void)
{
    uint32_t UserStartSector;
    uint32_t SectorError;
    FLASH_EraseInitTypeDef pEraseInit;

    /* Clear pending flags (if any) */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                           FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_ERSERR);

    /* Get the sector where start the user flash area */
    UserStartSector = getFlashSector(APPLICATION_ADDRESS);

    pEraseInit.TypeErase = TYPEERASE_SECTORS;
    pEraseInit.Sector = UserStartSector;
    pEraseInit.NbSectors = 2;
    pEraseInit.VoltageRange = VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&pEraseInit, &SectorError) != HAL_OK)
    {
       /* Error occurred while page erase */
       return FLASHIF_WRITINGCTRL_ERROR;
    }

    return FLASHIF_OK;
}

uint32_t doFlashWrite(uint32_t addr, uint64_t* pdata, uint32_t len)
{
    for (uint32_t i = 0; (i < len) && (addr <= (USER_FLASH_END_ADDRESS-8)); i++)
    {
      /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
         be done by word */
      if (HAL_FLASH_Program(TYPEPROGRAM_DOUBLEWORD, addr, *(uint64_t*)(pdata+i)) == HAL_OK)
      {
       /* Check the written value */
        if (*(uint32_t*)addr != *(uint32_t*)(pdata+i))
        {
          /* Flash content doesn't match SRAM content */
          return FLASHIF_WRITINGCTRL_ERROR;
        }
        /* Increment FLASH destination address */
        addr += 8;
      }
      else
      {
        /* Error occurred while writing data in Flash memory */
        return FLASHIF_WRITING_ERROR;
      }
    }

    return (FLASHIF_OK);
}

void setBuzzer(uint8_t bzr_vol)
{
    static uint8_t bzr_vol_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(bzr_vol_prev == bzr_vol) return;
    bzr_vol_prev = bzr_vol;

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);

    sConfigOC.Pulse = (125*(bzr_vol))/100;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    return;
}

void setLCDBL(uint8_t lcd_bl)
{
    static uint8_t lcd_bl_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(lcd_bl_prev == lcd_bl) return;
    lcd_bl_prev = lcd_bl;

    HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

    if(lcd_bl==0) sConfigOC.Pulse = 0;
    else sConfigOC.Pulse = 100*lcd_bl - 1;
//    else sConfigOC.Pulse = (0x0001<<((lcd_bl+1)/10))*8 - 1;
    printf("set lcd_bl[%d] Pulse[%d]\n", lcd_bl, sConfigOC.Pulse);
    //10 : 8192 0x2000
    // 9 : 4096 0x1000
    // 8 : 2048 0x0800
    // 7 : 1024 0x0400
    // 6 :  512 0x0200
    // 5 :  256 0x0100
    // 4 :  128 0x0080
    // 3 :   64 0x0040
    // 2 :   32 0x0020
    // 1 :   16 0x0010
    // 0 :    8 0x0008

    HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    return;
}

void setLCDTestImage(uint32_t img_sel)
{
    uint16_t *pbuf = (uint16_t*)0xC0000000;

    switch(img_sel)
    {
    case LCD_TST_IMG_CHESS:
        for(uint32_t y=0; y<480; y++)
        {
            for(uint32_t x=0; x<800; x++)
            {
                if(0<=x && x<160)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                }
                else if(160<=x && x<320)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                }
                else if(320<=x && x<480)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                }
                else if(480<=x && x<640)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                }
                else if(640<=x && x<800)
                {
                    if(0<=y && y<160)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                    else if(160<=y && y<320)
                    {
                        *(pbuf+x+y*800) = 0x0000;
                    }
                    else if(320<=y && y<480)
                    {
                        *(pbuf+x+y*800) = 0xFFFF;
                    }
                }
            }
        }
        printf("chess_800x480\n");
        break;
    case LCD_TST_IMG_WHITE:
        for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0xFFFF;
        printf("white_800x480\n");
        break;
    case LCD_TST_IMG_RED:
        for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0x00F8;
        printf("red_800x480\n");
        break;
    case LCD_TST_IMG_GREEN:
        for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0xE007;
        printf("green_800x480\n");
        break;
    case LCD_TST_IMG_BLUE:
        for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0x1F00;
        printf("blue_800x480\n");
        break;
    case LCD_TST_IMG_GRAY:
    {
        uint32_t x, y;

        for(uint32_t i=0; i<(800*480); i++)
        {
            x = i/800;
            y = i%800;

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
        printf("gray_800x480\n");
        break;
    }
    case LCD_TST_IMG_KITTEN:
        memcpy((uint8_t*)0xC0000000, &image_kitten_800x480[0], 800*480*2);
        printf("kitten_800x480\n");
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

#ifdef FI_DIN_LCD4
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
    }
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY3_GPIO_Port, MCU_KEY3_Pin)) printf("MCU_KEY3 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY4_GPIO_Port, MCU_KEY4_Pin)) printf("MCU_KEY4 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_PWR_SW_GPIO_Port, MCU_PWR_SW_Pin)) printf("MCU_PWR pressed\n");
}
#endif

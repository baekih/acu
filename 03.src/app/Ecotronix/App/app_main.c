/*
 * app.cpp
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "nmea2k.h"
#include "images.h"

#include "app_main.h"

void setBuzzer(uint8_t snd_vol)
{
    static uint8_t snd_vol_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(snd_vol_prev == snd_vol) return;
    snd_vol_prev = snd_vol;

    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);

    if(snd_vol != 0) sConfigOC.Pulse = 125 - 1;
    else             sConfigOC.Pulse = 0;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    return;
}

void setLCDBkl(uint8_t lcd_bl)
{
    static uint8_t lcd_bl_prev = 0;
    TIM_OC_InitTypeDef sConfigOC = {TIM_OCMODE_PWM1, 0, TIM_OCPOLARITY_HIGH, TIM_OCFAST_DISABLE, 0, 0};

    if(lcd_bl_prev == lcd_bl) return;
    lcd_bl_prev = lcd_bl;

    HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

    if(lcd_bl==0) sConfigOC.Pulse = 0;
    else sConfigOC.Pulse = (0x0008<<((lcd_bl+1)/10))*8 - 1;
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

void runEcoTaskMain(void *argument)
{
    uint32_t timer_sec_1 = 0;
    uint8_t  timer_pwroff = 0;
    int32_t tick = osKernelGetTickCount();

    printf("FI-DIN start...\n");

    memcpy((uint32_t*)0xC0000000, &image_kitten_800x480[0], 800*480*2);
//    memcpy((uint32_t*)0xC0000000, &image_chess_800x480[0], 800*480*2);

    /* Infinite loop */
    for(;;)
    {
        tick += 100;
        HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);

        if(tick%100 == 0)
        {
            setBuzzer(g_switch_bank[0]);
            setLCDBkl(g_lcd_bkl);
        }

        if(tick%1000 == 0)
        {
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_PWR_SW_GPIO_Port, MCU_PWR_SW_Pin))
            {
                printf("Push PWR_SW %d sec\n", timer_pwroff++);
                if(3 < timer_pwroff) NVIC_SystemReset();
            }
            else timer_pwroff = 0;

            printf("[%08ld] call Pgn126993HeartBeat()\n", ++timer_sec_1);
            Pgn126993HeartBeat();
        }

        osDelayUntil(tick);
    }
}

void runEcoTaskUART(void *argument)
{
    uint8_t buf[256];
    uint8_t buf_len;

    /* Infinite loop */
    for(;;)
    {
        memset(buf, 0, 256);
        buf_len = 0;

        if(osOK == osMessageQueueGet(EcoQueueUART1Handle, &buf[buf_len++], 0, portMAX_DELAY))
        {
            while(osOK == osMessageQueueGet(EcoQueueUART1Handle, &buf[buf_len], 0, 0)) buf_len++;

            while(HAL_BUSY == HAL_UART_Transmit_IT(&huart1, &buf[0], buf_len)) osDelay(1);

            while(huart1.TxXferCount != 0) osDelay(1);
        }
    }
}

void runEcoTaskNMEA2KRx(void *argument)
{
    RxProtocol RxPacket;
    uint32_t RxPGN;
    uint8_t  RxPF;
    uint8_t  RxDA;

    NMEA2KInit();

    for(;;)
    {
        EcoQueueWait(EcoQueueNMEA2KRX1Handle, (uint8_t*)&RxPacket, sizeof(RxProtocol));

#if 0
        printf("RxPacket canid[%ld] len[%d] dat[", RxPacket.canid, RxPacket.len);
        for(uint8_t i = 0; i<RxPacket.len - 1; i++) printf("%02x:", RxPacket.dat[i]);
        printf("%02x]\n",RxPacket.dat[RxPacket.len - 1]);
#endif

        RxPF = getRxPF(RxPacket.canid);
        if(RxPF <= 239) RxDA = getRxPS(RxPacket.canid);
        RxPGN = getRxPGN(RxPacket.canid);

        if(!IsKnownPGN(RxPGN))
        {
            printf("Unknown RxPGN[%ld] return\n", RxPGN);
            continue;
        }

        if(RxPF < 240 && (RxDA == NMEA2K_THIS_ADDR || RxDA == 255))
        {
            // PDU1 and DestAddr is matched or broadcast(255). do packet proc.
            NMEA2KProc(RxPacket);
        }
        else
        {
            // PDU2. do packet proc.
            NMEA2KProc(RxPacket);
        }
    }
}

void runEcoTaskNMEA2KTx(void *argument)
{
    TxProtocol TxPacket;
    CAN_TxHeaderTypeDef txhdr = {0, 0, CAN_ID_EXT, 0, DISABLE};
    uint8_t txdat[8];

    for(;;)
    {
        EcoQueueWait(EcoQueueNMEA2KTX1Handle, (uint8_t*)&TxPacket, sizeof(TxProtocol));

        while(0 == HAL_CAN_GetTxMailboxesFreeLevel(&hcan1))
        {
            osDelay(1);
        }

        txhdr.ExtId = TxPacket.canid;
        txhdr.DLC = TxPacket.len;
        memcpy(txdat, TxPacket.dat,TxPacket.len);

        if(HAL_OK != HAL_CAN_AddTxMessage(&hcan1, &txhdr, txdat, NULL))
        {
            osDelay(1);
        }
    }
}

static uint32_t getFlashSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_SECTOR_6;
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_SECTOR_7;
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_SECTOR_8;
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_SECTOR_9;
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_SECTOR_10;
    }
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
    {
        sector = FLASH_SECTOR_11;
    }

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
        uint16_t *pbuf = (uint16_t*)0xC0000000;
        switch(image_sel)
        {
        case 0:
            image_sel++;

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
            printf("MCU_KEY2 check pattern\n");
            break;
        case 1:
            image_sel++;
            for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0xFFFF;
            printf("MCU_KEY2 white_800x480\n");
            break;
        case 2:
            image_sel++;
            for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0x00F8;
            printf("MCU_KEY2 red_800x480\n");
            break;
        case 3:
            image_sel++;
            for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0xE007;
            printf("MCU_KEY2 green_800x480\n");
            break;
        case 4:
            image_sel++;
            for(uint32_t i=0; i<(800*480); i++) *(pbuf+i) = 0x1F00;
            printf("MCU_KEY2 blue_800x480\n");
            break;
        case 5:
            image_sel = 0;
            memcpy((uint8_t*)0xC0000000, &image_kitten_800x480[0], 800*480*2);
            printf("MCU_KEY2 image_kitten_800x480\n");
            break;
        default:
            image_sel = 0;
            printf("error\n");
        }
    }

//    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY1_GPIO_Port, MCU_KEY1_Pin)) printf("MCU_KEY1 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY2_GPIO_Port, MCU_KEY2_Pin)) printf("MCU_KEY2 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY3_GPIO_Port, MCU_KEY3_Pin)) printf("MCU_KEY3 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY4_GPIO_Port, MCU_KEY4_Pin)) printf("MCU_KEY4 pressed\n");
    if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_PWR_SW_GPIO_Port, MCU_PWR_SW_Pin)) printf("MCU_PWR pressed\n");
}

/*
 * app.cpp
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "eco.h"

void runEcoTaskMain(void *argument)
{
    uint32_t timer_sec_1 = 0;
#ifdef FEATURE_LCD4
    uint8_t  timer_pwroff = 0;
#endif
    int32_t tick = osKernelGetTickCount();

    initFlashData();

#ifdef FEATURE_LCD4
    printf("FI-DIN 1.0 start...\n");
#else
    printf("FI-DIN 1.5 start...\n");
#endif
    printf("Init app_dat[%d:%d:%d:0x%08x]\n", g_app_dat.bzr_vol, g_app_dat.lcd_bl, g_app_dat.rsv, g_app_dat.crc32);

#ifdef FEATURE_LCD5
    initTS();
#endif

#ifdef FEATURE_LCD4
    memcpy((uint32_t*)0xC0000000, &image_compass_480x480[0], 480*480*2);
#else
    memcpy((uint32_t*)0xC0000000, &image_autopilot_800x480[0], 800*480*2);
#endif

    /* Infinite loop */
    for(;;)
    {
        tick += 10;
        HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);

        if(tick%10 == 0)
        {
            //ToDo
        }

        if(tick%100 == 0)
        {

#ifdef FEATURE_LCD5
            uint16_t x, y;
            getTS(&x, &y);
#endif
            setLCDBL(g_app_dat.lcd_bl);

            if(g_switch_bank[0] != 0) setBuzzer(g_app_dat.bzr_vol);
            else                      setBuzzer(0);

            if(g_lcd_img_idx != g_switch_bank[1])
            {
                g_lcd_img_idx = g_switch_bank[1];

                switch(g_lcd_img_idx)
                {
                case 0:
                    setLCDTestImage(LCD_TST_IMG_DEF);
                    break;
                case 1:
                    setLCDTestImage(LCD_TST_IMG_WHITE);
                    break;
                case 2:
                    setLCDTestImage(LCD_TST_IMG_GRAY);
                    break;
                case 3:
                    setLCDTestImage(LCD_TST_IMG_CHESS);
                    break;
                }
            }
        }

        if(tick%1000 == 0)
        {
#ifdef FEATURE_LCD4
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PWR_GPIO_Port, KEY_PWR_Pin))
            {
                printf("Push KEY_PWR %d sec\n", timer_pwroff++);
                if(3 < timer_pwroff) NVIC_SystemReset();
            }
            else timer_pwroff = 0;
#endif
        }

        if(tick%3000 == 0)
        {
            timer_sec_1 += 3;

            printf("[%08ld] call Pgn126993HeartBeat()\n", timer_sec_1);
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

void runEcoTaskFlash(void *argument)
{
    /* Infinite loop */
    osDelay(1000);

    for(;;)
    {
        updateFlashData();
        osDelay(1);
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

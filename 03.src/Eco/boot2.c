/*
 * boot2.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#ifdef ECO_BOOT2

#include "eco.h"

uint32_t g_access_level;
uint32_t g_access_seed;
uint32_t boot_delay_time = DEFAULT_BOOT_DELAY_MS;
uint8_t g_flash_source_addr = 255;
bool    g_nvic_reset = false;

void test_proc(void)
{
    uint32_t timer_sec_1 = 0;
    int32_t tick = osKernelGetTickCount() - osKernelGetTickCount()%10;

    printf("%s() start\n",__func__);

    memcpy((uint32_t*)0xC0000000, &image_autopilot_800x480[0], 800*480*2);

    memset(&g_key_stat[0], 0x00, sizeof(key_stat)*KEY_MAX_IDX);

    /* Infinite loop */
    for(;;)
    {
        tick += 10;
        HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);

        if(tick%10 == 0)
        {
            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PWR_GPIO_Port, KEY_PWR_Pin))
            {
                if(g_key_stat[KEY_PWR_IDX].prv == false)
                {
                    g_key_stat[KEY_PWR_IDX].pnd = true;
                    printf("KEY_PWR pressed\n");
                }

                g_key_stat[KEY_PWR_IDX].prv = true;
            }
            else
            {
                g_key_stat[KEY_PWR_IDX].prv = false;
            }

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PREV_GPIO_Port, KEY_PREV_Pin))
            {
                if(g_key_stat[KEY_PREV_IDX].prv == false)
                {
                    g_key_stat[KEY_PREV_IDX].pnd = true;
                    printf("KEY_PREV pressed\n");
                }

                g_key_stat[KEY_PREV_IDX].prv = true;
            }
            else
            {
                g_key_stat[KEY_PREV_IDX].prv = false;
            }

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_SEL_GPIO_Port, KEY_SEL_Pin))
            {
                if(g_key_stat[KEY_SEL_IDX].prv == false)
                {
                    g_key_stat[KEY_SEL_IDX].pnd = true;
                    printf("KEY_SEL pressed\n");
                }

                g_key_stat[KEY_SEL_IDX].prv = true;
            }
            else
            {
                g_key_stat[KEY_SEL_IDX].prv = false;
            }

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin))
            {
                if(g_key_stat[KEY_UP_IDX].prv == false)
                {
                    g_key_stat[KEY_UP_IDX].pnd = true;
                    printf("KEY_UP pressed\n");
                }

                g_key_stat[KEY_UP_IDX].prv = true;
            }
            else
            {
                g_key_stat[KEY_UP_IDX].prv = false;
            }

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_DN_GPIO_Port, KEY_DN_Pin))
            {
                if(g_key_stat[KEY_DN_IDX].prv == false)
                {
                    g_key_stat[KEY_DN_IDX].pnd = true;
                    printf("KEY_DN pressed\n");
                }

                g_key_stat[KEY_DN_IDX].prv = true;
            }
            else
            {
                g_key_stat[KEY_DN_IDX].prv = false;
            }
        }

        if(tick%100 == 0)
        {
            {
                static uint32_t lcd_bl_prv = 0;

                if(getKeyPending(KEY_SEL_IDX))
                {
                    if(100 <= g_common_dat.lcd_bl) g_common_dat.lcd_bl = 0;
                    else                        g_common_dat.lcd_bl += 10;
                }

                if(g_common_dat.lcd_bl != lcd_bl_prv)
                {
                    lcd_bl_prv = g_common_dat.lcd_bl;
                    setLCDBL(g_common_dat.lcd_bl);
                }
            }

            {
                static uint32_t bzr_vol_prv = 0;

                if(getKeyPending(KEY_PREV_IDX))
                {
                    if(g_common_dat.bzr_vol != 0) g_common_dat.bzr_vol = 0;
                    else                       g_common_dat.bzr_vol = 100;
                }

                if(g_common_dat.bzr_vol != bzr_vol_prv)
                {
                    bzr_vol_prv = g_common_dat.bzr_vol;
                    setBuzzer(g_common_dat.bzr_vol);
                }
            }

            {
                static uint8_t lcd_img_idx_prv = 0;

                if(getKeyPending(KEY_UP_IDX)){g_lcd_img_idx == 6 ? g_lcd_img_idx = 0 : g_lcd_img_idx++;}
                if(getKeyPending(KEY_DN_IDX)){g_lcd_img_idx == 0 ? g_lcd_img_idx = 6 : g_lcd_img_idx--;}

                if(g_lcd_img_idx != lcd_img_idx_prv)
                {
                    lcd_img_idx_prv = g_lcd_img_idx;
                    setLCDTestImage(g_lcd_img_idx);
                }
            }
        }

        if(tick%1000 == 0)
        {
        }

        if(tick%3000 == 0)
        {
            timer_sec_1 += 3;

            printf("[%08ld] call Pgn126993HeartBeat()\n", timer_sec_1);
//            NMEA2000_126993_heartbeat();
        }

        osDelayUntil(tick);
    }
}

void runEcoTaskMain(void *argument)
{
    initFlashData();

    test_proc();

    for(;;)
    {
        osDelay(1000);
    }
}

void runEcoTaskKey(void *argument)
{
    uint8_t  timer_pwroff = 0;

    for(;;)
    {
//        printf("%s():%d\n",__func__,__LINE__);

        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_PWR_GPIO_Port, KEY_PWR_Pin))
        {
            printf("Push KEY_PWR %d sec\n", timer_pwroff++);
            if(3 < timer_pwroff) NVIC_SystemReset();
        }
        else timer_pwroff = 0;

        osDelay(1000);
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
    osDelay(1000);
    /* Infinite loop */
    for(;;)
    {
        updateFlashData();
        osDelay(1);
    }
}

void runEcoTaskNMEA2KRx(void *argument)
{
#if 0
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
#else
    int remain = 0;

    for(;;)
    {

        if(rxCanLastIndex >= rxCanFirstIndex)
        {
            remain =  rxCanLastIndex - rxCanFirstIndex;
        }
        else
        {
            remain = (CAN_RX_BUF_MAX - rxCanFirstIndex);
            remain += rxCanLastIndex;
        }

        if(remain != 0){
            NMEA2000_ReceiveParseMessages(g_RxCan[rxCanFirstIndex].canid, g_RxCan[rxCanFirstIndex].dat, g_RxCan[rxCanFirstIndex].len);

            rxCanFirstIndex++;
            rxCanFirstIndex %= CAN_RX_BUF_MAX;
        }
    }

#endif
}

void runEcoTaskNMEA2KTx(void *argument)
{
#if 0
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
#else
    for(;;)
    {
        osDelay(1);
    }

#endif
}

#endif // ECO_BOOT2

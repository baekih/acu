/*
 * app.cpp
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "eco.h"

/* variables -----------------------------------------------------------------*/
rudder g_rudder = {
    .instance           = 0,
    .direction_order    = 0x0,
    .angle_order        = N2K_DATA_NOT_AVAILABLE_INT16,
    .position           = N2K_DATA_NOT_AVAILABLE_INT16
};

ship_status g_ship = {
    .curr =
    {
            .heading_sensor_reading = 0
    },
    .prev =
    {
            .heading_sensor_reading = 0
    }
};

void runEcoTaskMain(void *argument)
{
    int32_t tick;

    initFlashData();
    NMEA2000_Open();

    HAL_GPIO_WritePin(LED_RG_CTL_GPIO_Port, LED_RG_CTL_Pin, GPIO_PIN_SET);

    tick = osKernelGetTickCount() - osKernelGetTickCount()%10;

    for(;;)
    {
        chkN2KLastAddrClaimTime();

        if(tick%60000 == 0)
        {
            NMEA2000_126993_heartbeat();
            printf("[%08ld]NMEA2000_126993_heartbeat() called \n", tick);
        }

        tick += 10;
        osDelayUntil(tick);
    }
}

void runEcoTaskTGFX(void *argument)
{
    touchgfx_taskEntry();
}

void runEcoTaskKey(void *argument)
{
    uint8_t  timer_pwroff = 0;

    for(;;)
    {
//        printf("%s():%d\n",__func__,__LINE__);

        if(GPIO_PIN_SET == HAL_GPIO_ReadPin(PWR_ON_GPIO_Port, PWR_ON_Pin))
        {
            printf("Push KEY_PWR %d sec\n", timer_pwroff++);
            if(5 < timer_pwroff) NVIC_SystemReset();
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

void runEcoTaskSync(void *argument)
{
    /* Infinite loop */
    for(;;)
    {
        if(memcmp(&g_ship.curr, &g_ship.prev, sizeof(ship_param)))
        {
            if(isValidDegreeAngle(g_ship.curr.heading_sensor_reading))
            {
                setHDGValue((double)g_ship.curr.heading_sensor_reading );
            }

            setVariation((double)g_ship.curr.magnetic_variation);

            if(g_ship.curr.speed.direction == 0 || g_ship.curr.speed.direction == 1)
            {
                if(isValidSpeed(g_ship.curr.speed.over_ground))
                {
                    setSOGValue( ((double)g_ship.curr.speed.over_ground / 100.0), SPEED_UNIT_MPS );
                }

                if(isValidSpeed(g_ship.curr.speed.through_water))
                {
                    setSTWValue( ((double)g_ship.curr.speed.through_water / 100.0), SPEED_UNIT_MPS );
                }
            }

            if(isValidLongInteger((long)g_ship.curr.water_depth))
            {
                if(((short)g_ship.curr.transducer_offset) <= 32764 && ((short)g_ship.curr.transducer_offset) >= -32764)
                {
                    setDepthMeterValue(((double)g_ship.curr.water_depth / 100.0) + (double)((short)g_ship.curr.transducer_offset) / 1000.0 );
                }
                else
                {
                    setDepthMeterValue(((double)g_ship.curr.water_depth / 100.0));
                }
            }

            setPositionRapid((double)g_ship.curr.position.latitude/10000000.0, (double)g_ship.curr.position.longitude/10000000.0);

            if(g_ship.curr.course.cog_reference == 0 || g_ship.curr.course.cog_reference == 1)
            {
                if(isValidSpeed(g_ship.curr.speed.over_ground))
                {
                    setSOGValue( ((double)g_ship.curr.speed.over_ground / 100.0), SPEED_UNIT_MPS);
                }
            }

            setXTE((double)g_ship.curr.xte.val/100.0, (unsigned char)g_ship.curr.xte.mode);

            setWindValue(g_ship.curr.wind.speed, g_ship.curr.wind.direction, g_ship.curr.wind.reference);


            g_ship.prev = g_ship.curr;
        }

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
    for(;;)
    {
        runN2KCANRXBuffer();
        osDelay(1);
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

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
        .rudder = {.cur = 0.0, .tgt = 0.0},
    },
    .prev =
    {
        .rudder = {.cur = 0.0, .tgt = 0.0},
    }
};

boat_status g_boat = {
    .heading_sensor_reading_em4 = N2K_DATA_NOT_AVAILABLE_UINT16,
    .heading_target_em4  = N2K_DATA_NOT_AVAILABLE_UINT16,
};

void printFuzzyControlTable(void)
{
    printf("fuzzy control table\r\n");
    printf("       E|");
    for(float hdgerr=-6.0; hdgerr <= 6.0; hdgerr += 0.5)
    {
        printf("%6.2f ", hdgerr);
        osDelay(1);
    }
    printf("\r\n\r\n");

    for(float roterr_deg=-6.0; roterr_deg <= 6.0; roterr_deg += 0.5)
    {
        printf("dE%6.2f|", roterr_deg);
        for(float hdgerr_deg=-6.0; hdgerr_deg <= 6.0; hdgerr_deg += 0.5)
        {
            printf("%6.2f ", calFuzzy(hdgerr_deg, roterr_deg));
            osDelay(5);
        }
        printf("\r\n");
    }
}

int64_t roundRADem4toDEG(float val)
{
    return (int64_t)lround((float)val/10000.0*RAD2DEG);
}

int64_t roundDEGtoRADem4(float val)
{
    return (int64_t)lround((float)val*10000.0*DEG2RAD);
}

void syncShipState(void)
{
    if(isValidDegreeAngle(g_ship.curr.course.over_ground))
    {
        setCOGValue((double)g_ship.curr.course.over_ground);
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

//    printf("rudcurValue[%f:%d]\n", ((float)g_ship.curr.rudder.cur)/10000.0, g_ship.curr.rudder.cur);
    setRUDcurValue(((float)g_ship.curr.rudder.cur)/10000.0);

    setRUDtgtValue(((float)g_ship.curr.rudder.tgt)/10000.0);

    g_ship.prev = g_ship.curr;

    return;
}

void runEcoTaskMain(void *argument)
{
    int32_t tick;

    initFlashData();
    NMEA2000_Open();

    tick = osKernelGetTickCount() - osKernelGetTickCount()%10;

    for(;;)
    {
        chkN2KLastAddrClaimTime();

        if(tick%1000 == 0)
        {
//            printf("heading_sensor_target[%d]\n",g_ship.curr.heading_sensor_target);

#if 0
            printf("rud inst[%03d] ang_pos:order[%03.2f:%03.2f]\n",
                   g_rudder.instance,
                   ((double)g_rudder.position)*180.0/M_PI/10000.0,
                   ((double)g_rudder.angle_order)*180.0/M_PI/10000.0);
#endif
        }

        if(tick%60000 == 0)
        {
            NMEA2000_126993_heartbeat();
//            printf("[%08ld]NMEA2000_126993_heartbeat() called \n", tick);
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

    HAL_GPIO_WritePin(LED_RG_CTL_GPIO_Port, LED_RG_CTL_Pin, GPIO_PIN_SET);

    setBuzzer(100);
    osDelay(100);
    setBuzzer(0);

    for(;;)
    {
//        printf("%s():%d\n",__func__,__LINE__);

        if(GPIO_PIN_SET == HAL_GPIO_ReadPin(PWR_ON_GPIO_Port, PWR_ON_Pin))
        {
            printf("Push KEY_PWR %d sec\n", timer_pwroff++);
            if(5 <= timer_pwroff)
            {
                setBuzzer(100);
                osDelay(500);
                setBuzzer(0);

                NVIC_SystemReset();
            }
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
    syncShipState();

    /* Infinite loop */
    for(;;)
    {
        if(memcmp(&g_ship.curr, &g_ship.prev, sizeof(ship_param)))
        {
            syncShipState();
        }

        osDelay(1);
    }
}

void runEcoTaskControl(void *argument)
{
    uint32_t tick_tgt= osKernelGetTickCount();
    uint32_t cnt=0;
    float rud_tgt_deg;

    osDelay(200);

//    printFuzzyControlTable();

    for(;;)
    {
        float hdgt_cur = ((float)g_boat.heading_sensor_reading_em4)/10000.0;
        float hdgt_tgt = ((float)g_boat.heading_target_em4)/10000.0;
        float rot_cur  = ((float)g_ship.curr.rate_of_turn)/32000000.0;
        float hdgt_cur_deg = hdgt_cur*RAD2DEG;
        float hdgt_tgt_deg = hdgt_tgt*RAD2DEG;
        float rot_cur_deg = rot_cur*RAD2DEG;
        float hdgt_err = hdgt_tgt - hdgt_cur;
        float rot_err = 0.0 - rot_cur;

        if(180.0*DEG2RAD < fabsf(hdgt_err)) hdgt_err += 360.0*DEG2RAD;

#if defined CTRL_FUZZY
        rud_tgt_deg = calFuzzy(RUD_FUZZY_HDG_ADJ*hdgt_err*RAD2DEG, RUD_FUZZY_ROT_ADJ*rot_err*RAD2DEG);
#elif defined CTRL_PID
        rud_tgt_deg = RAD2DEG * calPID(hdgt_err, rot_err);
#elif defined CTRL_TEST
        rud_ctrl_tgt_deg = 15.0*sinf(2.0*M_PI*((float)cnt)*0.01);
#else
#error "RUD_CTRL_??? must be defined."
#endif

        g_ship.curr.rudder.tgt = (int16_t)round(rud_tgt_deg*DEG2RAD*10000.0);
        PGN127245_ProcessNameField();

        printf("%s() hdgt_cur[%03.1f] hdgt_tgt[%03.1f] hdgt_err[%03.1f] rot[%03.1f] rud[%03.1f]\n",__FUNCTION__, hdgt_cur_deg, hdgt_tgt_deg, hdgt_err*RAD2DEG, rot_cur_deg, rud_tgt_deg);

//        printf("%s() rud_ctrl_tgt_deg[%5.1f]deg \r\n",__FUNCTION__, rud_ctrl_tgt_deg);

        osDelayUntil(tick_tgt);
        tick_tgt += 1000;
        cnt++;
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

/*
 * ecu.cpp
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "eco.h"

uint32_t g_access_level;
uint32_t g_access_seed;
uint32_t boot_delay_time = DEFAULT_BOOT_DELAY_MS;
uint8_t g_flash_source_addr = 255;

void runEcoTaskMain(void *argument)
{
    uint32_t cnt = 0;
    uint32_t vol_adc = 0;


    initMotor();

    for(;;)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 10);
        vol_adc = (HAL_ADC_GetValue(&hadc1)*101)/0xfff;
        vol_adc == 101 ? vol_adc = 100 : vol_adc;
        setMotor((uint8_t)vol_adc);

        printf("[%06d]ECU MTR_DRV8323_DRV_CTRL[0x%x]\r\n", ++cnt, readMotor(MTR_DRV8323_DRV_CTRL));
        printf("vol_adc[%ld]\r\n", vol_adc);

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

void runEcoTaskNMEA2KRx(void *argument)
{
    RxProtocol RxPacket;
    uint32_t RxPGN;
    uint8_t  RxPF;
    uint8_t  RxDA;

    initNMEA2K();

    for(;;)
    {
        EcoQueueWait(EcoQueueNMEA2KRX1Handle, (uint8_t*)&RxPacket, sizeof(RxProtocol));

#if ECO_DEBUG
        printf("RxPacket canid[%ld] len[%d] dat[0xullx]", RxPacket.canid, RxPacket.len, RxPacket.dat);
#endif

        RxPF = getRxPF(RxPacket.canid);
        if(RxPF <= 239) RxDA = getRxPS(RxPacket.canid);
        RxPGN = getRxPGN(RxPacket.canid);

        if(!isKnownPGN(RxPGN))
        {
            printf("Unknown RxPGN[%ld] return\n", RxPGN);
            continue;
        }

        if(RxPF <= 239 && (RxDA == NMEA2K_THIS_ADDR || RxDA == 255))
        {
            // PDU1 and DestAddr is matched or broadcast(255). do packet proc.
//            printf("%s():%d RxDA[%d]\n",__FUNCTION__,__LINE__, RxDA);
            opNMEA2K(RxPacket);
        }
        else
        {
            // PDU2. do packet proc.
//            printf("%s():%d\n",__FUNCTION__,__LINE__);
            opNMEA2K(RxPacket);
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

        while(0 == HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)) osDelay(1);

        txhdr.ExtId = TxPacket.canid;
        txhdr.DLC = TxPacket.len;
        memcpy(txdat, TxPacket.dat,TxPacket.len);

        if(HAL_OK != HAL_CAN_AddTxMessage(&hcan1, &txhdr, txdat, NULL)) osDelay(1);
    }
}

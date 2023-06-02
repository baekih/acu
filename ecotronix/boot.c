/*
 * app.cpp
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "boot.h"
#include "nmea2k.h"

void runEcoTask01(void *argument)
{
    uint32_t timer_sec_1 = 0;
    uint32_t *ptimer_sec_1;
    uint8_t print_buf[32] = {0,};
    ptimer_sec_1 = &timer_sec_1;
//    ptimer_sec_1 = (uint32_t*)0xC0000000;
    *ptimer_sec_1 = 0;
    int32_t tick = osKernelGetTickCount();
    /* Infinite loop */
    for(;;)
    {
        tick += 100;
        HAL_GPIO_TogglePin(WDI_GPIO_Port, WDI_Pin);
        if(tick%1000 == 0)
        {
            sprintf((char*)print_buf, "%07ld:\n", ++(*ptimer_sec_1));
//            sprintf((char*)print_buf, "%07ld:\n", ++timer_sec_1);
            printf("%s", print_buf);
            HAL_UART_Transmit_IT(&huart2, print_buf, strlen((char*)print_buf));
//            Pgn126993HeartBeat();
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
//	printf("%s() Enter...\n",__FUNCTION__);
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY1_GPIO_Port, MCU_KEY1_Pin)) printf("MCU_KEY1 pressed\n");
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY2_GPIO_Port, MCU_KEY2_Pin)) printf("MCU_KEY2 pressed\n");
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY3_GPIO_Port, MCU_KEY3_Pin)) printf("MCU_KEY3 pressed\n");
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_KEY4_GPIO_Port, MCU_KEY4_Pin)) printf("MCU_KEY4 pressed\n");
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(MCU_PWR_SW_GPIO_Port, MCU_PWR_SW_Pin)) printf("MCU_PWR pressed\n");
}

/*
 * common.c
 *
 *  Created on: Mar 30, 2023
 *      Author: ihbaek
 */

#include "eco.h"


void _putchar(char ch)
{
    if(ch == '\n') osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)"\r", 0, 0);

    osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)&ch, 0, 0);
}

void EcoQueuePut(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len)
{
    for(uint8_t i=0; i<len; i++)
    {
        osMessageQueuePut(handle, pbuf + i, 0, 0);
    }
}

void EcoQueueWait(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len)
{
    while(osOK != osMessageQueueGet(handle, pbuf, 0, portMAX_DELAY));

    for(uint8_t i = 1; i < len; i++)
    {
        if(osOK != osMessageQueueGet(handle, pbuf + i, 0, 0))
        {
            Error_Handler();
        }
    }
}

uint32_t chkCRC32(app_dat* papp_dat)
{
     if(papp_dat->crc32 == HAL_CRC_Calculate(&hcrc, (uint32_t *)papp_dat, sizeof(app_dat)/sizeof(uint32_t) - 1))
     {
         return true;
     }
     else
     {
         return false;
     }

}

portBASE_TYPE IdleTaskHook(void* p)
{
    if ((int)p) //idle task sched out
    {
    }
    else //idle task sched in
    {
    }
    return pdTRUE;
}



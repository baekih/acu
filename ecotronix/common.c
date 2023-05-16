/*
 * common.c
 *
 *  Created on: Mar 30, 2023
 *      Author: ihbaek
 */

#include "common.h"

int __io_putchar(int ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  if(ch == '\n') osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)"\r", 0, 0);
  osMessageQueuePut(EcoQueueUART1Handle, (uint8_t*)&ch, 0, 0);

  return ch;
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

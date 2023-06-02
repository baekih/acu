/*
 * common.h
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_COMMON_H_
#define APPLICATION_USER_ECOTRONIX_INC_COMMON_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "main.h"
#include "printf.h"
#include "stm32f767_fi-din_lcd.h"
#include "stm32f767_fi-din_sdram.h"

#pragma pack(push,1)
typedef struct __RxProtocol
{
    uint32_t canid;
    uint8_t  dat[8];
    uint8_t  len;
} RxProtocol ;

typedef struct __TxProtocol
{
    uint32_t canid;
    uint8_t  dat[8];
    uint8_t  len;
} TxProtocol ;
#pragma pack(pop)

extern UART_HandleTypeDef huart1, huart2;
extern CAN_HandleTypeDef hcan1;
extern QSPI_HandleTypeDef hqspi;
extern osMessageQueueId_t EcoQueueUART1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KRX1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KTX1Handle;
extern const osMessageQueueAttr_t EcoQueueNMEA2KRX1_attributes;

void EcoQueuePut(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);
void EcoQueueWait(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);
void EcoQSPIInit(void);


#endif /* APPLICATION_USER_ECOTRONIX_INC_COMMON_H_ */

/*
 * common.h
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_COMMON_H_
#define APPLICATION_USER_ECOTRONIX_INC_COMMON_H_

#include "eco.h"

#define HW_VERSION                  01
#define BOOT_VERSION                01
#define APP_VERSION                 01

typedef struct _version
{
    uint8_t hw[7];
    uint8_t boot[7];
    uint8_t app[7];
} version  __attribute__((aligned(1)));

extern osMessageQueueId_t EcoQueueUART1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KRX1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KTX1Handle;
extern const osMessageQueueAttr_t EcoQueueNMEA2KRX1_attributes;

extern version g_ver;

void EcoQueuePut(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);
void EcoQueueWait(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);

#endif /* APPLICATION_USER_ECOTRONIX_INC_COMMON_H_ */

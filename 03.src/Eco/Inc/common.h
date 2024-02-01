/*
 * common.h
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_COMMON_H_
#define APPLICATION_USER_ECOTRONIX_INC_COMMON_H_

#include "eco.h"

#if 0
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
#endif

extern osMessageQueueId_t EcoQueueUART1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KRX1Handle;
extern osMessageQueueId_t EcoQueueNMEA2KTX1Handle;
extern const osMessageQueueAttr_t EcoQueueNMEA2KRX1_attributes;

void EcoQueuePut(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);
void EcoQueueWait(osMessageQueueId_t handle, uint8_t* pbuf, uint8_t len);

#endif /* APPLICATION_USER_ECOTRONIX_INC_COMMON_H_ */

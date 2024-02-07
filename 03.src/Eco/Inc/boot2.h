/*
 * boot2.h
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_
#define APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_

#define DEFAULT_BOOT_DELAY_MS     (2000)
#define BOOT_STATE                1

#define TX_TIMEOUT                ((uint32_t)100)
#define RX_TIMEOUT                HAL_MAX_DELAY

#define UPLOAD_STATE_FLASH_OK               0
#define UPLOAD_STATE_FLASH_ING              1

extern uint32_t g_access_level;
extern uint32_t g_access_seed;
extern uint32_t boot_delay_time;
extern uint8_t g_flash_source_addr;
extern bool g_nvic_reset;

void runEcoTaskDefault(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);

#endif /* APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_ */

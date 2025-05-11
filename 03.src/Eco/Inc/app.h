/*
 * app.h
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_APP_H_
#define APPLICATION_USER_ECOTRONIX_INC_APP_H_

extern uint8_t g_rudder_instance;
extern uint8_t g_rudder_direction_order;
extern int16_t g_rudder_angle_order;
extern int16_t g_rudder_position;

void runEcoTaskDefault(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);
void touchgfx_taskEntry(void);

#endif /* APPLICATION_USER_ECOTRONIX_INC_APP_H_ */

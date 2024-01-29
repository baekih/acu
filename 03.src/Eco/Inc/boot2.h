/*
 * boot2.h
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_
#define APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_

void runEcoTaskDefault(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);

#endif /* APPLICATION_USER_ECOTRONIX_INC_BOOT2_H_ */

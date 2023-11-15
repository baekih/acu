/*
 * boot.h
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_APP_H_
#define APPLICATION_USER_ECOTRONIX_INC_APP_H_

void runEcoTaskDefault(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);

#endif /* APPLICATION_USER_ECOTRONIX_INC_APP_H_ */

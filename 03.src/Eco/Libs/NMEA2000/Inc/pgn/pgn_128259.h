/*
 * pgn_128259.h
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_128259_H_
#define NMEA2000_INC_PGN_PGN_128259_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN128259_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN128259_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN128259_ProcessNameField(NmeaPgn* pgnId);

#endif /* NMEA2000_INC_PGN_PGN_128259_H_ */

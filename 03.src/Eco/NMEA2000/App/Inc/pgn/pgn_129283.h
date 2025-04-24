/*
 * pgn_129283.h
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129283_H_
#define NMEA2000_INC_PGN_PGN_129283_H_


/* Includes ------------------------------------------------------------------*/
#include "eco.h"

/* Private define ------------------------------------------------------------*/
#define PGN129283_PRIORITY                  3

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129283_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129283_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129283_H_ */

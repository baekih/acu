/*
 * pgn_129026.h
 *
 *  Created on: 2025. 4. 21.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129026_H_
#define NMEA2000_INC_PGN_PGN_129026_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN129026_PRIORITY                  2

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129026_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129026_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129026_H_ */

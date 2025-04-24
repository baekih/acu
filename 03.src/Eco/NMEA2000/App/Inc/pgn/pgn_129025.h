/*
 * pgn_129025.h
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129025_H_
#define NMEA2000_INC_PGN_PGN_129025_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN129025_PRIORITY                  2

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129025_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129025_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129025_H_ */

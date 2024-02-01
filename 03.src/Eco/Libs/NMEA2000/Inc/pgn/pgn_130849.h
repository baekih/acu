/*
 * pgn_130849.h
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_130849_H_
#define NMEA2000_INC_PGN_PGN_130849_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN130849_PGN                       130849
#define PGN130849_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/
extern uint8_t pgn130849_datalen[4];

/* Exported functions --------------------------------------------------------*/
void PGN130849_ProcessNameField(NmeaPgn* pgnId);

#endif /* NMEA2000_INC_PGN_PGN_130849_H_ */

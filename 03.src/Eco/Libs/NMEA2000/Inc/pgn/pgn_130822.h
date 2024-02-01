/*
 * pgn_130822.h
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_130822_H_
#define NMEA2000_INC_PGN_PGN_130822_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN130822_PGN                       130822
#define PGN130822_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN130822_ProcessNameField(NmeaPgn* pgnId);

#endif /* NMEA2000_INC_PGN_PGN_130822_H_ */

/*
 * pgn_130823.h
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_130823_H_
#define NMEA2000_INC_PGN_PGN_130823_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN130823_PGN                       130823
#define PGN130823_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN130823_ProcessNameField(NmeaPgn* pgnId);

#endif /* NMEA2000_INC_PGN_PGN_130823_H_ */

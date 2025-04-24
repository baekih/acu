/*
 * pgn_127258.h
 *
 *  Created on: 2025. 4. 22.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_127258_H_
#define NMEA2000_INC_PGN_PGN_127258_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN127258_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN127258_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN127258_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_127258_H_ */

/*
 * pgn_127251.h
 *
 *  Created on: 2025. 7. 24.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_127251_H_
#define NMEA2000_INC_PGN_PGN_127251_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN127251_PGN                       127251
#define PGN127251_PRIORITY                  2

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN127251_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_127251_H_ */

/*
 * pgn_128267.h
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_128267_H_
#define NMEA2000_INC_PGN_PGN_128267_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN128267_PGN                       128267
#define PGN128267_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN128267_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN128267_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_128267_H_ */

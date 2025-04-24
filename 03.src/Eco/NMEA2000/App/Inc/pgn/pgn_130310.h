/*
 * pgn_130310.h
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_130310_H_
#define NMEA2000_INC_PGN_PGN_130310_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN130310_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN130310_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN130310_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_130310_H_ */

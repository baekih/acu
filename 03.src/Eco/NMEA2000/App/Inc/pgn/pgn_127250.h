/*
 * pgn_127250.h
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_127250_H_
#define NMEA2000_INC_PGN_PGN_127250_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN127250_PGN                       127250
#define PGN127250_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN127250_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN127250_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_127250_H_ */

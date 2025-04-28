/*
 * pgn_129285.h
 *
 *  Created on: 2025. 4. 25.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129285_H_
#define NMEA2000_INC_PGN_PGN_129285_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN129285_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129285_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129285_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129284_H_ */

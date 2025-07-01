/*
 * pgn_129284.h
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129284_H_
#define NMEA2000_INC_PGN_PGN_129284_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN129284_PGN                       129284
#define PGN129284_PRIORITY                  3

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129284_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129284_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129284_H_ */

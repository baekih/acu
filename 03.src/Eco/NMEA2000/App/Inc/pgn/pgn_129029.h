/*
 * pgn_129029.h
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_129029_H_
#define NMEA2000_INC_PGN_PGN_129029_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN129029_PGN                       129029
#define PGN129029_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129029_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129029_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129029_H_ */

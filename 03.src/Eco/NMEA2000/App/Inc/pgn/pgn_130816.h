/*
 * pgn_130816.h
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_130816_H_
#define NMEA2000_INC_PGN_PGN_130816_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN130816_PGN                       130816
#define PGN130816_PRIORITY                  7

/* Private typedef -----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN130816_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_130816_H_ */

/*
 * pgn_65287.h
 *
 *  Created on: 2022. 7. 8.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_065287_H_
#define NMEA2000_INC_PGN_PGN_065287_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN065287_PGN                     65287
#define PGN065287_PRIORITY                7

#define PGN065287_FIELD_MIN               3
#define PGN065287_FIELD_MAX               5

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

bool isFECPPGN(uint8_t *pbuf);
bool isAirmarPPGN(uint8_t *pbuf);

void PGN065287_FEC_GetFieldValue(NmeaPgn* pgnId, uint8_t*, uint8_t);
void PGN065287_Airmar_GetFieldValue(NmeaPgn* pgnId, uint8_t *pbuf, uint8_t len);

void PGN065287_FEC_SendNameField(void);
void PGN065287_Airmar_SendNameField(NmeaPgn* pgnId);

uint32_t calculate_level_2_key (uint32_t seed, uint32_t mask, uint8_t iterations);

#endif /* NMEA2000_INC_PGN_PGN_065287_H_ */

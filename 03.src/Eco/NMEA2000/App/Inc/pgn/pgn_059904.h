/*
 * pgn_059904.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_059904_H_
#define NMEA2000_INC_PGN_PGN_059904_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN059904_PGN                       59904
#define PGN059904_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN059904NAME
{
    uint32_t mPGN_being_requested;      // 24 bits
} PGN059904NAME;

/* Exported functions --------------------------------------------------------*/

void PGN059904_SetFieldValue(uint32_t _PGN);
void PGN059904_SendNameField(NmeaPgn* pgnId);
void PGN059904_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
uint32_t PGN059904_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_059904_H_ */

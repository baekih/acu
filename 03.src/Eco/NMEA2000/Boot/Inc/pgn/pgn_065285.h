/*
 * pgn_065285.h
 *
 *  Created on: 2022. 4. 22.
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_065285_H_
#define NMEA2000_INC_PGN_PGN_065285_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define PGN065285_NUM         65285
#define PGN065285_PRIORITY    7

/* typedef -----------------------------------------------------------*/
typedef struct __PGN065285NAME
{
	uint32_t mManufacturer_Code;	// 11 bits
	uint8_t mNMEA_Reserved_1;		// 2  bits
	uint8_t mIndustry_Group;		// 3  bits
	uint8_t mBoot_State;			// 3  bits
	uint64_t mNMEA_Reserved_2;		// 45 bits
} PGN065285NAME;


/* functions --------------------------------------------------------*/
void PGN065285_SetFieldValue(uint16_t _Manufacturer_Code,
                             uint8_t _NMEA_Reserved_1,
                             uint8_t _Industry_Group,
                             uint8_t _Boot_State,
                             uint64_t _NMEA_Reserved_2);
void PGN065285_SetInitialField();
void PGN065285_SendNameField();

#endif /* NMEA2000_INC_PGN_PGN_065285_H_ */

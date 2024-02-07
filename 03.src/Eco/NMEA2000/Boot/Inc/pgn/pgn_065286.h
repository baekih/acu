/*
 * pgn_065286.h
 *
 *  Created on: 2022. 4. 22.
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_065286_H_
#define NMEA2000_INC_PGN_PGN_065286_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define PGN065286_NUM         65286
#define PGN065286_PRIORITY    6

/* typedef -----------------------------------------------------------*/
typedef struct __PGN065286NAME
{
	uint32_t mManufacturer_Code;	// 11 bits
	uint32_t mNMEA_Reserved_1;		// 2  bits
	uint32_t mIndustry_Group;		// 3  bits
	uint32_t mNMEA_Reserved_2;		// 48  bits
} PGN065286NAME;


/* functions --------------------------------------------------------*/
void PGN065286_GetFieldValue(uint32_t pgnid, uint8_t len, uint8_t *buf);
uint32_t PGN065286_ProcessNameField(uint32_t pgnid, uint8_t len, uint8_t *buf);

#endif /* NMEA2000_INC_PGN_PGN_065286_H_ */

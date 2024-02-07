/*
 * pgn_126208_request.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126208_REQUEST_H_
#define NMEA2000_INC_PGN_PGN_126208_REQUEST_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* typedef -----------------------------------------------------------*/
typedef struct __PGN126208REQUESTNAME
{
	uint32_t mRequest_Group_Function_Code;						// 8  bits
	uint32_t mRequested_PGN;									// 24 bits
	uint32_t mTransmission_interval;							// 32 bits
	uint32_t mTransmission_interval_offset;						// 16 bits
	uint32_t mNumber_of_Pairs_of_Request_Parameters_to_follow;	// 8  bits
} PGN126208REQUESTNAME;

/* functions --------------------------------------------------------*/
void PGN126208REQUEST_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);
void PGN126208REQUEST_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);

#endif /* NMEA2000_INC_PGN_PGN_126208_REQUEST_H_ */

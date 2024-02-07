/*
 * pgn_059392.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_059392_H_
#define NMEA2000_INC_PGN_PGN_059392_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define POSITIVIE_ACKNOWLEDGMENT      0x00
#define NEGATIVIE_ACKNOWLEDGMENT      0x01
#define PGN_SUPPORTED_ACCESS_DENIED   0x02

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN059392NAME
{
	uint32_t mControl_Byte;						// 8  bits
	uint32_t mGroup_Function_Value;				// 8  bits
	uint32_t mNMEA_Reserved;					// 24 bits
	uint32_t mPGN_of_Requested_Information;		// 24 bits
} PGN059392NAME;

/* Exported functions --------------------------------------------------------*/
void PGN059392_SetFieldValue(uint32_t _Control_Byte,
                             uint32_t _Group_Function_Value,
                             uint32_t _NMEA_Reserved,
                             uint32_t _PGN_of_Requested_Information);
void PGN059392_SendNameField();

#endif /* NMEA2000_INC_PGN_PGN_059392_H_ */

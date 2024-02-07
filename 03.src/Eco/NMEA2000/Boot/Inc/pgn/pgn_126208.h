/*
 * pgn_126208.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126208_H_
#define NMEA2000_INC_PGN_PGN_126208_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define FUNCTION_CODE_REQEUST_MESSAGE       0
#define FUNCTION_CODE_COMMAND_MESSAGE       1
#define FUNCTION_CODE_ACKNOWLEDGE_MESSAGE   2
#define FUNCTION_CODE_READ_FIELDS           3
#define FUNCTION_CODE_READ_FIELDS_REPLY     4
#define FUNCTION_CODE_WRITE_FIELDS          5
#define FUNCTION_CODE_WRITE_FIELDS_REPLY    6

/* typedef -----------------------------------------------------------*/
typedef struct __PGN126208NAME
{
	uint32_t mGroup_Function_Code;	// 8  bits
} PGN126208NAME;

/* functions --------------------------------------------------------*/
void PGN126208_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);
void PGN126208_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);

#endif /* NMEA2000_INC_PGN_PGN_126208_H_ */

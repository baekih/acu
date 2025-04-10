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

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN065286NAME
{
    uint32_t mManufacturer_Code;    // 11 bits
    uint32_t mNMEA_Reserved_1;      // 2  bits
    uint32_t mIndustry_Group;       // 3  bits
    uint32_t mNMEA_Reserved_2;      // 48  bits
} PGN065286NAME;


/* Exported functions --------------------------------------------------------*/
extern void PGN065286_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
extern void PGN065286_ProcessNameField();


#endif /* NMEA2000_INC_PGN_PGN_065286_H_ */

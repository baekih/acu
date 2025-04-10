/*
 * pgn_126998.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126998_H_
#define NMEA2000_INC_PGN_PGN_126998_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern uint8_t  installationDescription1[72];
extern uint8_t  installationDescription2[72];
extern uint8_t  menufacturerDescription[72];

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN126998NAME
{
    uint8_t _InstallationDescription1[72];          // n  bits
    uint8_t _InstallationDescription2[72];          // n  bits
    uint8_t _MenufacturerDescription[72];           // n  bits
} PGN126998NAME;

/* Exported functions --------------------------------------------------------*/
void PGN126998_SetFieldValue();
void PGN126998_ProcessNameField(NmeaPgn* pgnId, uint32_t messagetype);


#endif /* NMEA2000_INC_PGN_PGN_126998_H_ */

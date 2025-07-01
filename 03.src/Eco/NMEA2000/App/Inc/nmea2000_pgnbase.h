/*
 * nmea2000_pgnbase.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_PGNBASE_H_
#define NMEA2000_INC_NMEA2000_PGNBASE_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#pragma pack(push,1)
typedef struct _NmeaPgn_
{
    uint32_t mCanNumericID;
    uint32_t mPGN;

    uint8_t mPriority;
    uint8_t mEDP;
    uint8_t mDP;
    uint8_t mPF;
    uint8_t mPS;
    uint8_t mSA;
} NmeaPgn;
#pragma pack(pop)

NmeaPgn* CanIDToNmeaPGN(uint32_t rawCanId);

NmeaPgn* getRcvNmeaPgn();
NmeaPgn* getCanId(uint8_t priority, uint32_t pgn, uint8_t destAddr, uint8_t sourceAddr);

#endif /* NMEA2000_INC_NMEA2000_PGNBASE_H_ */

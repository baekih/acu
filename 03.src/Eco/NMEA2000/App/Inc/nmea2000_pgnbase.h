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

    uint32_t mPriority;
    uint32_t mEDP;
    uint32_t mPGN;
    uint32_t mDP;
    uint32_t mPF;
    uint32_t mPS;
    uint32_t mSA;
} NmeaPgn;
#pragma pack(pop)

NmeaPgn* CanIDToNmeaPGN(uint32_t rawCanId);

NmeaPgn* getRcvNmeaPgn();
NmeaPgn* getCanId(uint32_t priority, uint32_t pgn, uint32_t destAddr, uint32_t sourceAddr);

#endif /* NMEA2000_INC_NMEA2000_PGNBASE_H_ */

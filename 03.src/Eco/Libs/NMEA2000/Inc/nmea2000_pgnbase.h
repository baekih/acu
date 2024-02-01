/*
 * nmea2000_pgnbase.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_PGNBASE_H_
#define NMEA2000_INC_NMEA2000_PGNBASE_H_


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

NmeaPgn* CanIDToNmeaPGN(uint32_t rawCanId);

NmeaPgn* getRcvNmeaPgn();
NmeaPgn* getCanId(uint32_t priority, uint32_t pgn, uint32_t destAddr, uint32_t sourceAddr);


#endif /* NMEA2000_INC_NMEA2000_PGNBASE_H_ */

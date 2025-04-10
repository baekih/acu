/*
 * pgn_128259.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "math.h"

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_128259.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

uint16_t    SpeedWaterRefereced;        // 16 bit
uint16_t    SpeedGroundRefereced;       // 16 bit
uint8_t     SpeedWaterReferecedType;    // 8  bit
uint8_t     SpeedDirection;             // 4  bit

void PGN128259_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{

//  SequenceID      		 =  GetBuf_1ByteUInt(len, 0, buf);              // 8  bit
    SpeedWaterRefereced      =  GetBuf_2ByteUInt(len, 1, buf);              // 16 bit
    SpeedGroundRefereced     =  GetBuf_2ByteUInt(len, 3, buf);              // 16 bit
    SpeedWaterReferecedType  =  GetBuf_1ByteUInt(len, 5, buf);              // 8  bit
    SpeedDirection      	 =  GetBuf_1ByteUInt(len, 6, buf) & 0x0F;       // 4  bit

/*  printf("Receive Speed Water Reference : speedWaterRefereced-%3.2f m/s: SpeedGroundRefereced-%3.2f m/s: SpeedWaterReferecedType-%d: SpeedDirection-%d\n",
            (float)SpeedWaterRefereced/100.0, (float)SpeedGroundRefereced/100.0,
            SpeedWaterReferecedType, SpeedDirection);*/

    if (SpeedDirection == 0 || SpeedDirection == 1) {
        if(isValidSpeed(SpeedGroundRefereced)) {
            setSOGValue( ((double)SpeedGroundRefereced / 100.0), SPEED_UNIT_MPS );
        }
        if(isValidSpeed(SpeedWaterRefereced)) {
            setSTWValue( ((double)SpeedWaterRefereced / 100.0), SPEED_UNIT_MPS );
        }
    }
}

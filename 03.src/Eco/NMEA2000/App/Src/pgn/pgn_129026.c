/*
 * pgn_129026.c
 *
 *  Created on: 2025. 4. 21.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "math.h"

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_129026.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

uint8_t     COGReference;               // 2  bit
uint16_t    CourceOverGround;           // 16 bit
uint16_t    SpeedOverGround;            // 16 bit

void PGN129026_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{

//  SequenceID                =  GetBuf_1ByteUInt(len, 0, buf);              // 8  bit
    COGReference            =  GetBuf_1ByteUInt(len, 1, buf) & 0x03;       // 8  bit
    CourceOverGround        =  GetBuf_2ByteUInt(len, 2, buf);              // 16 bit
    SpeedOverGround         =  GetBuf_2ByteUInt(len, 4, buf);              // 16 bit

//    printf("Receive 129026:COG[%3.1f]deg SOG[%3.2f]knot COGRef[%d]\n", (float)CourceOverGround / 10000.0 * 360.0 / (2.0 * (M_PI)),
//    (float)SpeedOverGround / 100.0 * 1.944, COGReference);
//    printf("Receive 129026:COG[%ld] SOG[%ld]m/s COGRef[%d]\n", CourceOverGround, SpeedOverGround, COGReference);

    if (COGReference == 0 || COGReference == 1)
    {
        if(isValidSpeed(SpeedOverGround))
        {
            setSOGValue( ((double)SpeedOverGround / 100.0), SPEED_UNIT_MPS );
        }
    }
}

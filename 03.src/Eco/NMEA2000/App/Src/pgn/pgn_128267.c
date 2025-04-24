/*
 * pgn_128267.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_128267.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>


void PGN128267_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{

//    uint8_t  SequenceID          =  GetBuf_1ByteUInt(len, 0, buf);               // 8  bit
    uint32_t WaterDepth          =  GetBuf_4ByteUInt(len, 1, buf);               // 32 bit
    uint16_t Offset              =  GetBuf_2ByteUInt(len, 5, buf);               // 16 bit
//    uint8_t  MaximumDepthRange   =  GetBuf_1ByteUInt(len, 7, buf);               // 8  bit

/*  printf("Receive Water Depth - %d:%f:%2.3f:%d\n",
        (double)((long)(WaterDepth & 0xFFFFFFFFL))/100.0, (double)((short)Offset)/1000.0,
        MaximumDepthRange * 10);*/

    long depth = (long) (WaterDepth & 0xFFFFFFFFL);

    if (isValidLongInteger(depth)) {
        double offset = 0;

        if (((short) Offset) <= 32764 && ((short) Offset) >= -32764) {
            offset = (double) ((short) Offset) / 1000.0;
        }

        setDepthMeterValue( ((float)depth / 100) + offset );
    }
}

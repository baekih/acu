/*
 * pgn_129025.c
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_129025.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

void PGN129025_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    int32_t latitude       =  GetBuf_4ByteUInt(len, 0, buf);   // 32 bit
    int32_t longitude      =  GetBuf_4ByteUInt(len, 4, buf);   // 32 bit

//    printf("Receive 129025:Latitude[%ld]deg Longitude[%ld]deg\n", latitude, longitude);

    setPositionRapid((double)latitude/10000000.0, (double)longitude/10000000.0);
}

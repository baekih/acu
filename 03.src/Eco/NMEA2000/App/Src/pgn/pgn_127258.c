/*
 * pgn_127258.c
 *
 *  Created on: 2025. 4. 22.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_127258.h"

#include <gui/common/validate_data.h>
#include <gui/common/DataBase.hpp>


void PGN127258_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  SequenceID         =  GetBuf_1ByteUInt(len, 0, buf);                    // 8  bits
//    uint16_t VariationSource    = (GetBuf_1ByteUInt(len, 1, buf) & 0x0F);            // 4  bits
//    uint16_t AgeofService       =  GetBuf_2ByteUInt(len, 2, buf);                    // 16 bits
    int16_t  Variation          =  GetBuf_2ByteUInt(len, 4, buf);                    // 16 bits
//    uint16_t NMEAReserved       =  GetBuf_2ByteUInt(len, 6, buf);                    // 16 bits

//    printf("PGN127258[%d] VarSrc[%d] Variation[%3.1f]deg\n", SequenceID, VariationSource, (float)Variation / 10000.0 * 180.0 / (M_PI) );

    setVariation((double)Variation);
}

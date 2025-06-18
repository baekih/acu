/*
 * pgn_130306.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN130306_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  mSequenceID     =  GetBuf_1ByteUInt(len, 0, buf);        // 8  bit
    uint16_t mWindSpeed      =  GetBuf_2ByteUInt(len, 1, buf);          // 16 bit
    uint16_t mWindDirection  =  GetBuf_2ByteUInt(len, 3, buf);          // 16 bit
    uint8_t  mWindReference  =  GetBuf_1ByteUInt(len, 5, buf) & 0x07;   // 3 bit

/*  printf("Receive Wind Data - %f:%f:%d\r\n",
                (double)mWindSpeed/100, (double)mWindDirection/10000, mWindReference);*/

    setWindValue(mWindSpeed, mWindDirection, mWindReference);
}

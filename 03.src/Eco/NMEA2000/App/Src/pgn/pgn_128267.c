/*
 * pgn_128267.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN128267_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  SequenceID          =  GetBuf_1ByteUInt(len, 0, buf);               // 8  bit
    g_ship.curr.water_depth          =  GetBuf_4ByteUInt(len, 1, buf);               // 32 bit
    g_ship.curr.transducer_offset    =  GetBuf_2ByteUInt(len, 5, buf);               // 16 bit
//    uint8_t  MaximumDepthRange   =  GetBuf_1ByteUInt(len, 7, buf);               // 8  bit

/*  printf("Receive Water Depth - %d:%f:%2.3f:%d\n",
        (double)((long)(WaterDepth & 0xFFFFFFFFL))/100.0, (double)((short)Offset)/1000.0,
        MaximumDepthRange * 10);*/
}

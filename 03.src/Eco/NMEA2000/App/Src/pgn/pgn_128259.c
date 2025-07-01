/*
 * pgn_128259.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN128259_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  SequenceID                 =  GetBuf_1ByteUInt(len, 0, buf);              // 8  bit
    g_ship.curr.speed.through_water          = GetBuf_2ByteUInt(len, 1, buf);              // 16 bit
    g_ship.curr.speed.over_ground            = GetBuf_2ByteUInt(len, 3, buf);              // 16 bit
    g_ship.curr.speed.water_reference_type   = GetBuf_1ByteUInt(len, 5, buf);              // 8  bit
    g_ship.curr.speed.direction              = GetBuf_1ByteUInt(len, 6, buf) & 0x0F;       // 4  bit

/*  printf("Receive Speed Water Reference : speedWaterRefereced-%3.2f m/s: SpeedGroundRefereced-%3.2f m/s: SpeedWaterReferecedType-%d: SpeedDirection-%d\n",
            (float)SpeedWaterRefereced/100.0, (float)SpeedGroundRefereced/100.0,
            SpeedWaterReferecedType, SpeedDirection);*/
}

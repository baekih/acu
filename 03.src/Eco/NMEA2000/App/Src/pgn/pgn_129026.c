/*
 * pgn_129026.c
 *
 *  Created on: 2025. 4. 21.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN129026_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  SequenceID              =  GetBuf_1ByteUInt(len, 0, buf);              // 8  bit
    g_ship.curr.course.cog_reference = GetBuf_1ByteUInt(len, 1, buf) & 0x03;       // 8  bit
    g_ship.curr.course.over_ground   =  GetBuf_2ByteUInt(len, 2, buf);              // 16 bit
    g_ship.curr.speed.over_ground = GetBuf_2ByteUInt(len, 4, buf);              // 16 bit

//    printf("Receive 129026:COG[%3.1f]deg SOG[%3.2f]knot COGRef[%d]\n", (float)CourceOverGround / 10000.0 * 360.0 / (2.0 * (M_PI)),
//    (float)SpeedOverGround / 100.0 * 1.944, COGReference);
//    printf("Receive 129026:COG[%ld] SOG[%ld]m/s COGRef[%d]\n", CourceOverGround, SpeedOverGround, COGReference);
}

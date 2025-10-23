/*
 * pgn_127251.c
 *
 *  Created on: 2025. 7. 24.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN127251_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t     SequenceID             = GetBuf_1ByteUInt(len, 0, buf);                    // 8  bits
    g_boat.rate_of_turn = GetBuf_4ByteUInt(len, 1, buf);                    // 16 bits
//    uint8_t     NMEAReserved           = GetBuf_3ByteUInt(len, 5, buf);       // 6  bits

//    printf("SID[%d]\r\n", pgnId->mSA);
//    printf("rate_of_turn[%6.4f]deg/sec\r\n", ((float)g_ship.curr.rate_of_turn)*180.0/M_PI * 0.000001 / 32);
}

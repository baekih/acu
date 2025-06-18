/*
 * pgn_129025.c
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN129025_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_ship.curr.position.latitude  = GetBuf_4ByteUInt(len, 0, buf);   // 32 bit
    g_ship.curr.position.longitude = GetBuf_4ByteUInt(len, 4, buf);   // 32 bit

//    printf("Receive 129025:Latitude[%ld]deg Longitude[%ld]deg\n", latitude, longitude);

}

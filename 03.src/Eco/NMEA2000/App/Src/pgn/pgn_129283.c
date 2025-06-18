/*
 * pgn_129283.c
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN129283_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  sequence       =  GetBuf_1ByteUInt(len, 0, buf);               // 8 bit
    uint8_t  xte_mode       = (GetBuf_1ByteUInt(len, 1, buf) & 0x0F);       // 4 bit
//    uint8_t  nav_terminate  = (GetBuf_1ByteUInt(len, 1, buf) & 0xC0) >> 6;  // 2 bit
    int32_t  xte            =  GetBuf_4ByteUInt(len, 2, buf);               // 32 bit

//    printf("Receive 129283:xte[%ld]m mode[%d] nav_terminated[%d]\n", xte/100, xte_mode, nav_terminate);

    setXTE((double)xte/100.0, (unsigned char)xte_mode);
}

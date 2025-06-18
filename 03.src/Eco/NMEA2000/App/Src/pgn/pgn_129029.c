/*
 * pgn_129029.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN129029_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  mSequenceID     =  GetBuf_1ByteUInt(len, 0, buf);               // 8  bit
//    uint16_t mPositionDate   =  GetBuf_2ByteUInt(len, 1, buf);               // 16 bit
//    uint16_t mPositionTime   =  GetBuf_4ByteUInt(len, 3, buf);               // 32 bit
//    g_ship_status.curr.position.latitude    = (int64_t)GetBuf_8ByteUInt(len, 7, buf);              // 64 bit
//    g_ship_status.curr.position.longitude   = (int64_t)GetBuf_8ByteUInt(len, 15, buf);             // 64 bit
//    int64_t  mAltitude       =  (int64_t)GetBuf_8ByteUInt(len, 23, buf);             // 64 bit
//    uint8_t  mTypeOfSystem   =  GetBuf_1ByteUInt(len, 31, buf) & 0x0F;       // 4  bit
    g_ship.curr.position.gnss_method = (GetBuf_1ByteUInt(len, 31, buf) & 0xF0) >> 4;// 4  bit
//  uint8_t  mIntergrity        =  GetBuf_1ByteUInt(len, 32, buf) & 0x03;       // 2  bit
//  uint8_t  mNMEAReserved    =  (GetBuf_1ByteUInt(len, 32, buf) & 0xFC) >> 2;// 6  bit
//  uint8_t  mNumberofSVs     =  GetBuf_1ByteUInt(len, 33, buf);              // 8  bit
//  uint16_t mHDOP            =  GetBuf_2ByteUInt(len, 34, buf);              // 16 bit
//  uint16_t mPDOP            =  GetBuf_2ByteUInt(len, 36, buf);              // 16 bit

//  printf("Receive GNSS Position Data - %04d:%f:%f:%f:%d:%d:%d:%d:%f:%f \n",
//              mPositionTime%10000,
//              (double)mLatitude/10000000000000000L, (double)mLongitude/10000000000000000L,
//              (double)mAltitude/1000000, mTypeOfSystem, mMethodGNSS, mIntergrity,
//              mNumberofSVs, (double)mHDOP / 100, (double)mPDOP / 100);
}

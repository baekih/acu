/*
 * pgn_129284.c
 *
 *  Created on: 2025. 4. 24.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_129284.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

void PGN129284_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t  sequenceid             =  GetBuf_1ByteUInt(len,  0, buf);               // 8 bit
    uint32_t dist_to_dest_wpt       =  GetBuf_4ByteUInt(len,  1, buf);               // 32 bit
//    uint8_t  course_bearing_ref     = (GetBuf_1ByteUInt(len,  5, buf) & 0x03) >> 0;  // 2 bit
//    uint8_t  perpendicular_crossed  = (GetBuf_1ByteUInt(len,  5, buf) & 0x0C) >> 2;  // 2 bit
//    uint8_t  arrival_circle_entered = (GetBuf_1ByteUInt(len,  5, buf) & 0x30) >> 4;  // 2 bit
//    uint8_t  calculation_type       = (GetBuf_1ByteUInt(len,  5, buf) & 0xC0) >> 6;  // 2 bit
//    uint32_t eta_time               =  GetBuf_4ByteUInt(len,  6, buf);               // 32 bit
//    uint16_t eta_date               =  GetBuf_2ByteUInt(len, 10, buf);               // 16 bit
    uint16_t bearing_org_to_dst_wpt =  GetBuf_2ByteUInt(len, 12, buf);               // 16 bit
    uint16_t bearing_pos_to_dst_wpt =  GetBuf_2ByteUInt(len, 14, buf);               // 16 bit
//    uint32_t org_wpt_num            =  GetBuf_4ByteUInt(len, 16, buf);               // 32 bit
//    uint32_t dst_wpt_num            =  GetBuf_4ByteUInt(len, 20, buf);               // 32 bit
    int32_t  dst_wpt_latitude       =  GetBuf_4ByteUInt(len, 24, buf);               // 32 bit
    int32_t  dst_wpt_longitude      =  GetBuf_4ByteUInt(len, 28, buf);               // 32 bit
    int16_t  wpt_closing_velocity   =  GetBuf_2ByteUInt(len, 32, buf);               // 16 bit

#if 0
    printf("RX:129284\n");
    printf("  dist_to_dest_wpt[%ld]m\n", dist_to_dest_wpt/100);
//    printf("  course_bearing_ref[%d]\n", course_bearing_ref);
//    printf("  perpendicular_crossed[%d]\n", perpendicular_crossed);
//    printf("  arrival_circle_entered[%d]\n", arrival_circle_entered);
//    printf("  calculation_type[%d]\n", calculation_type);
//    printf("  eta_time[%ld]\n", eta_time);
//    printf("  eta_date[%d]\n", eta_date);
    printf("  bearing_org_to_dst_wpt[%ld]deg\n", ((uint32_t)bearing_org_to_dst_wpt)*360000/6283/10000);
    printf("  bearing_pos_to_dst_wpt[%ld]deg\n", ((uint32_t)bearing_pos_to_dst_wpt)*360000/6283/10000);
    printf("  dst_wpt_latitude[%ld]\n", dst_wpt_latitude);
    printf("  dst_wpt_longitude[%ld]\n", dst_wpt_longitude);
    printf("  wpt_closing_velocity[%3.1f]m/s\n", (float)wpt_closing_velocity/100.0);
#endif
}

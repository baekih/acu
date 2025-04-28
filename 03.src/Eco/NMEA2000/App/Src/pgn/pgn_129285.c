/*
 * pgn_129285.c
 *
 *  Created on: 2025. 4. 25.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_129285.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

#pragma pack(push,1)
typedef struct __route
{
    uint16_t id;
    uint8_t  name[31];
    uint8_t  name_len;
    uint8_t  name_code;
} s_route;

typedef struct __waypoint
{
    uint16_t id;
    uint8_t  name[31];
    uint8_t  name_len;
    uint8_t  name_code;
    int32_t  latitude;
    int32_t  longitude;
} s_waypoint;
#pragma pack(pop)

s_route    route;
s_waypoint wpt[3] = {{0,},{0,},{0,}};

uint8_t route_name[31] = {0,};

void PGN129285_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint16_t start_rps              =  GetBuf_2ByteUInt(len,  0, buf);              // 16 bit
    uint16_t nitems                 =  GetBuf_2ByteUInt(len,  2, buf);              // 16 bit
//    uint16_t database_id            =  GetBuf_2ByteUInt(len,  4, buf);              // 16 bit
    route.id                        =  GetBuf_2ByteUInt(len,  6, buf);              // 16 bit
    uint8_t  nav_dir_in_route       = (GetBuf_1ByteUInt(len,  8, buf) & 0x07) >> 0; // 3  bit
//    uint8_t  sup_route_wp_dat_avail = (GetBuf_1ByteUInt(len,  8, buf) & 0x18) >> 3; // 2  bit

    route.name_len                  =  GetBuf_1ByteUInt(len,  9, buf);              // 8  bit
    route.name_code                 =  GetBuf_1ByteUInt(len, 10, buf);              // 8  bit
    memcpy(route.name, buf+11, route.name_len-2);                               // route.name_len byte

    uint32_t buf_ptr                 = 11 + (route.name_len-2) + 1;
    if(3 < nitems) nitems = 3;
    for (uint32_t i=0; i<nitems; i++)
    {
        wpt[i].id          =  GetBuf_2ByteUInt(len, buf_ptr, buf);  buf_ptr += 2;                      // 16  bit
        wpt[i].name_len    =  GetBuf_1ByteUInt(len, buf_ptr, buf);  buf_ptr += 1;                      // 8   bit
        wpt[i].name_code   =  GetBuf_1ByteUInt(len, buf_ptr, buf);  buf_ptr += 1;                      // 8   bit
        memcpy(&wpt[i].name[0], buf+buf_ptr, wpt[i].name_len-2);    buf_ptr += (wpt[i].name_len-2);    // wpt[i].name_len byte
        wpt[i].latitude    =  GetBuf_4ByteUInt(len, buf_ptr, buf);  buf_ptr += 4;                      // 32  bit
        wpt[i].longitude   =  GetBuf_4ByteUInt(len, buf_ptr, buf);  buf_ptr += 4;                      // 32  bit
    }

#if 1
    printf("RX:129285\n");
//    printf("  start_rps[%d]\n", start_rps);
    printf("  nitems[%d] dir[%d]\n", nitems, nav_dir_in_route);
//    printf("  database_id[%d]\n", database_id);
//    printf("  route_id[%d]\n", route_id);
//    printf("  nav_dir_in_route[%d]\n", nav_dir_in_route);
//    printf("  sup_route_wp_dat_avail[%d]\n", sup_route_wp_dat_avail);
    printf("  route id[%d] len[%d] code[%d] name[%s]\n", route.id, route.name_len, route.name_code, route.name);
    for (uint32_t i=0; i<2; i++)
    {
        printf("  wpt[%ld] id[%d] len[%d] code[%d] name[%s] lat:log[%ld:%ld]\n",
               i, wpt[i].id, wpt[i].name_len, wpt[i].name_code, wpt[i].name, wpt[i].latitude, wpt[i].longitude);
    }
//    printf("  wp  id[%d] len[%d] code[%d] str[%s] lat:log[%ld:%ld]\n", wp_id, wp_name_len, wp_name_code, wp_name, wp_latitude, wp_longitude);
//    printf("  wp2 id[%d] len[%d] code[%d] str[%s] lat:log[%ld:%ld]\n", wp2_id, wp2_name_len, wp2_name_code, wp2_name, wp2_latitude, wp2_longitude);
//    printf("  bearing_org_to_dst_wpt[%ld]deg\n", ((uint32_t)bearing_org_to_dst_wpt)*360000/6283/10000);
//    printf("  bearing_pos_to_dst_wpt[%ld]deg\n", ((uint32_t)bearing_pos_to_dst_wpt)*360000/6283/10000);
//    printf("  dst_wpt_latitude[%ld]\n", dst_wpt_latitude);
//    printf("  dst_wpt_longitude[%ld]\n", dst_wpt_longitude);
//    printf("  wpt_closing_velocity[%3.1f]m/s\n", (float)wpt_closing_velocity/100.0);
#endif
}

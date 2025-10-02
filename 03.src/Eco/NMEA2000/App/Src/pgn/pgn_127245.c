/*
 * pgn_127245.c
 *
 *  Created on: 2025. 5. 7.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN127245_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    uint8_t     rud_instance        =  GetBuf_1ByteUInt(len, 0, buf);                    // 8  bits
//    uint8_t     rud_direction_order = (GetBuf_1ByteUInt(len, 1, buf) && 0x03) >> 0;      // 8  bits
    int16_t     rud_position        =  GetBuf_2ByteUInt(len, 4, buf);                    // 16 bits

#if 0
    if(rud_instance != 252)
    {
        printf("rud inst[%03d] pos[%04.1f]deg\n",
               rud_instance,
               ((float)rud_position)*RAD2DEG/10000.0);
    }
#endif
    if(g_rudder.instance != rud_instance) return;
//    g_rudder.direction_order = rud_direction_order;
//    g_rudder.angle_order = rud_angle_order;
    g_rudder.position = rud_position;

    g_ship.curr.rudder.cur = g_rudder.position;
}

void PGN127245_ProcessNameField(void)
{
    InitializeSendNameField();

    g_rudder.angle_order = g_ship.curr.rudder.tgt;

    Add1ByteUInt(g_rudder.instance);
    Add1ByteUInt(g_rudder.direction_order & 0xF8);
    Add2ByteUInt(g_rudder.angle_order);
    Add2ByteUInt(g_rudder.position);
    Add2ByteUInt(0xFFFF);

    NMEA2000_SendParseMessages(getCanId(PGN127245_PRIORITY, PGN127245_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr), sendPacketLength, sendNMEAPackets, 0);
}

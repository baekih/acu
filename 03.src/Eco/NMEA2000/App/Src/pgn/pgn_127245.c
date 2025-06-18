/*
 * pgn_127245.c
 *
 *  Created on: 2025. 5. 7.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_127245.h"

void PGN127245_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    uint8_t     rud_instance        =  GetBuf_1ByteUInt(len, 0, buf);                    // 8  bits
    uint8_t     rud_direction_order = (GetBuf_1ByteUInt(len, 1, buf) && 0x03) >> 0;      // 8  bits
//    int16_t     rud_angle_order     =  GetBuf_2ByteUInt(len, 2, buf);                    // 16 bits
    int16_t     rud_position        =  GetBuf_2ByteUInt(len, 4, buf);                    // 16 bits

/*
    printf("rudder inst[%d] dir_order[%d] ang_order:pos[%3.2f:%3.2f]deg\n",
           rud_instance, rud_direction_order,
           ((double)rud_angle_order)*180.0/M_PI/10000.0,
           ((double)rud_position)*180.0/M_PI/10000.0);
*/
    if(g_rudder.instance != rud_instance) return;
    if(rud_direction_order == 0x03)
    g_rudder.position = rud_position;
}

void PGN127245_ProcessNameField(NmeaPgn* pgnId)
{
    InitializeSendNameField();

    Add1ByteUInt(g_rudder.instance);
    Add1ByteUInt(g_rudder.direction_order);
    Add2ByteUInt(g_rudder.angle_order);
    Add2ByteUInt(g_rudder.position);
    Add2ByteUInt(0xFFFF);

    NMEA2000_SendParseMessages(getCanId(PGN127245_PRIORITY, PGN127245_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr), sendPacketLength, sendNMEAPackets, 0);
}

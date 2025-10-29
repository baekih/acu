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
    uint8_t     rud_direction_order = (GetBuf_1ByteUInt(len, 1, buf) && 0x03) >> 0;      // 8  bits
    int16_t     rud_position        =  GetBuf_2ByteUInt(len, 4, buf);                    // 16 bits

#if 0
    if(rud_instance != 252)
    {
        printf("rud inst[%03d] pos[%04.1f]deg\n",
               rud_instance,
               ((float)rud_position)*RAD2DEG/10000.0);
    }
#endif
    if(g_boat.rudder_instance != rud_instance)  return;
    if(pgnId->mSA != 115)                       return;
    if(N2K_OUT_OF_ORDER_INT16 <= rud_position)  return;

    g_boat.rudder_position = rud_position;
}

void PGN127245_ProcessNameField(void)
{
    InitializeSendNameField();

    Add1ByteUInt(g_boat.rudder_instance);
    Add1ByteUInt(g_boat.rudder_direction_order | 0xF8);
    Add2ByteUInt(g_boat.rudder_angle_order);
    Add2ByteUInt(g_boat.rudder_position);
    Add2ByteUInt(0xFFFF);

    NMEA2000_SendParseMessages(getCanId(PGN127245_PRIORITY, PGN127245_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr), sendPacketLength, sendNMEAPackets, 0);
}

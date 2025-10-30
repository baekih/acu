/*
 * pgn_127237.h
 *
 *  Created on: Oct 30, 2025
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void PGN127237_ProcessNameField(void)
{
    InitializeSendNameField();

    Add1ByteUInt(0x3C);// 0b00111100 = 0x3C
    Add1ByteUInt(0x03);// 0b00000011 = 0x03
    Add1ByteUInt(0x1F);// 0b00011111 = 0x1F
    Add2ByteUInt(g_boat.rudder_angle_order);
    Add2ByteUInt(g_boat.heading_target);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_UINT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_UINT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_UINT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_INT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_INT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_INT16);
    Add2ByteUInt(N2K_DATA_NOT_AVAILABLE_UINT16);

    SendNonSingleFrame(getCanId(PGN127237_PRIORITY, PGN127237_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr),
                       sendPacketLength,
                       sendNMEAPackets,
                       REQUEST_MESSAGE_TYPE_FASTPACKET);
}

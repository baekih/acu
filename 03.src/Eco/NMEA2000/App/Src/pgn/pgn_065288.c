/*
 * pgn_065288.c
 *
 *  Created on: 2022. 7. 5.
 *      Author: ihbaek
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_065288.h"

/* typedef -------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/

/* functions -----------------------------------------------------------------*/
uint32_t PGN065288_ProcessNameField(uint8_t preset, uint8_t percentage)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

    InitializeSendNameField();

    Add2ByteUInt(PPGN_FURUNO_MFGCODE);
    Add1ByteUInt(PGN065288_BRIGHT_INSTANCE);                                      //Brightness Instance. (fixed to 0).
    Add1ByteUInt((0xF0 & (preset << 4 )) | PGN065288_BRIGHT_SOURCE);              //Brightness Preset|Source(fixed to 1)
    Add1ByteUInt(percentage);                                                     //Brightness persentage.
    Add3ByteUInt(0xFFFFF0 | (PGN065288_BRIGHT_COLOR_PALETTE << 0));               //Brightness Palette (fixed to 15)

    NMEA2000_SendParseMessages(getCanId(PGN065288_PRIORITY, PGN065288_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr), sendPacketLength, sendNMEAPackets, 0);

    return 0;
}

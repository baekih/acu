/*
 * pgn_130822.c
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN130822_ProcessNameField(NmeaPgn* pgnId)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);
    InitializeSendNameField();

    Add2ByteUInt(PPGN_FURUNO_MFGCODE);
    Add1ByteUInt(0); // 1
    Add1ByteUInt(0); // 2
    Add1ByteUInt(0); // 3
    Add1ByteUInt(0); // 4
    Add1ByteUInt(0); // 5
    Add1ByteUInt(0); // 6
    Add1ByteUInt(0); // 7
    Add1ByteUInt(0); // 8

    SendNonSingleFrame(getCanId(PGN130822_PRIORITY, PGN130822_PGN, pgnId->mSA, g_n2k_addr_curr),
                       sendPacketLength,
                       sendNMEAPackets,
                       REQUEST_MESSAGE_TYPE_FASTPACKET);
}

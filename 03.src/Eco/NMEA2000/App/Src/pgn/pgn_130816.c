/*
 * pgn_130816.c
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_130816.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN130816_ProcessNameField(NmeaPgn* pgnId)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);
    InitializeSendNameField();

    uint16_t romTestResult = 0;
    uint16_t ramTestResult = 0;
    uint16_t comTestResult = 3;
    uint16_t backupTestResult = 3;
    uint16_t etcTestResult = 0x03;
    uint16_t reservedTestResult = 0x3F;

    Add2ByteUInt(PPGN_FURUNO_MFGCODE);
    Add2ByteUInt(((romTestResult 		<<  0) & 0x0003) |
                 ((ramTestResult 		<<  2) & 0x000C) |
                 ((comTestResult 		<<  4) & 0x0030) |
                 ((backupTestResult		<<  6) & 0x00C0) |
                 ((etcTestResult 		<<  8) & 0x0300) |
                 ((reservedTestResult	<< 10) & 0xFC00));
    Add1ByteUInt(2);
    Add1ByteUInt(1);

    SendNonSingleFrame(getCanId(PGN130816_PRIORITY, PGN130816_PGN, 255, localSourceAddr), sendPacketLength,
                     sendNMEAPackets,
                     REQUEST_MESSAGE_TYPE_FASTPACKET);
}

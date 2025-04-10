/*
 * pgn_126993.c
 *
 *  Created on: 2022. 7. 19.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_126993.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

uint32_t PGN126993_ProcessNameField(uint8_t HeartbeatSeqCnt, uint8_t Can1stCtlState, uint8_t EquiptStatus)
{
//    printf("%s:%d Heartbeat Enter.\r\n",__FUNCTION__,__LINE__);

    InitializeSendNameField();

    Add2ByteUInt(PGN126993_UPDATE_RATE);        // Update Rate : 60sec.
    Add1ByteUInt(HeartbeatSeqCnt);              // Heartbeat Sequence Counter.
    Add1ByteUInt((0xC0) | ((EquiptStatus << 4) & 0x30) | ((PGN126993_CAN_CTLR_STATE_CLASS2_DAT << 2) & 0x0C) | ((Can1stCtlState << 0) & 0x03));
                                              // CAN class1 controller state(2bit) + CAN class2 controller state(2bit) + Equipment Status(2bit) + reserved(2bit)
    Add4ByteUInt(0xFFFFFFFF);                   //reserved 4byte

    NMEA2000_SendParseMessages(getCanId(PGN126993_PRIORITY, PGN126993_PGN, BROADCAST_DESTINATION_ADDR, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);

    return 0;
}

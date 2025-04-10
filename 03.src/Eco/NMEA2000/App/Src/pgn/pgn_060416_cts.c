/*
 * pgn_060416_cts.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_060416_cts.h"
#include "pgn_060160.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416CTSNAME g_PGN060416CTSNAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN060416CTS_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN060416CTS_SetFieldValue(uint32_t _CTS_Group_Function_Code,
                                uint32_t _Number_of_frames_that_can_be_sent,
                                uint32_t _Number_of_next_frame_to_be_transmitted,
                                uint32_t _NMEA_Reserved,
                                uint32_t _PGN_of_multipacket_message)
{

    InitializeSendNameField();

    Add1ByteUInt( _CTS_Group_Function_Code );
    Add1ByteUInt( _Number_of_frames_that_can_be_sent );
    Add1ByteUInt( _Number_of_next_frame_to_be_transmitted );
    Add2ByteUInt( _NMEA_Reserved );
    Add3ByteUInt( _PGN_of_multipacket_message );
}

void PGN060416CTS_SendNameField(NmeaPgn* pgnId)
{
    NMEA2000_SendParseMessages(pgnId, sendPacketLength, sendNMEAPackets, 0);
}

void PGN060416CTS_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN060416CTSNAME.mCTS_Group_Function_Code                 = GetBuf_1ByteUInt(len, 0, buf);
    g_PGN060416CTSNAME.mNumber_of_frames_that_can_be_sent       = GetBuf_1ByteUInt(len, 1, buf);
    g_PGN060416CTSNAME.mNumber_of_next_frame_to_be_transmitted  = GetBuf_1ByteUInt(len, 2, buf);
    g_PGN060416CTSNAME.mNMEA_Reserved                           = GetBuf_2ByteUInt(len, 3, buf);
    g_PGN060416CTSNAME.mPGN_of_multipacket_message              = GetBuf_3ByteUInt(len, 5, buf);
}

void PGN060416CTS_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    uint32_t sendFrame = 0;
    uint32_t endFrame = 0;
    uint8_t  multiPacketData[MULTI_PACKET_SIZE_BYTES];
    uint32_t startOffset = 0;
    uint32_t endOffset = 0;

    if (sendMultiPacket.mMergedMultiPacket != NULL) {
        sendFrame = g_PGN060416CTSNAME.mNumber_of_next_frame_to_be_transmitted;
        endFrame = g_PGN060416CTSNAME.mNumber_of_next_frame_to_be_transmitted + g_PGN060416CTSNAME.mNumber_of_frames_that_can_be_sent;

        while (sendFrame < endFrame) {
            memset(multiPacketData, 0xff, MULTI_PACKET_SIZE_BYTES);

            int bufferSize = sendMultiPacket.mTotalMessageByteSize;

            startOffset = (sendFrame - 1) * MULTI_PACKET_SIZE_BYTES;
            endOffset = startOffset + MULTI_PACKET_SIZE_BYTES;

            memcpy(multiPacketData, sendMultiPacket.mMergedMultiPacket+startOffset,
                    (endOffset > bufferSize) ? (bufferSize - startOffset) : MULTI_PACKET_SIZE_BYTES);

            HAL_Delay(1);
            PGN060160_SetFieldValue(sendFrame, multiPacketData);
            PGN060160_SendNameField(pgnId->mSA, localSourceAddr);

            sendFrame++;

            if (sendFrame > sendMultiPacket.mTotalNumberOfFrametoTransmit){
                break;
            }
        }
    }
}

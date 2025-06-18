/*
 * pgn_060416_rts.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_060160.h"

#include "pgn_060416.h"
#include "pgn_060416_rts.h"
#include "pgn_060416_cts.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416RTSNAME g_PGN060416RTSNAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN060416RTS_SetFieldValue(uint32_t _RTS_Group_Function_Code,
                                uint32_t _Total_message_size_bytes,
                                uint32_t _Total_number_of_frames_to_be_transmitted,
                                uint32_t _NMEA_Reserved,
                                uint32_t _PGN_of_multipacket_message)
{
    InitializeSendNameField();

    Add1ByteUInt( _RTS_Group_Function_Code );
    Add2ByteUInt( _Total_message_size_bytes );
    Add1ByteUInt( _Total_number_of_frames_to_be_transmitted );
    Add1ByteUInt( _NMEA_Reserved );
    Add3ByteUInt( _PGN_of_multipacket_message );

#if 0
    printf(" [_RTS_Group_Function_Code = %ld] !!\r\n", _RTS_Group_Function_Code);
    printf(" [_Total_message_size_bytes = %ld] !!\r\n", _Total_message_size_bytes);
    printf(" [_Total_number_of_frames_to_be_transmitted = %ld] !!\r\n", _Total_number_of_frames_to_be_transmitted);
    printf(" [_PGN_of_multipacket_message = %ld] !!\r\n", _PGN_of_multipacket_message);
#endif
}

void PGN060416RTS_SendNameField(NmeaPgn* sendPgn)
{
    NMEA2000_SendParseMessages(sendPgn, sendPacketLength, sendNMEAPackets, 0);
}

void PGN060416RTS_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN060416RTSNAME.mRTS_Group_Function_Code                     = GetBuf_1ByteUInt(len, 0, buf);
    g_PGN060416RTSNAME.mTotal_message_size_bytes                    = GetBuf_2ByteUInt(len, 1, buf);
    g_PGN060416RTSNAME.mTotal_number_of_frames_to_be_transmitted    = GetBuf_1ByteUInt(len, 3, buf);
    g_PGN060416RTSNAME.mNMEA_Reserved                               = GetBuf_1ByteUInt(len, 4, buf);
    g_PGN060416RTSNAME.mPGN_of_multipacket_message                  = GetBuf_3ByteUInt(len, 5, buf);

#if 0
    printf(" [mRTS_Group_Function_Code = %ld] !!\r\n", g_PGN060416RTSNAME.mRTS_Group_Function_Code);
    printf(" [mTotal_message_size_bytes = %ld] !!\r\n", g_PGN060416RTSNAME.mTotal_message_size_bytes);
    printf(" [mTotal_number_of_frames_to_be_transmitted = %ld] !!\r\n", g_PGN060416RTSNAME.mTotal_number_of_frames_to_be_transmitted);
    printf(" [mPGN_of_multipacket_message = %ld] !!\r\n", g_PGN060416RTSNAME.mPGN_of_multipacket_message);
#endif
}

uint32_t PGN060416RTS_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    PGN060160_InitializeReceiveRTSCTSMulitiPacket(   g_PGN060416RTSNAME.mTotal_message_size_bytes,
                                                     g_PGN060416RTSNAME.mTotal_number_of_frames_to_be_transmitted,
                                                     g_PGN060416RTSNAME.mPGN_of_multipacket_message,
                                                     pgnId->mSA,
                                                     pgnId->mPS);

    PGN060416CTS_SetFieldValue(PGN060416_FUNC_CODE_CTS, 2, 1, 0xFFFF, g_PGN060416RTSNAME.mPGN_of_multipacket_message);

    PGN060416CTS_SendNameField(getCanId(PGN060416_CTS_PRIORITY, PGN060416_CTS_PGN, pgnId->mSA, g_n2k_addr_curr));

    return 0;
}

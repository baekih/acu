/*
 * pgn_060416_eom.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_060416_eom.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416EOMNAME g_PGN060416EOMNAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN060416EOM_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN060416EOM_SetFieldValue(uint32_t _EOM_Group_Function_Code,
                                uint32_t _Total_message_size_bytes,
                                uint32_t _Total_number_of_frames_received,
                                uint32_t _NMEA_Reserved,
                                uint32_t _PGN_of_multipacket_message)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
    printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif


    InitializeSendNameField();

    Add1ByteUInt( _EOM_Group_Function_Code );
    Add2ByteUInt( _Total_message_size_bytes );
    Add1ByteUInt( _Total_number_of_frames_received );
    Add1ByteUInt( _NMEA_Reserved );
    Add3ByteUInt( _PGN_of_multipacket_message );

#if PRINTF_DEBUG_PGN060416_EOM_NON
    printf(" [_EOM_Group_Function_Code = %ld] !!\r\n", _EOM_Group_Function_Code);
    printf(" [_Total_message_size_bytes = %ld] !!\r\n", _Total_message_size_bytes);
    printf(" [_Total_number_of_frames_received = %ld] !!\r\n", _Total_number_of_frames_received);
    printf(" [_PGN_of_multipacket_message = %ld] !!\r\n", _PGN_of_multipacket_message);
#endif
}

void PGN060416EOM_SendNameField(NmeaPgn* pgnId)
{
    NMEA2000_SendParseMessages(pgnId, sendPacketLength, sendNMEAPackets, 0);
}

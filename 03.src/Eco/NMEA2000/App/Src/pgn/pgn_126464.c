/*
 * pgn_126464.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_126464.h"

/* Private typedef -----------------------------------------------------------*/
PGN126464NAME g_PGN126464NAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN_GROUP_TRANSMIT_LIST_MESSAGE = 0;
uint32_t PGN_GROUP_RECEIVE_LIST_MESSAGE = 1;

uint32_t PGN126464_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN126464_SetFieldValue(uint32_t _PGN_Group_Function_Code,
                             uint32_t _First_PGN_supported[],
                             uint8_t _PGN_List_Unit)
{
    InitializeSendNameField();

    Add1ByteUInt( _PGN_Group_Function_Code );

    for (uint8_t i = 0; i < _PGN_List_Unit; i++)
        Add3ByteUInt( _First_PGN_supported[i] );
}

void PGN126464_ProcessNameField(NmeaPgn* pgnId, uint32_t messagetype)
{
    SendNonSingleFrame(getCanId(PGN126464_priority, 126464, pgnId->mSA, g_n2k_addr_local), sendPacketLength, sendNMEAPackets, messagetype);
}


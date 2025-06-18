/*
 * pgn_059904.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_059392.h"
#include "pgn_059904.h"
#include "pgn_060928.h"
#include "pgn_126464.h"
#include "pgn_126720.h"
#include "pgn_126996.h"
#include "pgn_126998.h"
#include "pgn_127245.h"
#include "pgn_130816.h"
#include "pgn_130822.h"
#include "pgn_130823.h"

/* Private typedef -----------------------------------------------------------*/
PGN059904NAME g_PGN059904NAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN059904_SetFieldValue(uint32_t _PGN)
{
    InitializeSendNameField();

    Add3ByteUInt( _PGN );
}

void PGN059904_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN059904NAME.mPGN_being_requested = GetBuf_3ByteUInt(len, 0, buf);
}

uint32_t PGN059904_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);
//    printf(" [mPGN_being_requested = %ld] !!\r\n", g_PGN059904NAME.mPGN_being_requested);
    switch(g_PGN059904NAME.mPGN_being_requested)
    {
        case 60928: // ISO Address clame
            PGN060928_SetInitialField();
            PGN060928_ProcessNameField();
            break;
        case 126464:
            PGN126464_SetFieldValue(PGN_GROUP_TRANSMIT_LIST_MESSAGE, mTransmit_PGN_List, mTransmit_PGN_List_Size);
            PGN126464_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);

            PGN126464_SetFieldValue(PGN_GROUP_RECEIVE_LIST_MESSAGE, mReceive_PGN_List, mReceive_PGN_List_Size);
            PGN126464_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);
            break;
        case 126720:
            PGN126720_FactoryTest_SetFieldValue();
            PGN126720_ProcessNameField(pgnId);
            break;
        case 126996:
            PGN126996_ProcessNameField(pgnId, len, buf, REQUEST_MESSAGE_TYPE_FASTPACKET);
            break;
        case 126998:
            PGN126998_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);
            break;
        case 127245:
            PGN127245_ProcessNameField(pgnId);
            break;
        case 130816:
            PGN130816_ProcessNameField(pgnId);
            break;
        case 130822:
            PGN130822_ProcessNameField(pgnId);
            break;
        case 130823:
            PGN130823_ProcessNameField(pgnId);
            break;
        default:
            if(pgnId->mPS != BROADCAST_DESTINATION_ADDR)
            {
                PGN059392_SetFieldValue(NEGATIVIE_ACKNOWLEDGMENT, 0xFF, 0xFFFFFF, g_PGN059904NAME.mPGN_being_requested);
                PGN059392_SendNameField(pgnId);
            }
            break;
    }

    return 0;
}

void PGN059904_SendNameField(NmeaPgn* pgnId)
{
    NMEA2000_SendParseMessages(
            getCanId(PGN059904_PRIORITY, PGN059904_PGN, pgnId->mSA, g_n2k_addr_local),
            sendPacketLength,
            sendNMEAPackets,
            0);
}

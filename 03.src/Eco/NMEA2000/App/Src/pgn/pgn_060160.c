/*
 * pgn_060160.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "multipacketdata.h"

#include "pgn_060160.h"
#include "pgn_060416.h"
#include "pgn_060416_cts.h"
#include "pgn_060416_eom.h"

/* Private typedef -----------------------------------------------------------*/
PGN060160NAME g_PGN060160;

PGN060160RTSCTSNAME g_PGN060160RTSCTSNAME;
PGN060160BAMNAME g_PGN060160BAMNAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN060160_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN060160.mMultiPacketFrameCounter = GetBuf_1ByteUInt(len, 0, buf);
    g_PGN060160.mMultiPacketData         = GetBuf_7ByteUInt(len, 1, buf);
}

void PGN060160_SetFieldValue(uint32_t _Sequence_number_of_multipacket_frame, uint8_t *_Multipacket_packetized_data)
{
    InitializeSendNameField();

    Add1ByteUInt( _Sequence_number_of_multipacket_frame );
    for (uint8_t i = 0; i < 7; i++)
        Add1ByteUInt( _Multipacket_packetized_data[i] );

}

void PGN060160_InitializeReceiveMulitiPacket(uint32_t _TotalMessageByteSize, uint32_t _TotalNumberOfFrametoTransmit, uint32_t _PGNMultiPacketMessage,
                                             uint8_t _PacketSourceAddress, uint8_t _PacketDestinationAddress, uint8_t _RTSCTS)
{
    MultiPacket* pMultiPacket = (MultiPacket *)malloc(sizeof(MultiPacket));

    if(pMultiPacket == NULL){
        printf("malloc Error,%s,%d\r\n", __FUNCTION__, __LINE__);
        return;
    }

    pMultiPacket->mTotalMessageByteSize = _TotalMessageByteSize;
    pMultiPacket->mTotalNumberOfFrametoTransmit = _TotalNumberOfFrametoTransmit;
    pMultiPacket->mPGNMultiPacketMessage = _PGNMultiPacketMessage;

    pMultiPacket->mPacketSourceAddress = _PacketSourceAddress;
    pMultiPacket->mPacketDestinationAddress = _PacketDestinationAddress;

    pMultiPacket->mRTSCTS = _RTSCTS;

    pMultiPacket->mCurrentPacketFrameNumber = 1;
    pMultiPacket->mMergedMultiPacket = (uint8_t *)malloc(_TotalMessageByteSize);

    if(pMultiPacket->mMergedMultiPacket == NULL){
        printf("malloc Error,%s,%d\r\n", __FUNCTION__, __LINE__);
        return;
    }

    pMultiPacket->mReceivedByteSize = 0;

    pMultiPacket->mLastReceivePacketTime = HAL_GetTick();


    for(int i = 0; i < MULTIPACKET_MAX_COUNT; i++){
        if(arrayReceiveMultiPackets[i] == NULL){
            arrayReceiveMultiPackets[i] = pMultiPacket;
            break;
        }
    }
}

void PGN060160_InitializeReceiveRTSCTSMulitiPacket(uint32_t _TotalMessageByteSize, uint32_t _TotalNumberOfFrametoTransmit, uint32_t _PGNMultiPacketMessage,
                                                   uint8_t _PacketSourceAddress, uint8_t _PacketDestinationAddress){

    PGN060160_InitializeReceiveMulitiPacket(_TotalMessageByteSize, _TotalNumberOfFrametoTransmit, _PGNMultiPacketMessage,
                                            _PacketSourceAddress, _PacketDestinationAddress, 1);
}

void PGN060160_InitializeReceiveBAMMulitiPacket(uint32_t _TotalMessageByteSize, uint32_t _TotalNumberOfFrametoTransmit, uint32_t _PGNMultiPacketMessage,
                                                uint8_t _PacketSourceAddress, uint8_t _PacketDestinationAddress){

    PGN060160_InitializeReceiveMulitiPacket(_TotalMessageByteSize, _TotalNumberOfFrametoTransmit, _PGNMultiPacketMessage,
                                            _PacketSourceAddress, _PacketDestinationAddress, 0);
}

uint32_t PGN060160_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    for(int i = 0; i < MULTIPACKET_MAX_COUNT; i++){
        if(arrayReceiveMultiPackets[i] == NULL) continue;

        MultiPacket* pMultiPacket = (MultiPacket *)arrayReceiveMultiPackets[i];

        if(pMultiPacket->mPacketSourceAddress != pgnId->mSA || pMultiPacket->mPacketDestinationAddress != pgnId->mPS) continue;

        memcpy(&pMultiPacket->mMergedMultiPacket[(g_PGN060160.mMultiPacketFrameCounter-1) * 7], (uint8_t *)&g_PGN060160.mMultiPacketData,
                ((pMultiPacket->mTotalMessageByteSize - pMultiPacket->mReceivedByteSize) >= 7) ?
                        PGN060160_MULTI_PACKET_SIZE_BYTES : pMultiPacket->mTotalMessageByteSize - pMultiPacket->mReceivedByteSize);

        pMultiPacket->mReceivedByteSize += 7;
        pMultiPacket->mLastReceivePacketTime = HAL_GetTick();

/*        printf("PGN060160_ProcessNameField : %d,%s,%d, mCurrentPacketFrameNumber:%ld,%ld,%ld,%ld,%ld\r\n", i, __FUNCTION__, __LINE__,
                pMultiPacket->mPGNMultiPacketMessage,
                pMultiPacket->mPacketSourceAddress,
                pMultiPacket->mPacketDestinationAddress,
                pMultiPacket->mRTSCTS,
                pMultiPacket->mCurrentPacketFrameNumber);*/

        if(g_PGN060160.mMultiPacketFrameCounter == pMultiPacket->mTotalNumberOfFrametoTransmit){
            completeMultiPacketData = *pMultiPacket;

            completeMultiPacketData.mMergedMultiPacket = (uint8_t *)malloc(completeMultiPacketData.mTotalMessageByteSize);
            memcpy(completeMultiPacketData.mMergedMultiPacket, pMultiPacket->mMergedMultiPacket, completeMultiPacketData.mTotalMessageByteSize);

            free(pMultiPacket->mMergedMultiPacket);
            free(arrayReceiveMultiPackets[i]);

            arrayReceiveMultiPackets[i] = NULL;

            PGN060416EOM_SetFieldValue(PGN060416_FUNC_CODE_EOM, completeMultiPacketData.mTotalMessageByteSize,	completeMultiPacketData.mTotalNumberOfFrametoTransmit,
                    0xFF,completeMultiPacketData.mPGNMultiPacketMessage);
            PGN060416EOM_SendNameField(getCanId(PGN060416_EOM_PRIORITY, PGN060416_EOM_PGN, pgnId->mSA, g_n2k_addr_curr));

            return PGN060160_PACKET_PROC_RESULT_MULTIPACKET_DONE;
        }
        else{
            if ((pMultiPacket->mCurrentPacketFrameNumber % 2) == 0) {
                PGN060416CTS_SetFieldValue(PGN060416_FUNC_CODE_CTS, 2, pMultiPacket->mCurrentPacketFrameNumber + 1, 0xFFFF, pMultiPacket->mPGNMultiPacketMessage);
                PGN060416CTS_SendNameField(getCanId(PGN060416_CTS_PRIORITY, PGN060416_CTS_PGN, pgnId->mSA, g_n2k_addr_curr));
            }

            pMultiPacket->mCurrentPacketFrameNumber++;
        }
    }

    return PGN060160_PACKET_PROC_RESULT_NONE;
}

void PGN060160_SendNameField(uint8_t dest_addr)
{
    NMEA2000_SendParseMessages(getCanId(PGN060160_PRIORITY, PGN060160_PGN, dest_addr, g_n2k_addr_curr),
                               sendPacketLength,
                               sendNMEAPackets,
                               0);
}


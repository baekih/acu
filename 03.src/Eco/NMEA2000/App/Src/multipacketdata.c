/*
 * multipacketdata.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "multipacketdata.h"

/* Private variables ---------------------------------------------------------*/
MultiPacket* arrayReceiveMultiPackets[MULTIPACKET_MAX_COUNT];

MultiPacket sendMultiPacket;
MultiPacket completeMultiPacketData;

void InitMultiPacketArray()
{
    for(int i = 0; i < MULTIPACKET_MAX_COUNT; i++){
        arrayReceiveMultiPackets[i] = NULL;
    }

    sendMultiPacket.mMergedMultiPacket = NULL;
    completeMultiPacketData.mMergedMultiPacket = NULL;
}

void InitializeSendMultiPacketData(uint32_t _Total_Message_Byte_Size,
                                   uint32_t _Total_Number_Of_Frame_to_Transmit,
                                   uint32_t _MultiPacket_PGNMessage,
                                   uint32_t _Packet_Source_Address,
                                   uint32_t _Packet_Destination_Address,
                                   uint8_t _IsRTSCTS)
{
    sendMultiPacket.mTotalMessageByteSize = _Total_Message_Byte_Size;
    sendMultiPacket.mTotalNumberOfFrametoTransmit = _Total_Number_Of_Frame_to_Transmit;
    sendMultiPacket.mPGNMultiPacketMessage = _MultiPacket_PGNMessage;

    sendMultiPacket.mPacketSourceAddress = _Packet_Source_Address;
    sendMultiPacket.mPacketDestinationAddress = _Packet_Destination_Address;

    sendMultiPacket.mRTSCTS = _IsRTSCTS;

    sendMultiPacket.mCurrentPacketFrameNumber = 1;

    if(sendMultiPacket.mMergedMultiPacket != NULL){
        free(sendMultiPacket.mMergedMultiPacket);
    }

    sendMultiPacket.mMergedMultiPacket = (uint8_t *)malloc(_Total_Message_Byte_Size);
    sendMultiPacket.mReceivedByteSize = 0;
}

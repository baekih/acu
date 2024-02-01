/*
 * multipacketdata.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_MULTIPACKETDATA_H_
#define NMEA2000_INC_MULTIPACKETDATA_H_


/* Includes ------------------------------------------------------------------*/
#include "eco.h"

typedef struct __MultiPacket__ {
	uint32_t mTotalMessageByteSize;
	uint32_t mTotalNumberOfFrametoTransmit;
	uint32_t mPGNMultiPacketMessage;

	uint32_t mPacketSourceAddress;
	uint32_t mPacketDestinationAddress;

	uint32_t mRTSCTS;

	uint32_t mCurrentPacketFrameNumber;
	uint8_t* mMergedMultiPacket;
	uint32_t mReceivedByteSize;

	uint32_t mLastReceivePacketTime;
} MultiPacket;

#define MULTIPACKET_MAX_COUNT		128

extern MultiPacket* arrayReceiveMultiPackets[MULTIPACKET_MAX_COUNT];

extern MultiPacket sendMultiPacket;
extern MultiPacket completeMultiPacketData;

void InitMultiPacketArray();

/* Exported functions --------------------------------------------------------*/
void MultiPacketData(uint32_t _Total_Message_Byte_Size,
						  uint32_t _Total_Number_Of_Frame_to_Transmit,
						  uint32_t _MultiPacket_PGNMessage,
						  uint32_t _Packet_Source_Address,
						  uint32_t _Packet_Destination_Address,
						  uint8_t _IsRTSCTS);

void InitializeSendMultiPacketData(uint32_t _Total_Message_Byte_Size,
						   uint32_t _Total_Number_Of_Frame_to_Transmit,
						   uint32_t _MultiPacket_PGNMessage,
						   uint32_t _Packet_Source_Address,
						   uint32_t _Packet_Destination_Address,
						   uint8_t _IsRTSCTS);

#endif /* NMEA2000_INC_MULTIPACKETDATA_H_ */

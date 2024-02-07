/*
 * fastpacketdata.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_FASTPACKETDATA_H_
#define NMEA2000_INC_FASTPACKETDATA_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define FastPacketMaxDataLen 223

/* Exported variables --------------------------------------------------------*/
extern uint32_t mFastPacket_Total_Message_Byte_Size;
extern uint32_t mFastPacket_Identifier;
extern uint32_t mFastPacket_PGNMessage;
extern uint32_t mFastPacket_Source_Address;
extern uint32_t mFastPacket_Destination_Address;

extern uint8_t mMerged_FastPacket[FastPacketMaxDataLen];

extern uint32_t mFastPacket_Received_Byte_Size;
extern uint32_t mFastPacket_LastReceive_Packet_Time;

/* Exported functions --------------------------------------------------------*/
extern void FastPacketData(uint32_t _FastPacket_Total_Message_Byte_Size,
						   uint32_t _FastPacket_Identifier,
						   uint32_t _FastPacket_PGNMessage,
						   uint32_t _FastPacket_Source_Address,
						   uint32_t _FastPacket_Destination_Address);


#endif /* NMEA2000_INC_FASTPACKETDATA_H_ */

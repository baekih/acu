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
#include "nmea2000.h"

/* Private define ------------------------------------------------------------*/

#define FASTPACKET_PROC_RESULT_GOING        0
#define FASTPACKET_PROC_RESULT_DONE         1

#define FAST_PKT_MAX_DATA_LEN 223

#define FAST_MAX_PKT_SIZE 	  32

/* Exported variables --------------------------------------------------------*/

typedef struct _FastPacketData_ {

	uint32_t mFastPacket_Total_Message_Byte_Size;

	uint32_t mFastPacket_Identifier;
	uint32_t mFastPacket_PGNMessage;

	uint32_t mFastPacket_Source_Address;
	uint32_t mFastPacket_Destination_Address;

	uint32_t mFastPacket_Received_Byte_Size;
	uint8_t mMerged_FastPacket[FAST_PKT_MAX_DATA_LEN];

	uint32_t mFastPacket_LastReceive_Packet_Time;
	uint8_t mReceiveFrameCheck[256];

	uint8_t using;

} FastPacketData;

extern FastPacketData listFastPacket[FAST_MAX_PKT_SIZE];

uint32_t ProcessFastPacketData(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);

uint8_t* GetCompletedFastPacket();
uint32_t GetCompletedFastPacketSize();

#endif /* NMEA2000_INC_FASTPACKETDATA_H_ */

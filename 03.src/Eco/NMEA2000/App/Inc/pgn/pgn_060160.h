/*
 * pgn_060160.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060160_H_
#define NMEA2000_INC_PGN_PGN_060160_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060160NAME
{
	uint32_t mMultiPacketFrameCounter;	// 8  bits
	uint64_t mMultiPacketData;			// 56 bits
} PGN060160NAME;

typedef struct __PGN060160RTSCTSNAME
{
	uint32_t mSequence_number_of_multipacket_frame;	// 8  bits
	uint64_t mMultipacket_packetized_data;			// 56 bits
} PGN060160RTSCTSNAME;

typedef struct __PGN060160BAMNAME
{
	uint32_t mSequence_number_of_multipacket_frame;	// 8  bits
	uint64_t mMultipacket_packetized_data;			// 56 bits
} PGN060160BAMNAME;

/* Exported variables --------------------------------------------------------*/
extern uint32_t MULTI_PACKET_SIZE_BYTES;
extern uint32_t PACKET_PROC_RESULT_NONE;
extern uint32_t PACKET_PROC_RESULT_MULTIPACKET_DONE;

/* Exported functions --------------------------------------------------------*/
extern void PGN060160_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
extern uint32_t PGN060160_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);

extern void PGN060160_SetFieldValue(uint32_t _Sequence_number_of_multipacket_frame, uint8_t *_Multipacket_packetized_data);
extern void PGN060160_SendNameField(uint32_t Destination_Addr, uint32_t Source_Addr);

void PGN060160_InitializeReceiveRTSCTSMulitiPacket(uint32_t _TotalMessageByteSize, uint32_t _TotalNumberOfFrametoTransmit, uint32_t _PGNMultiPacketMessage,
		uint8_t _PacketSourceAddress, uint8_t _PacketDestinationAddress);
void PGN060160_InitializeReceiveBAMMulitiPacket(uint32_t _TotalMessageByteSize, uint32_t _TotalNumberOfFrametoTransmit, uint32_t _PGNMultiPacketMessage,
		uint8_t _PacketSourceAddress, uint8_t _PacketDestinationAddress);

#endif /* NMEA2000_INC_PGN_PGN_060160_H_ */

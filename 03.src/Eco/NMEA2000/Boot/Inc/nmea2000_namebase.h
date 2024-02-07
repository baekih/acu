/*
 * nmea2000_namebase.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_NAMEBASE_H_
#define NMEA2000_INC_NMEA2000_NAMEBASE_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define MAX_PACKET_LENGTH 223		// with fast packet 1. frame can have 6 byte and rest 31 frames 7 bytes

#define REQUEST_MESSAGE_TYPE_FASTPACKET         0
#define REQUEST_MESSAGE_TYPE_BAM_PACKET         1
#define REQUEST_MESSAGE_TYPE_RTSCTS_PACKET      2

#define FASTPACKET_PROC_RESULT_GOING            0
#define FASTPACKET_PROC_RESULT_DONE             1

/* Exported variables --------------------------------------------------------*/
extern uint32_t receivePacketLength;
extern uint32_t sendPacketLength;

extern uint8_t receiveNMEAPackets[MAX_PACKET_LENGTH];
extern uint8_t sendNMEAPackets[MAX_PACKET_LENGTH];


/* Exported functions --------------------------------------------------------*/
// *****************************************************************************
extern void InitializeSendNameBitPosition();
extern void InitializeSendNameField();
// *****************************************************************************
extern void Set8Buf(uint64_t v, size_t len, uint32_t index, uint8_t *buf);
extern void SetBuf7ByteUInt(uint64_t v, uint32_t index, uint8_t *buf);
extern void Add7ByteUInt(uint64_t v);
extern void Add6ByteUInt(uint64_t v);

extern void SetBuf(uint32_t v, size_t len, uint32_t index, uint8_t *buf);
extern void SetBuf4ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void SetBuf3ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void SetBuf2ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void Add4ByteUInt(uint32_t v);
extern void Add3ByteUInt(uint32_t v);
extern void Add2ByteUInt(uint32_t v);
extern void Add1ByteUInt(uint32_t v);

// *****************************************************************************
void InitializeReceNameBitPosition();
void InitializeReceNameField();
// *****************************************************************************
extern uint64_t Get8Buf(size_t len, uint8_t index, uint8_t *buf);
extern uint64_t GetBuf7ByteUInt(uint8_t index, uint8_t *buf);
extern uint64_t Get7ByteUInt(uint8_t Index);

extern uint32_t GetBuf(size_t len, uint8_t index, uint8_t *buf);
extern uint32_t GetBuf4ByteUInt(uint8_t index, uint8_t *buf);
extern uint32_t GetBuf3ByteUInt(uint8_t index, uint8_t *buf);
extern uint32_t GetBuf2ByteUInt(uint8_t index, uint8_t *buf);
extern uint32_t Get4ByteUInt(uint8_t Index);
extern uint32_t Get3ByteUInt(uint8_t Index);
extern uint32_t Get2ByteUInt(uint8_t Index);
extern uint32_t Get1ByteUInt(uint8_t Index);

/* ************************************************************************** */
extern uint64_t GetBuf_7ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_3ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_4ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_2ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_1ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);

/* ************************************************************************** */
extern void InitializeTSCTSMulitiPacket(uint32_t _Total_Message_Byte_Size,
										uint32_t _Total_Number_Of_Frame_to_Transmit,
										uint32_t _MultiPacket_PGNMessage,
										uint32_t _Packet_Source_Address,
										uint32_t _Packet_Destination_Address);
extern void InitializeReceiveBAMMulitiPacket(uint32_t _Total_Message_Byte_Size,
											 uint32_t _Total_Number_Of_Frame_to_Transmit,
											 uint32_t _MultiPacket_PGNMessage,
											 uint32_t _Packet_Source_Address,
											 uint32_t _Packet_Destination_Address);

extern uint32_t ProcessFastPacketData(uint32_t pgnid, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_NMEA2000_NAMEBASE_H_ */

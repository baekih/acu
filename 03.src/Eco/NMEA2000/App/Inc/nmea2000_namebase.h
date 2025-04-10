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
#define MAX_PACKET_LENGTH                   223     // with fast packet 1. frame can have 6 byte and rest 31 frames 7 bytes

#define REQUEST_MESSAGE_TYPE_FASTPACKET     0
#define REQUEST_MESSAGE_TYPE_BAM_PACKET     1
#define REQUEST_MESSAGE_TYPE_RTSCTS_PACKET  2

extern uint32_t sendPacketLength;
extern uint8_t sendNMEAPackets[MAX_PACKET_LENGTH];

/* Exported functions --------------------------------------------------------*/
// *****************************************************************************
extern void InitializeSendNameField();
// *****************************************************************************
extern void Set8Buf(uint64_t v, size_t len, uint32_t index, uint8_t *buf);
extern void SetBuf7ByteUInt(uint64_t v, uint32_t index, uint8_t *buf);
extern void Add7ByteUInt(uint64_t v);
extern void Add6ByteUInt(uint64_t v);

extern void SetBuf64(uint64_t v, size_t len, uint32_t index, uint8_t *buf);
extern void SetBuf(uint32_t v, size_t len, uint32_t index, uint8_t *buf);
extern void SetBuf4ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void SetBuf3ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void SetBuf2ByteUInt(uint32_t v, uint32_t index, uint8_t *buf);
extern void Add8ByteUInt(uint64_t v);
extern void Add4ByteUInt(uint32_t v);
extern void Add3ByteUInt(uint32_t v);
extern void Add2ByteUInt(uint32_t v);
extern void Add1ByteUInt(uint32_t v);
extern void Add1ByteInt(int8_t v);

// *****************************************************************************
extern uint64_t Get64bit(size_t len, uint8_t index, uint8_t *buf);
extern uint32_t Get32bit(size_t len, uint8_t index, uint8_t *buf);

extern uint64_t GetBuf_8ByteUInt(uint8_t len, uint8_t index, uint8_t *buf);
extern uint64_t GetBuf_7ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_3ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_4ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_2ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);
extern uint32_t GetBuf_1ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf);

extern uint64_t GetBuf_nByteUInt(uint8_t *pbuf, uint8_t index, uint8_t len);

#endif /* NMEA2000_INC_NMEA2000_NAMEBASE_H_ */

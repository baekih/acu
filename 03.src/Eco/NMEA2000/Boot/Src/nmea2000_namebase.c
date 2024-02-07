/*
 * nmea2000_namebase.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "fastpacketdata.h"

/* Private variables ---------------------------------------------------------*/
uint32_t receivePacketLength = 0;
uint32_t sendPacketLength = 0;

uint32_t mIndexReceBit = 0;
uint32_t mIndexSendBit = 0;

uint8_t receiveNMEAPackets[MAX_PACKET_LENGTH];
uint8_t sendNMEAPackets[MAX_PACKET_LENGTH];

/* Private functions ---------------------------------------------------------*/
void InitializeSendNameBitPosition()
{
	mIndexSendBit = 0;
}

void InitializeSendNameField()
{
	sendPacketLength = 0;
	memset(sendNMEAPackets, 0x00, MAX_PACKET_LENGTH);
}

/* ************************************************************************** */

void Set8Buf(uint64_t v, size_t len, uint32_t index, uint8_t *buf)
{
	memcpy(&buf[index], &v, len);
	sendPacketLength += len;
}

void SetBuf7ByteUInt(uint64_t v, uint32_t index, uint8_t *buf)
{
	Set8Buf(v, 7, index, buf);
}

void SetBuf6ByteUInt(uint64_t v, uint32_t index, uint8_t *buf)
{
	Set8Buf(v, 6, index, buf);
}

void Add7ByteUInt(uint64_t v)
{
	SetBuf7ByteUInt(v, sendPacketLength, sendNMEAPackets);
}

void Add6ByteUInt(uint64_t v)
{
	SetBuf6ByteUInt(v, sendPacketLength, sendNMEAPackets);
}

void SetBuf(uint32_t v, size_t len, uint32_t index, uint8_t *buf)
{
	memcpy(&buf[index], &v, len);
	sendPacketLength += len;
}

void SetBuf4ByteUInt(uint32_t v, uint32_t index, uint8_t *buf)
{
	SetBuf(v, 4, index, buf);
}

void SetBuf3ByteUInt(uint32_t v, uint32_t index, uint8_t *buf)
{
	SetBuf(v, 3, index, buf);
}

void SetBuf2ByteUInt(uint32_t v, uint32_t index, uint8_t *buf)
{
	SetBuf(v, 2, index, buf);
}

void Add4ByteUInt(uint32_t v)
{
	SetBuf4ByteUInt(v, sendPacketLength, sendNMEAPackets);
}

void Add3ByteUInt(uint32_t v)
{
	SetBuf3ByteUInt(v, sendPacketLength, sendNMEAPackets);
}

void Add2ByteUInt(uint32_t v)
{
	SetBuf2ByteUInt(v, sendPacketLength, sendNMEAPackets);
}

void Add1ByteUInt(uint32_t v)
{
	sendNMEAPackets[sendPacketLength]=v;
	sendPacketLength++;
}

/* ************************************************************************** */

void InitializeReceNameBitPosition()
{
	mIndexReceBit = 0;
}

void InitializeReceNameField()
{
	receivePacketLength = 0;
	
	for (int i = 0; i < MAX_PACKET_LENGTH; i++)
		receiveNMEAPackets[i] = 0;
}

/* ************************************************************************** */

uint64_t Get8Buf(size_t len, uint8_t index, uint8_t *buf)
{
	uint64_t v = 0;

	memcpy(&v, &buf[index], len);
	index += len;

	return v;
}

uint64_t GetBuf7ByteUInt(uint8_t index, uint8_t *buf)
{
	return Get8Buf(7, index, buf);
}

uint64_t Get7ByteUInt(uint8_t Index)
{
	if (Index+7 <= receivePacketLength)
		return GetBuf7ByteUInt(Index, receiveNMEAPackets);
	else
		return 0xFFFFFFFFFFFFFF;
}

uint32_t GetBuf(size_t len, uint8_t index, uint8_t *buf)
{
	uint32_t v = 0;

	memcpy(&v, &buf[index], len);
	index += len;

	return v;
}

uint32_t GetBuf4ByteUInt(uint8_t index, uint8_t *buf)
{
	return GetBuf(4, index, buf);
}

uint32_t GetBuf3ByteUInt(uint8_t index, uint8_t *buf)
{
	return GetBuf(3, index, buf);
}

uint32_t GetBuf2ByteUInt(uint8_t index, uint8_t *buf)
{
	return GetBuf(2, index, buf);
}

uint32_t Get4ByteUInt(uint8_t Index)
{
	if (Index+4 <= receivePacketLength)
		return GetBuf4ByteUInt(Index, receiveNMEAPackets);
	else
		return 0xFFFFFFFF;
}

uint32_t Get3ByteUInt(uint8_t Index)
{
	if (Index+3 <= receivePacketLength)
		return GetBuf3ByteUInt(Index, receiveNMEAPackets);
	else
		return 0x00FFFFFF;
}

uint32_t Get2ByteUInt(uint8_t Index)
{
	if (Index+2 <= receivePacketLength)
		return GetBuf2ByteUInt(Index, receiveNMEAPackets);
	else
		return 0x0000FFFF;
}

uint32_t Get1ByteUInt(uint8_t Index)
{
	if (Index < receivePacketLength)
		return receiveNMEAPackets[Index++];
	else
		return 0x000000FF;
}

/* ************************************************************************** */

uint64_t GetBuf_7ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+7 <= len)
		return Get8Buf(7, Index, buf);
	else
		return 0xFFFFFFFF;
}

uint32_t GetBuf_4ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+4 <= len)
		return GetBuf(4, Index, buf);
	else
		return 0xFFFFFFFF;
}

uint32_t GetBuf_3ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+3 <= len)
		return GetBuf(3, Index, buf);
	else
		return 0x00FFFFFF;
}

uint32_t GetBuf_2ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+2 <= len)
		return GetBuf(2, Index, buf);
	else
		return 0x0000FFFF;
}

uint32_t GetBuf_1ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index < len)
		return buf[Index++];
	else
		return 0x000000FF;
}
/* ************************************************************************** */

uint32_t ProcessFastPacketData(uint32_t pgnid, uint8_t len, uint8_t *buf)
{

	uint32_t fastpacketIdentifier = (buf[0] & 0xE0) >> 5;
	uint32_t fastpacketFrameCounter = buf[0] & 0x1F;
	uint32_t fastpacketDataSize;

	if (fastpacketFrameCounter == 0) {
		fastpacketDataSize = buf[1] & 0xFF;

		FastPacketData(fastpacketDataSize, fastpacketIdentifier, get_R_PGN(), get_R_SA(), get_R_PS());

		if (fastpacketDataSize > 6) {
			memcpy(&mMerged_FastPacket[0], &buf[2], 6);
			mFastPacket_Received_Byte_Size += 6;
		} else {
			memcpy(&mMerged_FastPacket[0], &buf[2], fastpacketDataSize);

			return FASTPACKET_PROC_RESULT_DONE;
		}
	} else {
		mFastPacket_LastReceive_Packet_Time = HAL_GetTick();

		if ((mFastPacket_Total_Message_Byte_Size - mFastPacket_Received_Byte_Size) > 7) {
			memcpy(&mMerged_FastPacket[mFastPacket_Received_Byte_Size], &buf[1], 7);
			mFastPacket_Received_Byte_Size += 7;
		} else {
			memcpy(&mMerged_FastPacket[mFastPacket_Received_Byte_Size], &buf[1], 7);
			mFastPacket_Received_Byte_Size += 7;

			return FASTPACKET_PROC_RESULT_DONE;
		}
	}

	return FASTPACKET_PROC_RESULT_GOING;
}

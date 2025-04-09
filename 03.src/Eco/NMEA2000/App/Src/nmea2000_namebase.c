/*
 * nmea2000_namebase.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "multipacketdata.h"
#include "fastpacketdata.h"

/* Private variables ---------------------------------------------------------*/
uint32_t sendPacketLength = 0;
uint8_t sendNMEAPackets[MAX_PACKET_LENGTH];

/* Private functions ---------------------------------------------------------*/
void InitializeSendNameField()
{
	sendPacketLength = 0;
	memset(&sendNMEAPackets[0], 0, MAX_PACKET_LENGTH);
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

void SetBuf64(uint64_t v, size_t len, uint32_t index, uint8_t *buf)
{
  memcpy(&buf[index], &v, len);
  sendPacketLength += len;
}

void SetBuf(uint32_t v, size_t len, uint32_t index, uint8_t *buf)
{
  memcpy(&buf[index], &v, len);
  sendPacketLength += len;
}

void SetBuf8ByteUInt(uint64_t v, uint32_t index, uint8_t *buf)
{
  SetBuf64(v, 8, index, buf);
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

void Add8ByteUInt(uint64_t v)
{
  SetBuf8ByteUInt(v, sendPacketLength, sendNMEAPackets);
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
	sendNMEAPackets[sendPacketLength]=(uint8_t)v;
	sendPacketLength++;
}

void Add1ByteInt(int8_t v)
{
    sendNMEAPackets[sendPacketLength]=v;
    sendPacketLength++;
}

uint64_t Get64bit(size_t len, uint8_t index, uint8_t *buf)
{
	uint64_t v = 0;

	memcpy(&v, &buf[index], len);

	return v;
}

uint32_t Get32bit(size_t len, uint8_t index, uint8_t *buf)
{
	uint32_t v = 0;

	memcpy(&v, &buf[index], len);

	return v;
}

/* ************************************************************************** */

uint64_t GetBuf_8ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+8 <= len)
		return Get64bit(8, Index, buf);
	else
		return 0xFFFFFFFFFFFFFFFF;
}

uint64_t GetBuf_7ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+7 <= len)
		return Get64bit(7, Index, buf);
	else
		return 0xFFFFFFFFFFFFFFFF;
}

uint32_t GetBuf_4ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+4 <= len)
		return Get32bit(4, Index, buf);
	else
		return 0xFFFFFFFF;
}

uint32_t GetBuf_3ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+3 <= len)
		return Get32bit(3, Index, buf);
	else
		return 0x00FFFFFF;
}

uint32_t GetBuf_2ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+2 <= len)
		return Get32bit(2, Index, buf);
	else
		return 0x0000FFFF;
}

uint32_t GetBuf_1ByteUInt(uint8_t len, uint8_t Index, uint8_t *buf)
{
	if (Index+1 <= len)
		return Get32bit(1, Index, buf);
	else
		return 0x000000FF;
}

uint64_t GetBuf_nByteUInt(uint8_t *pbuf, uint8_t index, uint8_t len)
{
	uint64_t ret = 0;

	if(8 < len) return 0xFFFFFFFFFFFFFFFF;

	for(int i = 0; i < len; i++)
	{
		ret |= (uint64_t)(*(pbuf + index + i))<<(8*i);
	}

	return ret;
}


/*
 * fastpacketdata.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "fastpacketdata.h"

/* Private variables ---------------------------------------------------------*/
uint32_t mFastPacket_Total_Message_Byte_Size;
uint32_t mFastPacket_Identifier;
uint32_t mFastPacket_PGNMessage;
uint32_t mFastPacket_Source_Address;
uint32_t mFastPacket_Destination_Address;

uint8_t mMerged_FastPacket[FastPacketMaxDataLen];

uint32_t mFastPacket_Received_Byte_Size;
uint32_t mFastPacket_LastReceive_Packet_Time;

/* Private functions ---------------------------------------------------------*/
void FastPacketData(uint32_t _FastPacket_Total_Message_Byte_Size,
					uint32_t _FastPacket_Identifier,
					uint32_t _FastPacket_PGNMessage,
					uint32_t _FastPacket_Source_Address,
					uint32_t _FastPacket_Destination_Address)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	mFastPacket_Total_Message_Byte_Size				= _FastPacket_Total_Message_Byte_Size;
	mFastPacket_Identifier							= _FastPacket_Identifier;
	mFastPacket_PGNMessage							= _FastPacket_PGNMessage;
	mFastPacket_Source_Address						= _FastPacket_Source_Address;
	mFastPacket_Destination_Address					= _FastPacket_Destination_Address;

	for (int i = 0; i < mFastPacket_Total_Message_Byte_Size; i++)
		mMerged_FastPacket[i] = 0;
	mFastPacket_Received_Byte_Size					= 0;
	mFastPacket_LastReceive_Packet_Time				= HAL_GetTick();

#if PRINTF_DEBUG_FASTPACKETDATA_NON
	printf(" [mFastPacket_Total_Message_Byte_Size = %ld] !!\r\n", mFastPacket_Total_Message_Byte_Size);
	printf(" [mFastPacket_Identifier = %ld] !!\r\n", mFastPacket_Identifier);
	printf(" [mFastPacket_PGNMessage = %ld] !!\r\n", mFastPacket_PGNMessage);
	printf(" [mFastPacket_Source_Address = %ld] !!\r\n", mFastPacket_Source_Address);
	printf(" [mFastPacket_Destination_Address = %ld] !!\r\n", mFastPacket_Destination_Address);
	printf(" [mMerged_FastPacket = ");	for (uint8_t i = 0; i < mFastPacket_Total_Message_Byte_Size; i++)	printf("0x%02x ", mMerged_FastPacket[i]);	printf("] !!\r\n");
	printf(" [mFastPacket_Received_Byte_Size = %ld] !!\r\n", mFastPacket_Received_Byte_Size);
	printf(" [mFastPacket_LastReceive_Packet_Time = %ld] !!\r\n", mFastPacket_LastReceive_Packet_Time);
#endif
}

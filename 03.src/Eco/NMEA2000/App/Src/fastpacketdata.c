/*
 * fastpacketdata.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "fastpacketdata.h"

/* Private variables ---------------------------------------------------------*/
FastPacketData listFastPacket[FAST_MAX_PKT_SIZE];
uint8_t mCompletedFastPacket[FAST_PKT_MAX_DATA_LEN];
uint16_t mCompletedFastPacketSize = 0;

/* Private functions ---------------------------------------------------------*/
uint32_t NewFastPacketData(uint32_t _FastPacket_Total_Message_Byte_Size,
					uint32_t _FastPacket_Identifier,
					uint32_t _FastPacket_PGNMessage,
					uint32_t _FastPacket_Source_Address,
					uint32_t _FastPacket_Destination_Address)
{
	for(int i = 0; i < FAST_MAX_PKT_SIZE; i++){
		FastPacketData* pFastPacket = listFastPacket + i;

		if((HAL_GetTick() - pFastPacket->mFastPacket_LastReceive_Packet_Time) > 2000){
			pFastPacket->using = 0;
		}

		if(pFastPacket->mFastPacket_Identifier == _FastPacket_Identifier){
			printf("fast packet (%d) invalid identifier %d/%d\r\n",
				i, pFastPacket->mFastPacket_Identifier, _FastPacket_Identifier);

			pFastPacket->using = 0;
		}

		if(pFastPacket->using == 0){
			pFastPacket->mFastPacket_Total_Message_Byte_Size 	= _FastPacket_Total_Message_Byte_Size;
			pFastPacket->mFastPacket_Identifier					= _FastPacket_Identifier;
			pFastPacket->mFastPacket_PGNMessage					= _FastPacket_PGNMessage;
			pFastPacket->mFastPacket_Source_Address				= _FastPacket_Source_Address;
			pFastPacket->mFastPacket_Destination_Address		= _FastPacket_Destination_Address;

			memset(pFastPacket->mMerged_FastPacket, 0, FAST_PKT_MAX_DATA_LEN);

			pFastPacket->mFastPacket_Received_Byte_Size			= 0;
			pFastPacket->mFastPacket_LastReceive_Packet_Time	= HAL_GetTick();

			memset(pFastPacket->mReceiveFrameCheck, 0, sizeof(pFastPacket->mReceiveFrameCheck));

			pFastPacket->using = 1;

			return i;
		}
	}

	return -1;
}


uint8_t* GetCompletedFastPacket(){
	return mCompletedFastPacket;
}

uint32_t GetCompletedFastPacketSize(){
	return mCompletedFastPacketSize;
}

uint32_t ProcessFastPacketData(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	uint32_t fastpacketIdentifier = (buf[0] & 0xE0) >> 5;
	uint32_t fastpacketFrameCounter = buf[0] & 0x1F;
	uint32_t fastpacketDataSize;

	if (fastpacketFrameCounter == 0) {
		fastpacketDataSize = buf[1] & 0xFF;

//		printf("fastpacketDataSize[%ld]\r\n",fastpacketDataSize);

		if (fastpacketDataSize > 6) {
			int newIndex = NewFastPacketData(fastpacketDataSize, fastpacketIdentifier, pgnId->mPGN, pgnId->mSA, pgnId->mPS);

			if(newIndex != -1){
//				printf("fast packet (%d) new\r\n",  newIndex);

				memcpy(&listFastPacket[newIndex].mMerged_FastPacket[0], &buf[2], 6);

				listFastPacket[newIndex].mFastPacket_Received_Byte_Size += 6;
			}
			else {
				printf("fast packet is full......\r\n");
			}
		}
		else {
			memcpy(&mCompletedFastPacket[0], &buf[2], fastpacketDataSize);
			mCompletedFastPacketSize = fastpacketDataSize;

			return FASTPACKET_PROC_RESULT_DONE;
		}
	}
	else {
		for(int i = 0; i < FAST_MAX_PKT_SIZE; i++){
			FastPacketData* pFastPacket = listFastPacket + i;

			if((HAL_GetTick() - pFastPacket->mFastPacket_LastReceive_Packet_Time) > 2000){
				pFastPacket->using = 0;
			}

			if(pFastPacket->mFastPacket_Source_Address != pgnId->mSA ||
					pFastPacket->mFastPacket_Destination_Address != pgnId->mPS ||
					pFastPacket->mFastPacket_Identifier != fastpacketIdentifier ||
					pFastPacket->mFastPacket_PGNMessage != pgnId->mPGN) continue;

			pFastPacket->mFastPacket_LastReceive_Packet_Time = HAL_GetTick();

			if ((pFastPacket->mFastPacket_Total_Message_Byte_Size - pFastPacket->mFastPacket_Received_Byte_Size) > 7) {
				memcpy(pFastPacket->mMerged_FastPacket + pFastPacket->mFastPacket_Received_Byte_Size, buf + 1, 7);
				pFastPacket->mFastPacket_Received_Byte_Size += 7;
			}
			else {
				memcpy(pFastPacket->mMerged_FastPacket + pFastPacket->mFastPacket_Received_Byte_Size, buf + 1, 7);
				pFastPacket->mFastPacket_Received_Byte_Size += 7;

				pFastPacket->using = 0;

//				printf("fast packet (%d) done\r\n",  i);

				mCompletedFastPacketSize = pFastPacket->mFastPacket_Received_Byte_Size;

				memcpy(mCompletedFastPacket, pFastPacket->mMerged_FastPacket, mCompletedFastPacketSize);

				return FASTPACKET_PROC_RESULT_DONE;
			}
		}
	}

	return FASTPACKET_PROC_RESULT_GOING;
}


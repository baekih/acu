/*
 * pgn_060928.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_060928.h"

/* Private typedef -----------------------------------------------------------*/
PGN060928NAME g_PGN060928NAME;
PGN060928NAME mMyPGN060928;

uint64_t my_name_field;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


void InitializeMyNMEAData()
{
	mMyPGN060928.mUnique_Number			= mUnique_Number;
	mMyPGN060928.mManufacturer_Code		= mApp_FEC_Manufacturer_Code;
	mMyPGN060928.mDevice_Instance_Lower = mDevice_Intance & 0x7;
	mMyPGN060928.mDevice_Instance_Upper = (mDevice_Intance & 0xF8) >> 3;
	mMyPGN060928.mDevice_Function		= mDevice_Function;				// 8  bits
	mMyPGN060928.mNMEA_Reserved			= 0;		// 1  bits
	mMyPGN060928.mDevice_Class			= mDevice_Class;					// 7  bits
	mMyPGN060928.mSystem_Instance		= mSystem_Instance;				// 4  bits
	mMyPGN060928.mIndustry_Group		= mIndustry_Group;				// 3  bits
	mMyPGN060928.mISO_Self_Configuration= mISO_Self_Configuration;		// 1  bits
}

void PGN060928_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf, PGN060928NAME *pPGN060928)
{
	uint8_t Index = 0;

//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	receivePacketLength = len;
	memcpy(&receiveNMEAPackets, buf, receivePacketLength);

	pPGN060928->mUnique_Number = Get4ByteUInt(Index) & 0x1FFFFF;
	pPGN060928->mManufacturer_Code = (Get4ByteUInt(Index) >> 21) & 0x7FF;
	Index = Index+4;
	pPGN060928->mDevice_Instance_Lower = Get1ByteUInt(Index) & 0x07;
	pPGN060928->mDevice_Instance_Upper = (Get1ByteUInt(Index) >> 3) & 0x1F;
	Index = Index+1;
	pPGN060928->mDevice_Function = Get1ByteUInt(Index);
	Index = Index+1;
	pPGN060928->mNMEA_Reserved = Get1ByteUInt(Index) & 0x01;
	pPGN060928->mDevice_Class = (Get1ByteUInt(Index) >> 1) & 0x7F;
	Index = Index+1;
	pPGN060928->mSystem_Instance = Get1ByteUInt(Index) & 0x0F;
	pPGN060928->mIndustry_Group = (Get1ByteUInt(Index) >> 4) & 0x07;
	pPGN060928->mISO_Self_Configuration = (Get1ByteUInt(Index) >> 7) & 0x01;
}

void PGN060928_SetInitialField()
{
	InitializeSendNameField();

	Add4ByteUInt(((mMyPGN060928.mManufacturer_Code & 0x7FF) << 21) | (mMyPGN060928.mUnique_Number & 0x1FFFFF) );
	Add1ByteUInt((mMyPGN060928.mDevice_Instance_Lower & 0x07) | ((mMyPGN060928.mDevice_Instance_Upper << 3) & 0xF8));
	Add1ByteUInt(mMyPGN060928.mDevice_Function);
	Add1ByteUInt(((mMyPGN060928.mDevice_Class & 0x7F) << 1) | 0);
	Add1ByteUInt(((mMyPGN060928.mISO_Self_Configuration & 0x01) << 7) | ((mMyPGN060928.mIndustry_Group & 0x07) << 4) | (mMyPGN060928.mSystem_Instance & 0x0F) );

	my_name_field  = *((uint64_t *)sendNMEAPackets);
}

void PGN060928_SendAddressClaim(void)
{
	  NmeaPgn* sendPgnId = getCanId(PGN060928_PRIORITY, PGN060928_PGN, BROADCAST_DESTINATION_ADDR, localSourceAddr);

	  NMEA2000_SendParseMessages(sendPgnId, sendPacketLength, sendNMEAPackets, 0);

	  mAddress_Claiming = true;
	  mLast_Send_Address_Claim_Time = HAL_GetTick();
}

void PGN060928_ProcessNameField()
{
	if (mAddress_Claiming == true)
	{
		printf("%s:%d In Address Claiming... \r\n",__FUNCTION__,__LINE__);
		return;
	}

	NmeaPgn* sendPgnId = getCanId(PGN060928_PRIORITY, PGN060928_PGN, BROADCAST_DESTINATION_ADDR, localSourceAddr);

	NMEA2000_SendParseMessages(sendPgnId, sendPacketLength, sendNMEAPackets, 0);
}


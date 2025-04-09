/*
 * pgn_126720.c
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_126720.h"

/* Private typedef -----------------------------------------------------------*/
uint32_t PGN126720_priority = 7;

/* Private variables ---------------------------------------------------------*/
PGN126720NAME g_PGN126720;

/* Private functions ---------------------------------------------------------*/
PGN126720NAME PGN126720_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	uint16_t Manufacturer_Code;
	uint8_t Industry_Group;
	uint8_t IdentifyCode;
	uint8_t ProperietaryID;
	uint8_t ControlFunction;

	Manufacturer_Code 		=  GetBuf_2ByteUInt(len, 0, buf) & 0x7FF;			// 11 bits
	Industry_Group 			= (GetBuf_2ByteUInt(len, 0, buf) & 0xE000) >> 13;	// 3  bits
	IdentifyCode 			=  GetBuf_1ByteUInt(len, 2, buf);					// 8  bits
	ProperietaryID 			=  GetBuf_1ByteUInt(len, 3, buf);					// 8  bits
	ControlFunction 		=  GetBuf_1ByteUInt(len, 4, buf);    				// 8  bits

	if(g_access_level == 1)// for Boot-loader - AIRMAR: Master Reset
	{
		if(Manufacturer_Code == mBoot_Airmar_Manufacturer_Code
			&& Industry_Group == mIndustry_Group
			&& ProperietaryID == 1)
		{ // for Boot-loader - AIRMAR: Master Reset
		  NVIC_SystemReset();
		}
	}
	else
	{
		if(Manufacturer_Code == mApp_FEC_Manufacturer_Code && Industry_Group == mIndustry_Group)
		{ // for Boot-loader - AIRMAR: Master Reset
			if(IdentifyCode == 1 && ControlFunction == 0)
			{
//				g_spayload_curr = g_spayload_org;

				PGN126720_MemoryClearGroup_SetFieldValue();
				PGN126720_ProcessNameField(pgnId);
			}
			else if(IdentifyCode == 2 && ControlFunction == 0)
			{
				PGN126720_ResetGoup_SetFieldValue();
				PGN126720_ProcessNameField(pgnId);

				HAL_Delay(100);

				NVIC_SystemReset();
			}
			else if(IdentifyCode == 11){
				PGN126720_FactoryTest_SetFieldValue();
				PGN126720_ProcessNameField(pgnId);
			}
		}
	}

	return g_PGN126720;
}

void PGN126720_FactoryTest_SetFieldValue()
{

	InitializeSendNameField();

	Add2ByteUInt(PPGN_FURUNO_MFGCODE);
	Add1ByteUInt(11);
	Add1ByteUInt(2);
	Add1ByteUInt(1);
	Add8ByteUInt(0);
}

void PGN126720_MemoryClearGroup_SetFieldValue()
{

	InitializeSendNameField();

	Add2ByteUInt(PPGN_FURUNO_MFGCODE);
	Add1ByteUInt(1);
	Add1ByteUInt(3);
	Add8ByteUInt(0);
}

void PGN126720_ResetGoup_SetFieldValue()
{

	InitializeSendNameField();

	Add2ByteUInt(PPGN_FURUNO_MFGCODE);
	Add1ByteUInt(2);
	Add1ByteUInt(3);
	Add8ByteUInt(0);
}

void PGN126720_ProcessNameField(NmeaPgn* pgnId)
{
	SendNonSingleFrame(getCanId(PGN126720_priority, 126720, pgnId->mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, REQUEST_MESSAGE_TYPE_FASTPACKET);
}


/*
 * pgn_065285.c
 *
 *  Created on: 2022. 4. 22.
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_065285.h"

/* Private typedef -----------------------------------------------------------*/
PGN065285NAME g_PGN065285NAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN065285_priority = 7;

/* Private functions ---------------------------------------------------------*/
void PGN065285_SetFieldValue(uint16_t _Manufacturer_Code,
							 uint8_t _NMEA_Reserved_1,
							 uint8_t _Industry_Group,
							 uint8_t _Boot_State,
							 uint64_t _NMEA_Reserved_2)
{
	InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add2ByteUInt( ((_Industry_Group & 0x07) << 13) | ((_NMEA_Reserved_1 & 0x3) << 11) | (_Manufacturer_Code & 0x7FF) );
	Add6ByteUInt( ((_NMEA_Reserved_2 & 0x1FFFFFFFFFFF) << 3) | (_Boot_State & 0x07) );
}

void PGN065285_SetInitialField()
{
	uint8_t Boot_State = 0; // temp.

	PGN065285_SetFieldValue(mBoot_Airmar_Manufacturer_Code,
							0x3,
							mIndustry_Group,
							Boot_State,
							0x1FFFFFFFFFFF);
}

void PGN065285_SendNameField()
{
	PGN065285_SetInitialField();

	NMEA2000_SendParseMessages(getCanId(PGN065285_priority, 65285, BROADCAST_DESTINATION_ADDR, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}

/*
 * pgn_065287.c
 *
 *  Created on: 2022. 7. 8.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_065287.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t pgn65287_datalen[5] = {2, 1, 1, 1, 8};

uint32_t PGN065287_priority = 7;

void PGN065287_Airmar_SendNameField(uint32_t pgnid)
{
//	printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

	InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add2ByteUInt( ((mIndustry_Group & 0x7) << 13) | (mManufacturer_Code & 0x7FF) );
	Add1ByteUInt( 3 );
	Add1ByteUInt( 0xF8 | g_access_level );

	uint32_t access_key = 0;

	switch(g_access_level){
		case 0 : access_key = 0;
			break;
		case 1 : access_key = 0x12345678;
			break;
		case 5 : access_key = g_access_seed;
			break;
	}

	Add4ByteUInt( access_key );

	NMEA2000_SendParseMessages(NmeaPGN_CanID(PGN065287_priority, 65287, get_R_SA(), localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}

uint32_t calculate_level_2_key (uint32_t seed, uint32_t mask, uint8_t iterations)
{
	uint8_t i;
	uint32_t shift_reg;
	uint32_t temp;

	shift_reg = seed ^ mask;

	if (shift_reg == 0) /* input must be nonzero */
	{
		shift_reg = 0xFFFFFFFF;
	}

	for (i=0; i<iterations; i++) {
		temp = shift_reg >> 31;
		temp ^= shift_reg >> 6;
		temp ^= shift_reg >> 5;
		temp ^= shift_reg >> 1;
		temp &= 0x00000001;

		shift_reg = (temp << 31) | (shift_reg >> 1);
	}

	return shift_reg;
}

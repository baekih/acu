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

/* typedef -----------------------------------------------------------*/
PGN065285NAME g_PGN065285NAME;

/* functions ---------------------------------------------------------*/
void PGN065285_SetFieldValue(uint16_t _Manufacturer_Code,
                             uint8_t _NMEA_Reserved_1,
                             uint8_t _Industry_Group,
                             uint8_t _Boot_State,
                             uint64_t _NMEA_Reserved_2)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);
	InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add2ByteUInt( ((_Industry_Group & 0x07) << 13) | ((_NMEA_Reserved_1 & 0x3) << 11) | (_Manufacturer_Code & 0x7FF) );
	Add6ByteUInt( ((_NMEA_Reserved_2 & 0x1FFFFFFFFFFF) << 3) | (_Boot_State & 0x07) );
}

void PGN065285_SetInitialField()
{
//  printf("%s() MFGCode[%ld] INDGrp[%ld] BOOTStat[%d]...\r\n",__FUNCTION__, mManufacturer_Code, mIndustry_Group, BOOT_STATE);
	PGN065285_SetFieldValue(mManufacturer_Code,
                          0x3,
                          mIndustry_Group,
                          BOOT_STATE,
                          0x1FFFFFFFFFFF);
}

void PGN065285_SendNameField()
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	NMEA2000_SendParseMessages(NmeaPGN_CanID(PGN065285_PRIORITY, PGN065285_NUM, BROADCAST_DESTINATION_ADDR, localSourceAddr),
	                           sendPacketLength, sendNMEAPackets, 0);
}

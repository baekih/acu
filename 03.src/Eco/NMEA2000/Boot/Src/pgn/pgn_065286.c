/*
 * pgn_065286.c
 *
 *  Created on: 2022. 4. 22.
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_065286.h"
#include "pgn_065285.h"

/* typedef -----------------------------------------------------------*/
PGN065286NAME g_PGN065286NAME;

/* functions ---------------------------------------------------------*/
void PGN065286_GetFieldValue(uint32_t pgnid, uint8_t len, uint8_t *buf)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	sendPacketLength = len;
	memcpy(&sendPacketLength, buf, sendPacketLength);

	g_PGN065286NAME.mManufacturer_Code = Get2ByteUInt(0) & 0x7FF;
	g_PGN065286NAME.mIndustry_Group = (Get2ByteUInt(0) >> 11) & 0x07;

//	printf("%s() mMFGCode[%ld] mINDGrp[%ld]\r\n",__FUNCTION__, g_PGN065286NAME.mManufacturer_Code, g_PGN065286NAME.mIndustry_Group);
}

uint32_t PGN065286_ProcessNameField(uint32_t pgnid, uint8_t len, uint8_t *buf)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	PGN065285_SetInitialField();
	PGN065285_SendNameField();

	return 0;
}


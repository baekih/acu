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

/* Private typedef -----------------------------------------------------------*/
PGN065286NAME g_PGN065286NAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN065286_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN065286_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	uint8_t Index = 0;

#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	receivePacketLength = len;
	memcpy(&receiveNMEAPackets, buf, receivePacketLength);

	g_PGN065286NAME.mManufacturer_Code = Get2ByteUInt(Index) & 0x7FF;
	g_PGN065286NAME.mIndustry_Group = (Get2ByteUInt(Index) >> 11) & 0x07;
	Index = Index+2;

#if PRINTF_DEBUG_PGN065286
	printf(" [mManufacturer_Code = %ld] !!\r\n", g_PGN065286NAME.mManufacturer_Code);
	printf(" [mIndustry_Group = %ld] !!\r\n", g_PGN065286NAME.mIndustry_Group);
#endif
}

void PGN065286_ProcessNameField()
{
	PGN065285_SendNameField();
}


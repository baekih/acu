/*
 * pgn_060416.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_060416.h"
#include "pgn_060416_rts.h"
#include "pgn_060416_cts.h"
#include "pgn_060416_bam.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416NAME g_PGN060416NAME;

/* Private variables ---------------------------------------------------------*/
uint32_t FunctionCodeRTS = 16;		// 0x10
uint32_t FunctionCodeCTS = 17;		// 0x11
uint32_t FunctionCodeEOM = 19;		// 0x13
uint32_t FunctionCodeBAM = 32;		// 0x20
uint32_t FunctionCodeABORT = 255;	// 0xFF

uint32_t PGN060416_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN060416_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	uint8_t Index = 0;

#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	receivePacketLength = len;
	memcpy(&receiveNMEAPackets, buf, receivePacketLength);

	g_PGN060416NAME.mGroup_Function_Code = Get1ByteUInt(Index);
}

uint32_t PGN060416_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	if(g_PGN060416NAME.mGroup_Function_Code == FunctionCodeRTS) {
		PGN060416RTS_GetFieldValue(pgnId, len, buf);
		PGN060416RTS_ProcessNameField(pgnId, len, buf);
	}
	else if(g_PGN060416NAME.mGroup_Function_Code == FunctionCodeCTS) {
		PGN060416CTS_GetFieldValue(pgnId, len, buf);
		PGN060416CTS_ProcessNameField(pgnId, len, buf);
	}
	else if(g_PGN060416NAME.mGroup_Function_Code == FunctionCodeEOM) {

	}
	else if(g_PGN060416NAME.mGroup_Function_Code == FunctionCodeABORT) {

	}
	else if(g_PGN060416NAME.mGroup_Function_Code == FunctionCodeBAM) {
		PGN060416BAM_GetFieldValue(pgnId, len, buf);
		PGN060416BAM_ProcessNameField(pgnId, len, buf);
	}


	return 0;
}


/*
 * pgn_061184_stgf.c
 *
 *  Created on: Feb 22, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_061184_stgf.h"

/* Private typedef -----------------------------------------------------------*/
PGN061184STGFNAME g_PGN061184STGFNAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN061184STGF_priority = 7;

/* Private functions ---------------------------------------------------------*/
void PGN061184STGF_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	g_PGN061184STGFNAME.m61184Manufacturer_Code 	=  GetBuf_2ByteUInt(len, 0, buf) & 0x7FF;
	g_PGN061184STGFNAME.m61184Industry_Group 		= (GetBuf_2ByteUInt(len, 0, buf) & 0xE000) >> 13;
	g_PGN061184STGFNAME.m61184Identification_Code 	=  GetBuf_1ByteUInt(len, 2, buf);
	g_PGN061184STGFNAME.m61184Control_Function 		=  GetBuf_1ByteUInt(len, 3, buf);


#if 0
	printf(" [m61184Control_Function = %ld] !!\r\n", g_PGN061184STGFNAME.m61184Manufacturer_Code);
	printf(" [m61184Industry_Group = %ld] !!\r\n", g_PGN061184STGFNAME.m61184Industry_Group);
	printf(" [m61184Identification_Code = %ld] !!\r\n", g_PGN061184STGFNAME.m61184Identification_Code);
	printf(" [m61184Control_Function = %ld] !!\r\n", g_PGN061184STGFNAME.m61184Control_Function);
#endif
}

void PGN061184STGF_SetInitialField(uint32_t _61184Manufacturer_Code,
								   uint32_t _61184Industry_Group,
								   uint32_t _61184Identification_Code,
								   uint32_t _61184Control_Function)
{
//	printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

	InitializeSendNameField();

	Add2ByteUInt(PPGN_FURUNO_MFGCODE);
	Add1ByteUInt( _61184Identification_Code );
	Add1ByteUInt( _61184Control_Function );
	Add4ByteUInt( 0xFFFFFFFF );
}

void PGN061184STGF_SendNameField(uint8_t mSA)
{
//	printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);
	PGN061184STGF_SetInitialField(mApp_FEC_Manufacturer_Code, mIndustry_Group, 0, 3);

	NMEA2000_SendParseMessages(getCanId(PGN061184STGF_priority, 61184, mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}

void  PGN061184STGF_ProcessNameField(uint8_t mSA)
{
	/*	uint8_t rcvSourceAddr = mSA;

	g_spayload_curr.keypad_en = (g_spayload_curr.keypad_en & 0x01) | (rcvSourceAddr & 0xF0);
	g_spayload_curr.keybeep_en = ((rcvSourceAddr & 0x0F) << 4) | (g_spayload_curr.keybeep_en & 0x03);

	g_spayload_curr.keypad_en |= SELF_TEST_BOOT_FLAG;*/
}

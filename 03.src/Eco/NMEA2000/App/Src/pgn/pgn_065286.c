/*
 * pgn_065286.c
 *
 *  Created on: 2022. 4. 22.
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/
PGN065286NAME g_PGN065286NAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN065286_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN065286NAME.mManufacturer_Code =  GetBuf_2ByteUInt(len, 0, buf) & 0x7FF;
    g_PGN065286NAME.mIndustry_Group    = (GetBuf_2ByteUInt(len, 0, buf) >> 11) & 0x07;

#if 0
	printf(" [mManufacturer_Code = %ld] !!\r\n", g_PGN065286NAME.mManufacturer_Code);
	printf(" [mIndustry_Group = %ld] !!\r\n", g_PGN065286NAME.mIndustry_Group);
#endif
}

void PGN065286_ProcessNameField()
{
    PGN065285_SendNameField();
}


/*
 * pgn_126464.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_126464.h"

/* define ------------------------------------------------------------*/
#define PGN126464_NUM         126464
#define PGN126464_PRIORITY    3

/* typedef -----------------------------------------------------------*/
PGN126464NAME g_PGN126464NAME;

/* functions ---------------------------------------------------------*/
void PGN126464_SetFieldValue(uint32_t _PGN_Group_Function_Code,
                             uint32_t _First_PGN_supported[],
                             uint8_t _PGN_List_Unit)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);
  InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add1ByteUInt( _PGN_Group_Function_Code );
	for(uint8_t i = 0; i < _PGN_List_Unit; i++) Add3ByteUInt( _First_PGN_supported[i] );

#if 0
	printf(" [_PGN_Group_Function_Code = %ld] !!\r\n", _PGN_Group_Function_Code);
	printf(" [_First_PGN_supported = ");		for (uint8_t i = 0; i < _PGN_List_Unit; i++)	printf("0x%06lx ", _First_PGN_supported[i]);	printf("] !!\r\n");
	printf(" [_PGN_List_Unit = %d] !!\r\n", _PGN_List_Unit);
#endif
}

void PGN126464_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

#if 0
	printf(" [DataLenSend = %ld] !!\r\n", sendPacketLength);
	printf(" [mNMEA2000SendNameField = ");	for (uint8_t i = 0; i < sendPacketLength; i++)	printf("0x%02x ", sendNMEAPackets[i]);	printf("] !!\r\n");
#endif

	SendNonSingleFrame(NmeaPGN_CanID(PGN126464_PRIORITY, PGN126464_NUM, get_R_SA(), localSourceAddr),
	                   sendPacketLength, sendNMEAPackets, messagetype);
}


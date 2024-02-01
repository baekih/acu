/*
 * pgn_059392.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_059392.h"

/* Private typedef -----------------------------------------------------------*/
PGN059392NAME g_PGN059392NAME;

/* Private variables ---------------------------------------------------------*/
uint8_t POSITIVIE_ACKNOWLEDGMENT		= 0x00;
uint8_t NEGATIVIE_ACKNOWLEDGMENT		= 0x01;
uint32_t PGN_SUPPORTED_ACCESS_DENIED	= 0x02;

uint32_t PGN059392_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN059392_SetFieldValue(uint8_t _Control_Byte,
							 uint8_t _Group_Function_Value,
							 uint32_t _NMEA_Reserved,
							 uint32_t _PGN_of_Requested_Information)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeSendNameBitPosition();
	InitializeSendNameField();
	
	Add1ByteUInt( _Control_Byte );
	Add1ByteUInt( _Group_Function_Value );
	Add3ByteUInt( _NMEA_Reserved );
	Add3ByteUInt( _PGN_of_Requested_Information );

#if PRINTF_DEBUG_PGN059392_NON
	printf(" [_Control_Byte = %ld] !!\r\n", _Control_Byte);
	printf(" [_Group_Function_Value = %ld] !!\r\n", _Group_Function_Value);
	printf(" [_NMEA_Reserved = %ld] !!\r\n", _NMEA_Reserved);
	printf(" [_PGN_of_Requested_Information = %ld] !!\r\n", _PGN_of_Requested_Information);
#endif
}

void PGN059392_SendNameField(NmeaPgn* pgnId)
{
	NMEA2000_SendParseMessages(getCanId(PGN059392_priority, 59392, pgnId->mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}


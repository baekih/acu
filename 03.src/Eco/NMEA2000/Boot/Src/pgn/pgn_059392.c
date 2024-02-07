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

/* define ------------------------------------------------------------*/
#define PGN059392_NUM         59392
#define PGN059392_PRIORITY    6

/* typedef -----------------------------------------------------------*/
PGN059392NAME g_PGN059392NAME;

/* functions ---------------------------------------------------------*/
void PGN059392_SetFieldValue(uint32_t _Control_Byte,
                             uint32_t _Group_Function_Value,
                             uint32_t _NMEA_Reserved,
                             uint32_t _PGN_of_Requested_Information)
{
//	printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	InitializeSendNameBitPosition();
	InitializeSendNameField();
	
	Add1ByteUInt( _Control_Byte );
	Add1ByteUInt( _Group_Function_Value );
	Add3ByteUInt( _NMEA_Reserved );
	Add3ByteUInt( _PGN_of_Requested_Information );

#if 0
	printf(" ControlByte[%ld] GroupFunctionValue[%ld] PGN_of_Requested_Information[%ld] !!\r\n",
	       _Control_Byte, _Group_Function_Value, _PGN_of_Requested_Information);
#endif
}

void PGN059392_SendNameField()
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	NMEA2000_SendParseMessages(NmeaPGN_CanID(PGN059392_PRIORITY, PGN059392_NUM, get_R_SA(), localSourceAddr),
	                           sendPacketLength, sendNMEAPackets, 0);
}


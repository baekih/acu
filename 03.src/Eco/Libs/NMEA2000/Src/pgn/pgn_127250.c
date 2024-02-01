/*
 * pgn_127250.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_127250.h"

#include <gui/common/DataBase.hpp>
#include "validate_data.h"

uint8_t 	SequenceID;                 // 8  bit
uint16_t 	HeadingSensorReading;       // 16 bit
uint16_t 	Deviation;                  // 16 bit
uint16_t 	Variation;                  // 16 bit
uint8_t 	HeadingSensorReference;     // 2  bit
uint8_t 	NMEAReserved;               // 6  bit

void PGN127250_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{

	SequenceID             =  GetBuf_1ByteUInt(len, 0, buf); 	// 8  bits
	HeadingSensorReading   =  GetBuf_2ByteUInt(len, 1, buf); 	// 16 bits
	Deviation              =  GetBuf_2ByteUInt(len, 3, buf); 	// 16 bits
	Variation              =  GetBuf_2ByteUInt(len, 5, buf); 	// 16 bits
	HeadingSensorReference = (GetBuf_1ByteUInt(len, 7, buf) & 0x03); // 2  bits
	NMEAReserved           = (GetBuf_1ByteUInt(len, 7, buf) & 0xC0) >> 6;		// 6  bits

/*	printf("(%d)", pgnId->mSA);
	printf("Receive Vessel Heading - %3d,", SequenceID);
	printf("mHeadingSensorReading(%1.4f),", (float)HeadingSensorReading/10000);
	printf("mVariation(%1.4f),", (float)Deviation/10000);
	printf("mDeviation(%1.4f),", (float)Variation/10000);
	printf("mHeadingSensorReference(%d)\n", HeadingSensorReference);
	printf("mHeadingSensorReading : %3.1f,", (((float)HeadingSensorReading/10000) / (2*M_PI)) * 360);
	printf("mDeviation : %3.1f,", (((float)Deviation/10000) / M_PI) * 180);
	printf("mVariation : %3.1f\n", (((float)Variation/10000) / M_PI) * 180);*/

	setHDGValue( (float)HeadingSensorReading / 10000 );
}

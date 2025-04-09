/*
 * pgn_130310.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "math.h"

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_130310.h"

#include <gui/common/DataBase.hpp>
#include <gui/common/validate_data.h>

uint16_t 	mWaterTemp;     		// 16 bit
uint16_t 	mOutsideAmbientAirTemp; // 16 bit
uint16_t 	mAtmosphericPressure;   // 16 bit

void PGN130310_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{

//	mSequenceID      		=  GetBuf_1ByteUInt(len, 0, buf); 				// 8  bit
	mWaterTemp      		=  GetBuf_2ByteUInt(len, 1, buf); 				// 16 bit
	mOutsideAmbientAirTemp  =  GetBuf_2ByteUInt(len, 3, buf); 				// 16 bit
	mAtmosphericPressure  	=  GetBuf_2ByteUInt(len, 5, buf); 				// 16 bit

/*	printf("Environmental Parameters - %3.2f 'C:%3.2f 'C:%d Pa\r\n",
			(double)mWaterTemp/100, (double)mOutsideAmbientAirTemp/100,
			mAtmosphericPressure*100);*/

	if(isValidWaterTemp(mWaterTemp)) {
		setWTempValue((float)mWaterTemp / 100, UNIT_TEMP_CELSIUS);
	}
	if(isValidPressure(mAtmosphericPressure)) {
//		NMEADataParser.dataBaromPress.SetPressValuesByN2K(this, 130310, mAtmosphericPressure);
	}
}

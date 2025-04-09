/*
 * pgn_130823.c
 *
 *  Created on: 2022. 7. 11.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_130823.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN130823_ProcessNameField(NmeaPgn* pgnId)
{

	InitializeSendNameField();

	Add2ByteUInt(PPGN_FURUNO_MFGCODE);
	Add1ByteUInt(0);
	Add1ByteUInt(0);
	Add4ByteUInt(0xFFFFFFFF);

	SendNonSingleFrame(getCanId(PGN130823_PRIORITY, PGN130823_PGN, pgnId->mSA, localSourceAddr), sendPacketLength,
					 sendNMEAPackets,
					 REQUEST_MESSAGE_TYPE_FASTPACKET);

	return;
}

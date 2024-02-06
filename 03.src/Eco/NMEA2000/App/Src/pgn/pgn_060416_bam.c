/*
 * pgn_060416_bam.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_060160.h"

#include "pgn_060416_bam.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416BAMNAME g_PGN060416BAMNAME;

/* Private variables ---------------------------------------------------------*/
uint32_t PGN060416BAM_priority = 6;

/* Private functions ---------------------------------------------------------*/
void PGN060416BAM_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	uint8_t Index = 0;

#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	receivePacketLength = len;
	memcpy(&receiveNMEAPackets, buf, receivePacketLength);

	g_PGN060416BAMNAME.mBAM_Group_Function_Code = Get1ByteUInt(Index);
	Index = Index+1;
	g_PGN060416BAMNAME.mTotal_message_size_bytes = Get2ByteUInt(Index);
	Index = Index+2;
	g_PGN060416BAMNAME.mTotal_number_of_frames_to_be_transmitted = Get1ByteUInt(Index);
	Index = Index+1;
	g_PGN060416BAMNAME.mNMEA_Reserved = Get1ByteUInt(Index);
	Index = Index+1;
	g_PGN060416BAMNAME.mPGN_of_multipacket_message = Get3ByteUInt(Index);

#if PRINTF_DEBUG_PGN060416_BAM_NON
	printf(" [mBAM_Group_Function_Code = %ld] !!\r\n", g_PGN060416BAMNAME.mBAM_Group_Function_Code);
	printf(" [mTotal_message_size_bytes = %ld] !!\r\n", g_PGN060416BAMNAME.mTotal_message_size_bytes);
	printf(" [mTotal_number_of_frames_to_be_transmitted = %ld] !!\r\n", g_PGN060416BAMNAME.mTotal_number_of_frames_to_be_transmitted);
	printf(" [mPGN_of_multipacket_message = %ld] !!\r\n", g_PGN060416BAMNAME.mPGN_of_multipacket_message);
#endif
}

uint32_t PGN060416BAM_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
	PGN060160_InitializeReceiveBAMMulitiPacket(g_PGN060416BAMNAME.mTotal_message_size_bytes,
									 g_PGN060416BAMNAME.mTotal_number_of_frames_to_be_transmitted,
									 g_PGN060416BAMNAME.mPGN_of_multipacket_message,
									 pgnId->mSA,
									 pgnId->mPS);

	return 0;
}

void PGN060416BAM_SetFieldValue(uint32_t _BAM_Group_Function_Code,
								uint32_t _Total_message_size_bytes,
								uint32_t _Total_number_of_frames_to_be_transmitted,
								uint32_t _NMEA_Reserved,
								uint32_t _PGN_of_multipacket_message)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add1ByteUInt( _BAM_Group_Function_Code );
	Add2ByteUInt( _Total_message_size_bytes );
	Add1ByteUInt( _Total_number_of_frames_to_be_transmitted );
	Add1ByteUInt( _NMEA_Reserved );
	Add3ByteUInt( _PGN_of_multipacket_message );

#if PRINTF_DEBUG_PGN060416_BAM_NON
	printf(" [_BAM_Group_Function_Code = %ld] !!\r\n", _BAM_Group_Function_Code);
	printf(" [_Total_message_size_bytes = %ld] !!\r\n", _Total_message_size_bytes);
	printf(" [_Total_number_of_frames_to_be_transmitted = %ld] !!\r\n", _Total_number_of_frames_to_be_transmitted);
	printf(" [_PGN_of_multipacket_message = %ld] !!\r\n", _PGN_of_multipacket_message);
#endif
}

void PGN060416BAM_SendNameField(NmeaPgn* sendPgn)
{
	NMEA2000_SendParseMessages(sendPgn, sendPacketLength, sendNMEAPackets, 0);
}


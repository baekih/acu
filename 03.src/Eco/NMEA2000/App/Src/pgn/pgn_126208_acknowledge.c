/*
 * pgn_126208_acknowledge.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "multipacketdata.h"

#include "pgn_126208_acknowledge.h"

/* Private typedef -----------------------------------------------------------*/
PGN126208ACKNOWLEDGENAME g_PGN126208ACKNOWLEDGENAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN126208ACKNOWLEDGE_SetFieldValue(uint32_t _Acknowledgment_Group_Function_Code,
                                        uint32_t _Requested_or_Commanded_PGN_being_acknowledged,
                                        uint32_t _PGN_error_code,
                                        uint32_t _Transmission_Interval_Priority_error_code,
                                        uint32_t _Number_of_Requested_or_Commanded_Parameters,
                                        uint8_t* _First_parameter_error_code)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif

	InitializeSendNameBitPosition();
	InitializeSendNameField();

	Add1ByteUInt( _Acknowledgment_Group_Function_Code );
	Add3ByteUInt( _Requested_or_Commanded_PGN_being_acknowledged );
	Add1ByteUInt( ((_Transmission_Interval_Priority_error_code & 0x0F) << 4) | (_PGN_error_code & 0x0F) );
	Add1ByteUInt( _Number_of_Requested_or_Commanded_Parameters );

	if (_Number_of_Requested_or_Commanded_Parameters == 1) {
		Add1ByteUInt( (_First_parameter_error_code[0] & 0x0F) );
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 2) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 3) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
		Add1ByteUInt( (_First_parameter_error_code[2] & 0x0F) );
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 4) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[3] & 0x0F) << 4) | (_First_parameter_error_code[2] & 0x0F));
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 5) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[3] & 0x0F) << 4) | (_First_parameter_error_code[2] & 0x0F));
		Add1ByteUInt( (_First_parameter_error_code[4] & 0x0F) );
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 6) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[3] & 0x0F) << 4) | (_First_parameter_error_code[2] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[5] & 0x0F) << 4) | (_First_parameter_error_code[4] & 0x0F));
	} else 	if (_Number_of_Requested_or_Commanded_Parameters == 7) {
		Add1ByteUInt( ((_First_parameter_error_code[1] & 0x0F) << 4) | (_First_parameter_error_code[0] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[3] & 0x0F) << 4) | (_First_parameter_error_code[2] & 0x0F));
		Add1ByteUInt( ((_First_parameter_error_code[5] & 0x0F) << 4) | (_First_parameter_error_code[4] & 0x0F));
		Add1ByteUInt( (_First_parameter_error_code[6] & 0x0F) );
	}

#if PRINTF_DEBUG_PGN126208ACKNOWLEDGE_NON
	printf(" [_Acknowledgment_Group_Function_Code = %ld] !!\r\n", _Acknowledgment_Group_Function_Code);
	printf(" [_Requested_or_Commanded_PGN_being_acknowledged = %ld] !!\r\n", _Requested_or_Commanded_PGN_being_acknowledged);
	printf(" [_PGN_error_code = %ld] !!\r\n", _PGN_error_code);
	printf(" [_Transmission_Interval_Priority_error_code = %ld] !!\r\n", _Transmission_Interval_Priority_error_code);
	printf(" [_Number_of_Requested_or_Commanded_Parameters = %ld] !!\r\n", _Number_of_Requested_or_Commanded_Parameters);
	printf(" [_First_parameter_error_code = ");	for (int i = 0; i < _Number_of_Requested_or_Commanded_Parameters; i++)	printf("0x%02x ", _First_parameter_error_code[i]);	printf("]\r\n");
#endif
}

void PGN126208ACKNOWLEDGE_ProcessNameField(NmeaPgn* pgnId, uint8_t *buf, uint32_t messagetype)
{
#if PRINTF_DEBUG_FUNC_LINE_NON
	printf("===>> Func:%s, Line:%d !!\r\n", __FUNCTION__, __LINE__);
#endif
	SendNonSingleFrame(getCanId(PGN126208ACKNOWLEDGE_PRIORITY, PGN126208_PGN, pgnId->mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, messagetype);
}


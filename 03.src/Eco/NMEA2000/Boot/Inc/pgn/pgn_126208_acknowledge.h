/*
 * pgn_126208_acknowledge.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126208_ACKNOWLEDGE_H_
#define NMEA2000_INC_PGN_PGN_126208_ACKNOWLEDGE_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define PGN126208_ACK_ERRORCODE_NO_ERROR_ACK                       0x0
#define PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT                    0x1
#define PGN126208_ACK_ERRORCODE_PGN_TEMPORARILY_NOT_AVAILABLE      0x2
#define PGN126208_ACK_ERRORCODE_ACCESS_DENIED                      0x3
#define PGN126208_ACK_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED          0x4
#define PGN126208_ACK_ERRORCODE_DEFINER_TAG_NOT_SUPPORTED          0x5
#define PGN126208_ACK_ERRORCODE_READ_WRITE_NOT_SUPPORTED           0x6

#define PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK                    0x0
#define PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_PRIORITY_NOT_SUP    0x1
#define PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_CALC_ERROR          0x2
#define PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED                   0x3
#define PGN126208_TX_ACK_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED       0x4

#define PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK                     0x0
#define PGN126208_PARAM_ERRORCODE_INVALID_REQUEST_CMD_PARAM_FLD    0x1
#define PGN126208_PARAM_ERRORCODE_TEMPORARILY_AVAILABLE_COMPLY     0x2
#define PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM               0x3
#define PGN126208_PARAM_ERRORCODE_ACCESS_DENIED                    0x4
#define PGN126208_PARAM_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED        0x5
#define PGN126208_PARAM_ERRORCODE_READ_WRITE_NOT_SUPPORTED         0x6

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN126208ACKNOWLEDGENAME
{
	uint32_t mAcknowledgment_Group_Function_Code;				// 8  bits
	uint32_t mRequested_or_Commanded_PGN_being_acknowledged;	// 24 bits
	uint32_t mPGN_error_code;									// 4  bits
	uint32_t mTransmission_Interval_Priority_error_code;		// 4  bits
	uint32_t mNumber_of_Requested_or_Commanded_Parameters;		// 8  bits
	uint8_t* mFirst_parameter_error_code;						// ?  bits
} PGN126208ACKNOWLEDGENAME;

/* functions --------------------------------------------------------*/
void PGN126208ACKNOWLEDGE_SetFieldValue(uint32_t _Acknowledgment_Group_Function_Code,
                                        uint32_t _Requested_or_Commanded_PGN_being_acknowledged,
                                        uint32_t _PGN_error_code,
                                        uint32_t _Transmission_Interval_Priority_error_code,
                                        uint32_t _Number_of_Requested_or_Commanded_Parameters,
                                        uint8_t* _First_parameter_error_code);
void PGN126208ACKNOWLEDGE_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);

#endif /* NMEA2000_INC_PGN_PGN_126208_ACKNOWLEDGE_H_ */

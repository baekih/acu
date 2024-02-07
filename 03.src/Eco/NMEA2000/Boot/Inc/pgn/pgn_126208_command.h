/*
 * pgn_126208_command.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126208_COMMAND_H_
#define NMEA2000_INC_PGN_PGN_126208_COMMAND_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* typedef -----------------------------------------------------------*/
typedef struct __PGN126208COMMANDNAME
{
	uint32_t mCommand_Group_Function_Code;							// 8  bits
	uint32_t mCommanded_PGN;										// 24 bits
	uint32_t mPriority_Setting;										// 4  bits
	uint32_t mNMEA_Reserved;										// 4  bits
	uint32_t mNumber_of_Pairs_of_Commanded_Parameters_to_follow;	// 8  bits
	uint32_t mField_number_of_first_commanded_parameter;			// 8  bits
	uint32_t mValue_of_first_command_parameter;						// ?  bits
	uint32_t mVariable_Number_of_fields_Field_number_6_repeated;	// 8  bits
	uint32_t mVariable_Number_of_fields_Field_number_7_repeated;	// ?  bits
} PGN126208COMMANDNAME;

/* functions --------------------------------------------------------*/
void PGN126208COMMAND_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);
void PGN126208COMMAND_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);

#endif /* NMEA2000_INC_PGN_PGN_126208_COMMAND_H_ */

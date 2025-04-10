/*
 * pgn_126464.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126464_H_
#define NMEA2000_INC_PGN_PGN_126464_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN126464NAME
{
    uint32_t mPGN_Group_Function_Code;                              // 8  bits
    uint32_t mFirst_PGN_supported;                                  // 24 bits
    uint32_t mVariable_Number_of_fields_Field_number_2_repeated;    // 24 bits
} PGN126464NAME;

/* Exported variables ---------------------------------------------------------*/
extern uint32_t PGN_GROUP_TRANSMIT_LIST_MESSAGE;
extern uint32_t PGN_GROUP_RECEIVE_LIST_MESSAGE;

/* Exported functions --------------------------------------------------------*/
extern void PGN126464_SetFieldValue(uint32_t _PGN_Group_Function_Code,
                                    uint32_t _First_PGN_supported[],
                                    uint8_t _PGN_List_Unit);
extern void PGN126464_ProcessNameField(NmeaPgn* pgnId, uint32_t messagetype);


#endif /* NMEA2000_INC_PGN_PGN_126464_H_ */

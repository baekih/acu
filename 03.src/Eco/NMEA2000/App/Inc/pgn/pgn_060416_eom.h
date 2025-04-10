/*
 * pgn_060416_eom.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_EOM_H_
#define NMEA2000_INC_PGN_PGN_060416_EOM_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern uint32_t PGN060416EOM_priority;

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416EOMNAME
{
    uint32_t mEOM_Group_Function_Code;                  // 8  bits
    uint32_t mTotal_message_size_bytes;                 // 16 bits
    uint32_t mTotal_number_of_frames_received;          // 8  bits
    uint32_t mNMEA_Reserved;                            // 8 bits
    uint32_t mPGN_of_multipacket_message;               // 24 bits
} PGN060416EOMNAME;

/* Exported functions --------------------------------------------------------*/
extern void PGN060416EOM_SetFieldValue(uint32_t _EOM_Group_Function_Code,
                                       uint32_t _Total_message_size_bytes,
                                       uint32_t _Total_number_of_frames_received,
                                       uint32_t _NMEA_Reserved,
                                       uint32_t _PGN_of_multipacket_message);
extern void PGN060416EOM_SendNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_060416_EOM_H_ */

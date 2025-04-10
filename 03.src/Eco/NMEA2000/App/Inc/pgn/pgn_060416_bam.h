/*
 * pgn_060416_bam.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_BAM_H_
#define NMEA2000_INC_PGN_PGN_060416_BAM_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416BAMNAME
{
    uint32_t mBAM_Group_Function_Code;                  // 8  bits
    uint32_t mTotal_message_size_bytes;                 // 16 bits
    uint32_t mTotal_number_of_frames_to_be_transmitted; // 8  bits
    uint32_t mNMEA_Reserved;                            // 8 bits
    uint32_t mPGN_of_multipacket_message;               // 24 bits
} PGN060416BAMNAME;

extern uint32_t PGN060416BAM_priority;

/* Exported functions --------------------------------------------------------*/
extern void PGN060416BAM_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
extern uint32_t PGN060416BAM_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);

extern void PGN060416BAM_SetFieldValue(uint32_t _BAM_Group_Function_Code,
                                       uint32_t _Total_message_size_bytes,
                                       uint32_t _Total_number_of_frames_to_be_transmitted,
                                       uint32_t _NMEA_Reserved,
                                       uint32_t _PGN_of_multipacket_message);
extern void PGN060416BAM_SendNameField();


#endif /* NMEA2000_INC_PGN_PGN_060416_BAM_H_ */

/*
 * pgn_060416_rts.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_RTS_H_
#define NMEA2000_INC_PGN_PGN_060416_RTS_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN060416_RTS_PGN                       60416
#define PGN060416_RTS_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416RTSNAME
{
    uint32_t mRTS_Group_Function_Code;                  // 8  bits
    uint32_t mTotal_message_size_bytes;                 // 16 bits
    uint32_t mTotal_number_of_frames_to_be_transmitted; // 8  bits
    uint32_t mNMEA_Reserved;                            // 8  bits
    uint32_t mPGN_of_multipacket_message;               // 24 bits
} PGN060416RTSNAME;

/* Exported functions --------------------------------------------------------*/
void PGN060416RTS_SetFieldValue(uint32_t _RTS_Group_Function_Code,
                                uint32_t _Total_message_size_bytes,
                                uint32_t _Total_number_of_frames_to_be_transmitted,
                                uint32_t _NMEA_Reserved,
                                uint32_t _PGN_of_multipacket_message);
void PGN060416RTS_SendNameField(NmeaPgn* pgnId);

void PGN060416RTS_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
uint32_t PGN060416RTS_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_060416_RTS_H_ */

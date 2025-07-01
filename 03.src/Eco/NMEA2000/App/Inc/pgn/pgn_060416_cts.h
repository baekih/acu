/*
 * pgn_060416_cts.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_CTS_H_
#define NMEA2000_INC_PGN_PGN_060416_CTS_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN060416_CTS_PGN                       60416
#define PGN060416_CTS_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416CTSNAME
{
    uint32_t mCTS_Group_Function_Code;                  // 8  bits
    uint32_t mNumber_of_frames_that_can_be_sent;        // 8  bits
    uint32_t mNumber_of_next_frame_to_be_transmitted;   // 8  bits
    uint32_t mNMEA_Reserved;                            // 16 bits
    uint32_t mPGN_of_multipacket_message;               // 24 bits
} PGN060416CTSNAME;

/* Exported functions --------------------------------------------------------*/
void PGN060416CTS_SetFieldValue(uint32_t _CTS_Group_Function_Code,
                                uint32_t _Number_of_frames_that_can_be_sent,
                                uint32_t _Number_of_next_frame_to_be_transmitted,
                                uint32_t _NMEA_Reserved,
                                uint32_t _PGN_of_multipacket_message);
void PGN060416CTS_SendNameField(NmeaPgn* pgnId);

void PGN060416CTS_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN060416CTS_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_060416_CTS_H_ */

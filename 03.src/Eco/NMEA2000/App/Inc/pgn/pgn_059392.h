/*
 * pgn_059392.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_059392_H_
#define NMEA2000_INC_PGN_PGN_059392_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN059392_PGN                       59392
#define PGN059392_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN059392NAME
{
    uint8_t mControl_Byte;                      // 8  bits
    uint8_t mGroup_Function_Value;              // 8  bits
    uint32_t mNMEA_Reserved;                    // 24 bits
    uint32_t mPGN_of_Requested_Information;     // 24 bits
} PGN059392NAME;

/* Exported variables ---------------------------------------------------------*/
extern uint8_t POSITIVIE_ACKNOWLEDGMENT;
extern uint8_t NEGATIVIE_ACKNOWLEDGMENT;
extern uint32_t PGN_SUPPORTED_ACCESS_DENIED;

/* Exported functions --------------------------------------------------------*/
void PGN059392_SetFieldValue(uint8_t _Control_Byte,
                             uint8_t _Group_Function_Value,
                             uint32_t _NMEA_Reserved,
                             uint32_t _PGN_of_Requested_Information);
void PGN059392_SendNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_059392_H_ */

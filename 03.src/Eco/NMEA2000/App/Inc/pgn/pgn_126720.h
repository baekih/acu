/*
 * pgn_126720.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126720_H_
#define NMEA2000_INC_PGN_PGN_126720_H_


/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN126720NAME
{
    uint16_t mManufacturer_Code;            // 11 bits
    uint8_t  mIndustry_Group;               // 3  bits
    uint8_t  IdentifyCode;                  // 8  bits
    uint8_t  ControlFunction;               // 8  bits
} PGN126720NAME;

/* Exported functions --------------------------------------------------------*/
PGN126720NAME PGN126720_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);

void PGN126720_FactoryTest_SetFieldValue();
void PGN126720_MemoryClearGroup_SetFieldValue();
void PGN126720_ResetGoup_SetFieldValue();
void PGN126720_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_126720_H_ */

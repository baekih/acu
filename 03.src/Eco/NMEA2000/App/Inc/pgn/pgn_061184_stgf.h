/*
 * pgn_061184_stgf.h
 *
 *  Created on: Feb 22, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_061184_STGF_H_
#define NMEA2000_INC_PGN_PGN_061184_STGF_H_


/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN061184STGFNAME
{
    uint32_t m61184Manufacturer_Code;       // 11 bits
    uint32_t m61184NMEA_Reserved1;          // 2  bits
    uint32_t m61184Industry_Group;          // 3  bits
    uint32_t m61184Identification_Code;     // 8  bits
    uint32_t m61184Control_Function;        // 8  bits
    uint32_t m61184NMEA_Reserved2;          // 32 bits
} PGN061184STGFNAME;

extern PGN061184STGFNAME g_PGN061184STGFNAME;

/* Exported functions --------------------------------------------------------*/
void PGN061184STGF_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN061184STGF_SendNameField(uint8_t mSA);
void PGN061184STGF_ProcessNameField(uint8_t mSA);


#endif /* NMEA2000_INC_PGN_PGN_061184_STGF_H_ */

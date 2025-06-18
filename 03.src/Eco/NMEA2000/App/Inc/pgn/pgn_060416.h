/*
 * pgn_060416.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_H_
#define NMEA2000_INC_PGN_PGN_060416_H_

/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN060416_FUNC_CODE_RTS                 16  // 0x10
#define PGN060416_FUNC_CODE_CTS                 17  // 0x11
#define PGN060416_FUNC_CODE_EOM                 19  // 0x13
#define PGN060416_FUNC_CODE_BAM                 32  // 0x20
#define PGN060416_FUNC_CODE_ABORT               255  // 0xFF

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416NAME
{
    uint32_t mGroup_Function_Code;      // 8  bits
} PGN060416NAME;

/* Exported variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN060416_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
uint32_t PGN060416_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_060416_H_ */

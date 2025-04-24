/*
 * pgn_060416.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060416_H_
#define NMEA2000_INC_PGN_PGN_060416_H_


/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060416NAME
{
    uint32_t mGroup_Function_Code;      // 8  bits
} PGN060416NAME;

/* Exported variables ---------------------------------------------------------*/
extern uint32_t FunctionCodeRTS;
extern uint32_t FunctionCodeCTS;
extern uint32_t FunctionCodeEOM;
extern uint32_t FunctionCodeBAM;
extern uint32_t FunctionCodeABORT;

/* Exported functions --------------------------------------------------------*/
void PGN060416_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
uint32_t PGN060416_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_PGN_PGN_060416_H_ */

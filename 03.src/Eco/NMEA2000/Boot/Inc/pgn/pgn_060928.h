/*
 * pgn_060928.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060928_H_
#define NMEA2000_INC_PGN_PGN_060928_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* typedef -----------------------------------------------------------*/
typedef struct __PGN060928NAME
{
	uint32_t mUnique_Number;				// 21 bits
	uint32_t mManufacturer_Code;			// 11 bits
	uint32_t mDevice_Instance_Lower;		// 3  bits
	uint32_t mDevice_Instance_Upper;		// 5  bits
	uint32_t mDevice_Function;				// 8  bits
	uint32_t mNMEA_Reserved;				// 1  bits
	uint32_t mDevice_Class;					// 7  bits
	uint32_t mSystem_Instance;				// 4  bits
	uint32_t mIndustry_Group;				// 3  bits
	uint32_t mISO_Self_Configuration;		// 1  bits
} PGN060928NAME;

extern PGN060928NAME g_PGN060928NAME;
extern PGN060928NAME mMyPGN060928;

/* functions --------------------------------------------------------*/
void InitializeMyNMEAData();

void PGN060928_SetInitialField(PGN060928NAME *pPGN060928);
void PGN060928_GetFieldValue(uint8_t len, uint8_t *buf, PGN060928NAME *pPGN060928);

void PGN060928_SendAddressClaim();
void PGN060928_ProcessNameField();

#endif /* NMEA2000_INC_PGN_PGN_060928_H_ */

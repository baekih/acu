/*
 * pgn_126996.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_126996_H_
#define NMEA2000_INC_PGN_PGN_126996_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN126996NAME
{
	uint32_t mNMEA_Network_Message_Database_Version;	// 16 bits
	uint32_t mNMEA_Manufacturers_Product_Code;			// 16 bits
	uint8_t mManufacturers_Model_ID[32];				// n  bits
	uint8_t mManufacturers_Software_Version_Code[32];	// n  bits
	uint8_t mManufacturers_Model_Version[32];			// n  bits
	uint8_t mManufacturers_Model_Serial_Code[32];		// n  bits
	uint32_t mNMEA_2000_Certification_Level;			// 8  bits
	uint32_t mLoad_Equivalency;							// 8  bits
} PGN126996NAME;

/* Exported functions --------------------------------------------------------*/
extern void PGN126996_SetFieldValue(uint32_t _NMEA_Network_Message_Database_Version,
									uint32_t _NMEA_Manufacturers_Product_Code,
									uint8_t _Manufacturers_Model_ID[],
									uint8_t _Manufacturers_Software_Version_Code[],
									uint8_t _Manufacturers_Model_Version[],
									uint8_t _Manufacturers_Model_Serial_Code[],
									uint32_t _NMEA_2000_Certification_Level,
									uint32_t _Load_Equivalency);

extern void PGN126996_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf, uint32_t messagetype);


#endif /* NMEA2000_INC_PGN_PGN_126996_H_ */

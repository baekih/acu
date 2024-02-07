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

/* define ------------------------------------------------------------*/
#define PGN126996_NUM                         126996
#define PGN126996_PRIORITY                    6

#define PGN126996_NMEA2000_DB_VER             2100
#define PGN126996_NMEA2000_MFG_PRODUCT_CODE   6
#define PGN126996_NMEA2000_MODEL_SERIAL_CODE  "0000"
#define PGN126996_NMEA2000_CERT_LEVEL         2
#define PGN126996_LOAD_EQUIVALENCY            8

#define PGN126996_MFG_DATA_LEN                32

/* typedef -----------------------------------------------------------*/
typedef struct __PGN126996NAME
{
	uint16_t mNMEA2000DBVer;
	uint16_t mNMEA2000MfgProductCode;
	uint8_t  mMfgModelID[PGN126996_MFG_DATA_LEN];
	uint8_t  mMfgSWVerCode[PGN126996_MFG_DATA_LEN];
	uint8_t  mMfgModelVer[PGN126996_MFG_DATA_LEN];
	uint8_t  mMfgModelSerialCode[PGN126996_MFG_DATA_LEN];
	uint8_t  mNMEA2000CertLevel;
	uint8_t  mNMEA2000LoadEquivalency;
} PGN126996NAME;

/* functions --------------------------------------------------------*/
void PGN126996_SetFieldValue(uint32_t _NMEA_Network_Message_Database_Version,
							 uint32_t _NMEA_Manufacturers_Product_Code,
							 uint8_t _Manufacturers_Model_ID[],
							 uint8_t _Manufacturers_Software_Version_Code[],
							 uint8_t _Manufacturers_Model_Version[],
							 uint8_t _Manufacturers_Model_Serial_Code[],
							 uint32_t _NMEA_2000_Certification_Level,
							 uint32_t _Load_Equivalency);

void PGN126996_ProcessNameField(uint32_t pgnid, uint8_t len, uint8_t *buf, uint32_t messagetype);

#endif /* NMEA2000_INC_PGN_PGN_126996_H_ */

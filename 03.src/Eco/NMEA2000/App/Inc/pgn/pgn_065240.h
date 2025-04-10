/*
 * pgn_065240.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_065240_H_
#define NMEA2000_INC_PGN_PGN_065240_H_


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"

/* Private define ------------------------------------------------------------*/
#define PGN065240_PGN                       60928
#define PGN065240_PRIORITY                  6

#define PGN065240_HIDHOST_MFG_CODE          1855
#define PGN065240_HIDHOST_DEVICE_CLASS      60
#define PGN065240_HIDHOST_DEVICE_FUNC       175

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN065240NAME
{
    uint32_t mUnique_Number;                // 21 bits
    uint32_t mManufacturer_Code;            // 11 bits
    uint32_t mDevice_Instance_Lower;        // 3  bits
    uint32_t mDevice_Instance_Upper;        // 5  bits
    uint32_t mDevice_Function;              // 8  bits
    uint32_t mNMEA_Reserved;                // 1  bits
    uint32_t mDevice_Class;                 // 7  bits
    uint32_t mSystem_Instance;              // 4  bits
    uint32_t mIndustry_Group;               // 3  bits
    uint32_t mISO_Self_Configuration;       // 1  bits
    uint32_t mNew_Address;                  // 8  bits
} PGN065240NAME;

/* Exported functions --------------------------------------------------------*/
void PGN065240_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);

void PGN065240_SetInitialField(PGN065240NAME *pPGN065240);
void PGN065240_ProcessNameField(void);


#endif /* NMEA2000_INC_PGN_PGN_065240_H_ */

/*
 * pgn_060928.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_PGN_PGN_060928_H_
#define NMEA2000_INC_PGN_PGN_060928_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN060928_PGN                       60928
#define PGN060928_PRIORITY                  6

//#define PGN060928_HIDHOST_MFG_CODE          MFG_CODE_FURUNO
//#define PGN060928_HIDHOST_DEVICE_CLASS      60
//#define PGN060928_HIDHOST_DEVICE_FUNC       175

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN060928NAME
{
    uint32_t mUnique_Number;                // 21 bits
    uint16_t mManufacturer_Code;            // 11 bits
    uint8_t  mDevice_Instance;              // 8  bits
    uint8_t  mDevice_Instance_Lower;        // 3  bits
    uint8_t  mDevice_Instance_Upper;        // 5  bits
    uint8_t  mDevice_Function;              // 8  bits
    uint8_t  mNMEA_Reserved;                // 1  bits
    uint8_t  mDevice_Class;                 // 7  bits
    uint8_t  mSystem_Instance;              // 4  bits
    uint8_t  mIndustry_Group;               // 3  bits
    uint8_t  mISO_Self_Configuration;       // 1  bits
} PGN060928NAME;

extern uint64_t my_name_field;

/* Exported functions --------------------------------------------------------*/
void PGN060928_SaveHIDHostISOAddrClaim(NmeaPgn* pgnId, uint8_t *pbuf, uint8_t len);
void PGN060928_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf, PGN060928NAME *pPGN060928);

void PGN060928_SetInitialField();
void PGN060928_SendAddressClaim(void);
void PGN060928_ProcessNameField(void);


#endif /* NMEA2000_INC_PGN_PGN_060928_H_ */

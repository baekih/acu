/*
 * nmea2000_deviceinfo.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_DEVICEINFO_H_
#define NMEA2000_INC_NMEA2000_DEVICEINFO_H_


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Private define ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern uint32_t mUnique_Number;
extern const uint8_t mIndustry_Group;
extern const uint8_t mDevice_Function;
extern const uint8_t mDevice_Class;
extern uint8_t mDevice_Intance;
extern uint8_t mSystem_Instance;
//extern uint32_t mName_Field_Reserved;
extern uint8_t mISO_Self_Configuration;

extern uint32_t mNMEANetworkMessageDatabaseVersion;
extern uint32_t mNMEAManufacturersProductCode;
extern uint8_t  mManufacturersModelID[32];
extern uint8_t  mManufacturersSoftwareVersionCode[32];
extern uint8_t  mManufacturersModelVersion[32];
extern uint8_t  mManufacturersModelSerialCode[32];
extern uint32_t mNMEA2000CertificationLevel;
extern uint32_t mLoadEquivalency;

extern uint8_t  mInstallationDescription1[];
extern uint8_t  mInstallationDescription2[];
extern uint8_t  mMenufacturerDescription[];

extern uint32_t mTransmit_PGN_List[];
extern uint32_t mReceive_PGN_List[];

extern const uint32_t mTransmit_PGN_List_Size;
extern const uint32_t mReceive_PGN_List_Size;

void LoadMyNMEAData();

#endif /* NMEA2000_INC_NMEA2000_DEVICEINFO_H_ */

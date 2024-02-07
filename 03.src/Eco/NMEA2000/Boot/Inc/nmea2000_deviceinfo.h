/*
 * nmea2000_deviceinfo.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_DEVICEINFO_H_
#define NMEA2000_INC_NMEA2000_DEVICEINFO_H_


/* Includes ------------------------------------------------------------------*/
#include "eco.h"

/* Private define ------------------------------------------------------------*/
#define Transmit_PGN_List_Unit	      8
#define Receive_PGN_List_Unit	        7

/* Exported variables --------------------------------------------------------*/
extern uint32_t mUnique_Number;
extern uint32_t mManufacturer_Code;
extern uint32_t mDevice_Intance;
extern uint32_t mDevice_Function;
extern uint32_t mDevice_Class;
extern uint32_t mSystem_Instance;
extern uint32_t mIndustry_Group;
extern uint32_t mISO_Self_Configuration;

extern uint32_t mLast_Send_Address_Claim_Time;
extern uint8_t  mAddress_Claiming;

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

extern uint32_t mTransmit_PGN_List[Transmit_PGN_List_Unit];
extern uint32_t mReceive_PGN_List[Receive_PGN_List_Unit];

#endif /* NMEA2000_INC_NMEA2000_DEVICEINFO_H_ */

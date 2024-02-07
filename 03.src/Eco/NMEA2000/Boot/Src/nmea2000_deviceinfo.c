/*
 * nmea2000_deviceinfo.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_deviceinfo.h"

/* Private variables ---------------------------------------------------------*/
uint32_t mUnique_Number;
uint32_t mManufacturer_Code = MFG_CODE_AIRMAR;
uint32_t mDevice_Intance = 0;
uint32_t mDevice_Function = 130;
uint32_t mDevice_Class = 110;
uint32_t mSystem_Instance = 0;
uint32_t mIndustry_Group = 4;
uint32_t mISO_Self_Configuration = 1;

uint32_t mLast_Send_Address_Claim_Time = 0;
uint8_t  mAddress_Claiming = true;

uint32_t mNMEANetworkMessageDatabaseVersion = 2100;
uint32_t mNMEAManufacturersProductCode = 6;

uint8_t  mManufacturersModelID[32];
uint8_t  mManufacturersModelVersion[32];
uint8_t  mManufacturersSoftwareVersionCode[32];             //"BOOTVER:APPVER"
uint8_t  mManufacturersModelSerialCode[32] =  "0000";       // No used
uint32_t mNMEA2000CertificationLevel = 2;
uint32_t mLoadEquivalency = 1;

uint8_t  mInstallationDescription1[72];
uint8_t  mInstallationDescription2[72];
uint8_t  mMenufacturerDescription[250];

uint32_t mTransmit_PGN_List[Transmit_PGN_List_Unit] = {
		59392,	60928,	65285,	65287,	126208,	//E800,		EE00,	FF05,	FF07,	1ED00
		126464,	126720,	126996					//1EE00,	1EF00,	1F014
};

uint32_t mReceive_PGN_List[Receive_PGN_List_Unit] = {
		59904,	60160,	60416,	60928,	65286,	//EA00,		EB00,	EC00,	EE00,	FF06,
		126208,	126720							//1ED00,	1EF00
};

/* Private functions ---------------------------------------------------------*/

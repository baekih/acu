/*
 * nmea2000_deviceinfo.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_deviceinfo.h"

#include "common.h"
#include "app.h"

#include "nmea2000.h"

/* Private variables ---------------------------------------------------------*/
uint32_t mUnique_Number;
const uint16_t mApp_FEC_Manufacturer_Code = N2K_MFG_CODE_FURUNO;
const uint16_t mBoot_Airmar_Manufacturer_Code = N2K_MFG_CODE_AIRMAR;
const uint8_t mIndustry_Group = N2K_INDUSTRY_GROUP;
const uint8_t mDevice_Function = N2K_DEVICE_FUNCTION;
const uint8_t mDevice_Class = N2K_DEVICE_CLASS;
uint8_t mDevice_Intance;
uint8_t mSystem_Instance;
//uint32_t mName_Field_Reserved = 0x3;
uint8_t mISO_Self_Configuration = N2K_ISO_SELF_CONFIG;

uint32_t mNMEANetworkMessageDatabaseVersion = 2100;
uint32_t mNMEAManufacturersProductCode;

uint8_t  mManufacturersModelID[32];
uint8_t  mManufacturersModelVersion[32];                    //"N/A"
uint8_t  mManufacturersSoftwareVersionCode[32];             //"APPVER:BOOTVER"
uint8_t  mManufacturersModelSerialCode[32] = "0000";        //"88Y2022W04N000001"
uint32_t mNMEA2000CertificationLevel = 2;
uint32_t mLoadEquivalency = 2;

uint8_t  mInstallationDescription1[72];
uint8_t  mInstallationDescription2[72];
uint8_t  mMenufacturerDescription[250];

uint32_t mTransmit_PGN_List[] = {
    59392,   59904,  60160,  60416,  60928,
    61184,   65285,  65287, 126208, 126464,
    126720, 126993, 126996, 126998, 130816,
    130822, 130823
};

const uint32_t mTransmit_PGN_List_Size = 20;

uint32_t mReceive_PGN_List[] = {
    59392,  59904,  60160,  60416,  60928,
    61184,  65240,  65287, 126208, 126720
};

const uint32_t mReceive_PGN_List_Size = 10;

/* Private functions ---------------------------------------------------------*/

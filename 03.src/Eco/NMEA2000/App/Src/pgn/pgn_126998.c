/*
 * pgn_126998.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/
PGN126998NAME g_PGN126998NAME;

/* Private variables ---------------------------------------------------------*/
uint8_t     installationDescription1[72];
uint8_t     installationDescription2[72];
uint8_t     menufacturerDescription[72];

/* Private functions ---------------------------------------------------------*/
void PGN126998_SetFieldValue(uint8_t _InstallationDescription1[],
                             uint8_t _InstallationDescription2[],
                             uint8_t _MenufacturerDescription[])
{
    InitializeSendNameField();

    int byteCount1 = _InstallationDescription1[0];
    int byteCount2 = _InstallationDescription2[0];
    int byteCount3 = _MenufacturerDescription[0];

//  printf("_InstallationDescription1 - %d / %d\r\n",_InstallationDescription1[0], _InstallationDescription1[1]);
//  printf("_InstallationDescription2 - %d / %d\r\n",_InstallationDescription2[0], _InstallationDescription2[1]);
//  printf("_MenufacturerDescription - %d / %d\r\n",_MenufacturerDescription[0], _MenufacturerDescription[1]);

    if(byteCount1 <= 72){
        memcpy(sendNMEAPackets + sendPacketLength, _InstallationDescription1, byteCount1);
        sendPacketLength += byteCount1;
    }
    if(byteCount2 <= 72){
        memcpy(sendNMEAPackets + sendPacketLength, _InstallationDescription2, byteCount2);
        sendPacketLength += byteCount2;
    }
    if(byteCount3 <= 72){
        memcpy(sendNMEAPackets + sendPacketLength, _MenufacturerDescription, byteCount3);
        sendPacketLength += byteCount3;
    }
}

void PGN126998_ProcessNameField(NmeaPgn* pgnId, uint32_t messagetype)
{
    memset(installationDescription1, 0, sizeof(installationDescription1));
    memset(installationDescription2, 0, sizeof(installationDescription2));
    memset(menufacturerDescription, 0, sizeof(menufacturerDescription));

    installationDescription1[0] = 2;
    installationDescription2[0] = 2;
    menufacturerDescription[0] = 2;

    installationDescription1[1] = 1;
    installationDescription2[1] = 1;
    menufacturerDescription[1] = 1;

    PGN126998_SetFieldValue(installationDescription1, installationDescription2,
                            menufacturerDescription);

    SendNonSingleFrame(getCanId(PGN126998_PRIORITY, PGN126998_PGN, pgnId->mSA, g_n2k_addr_curr),
                       sendPacketLength,
                       sendNMEAPackets,
                       messagetype);
}

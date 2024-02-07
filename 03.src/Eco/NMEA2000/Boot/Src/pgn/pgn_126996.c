/*
 * pgn_126996.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_126996.h"

/* typedef -----------------------------------------------------------*/
PGN126996NAME g_PGN126996NAME =
{
    PGN126996_NMEA2000_DB_VER,
    PGN126996_NMEA2000_MFG_PRODUCT_CODE,
    "\0",
    "\0",
    "\0",
    PGN126996_NMEA2000_MODEL_SERIAL_CODE,
    PGN126996_NMEA2000_CERT_LEVEL,
    PGN126996_LOAD_EQUIVALENCY
};

/* functions ---------------------------------------------------------*/
void PGN126996_SetFieldValue(uint32_t _NMEA_Network_Message_Database_Version,
                             uint32_t _NMEA_Manufacturers_Product_Code,
                             uint8_t _Manufacturers_Model_ID[],
                             uint8_t _Manufacturers_Software_Version_Code[],
                             uint8_t _Manufacturers_Model_Version[],
                             uint8_t _Manufacturers_Model_Serial_Code[],
                             uint32_t _NMEA_2000_Certification_Level,
                             uint32_t _Load_Equivalency)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

    InitializeSendNameBitPosition();
    InitializeSendNameField();

#if 0
    printf(" [_Manufacturers_Model_ID =              ");	for (uint8_t i = 0; i < 32; i++)	printf("0x%02x ", _Manufacturers_Model_ID[i]);				printf("] !!\r\n");
    printf(" [_Manufacturers_Software_Version_Code = ");	for (uint8_t i = 0; i < 32; i++)	printf("0x%02x ", _Manufacturers_Software_Version_Code[i]);	printf("] !!\r\n");
    printf(" [_Manufacturers_Model_Version =         ");	for (uint8_t i = 0; i < 32; i++)	printf("0x%02x ", _Manufacturers_Model_Version[i]);			printf("] !!\r\n");
    printf(" [_Manufacturers_Model_Serial_Code =     ");	for (uint8_t i = 0; i < 32; i++)	printf("0x%02x ", _Manufacturers_Model_Serial_Code[i]);		printf("] !!\r\n");
#endif

    Add2ByteUInt( _NMEA_Network_Message_Database_Version );
    Add2ByteUInt( _NMEA_Manufacturers_Product_Code );
    for (uint8_t i = 0; i < 32; i++)
        sendNMEAPackets[sendPacketLength++] = _Manufacturers_Model_ID[i];
    for (uint8_t i = 0; i < 32; i++)
        sendNMEAPackets[sendPacketLength++] = _Manufacturers_Software_Version_Code[i];
    for (uint8_t i = 0; i < 32; i++)
        sendNMEAPackets[sendPacketLength++] = _Manufacturers_Model_Version[i];
    for (uint8_t i = 0; i < 32; i++)
        sendNMEAPackets[sendPacketLength++] = _Manufacturers_Model_Serial_Code[i];
    Add1ByteUInt( _NMEA_2000_Certification_Level );
    Add1ByteUInt( _Load_Equivalency );
}

void PGN126996_ProcessNameField(uint32_t pgnid, uint8_t len, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

    strcpy((char*)mManufacturersModelID, _MODEL_ID_);

    PGN126996_SetFieldValue(mNMEANetworkMessageDatabaseVersion,
                            mNMEAManufacturersProductCode,
                            mManufacturersModelID,
                            mManufacturersSoftwareVersionCode,
                            mManufacturersModelVersion,
                            mManufacturersModelSerialCode,
                            mNMEA2000CertificationLevel,
                            mLoadEquivalency);

    //	SendNonSingleFrame(getCanId(PGN126996_priority, 126996, pgnId->mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, messagetype);

    SendNonSingleFrame(NmeaPGN_CanID(PGN126996_PRIORITY, PGN126996_NUM, get_R_SA(), localSourceAddr),
                       sendPacketLength, sendNMEAPackets, messagetype);
}


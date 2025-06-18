/*
 * pgn_060928.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/
uint64_t my_name_field;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void PGN060928_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf, PGN060928NAME *pPGN060928)
{
    pPGN060928->mUnique_Number          =  GetBuf_4ByteUInt(len, 0, buf) & 0x1FFFFF;
    pPGN060928->mManufacturer_Code      = (GetBuf_4ByteUInt(len, 0, buf) >> 21) & 0x7FF;
    pPGN060928->mDevice_Instance_Lower  =  GetBuf_1ByteUInt(len, 4, buf) & 0x07;
    pPGN060928->mDevice_Instance_Upper  = (GetBuf_1ByteUInt(len, 4, buf) >> 3) & 0x1F;
    pPGN060928->mDevice_Function        =  GetBuf_1ByteUInt(len, 5, buf);
    pPGN060928->mNMEA_Reserved          =  GetBuf_1ByteUInt(len, 6, buf) & 0x01;
    pPGN060928->mDevice_Class           = (GetBuf_1ByteUInt(len, 6, buf) >> 1) & 0x7F;
    pPGN060928->mSystem_Instance        =  GetBuf_1ByteUInt(len, 7, buf) & 0x0F;
    pPGN060928->mIndustry_Group         = (GetBuf_1ByteUInt(len, 7, buf) >> 4) & 0x07;
    pPGN060928->mISO_Self_Configuration = (GetBuf_1ByteUInt(len, 7, buf) >> 7) & 0x01;

/*  printf("NMEAUniqueNumber        :%d\n", pPGN060928->mUnique_Number);
    printf("NMEAManufacturerCode    :%d\n", pPGN060928->mManufacturer_Code);
    printf("Instance_Lower          :%d\n", pPGN060928->mDevice_Instance_Lower);
    printf("Instance_Upper          :%d\n", pPGN060928->mDevice_Instance_Upper);
    printf("NMEAFunctionCode        :%d\n", pPGN060928->mDevice_Function);
    printf("NMEAReserved1           :%d\n", pPGN060928->mNMEA_Reserved);
    printf("NMEADeviceClass         :%d\n", pPGN060928->mDevice_Class);
    printf("SystemInstance          :%d\n", pPGN060928->mSystem_Instance);
    printf("IndustryGroup           :%d\n", pPGN060928->mIndustry_Group);*/
}

void PGN060928_SetInitialField()
{
    InitializeSendNameField();

    Add4ByteUInt(((g_pgn060928_curr.mManufacturer_Code & 0x7FF) << 21) | (g_pgn060928_curr.mUnique_Number & 0x1FFFFF) );
    Add1ByteUInt((g_pgn060928_curr.mDevice_Instance_Lower & 0x07) | ((g_pgn060928_curr.mDevice_Instance_Upper << 3) & 0xF8));
    Add1ByteUInt(g_pgn060928_curr.mDevice_Function);
    Add1ByteUInt(((g_pgn060928_curr.mDevice_Class & 0x7F) << 1) | 0);
    Add1ByteUInt(((g_pgn060928_curr.mISO_Self_Configuration & 0x01) << 7) | ((g_pgn060928_curr.mIndustry_Group & 0x07) << 4) | (g_pgn060928_curr.mSystem_Instance & 0x0F) );

    my_name_field  = *((uint64_t *)sendNMEAPackets);
}

void PGN060928_SendAddressClaim(void)
{
    NmeaPgn* sendPgnId = getCanId(PGN060928_PRIORITY, PGN060928_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr);

    NMEA2000_SendParseMessages(sendPgnId, sendPacketLength, sendNMEAPackets, 0);

    g_n2k_addr_is_claiming = true;
    g_n2k_addr_claim_time_last = HAL_GetTick();
}

void PGN060928_ProcessNameField()
{
    if (g_n2k_addr_is_claiming == true)
    {
        printf("%s:%d In Address Claiming... \r\n",__FUNCTION__,__LINE__);
        return;
    }

    NmeaPgn* sendPgnId = getCanId(PGN060928_PRIORITY, PGN060928_PGN, BROADCAST_DESTINATION_ADDR, g_n2k_addr_curr);

    NMEA2000_SendParseMessages(sendPgnId, sendPacketLength, sendNMEAPackets, 0);
}


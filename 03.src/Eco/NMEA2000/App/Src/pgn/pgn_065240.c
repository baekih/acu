/*
 * pgn_065240.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_065240.h"
#include "pgn_060928.h"

/* Private typedef -----------------------------------------------------------*/
PGN065240NAME g_PGN065240;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void PGN065240_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN065240.mUnique_Number          =  GetBuf_4ByteUInt(len, 0, buf) & 0x1FFFFF;
    g_PGN065240.mManufacturer_Code      = (GetBuf_4ByteUInt(len, 0, buf) >> 21) & 0x7FF;
    g_PGN065240.mDevice_Instance_Lower  =  GetBuf_1ByteUInt(len, 4, buf) & 0x07;
    g_PGN065240.mDevice_Instance_Upper  = (GetBuf_1ByteUInt(len, 4, buf) >> 3) & 0x1F;
    g_PGN065240.mDevice_Function        =  GetBuf_1ByteUInt(len, 5, buf);
    g_PGN065240.mNMEA_Reserved          =  GetBuf_1ByteUInt(len, 6, buf) & 0x01;
    g_PGN065240.mDevice_Class           = (GetBuf_1ByteUInt(len, 6, buf) >> 1) & 0x7F;
    g_PGN065240.mSystem_Instance        =  GetBuf_1ByteUInt(len, 7, buf) & 0x0F;
    g_PGN065240.mIndustry_Group         = (GetBuf_1ByteUInt(len, 7, buf) >> 4) & 0x07;
    g_PGN065240.mISO_Self_Configuration = (GetBuf_1ByteUInt(len, 7, buf) >> 7) & 0x01;
    g_PGN065240.mNew_Address            =  GetBuf_1ByteUInt(len, 8, buf);
}

void PGN065240_SetInitialField(PGN065240NAME *pPGN065240)
{
    InitializeSendNameField();

    Add4ByteUInt(((pPGN065240->mManufacturer_Code & 0x7FF) << 21) | (pPGN065240->mUnique_Number & 0x1FFFFF) );
    Add1ByteUInt((pPGN065240->mDevice_Instance_Lower & 0x07) | ((pPGN065240->mDevice_Instance_Upper << 3) & 0xF8));
    Add1ByteUInt(pPGN065240->mDevice_Function);
    Add1ByteUInt(((pPGN065240->mDevice_Class & 0x7F) << 1) | 0);
    Add1ByteUInt(((pPGN065240->mISO_Self_Configuration & 0x01) << 7) | ((pPGN065240->mIndustry_Group & 0x07) << 4) | (pPGN065240->mSystem_Instance & 0x0F) );
}

void PGN065240_SendAddressClaim(void)
{
    NmeaPgn* sendPgnId = getCanId(PGN065240_PRIORITY, PGN065240_PGN, BROADCAST_DESTINATION_ADDR, localSourceAddr);

    NMEA2000_SendParseMessages(sendPgnId, sendPacketLength, sendNMEAPackets, 0);

    mAddress_Claiming = true;
    mLast_Send_Address_Claim_Time = HAL_GetTick();
}

void PGN065240_ProcessNameField()
{
    if(g_PGN065240.mUnique_Number           != mMyPGN060928.mUnique_Number) return;
    if(g_PGN065240.mManufacturer_Code       != mMyPGN060928.mManufacturer_Code) return;
    if(g_PGN065240.mDevice_Instance_Lower   != mMyPGN060928.mDevice_Instance_Lower) return;
    if(g_PGN065240.mDevice_Instance_Upper   != mMyPGN060928.mDevice_Instance_Upper) return;
    if(g_PGN065240.mDevice_Function         != mMyPGN060928.mDevice_Function) return;
    if(g_PGN065240.mNMEA_Reserved           != 0) return;
    if(g_PGN065240.mDevice_Class            != mMyPGN060928.mDevice_Class) return;
    if(g_PGN065240.mSystem_Instance         != mMyPGN060928.mSystem_Instance) return;
    if(g_PGN065240.mIndustry_Group          != mMyPGN060928.mIndustry_Group) return;
    if(g_PGN065240.mISO_Self_Configuration  != mMyPGN060928.mISO_Self_Configuration) return;

    if(g_PGN065240.mNew_Address >= 0 && g_PGN065240.mNew_Address <= 251)
    {
        printf("AddrChange[%ld->%ld]\r\n", localSourceAddr, g_PGN065240.mNew_Address);
        localSourceAddr = g_PGN065240.mNew_Address;
//      g_spayload_curr.nmea2000_addr = localSourceAddr;

        PGN060928_SetInitialField();
        PGN060928_SendAddressClaim();
    }
}

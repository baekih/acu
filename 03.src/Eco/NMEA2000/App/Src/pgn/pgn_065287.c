/*
 * pgn_065287.c
 *
 *  Created on: 2022. 7. 8.
 *      Author: ihbaek
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_065287.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t pgn65287_datalen[5] = {2, 1, 1, 1, 8};

uint32_t PGN065287_priority = 7;

/* Private functions ---------------------------------------------------------*/
bool isFECPPGN(uint8_t *pbuf)
{
    if((uint16_t)GetBuf_nByteUInt(pbuf, 0, 2) == PPGN_FURUNO_MFGCODE) return true;

    return false;
}

bool isAirmarPPGN(uint8_t *pbuf)
{
    if((uint16_t)GetBuf_nByteUInt(pbuf, 0, 2) == PPGN_AIRMAR_MFGCODE) return true;

    return false;
}

uint32_t calculate_level_2_key (uint32_t seed, uint32_t mask, uint8_t iterations)
{
    uint8_t i;
    uint32_t shift_reg;
    uint32_t temp;

    shift_reg = seed ^ mask;

    if (shift_reg == 0) /* input must be nonzero */
    {
        shift_reg = 0xFFFFFFFF;
    }

    for (i=0; i<iterations; i++) {
        temp = shift_reg >> 31;
        temp ^= shift_reg >> 6;
        temp ^= shift_reg >> 5;
        temp ^= shift_reg >> 1;
        temp &= 0x00000001;

        shift_reg = (temp << 31) | (shift_reg >> 1);
    }

    return shift_reg;
}

void PGN065287_FEC_GetFieldValue(NmeaPgn* pgnId, uint8_t *pbuf, uint8_t len)
{
    if(((uint8_t)GetBuf_nByteUInt(pbuf, 2, 1) & 0x01) == 1)
    {
//      g_hidhost_addr = (uint8_t)pgnId->mSA;
//      g_hidhost_canname_req = true;
    }
}

void PGN065287_Airmar_GetFieldValue(NmeaPgn* pgnId, uint8_t *pbuf, uint8_t len)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

/*  if(((uint8_t)GetBuf_nByteUInt(pbuf, 2, 1) & 0x01) == 1)
    {
        HIDTargetHostAddr = (uint8_t)pgnId->mSA;
        printf("%s:%d Get HIDTargetHostAddr[%d]...\r\n",__FUNCTION__,__LINE__, HIDTargetHostAddr);
    }
    else{
        printf("%s:%d Get hidhost.addr Fail...\r\n",__FUNCTION__,__LINE__);
    }*/
}

void PGN065287_FEC_SendNameField(void)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

    InitializeSendNameField();

    Add2ByteUInt(PPGN_FURUNO_MFGCODE);
    Add1ByteUInt( 2 );
    Add4ByteUInt( 0xFFFFFFFF );
    Add1ByteUInt( 0xFF );

    NMEA2000_SendParseMessages(getCanId(PGN065287_priority, 65287, 255, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}

void PGN065287_Airmar_SendNameField(NmeaPgn* pgnId)
{
//  printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);

    InitializeSendNameField();

    Add2ByteUInt(PPGN_AIRMAR_MFGCODE);
    Add1ByteUInt( 3 );
    Add1ByteUInt( 0xF8 | g_access_level );

    uint32_t access_key = 0;

    switch(g_access_level){
        case 0 : access_key = 0;
            break;
        case 1 : access_key = 0x12345678;
            break;
    }

    Add4ByteUInt( access_key );

    NMEA2000_SendParseMessages(getCanId(PGN065287_priority, 65287, pgnId->mSA, localSourceAddr), sendPacketLength, sendNMEAPackets, 0);
}

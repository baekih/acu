/*
 * nmea2k_boot.c
 *
 *  Created on: Mar 13, 2024
 *      Author: ihbaek
 */

#include "eco.h"

#define BOOT_STATE_SIZE         (3ULL)

typedef enum
{
    IN_STARTUP_MON  = 0x0,
    RUN_BOOT        = 0x1,
    RUN_APP         = 0x2,
    RSV1            = 0x3,
    RSV2            = 0x6,
    DAT_NOT_AVAIL   = 0x7
} _boot_state;

_boot_state boot_state = RUN_BOOT;

int32_t Pgn065285BootStatAck(void)//    Boot State Acknowledgment
{
    printf("%s() Called\n",__FUNCTION__);
    printf("%s() PPGN+MFGCODE[0x%X]\n",__FUNCTION__, PPGN_MFGCODE);
    uint64_t g_pgn065285_boot_state_req_raw =   \
        ((RSV_32BIT)                        << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE + 5 + 8)) | \
        ((RSV_8BIT)                         << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE + 5)) | \
        ((RSV_5BIT)                         << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE)) | \
        ((boot_state)                       << (PPGN_MFGCODE_SIZE)) | \
        ((PPGN_MFGCODE)                     << 0);

    TxProtocol txpkt =
    {
        .canid = (PGN065285_PRI<<26)|((PGN065285_NUM)<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = g_pgn065285_boot_state_req_raw,
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

void opBootStatChk(RxProtocol rxpkt)//    Boot State Acknowledgment
{
    uint16_t ppgn_mfgcode = (uint16_t)PPGN_MFGCODE;

    if(!memcmp((uint8_t*)&rxpkt.dat64, (uint8_t*)(&ppgn_mfgcode), sizeof(ppgn_mfgcode)))
    {
        Pgn065285BootStatAck();
    }
    else
    {
        printf("%s() Error PPGN_MFGCODE[%d]\n",__FUNCTION__, MFG_CODE);
    }
}


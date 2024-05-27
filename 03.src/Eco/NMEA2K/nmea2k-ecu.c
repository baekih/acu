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

version_dat g_version_dat;

int32_t Pgn065285BootStatAck(void)//    Boot State Acknowledgment
{
    printf("%s() Called\n",__FUNCTION__);
    printf("%s() PPGN+MFGCODE[0x%X]\n",__FUNCTION__, PPGN_MFGCODE);

    TxProtocol txpkt =
    {
        .canid = (PGN065285_PRI<<26)|((PGN065285_NUM)<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = \
            ((uint64_t)(RSV_32BIT)          << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE + 5 + 8)) | \
            ((uint64_t)(RSV_8BIT)           << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE + 5)) | \
            ((uint64_t)(RSV_5BIT)           << (PPGN_MFGCODE_SIZE + BOOT_STATE_SIZE)) | \
            ((uint64_t)(boot_state)         << (PPGN_MFGCODE_SIZE)) | \
            ((uint64_t)(PPGN_MFGCODE)       << 0),
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

void Pgn126720BootVer(fastpacket* pfastpkt)
{
    printf("%s() Enter\n",__FUNCTION__);
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(PGN126720_05_LEN);

    TxProtocol txpkt =
    {
        .canid = (PGN126720_PRI<<26)|((PGN126720_NUM+pfastpkt->src_addr)<<8)|(NMEA2K_THIS_ADDR<<0),
        .len = sizeof(uint64_t)
    };

    uint8_t *pfpkt_dat = pvPortMalloc(fastdat_len_trunc);
    printf("%s() fastdat_len_trunc[%d]\n",__FUNCTION__, fastdat_len_trunc);
    memset(pfpkt_dat, 0xFF, fastdat_len_trunc);
    uint32_t pfpkt_ofst = 0;

    *(uint8_t *)(pfpkt_dat + pfpkt_ofst) = PGN126720_05_LEN;                      pfpkt_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pfpkt_dat + pfpkt_ofst) = PPGN_MFGCODE;                          pfpkt_ofst+=sizeof(uint16_t);
    *(uint8_t *)(pfpkt_dat + pfpkt_ofst) = PGN126720_PID_BOOTLDR_VER;             pfpkt_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pfpkt_dat + pfpkt_ofst) = PGN126720_05_PRODUCTCODE;              pfpkt_ofst+=sizeof(uint16_t);
    *(uint8_t *)(pfpkt_dat + pfpkt_ofst) = PGN126720_05_PROCESSORCODE_MASTER;     pfpkt_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pfpkt_dat + pfpkt_ofst) = PGN126720_05_BOOTVER;                  pfpkt_ofst+=sizeof(uint16_t);
    *(uint16_t*)(pfpkt_dat + pfpkt_ofst) = PGN126720_05_APPVER;                   pfpkt_ofst+=sizeof(uint16_t);

    putFastpktQueue(&txpkt, pfpkt_dat, fastdat_len_trunc);

    vPortFree(pfpkt_dat);
}

void chkBootStat(RxProtocol rxpkt)//    Boot State Acknowledgment
{
    uint16_t ppgn_mfgcode = (uint16_t)PPGN_MFGCODE;

    if(memcmp((uint8_t*)&rxpkt.dat64, (uint8_t*)(&ppgn_mfgcode), sizeof(ppgn_mfgcode)))
    {
        printf("%s() Error PPGN_MFGCODE[%d]\n",__FUNCTION__, MFG_CODE);
        return;
    }

    Pgn065285BootStatAck();
}

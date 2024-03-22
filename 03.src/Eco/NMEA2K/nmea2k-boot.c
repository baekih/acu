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
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126720_05_LEN);

    TxProtocol txpkt =
    {
        .canid = (PGN126720_PRI<<26)|((PGN126720_NUM+pfastpkt->src_addr)<<8)|(NMEA2K_THIS_ADDR<<0),
        .len = PGN126720_05_LEN
    };

    uint8_t *pfastpkt_dat = pvPortMalloc(fastdat_len_trunc);

    *(uint64_t*)(pfastpkt_dat+8) =
        ((uint64_t)(PGN126720_05_APPVER)                  << (8));

    *(uint64_t*)(pfastpkt_dat+0) =
        ((uint64_t)(PGN126720_05_BOOTVER)                 << (8+16+8+16+8))       | \
        ((uint64_t)(PGN126720_05_PROCESSORCODE_MASTER)    << (8+16+8+16))         | \
        ((uint64_t)(PGN126720_05_PRODUCTCODE)             << (8+16+8))              | \
        ((uint64_t)(PGN126720_PID_BOOTLDR_VER)            << (8+16))              | \
        ((uint64_t)(PPGN_MFGCODE)                         << (8))                 | \
        ((uint64_t)(PGN126720_05_LEN)                     << (0));

    opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
}

void opBootStatChk(RxProtocol rxpkt)//    Boot State Acknowledgment
{
    uint16_t ppgn_mfgcode = (uint16_t)PPGN_MFGCODE;

    if(memcmp((uint8_t*)&rxpkt.dat64, (uint8_t*)(&ppgn_mfgcode), sizeof(ppgn_mfgcode)))
    {
        printf("%s() Error PPGN_MFGCODE[%d]\n",__FUNCTION__, MFG_CODE);
        return;
    }

    Pgn065285BootStatAck();
}

void Pgn126720Proc(fastpacket* pfastpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    int32_t op = 0;
    uint8_t pid = pfastpkt->dat[2]; // properity id(command code)
    uint8_t sid = pfastpkt->dat[3]; // sequence id
    uint8_t cmd = pfastpkt->dat[4]; // flash command
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126720_06_LEN);

    uint8_t *pfastpkt_dat = pvPortMalloc(fastdat_len_trunc);
    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

    TxProtocol txpkt =
    {
        .canid = (PGN126720_PRI<<26)|((PGN126720_NUM+pfastpkt->src_addr)<<8)|(NMEA2K_THIS_ADDR<<0),
        .len = PGN126720_06_LEN
    };


    *(uint64_t*)(pfastpkt_dat+0) =
        ((uint64_t)(PGN126720_06_PROCESSCODE_MASTER)      << (8+16+8+8))          | \
        ((uint64_t)(sid)                                  << (8+16+8))            | \
        ((uint64_t)(PGN126720_PID_BOOTLDR_STAT)           << (8+16))              | \
        ((uint64_t)(PPGN_MFGCODE)                         << (8))                 | \
        ((uint64_t)(PGN126720_06_LEN)                     << (0));

//    printf("pid[%d]\n",pid);

    switch(pid)
    {
    case PGN126720_PID_BOOTLDR_CMD:
    {
        *(pfastpkt_dat+6) = (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
//        printf("cmd[%d]\n",cmd);

        switch(cmd)
        {
        case PGN126720_CMD_0LOCK_FLASH:
            printf("lockFlash\n");
            HAL_FLASH_Lock();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
            break;
        case PGN126720_CMD_1UNLOCK_FLASH:
            printf("unlockFlash\n");
            HAL_FLASH_Unlock();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED & 0x07;
            break;
        case PGN126720_CMD_2ERASE_FLASH:
            printf("eraseFlash\n");
//            doFlashErase();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED & 0x07;
            break;
        case PGN126720_CMD_3READY_FLASH:
            printf("readyFlash\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM & 0x07;
            break;
        case PGN126720_CMD_4VERIFY_FLASH:
            printf("verifyFlash\n");
            //No need ToDo
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_3UNLOCKVERIFY & 0x07;
            break;
        case PGN126720_CMD_5ABORT_FLASH:
            printf("abortFlash\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        case PGN126720_CMD_6CHANGE_PROCESSOR:
            printf("chgProcessor\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
            break;
        case PGN126720_CMD_254OUTOFRANGE:
            printf("errOutofRangeFlash\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        case PGN126720_CMD_255DONTCHANGE:
            printf("errDontChgFlash\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        default:
            break;
        }
    }
        break;
    case PGN126720_PID_BOOTLDR_DAT:
    {
        uint8_t  *psrec_base = &pfastpkt->dat[6];
        uint8_t  srec_typ = getStr2Uint(psrec_base + PGN126720_11DAT_TYP_SIZE, PGN126720_11DAT_TYP_SIZE);
        uint8_t  srec_len;
        uint8_t  srec_dat_len;
        uint32_t srec_adr;
        uint8_t  *psrec_dat = NULL;
        uint8_t  srec_chksum;
        uint8_t  srec_chksum_local;
        uint8_t  srec_bin_dat[PGN126720_11DAT_DAT_MAX];

        *(pfastpkt_dat+6) = PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM & 0x07;
//        printf("srec_typ[%d]\n",srec_typ);
        switch(srec_typ)
        {
        case PGN126720_11DAT_TYP_0START_FLASH:
            psrec_dat = psrec_base + 8;
            g_version_dat.boot_ver = getStr2Uint(psrec_dat + 16, 4);
            g_version_dat.app_ver  = getStr2Uint(psrec_dat + 20, 4);
            printf("StartFlash boot[%d] app[%d]\n", g_version_dat.boot_ver, g_version_dat.app_ver);
            *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        case PGN126720_11DAT_TYP_3WRITE_FLASH:
            psrec_dat = psrec_base + 12;
            srec_len = getStr2Uint(psrec_base + 2, PGN126720_11DAT_LEN_SIZE);
            srec_dat_len = srec_len - 1 - 4;
            srec_adr = getStr2Uint(psrec_base + 4, PGN126720_11DAT_ADR_SIZE);
            srec_chksum = getStr2Uint(psrec_base + 2*(1 + srec_len), PGN126720_11DAT_CHKSUM_SIZE);
            srec_chksum_local = 0;
            printf("WriteToFlash adr[0x%lx] len[%d] chksum[0x%x]\n", srec_adr, srec_dat_len, srec_chksum);

            if(0 != srec_adr%0x20)
            {
                *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_5BUFOVERFLOW<<3) & 0xF8;
                break;
            }

            for(uint32_t i=1; i<1+1+4; i++)
            {
                srec_chksum_local += getStr2Uint(psrec_base + 2*i, 2);
            }

            for(uint32_t i=0; i<srec_dat_len; i++)
            {
                srec_bin_dat[i] = getStr2Uint(psrec_dat + 2*i, 2);
                srec_chksum_local += srec_bin_dat[i];
            }
            srec_chksum_local = 0xFF - srec_chksum_local;

            if(srec_chksum_local != srec_chksum)
            {
                *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_7CHKSUMERR<<3) & 0xF8;
                printf("chksum err [0x%02x:0x%02x]\n", srec_chksum, srec_chksum_local);
                break;
            }

            if(0/*FLASHIF_OK != IAP_Write(srec_adr, (uint64_t*)(&srec_bin_dat[0]), g_PGN126720DATS3NAME.mLEN/8)*/)
            {
                *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_2PROGERR<<3) & 0xF8;
                break;
            }

            *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        case PGN126720_11DAT_TYP_5COUNT_FLASH:
            printf("CountFlash\n");
            *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        case PGN126720_11DAT_TYP_7END_FLASH:
            printf("FinishFlash\n");
            *(pfastpkt_dat+6) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        }
    }
        break;
    case PGN126720_PID_MASTER_RESET:
    case PGN126720_PID_APP_LAUNCH:
        printf("Launch APP... reset device.\r\n");
        op = 1;
        break;
    default:
        printf("PGN1276720:?? Invaild prop_id\n");
        op = 2;
        break;
    }

    if(op == 0) opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
}

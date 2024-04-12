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

    uint8_t *pf_dat = pvPortMalloc(fastdat_len_trunc);
    uint32_t pf_ofst = 0;

    *(uint8_t *)(pf_dat + pf_ofst) = PGN126720_05_LEN;                      pf_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pf_dat + pf_ofst) = PPGN_MFGCODE;                          pf_ofst+=sizeof(uint16_t);
    *(uint8_t *)(pf_dat + pf_ofst) = PGN126720_PID_BOOTLDR_VER;             pf_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pf_dat + pf_ofst) = PGN126720_05_PRODUCTCODE;              pf_ofst+=sizeof(uint16_t);
    *(uint8_t *)(pf_dat + pf_ofst) = PGN126720_05_PROCESSORCODE_MASTER;     pf_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pf_dat + pf_ofst) = PGN126720_05_BOOTVER;                  pf_ofst+=sizeof(uint16_t);
    *(uint16_t*)(pf_dat + pf_ofst) = PGN126720_05_APPVER;                   pf_ofst+=sizeof(uint16_t);

    putFastpktQueue(&txpkt, pf_dat, fastdat_len_trunc);

    vPortFree(pf_dat);
}

void Pgn126720Proc(fastpacket* pfastpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
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

    *(uint64_t*)(pfastpkt_dat) = \
        ((uint64_t)(PGN126720_06_LEN)                     << (0))           |\
        ((uint64_t)(PPGN_MFGCODE)                         << (8))           |\
        ((uint64_t)(PGN126720_PID_BOOTLDR_STAT)           << (8+16))        | \
        ((uint64_t)(sid)                                  << (8+16+8))      | \
        ((uint64_t)(PGN126720_06_PROCESSCODE_MASTER)      << (8+16+8+8));

//    printf("pid[%d]\n",pid);

    switch(pid)
    {
    case PGN126720_PID_BOOTLDR_CMD:
    {
        *(pfastpkt_dat+6) = (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
        printf("cmd[%d]\n",cmd);

        switch(cmd)
        {
        case PGN126720_CMD_0LOCK_FLASH:
            printf("PGN126720_CMD_0LOCK_FLASH\n");
            HAL_FLASH_Lock();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
            break;
        case PGN126720_CMD_1UNLOCK_FLASH:
            printf("PGN126720_CMD_1UNLOCK_FLASH\n");
            HAL_FLASH_Unlock();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED & 0x07;
            break;
        case PGN126720_CMD_2ERASE_FLASH:
            printf("PGN126720_CMD_2ERASE_FLASH\n");
            eraseFlashApp();
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED & 0x07;
            break;
        case PGN126720_CMD_3READY_FLASH:
            printf("PGN126720_CMD_3READY_FLASH\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM & 0x07;
            break;
        case PGN126720_CMD_4VERIFY_FLASH:
            printf("PGN126720_CMD_4VERIFY_FLASH\n");
            //No need ToDo
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_3UNLOCKVERIFY & 0x07;
            break;
        case PGN126720_CMD_5ABORT_FLASH:
            printf("PGN126720_CMD_5ABORT_FLASH\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        case PGN126720_CMD_6CHANGE_PROCESSOR:
            printf("PGN126720_CMD_6CHANGE_PROCESSOR\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
            break;
        case PGN126720_CMD_254OUTOFRANGE:
            printf("PGN126720_CMD_254OUTOFRANGE\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        case PGN126720_CMD_255DONTCHANGE:
            printf("PGN126720_CMD_255DONTCHANGE\n");
            *(pfastpkt_dat+6) |= PGN126720_6STATUS_PROGRAMMODE_0LOCKED & 0x07;
          break;
        default:
            break;
        }
    }
        break;
    case PGN126720_PID_BOOTLDR_DAT:
    {
        pgn126720_boot_srec srec =
        {
            .pbase = &pfastpkt->dat[6],
            .typ = getStr2Uint(srec.pbase + PGN126720_11DAT_TYP_SIZE, PGN126720_11DAT_TYP_SIZE)
        };


        *(srec.pbase) = PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM & 0x07;
        printf("srec.typ[%d]\n",srec.typ);

        switch(srec.typ)
        {
        case PGN126720_11DAT_TYP_0START_FLASH:
        {
            pgn126720_boot_srec_hdr1_raw srec_hdr1_raw;
            memcpy(&srec_hdr1_raw, srec.pbase, sizeof(pgn126720_boot_srec_hdr1_raw)-2-4-4);

            g_version_dat.boot_ver = getStr2Uint(srec_hdr1_raw.bootver, sizeof(srec_hdr1_raw.bootver));
            g_version_dat.app_ver  = getStr2Uint(srec_hdr1_raw.appver,  sizeof(srec_hdr1_raw.appver ));
            printf("StartFlash boot[%d] app[%d]\n", g_version_dat.boot_ver, g_version_dat.app_ver);
            printf("curr date %d/%d/%d-%d:%d:%d\n", getStr2Uint(srec_hdr1_raw.year,  sizeof(srec_hdr1_raw.year )),
                                                    getStr2Uint(srec_hdr1_raw.month, sizeof(srec_hdr1_raw.month)),
                                                    getStr2Uint(srec_hdr1_raw.day,   sizeof(srec_hdr1_raw.day  )),
                                                    getStr2Uint(srec_hdr1_raw.hour,  sizeof(srec_hdr1_raw.hour )),
                                                    getStr2Uint(srec_hdr1_raw.min,   sizeof(srec_hdr1_raw.min  )),
                                                    getStr2Uint(srec_hdr1_raw.sec,   sizeof(srec_hdr1_raw.sec  )));

            *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
        }
            break;
        case PGN126720_11DAT_TYP_3WRITE_FLASH:
        {
            printf("write flash Enter\n");

            srec.psdat = srec.pbase + 12;
            srec.sdat_len = getStr2Uint(srec.pbase + 2, PGN126720_11DAT_LEN_SIZE);
            srec.adr = getStr2Uint(srec.pbase + 4, PGN126720_11DAT_ADR_SIZE);
            srec.crc_rcv = getStr2Uint(srec.pbase + 2*(1 + srec.sdat_len), PGN126720_11DAT_CHKSUM_SIZE);

            srec.dat_len = srec.sdat_len - 4 - 1; // 4:addr size. 1:crc8 size.
            srec.crc_cal = 0;

            if(0 != srec.adr%0x10) //address align error. address jump unit must be 16byte, 32byte, 64byte.
            {
                printf("addr misalign Error.\n");
                *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_9SRECORDERR<<3) & 0xF8;
                break;
            }

            // calculate crc8 per .srec(aka. .ax) line.
            for(uint32_t i=1; i<1+1+4; i++) //calculate crc8 in header.
            {
                srec.crc_cal += getStr2Uint(srec.pbase + 2*i, 2);
            }

            for(uint32_t i=0; i<srec.dat_len; i++) //calculate crc8 in dat field.
            {
                srec.dat[i] = getStr2Uint(srec.psdat + 2*i, 2);
                srec.crc_cal += srec.dat[i];
            }

            srec.crc_cal = 0xFF - srec.crc_cal; //bit upset crc8 data.

            if(srec.crc_cal != srec.crc_rcv) //if crc_cal and crc_rcv different. crc error.
            {
                *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_7CHKSUMERR<<3) & 0xF8;
                printf("crc8 Error[0x%02x:0x%02x]\n", srec.crc_rcv, srec.crc_cal);
                break;
            }
            // crc8 chk end.

            // flash app binary.
            if(0/*FLASHIF_OK != IAP_Write(srec.adr, (uint64_t*)srec.dat, g_PGN126720DATS3NAME.mLEN/8)*/)
            {
                printf("write to flash[adr:0x%02x dat0:0x%02x len:%d] Error.\n", srec.adr, srec.dat[0], srec.dat_len);
                *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_2PROGERR<<3) & 0xF8;
                break;
            }

            printf("write to flash[adr:0x%02x dat0:0x%02x len:%d crc8:%x:%x] ok.\n",
                   srec.adr, srec.dat[0], srec.dat_len, srec.crc_rcv, srec.crc_cal);
            *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
        }
            break;
        case PGN126720_11DAT_TYP_5COUNT_FLASH:
            printf("CountFlash\n");
            *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        case PGN126720_11DAT_TYP_7END_FLASH:
            printf("FinishFlash\n");
            *(srec.pbase) |= (PGN126720_6STATUS_STATUSOFOP_0NOERR<<3) & 0xF8;
            break;
        }
    }
        break;
    case PGN126720_PID_MASTER_RESET:
    case PGN126720_PID_APP_LAUNCH:
        printf("Launch APP... reset device.\r\n");
        osDelay(NVIC_RST_CNT_MAX);
        HAL_NVIC_SystemReset();
        break;
    default:
        printf("PGN1276720:?? Invaild prop_id\n");
        break;
    }

    putFastpktQueue(&txpkt, pfastpkt_dat, fastdat_len_trunc);
    vPortFree(pfastpkt_dat);
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

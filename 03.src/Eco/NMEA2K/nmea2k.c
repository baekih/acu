/*
 * nmea2k.c
 *
 *  Created on: Mar 30, 2023
 *      Author: ihbaek
 */

#include "eco.h"

uint64_t g_pgn060928_dat64 =       \
    (ISO_SELF_CFG       << 63) | \
    (IND_GRP            << 60) | \
    (SYS_INSTANCE       << 56) | \
    (DEV_CLASS          << 49) | \
    (NMEA_RSV           << 48) | \
    (DEV_FUNC           << 40) | \
    (DEV_INSTANCE_HI    << 35) | \
    (DEV_INSTANCE_LO    << 32) | \
    (MFG_CODE           << 21) | \
    (NMEA2K_ID          << 0);

pgn126996_dat g_pgn126996_dat =
{
    NMEA_NET_MSG_DB_VER,
    NMEA_MFG_PROD_CODE,
    MFG_MODEL_ID,
    MFG_SW_VER_CODE,
    MFG_MODEL_VER,
    MFG_MODEL_SER_CODE,
    MFG_NMEA2K_CERT_LVL,
    LOAD_EQ
};

pgn127502_dat g_swbnkctl_dat =
{
    .dat =
    {
        0,
        0x00,
        0x00,
        0xf0,
        0xff,
        0xff,
        0xff,
        0xff
    }
};

int32_t Pgn059392ISOAck(RxProtocol rxpkt)
{
    uint32_t txpgn = 0;

    memcpy(&txpgn, rxpkt.dat, rxpkt.len);

    TxProtocol txpkt =
    {
        .canid = (PGN059392_PRI<<26)|((PGN059392_NUM+getRxSA(rxpkt.canid))<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = \
            ((uint64_t)(txpgn)       << 40) | \
            (0xFFFFFFUL              << 16) | \
            (0xFF                    <<  8) | \
            (PGN059392_NEGATIVIE_ACK <<  0),
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}


int32_t Pgn060928ISOAddrClame(RxProtocol rxpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        .canid = (PGN060928_PRI<<26)|((PGN060928_NUM+getRxSA(rxpkt.canid))<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = g_pgn060928_dat64,
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn065240ISOCmdAddr(void)
{
//    printf("%s() Enter\n",__FUNCTION__);
//    dumpMultiPacket(__FUNCTION__, &g_multipacket);

    if(!memcmp(&g_pgn060928_dat64, &g_multipacket.dat[0], sizeof(uint64_t)))
    {
        printf("ISOAddrClameDat matched. new address is %d\n", g_multipacket.dat[8]);
    }
    else printf("ISOAddrClameDat diff\n");

    g_multipacket.status = MULTIPACKET_BUF_EMPTY;

    return 0;
}

int32_t Pgn065288Brightness(void)//    Brightness
{
    printf("%s() Called\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        .canid = (PGN065288_PRI<<26)|((PGN065288_NUM)<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = \
            ((uint64_t)(PPGN_MFGCODE)       << (0))               |\
            ((uint64_t)(0)                  << (16))              |\
            ((uint64_t)(0x40)               << (16+8))            |\
            ((uint64_t)(50)                 << (16+8+8))          |\
            ((uint64_t)(RSV_24BIT)          << (16+8+8+8)),
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn126993HeartBeat(void)
{
    static uint8_t cnt_heartbeat = 0;
    TxProtocol txpkt =
    {
        .canid = (PGN126993_PRI<<26)|(PGN126993_NUM<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 =
            ((uint64_t)RSV_32BIT               << 32) | \
            ((uint64_t)RSV_2BIT                << 30) | \
            ((uint64_t)PGN126993_EQUIP_STATUS  << 28) | \
            ((uint64_t)PGN126993_CAN_STATUS_2  << 26) | \
            ((uint64_t)PGN126993_CAN_STATUS_1  << 24) | \
            ((uint64_t)cnt_heartbeat           << 16) | \
            ((uint64_t)PGN126993_UPDATE_RATE   <<  0),

        .len = sizeof(uint64_t)
    };

    if(252 < ++cnt_heartbeat) cnt_heartbeat = 0;

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn126996ProdInfo(void)
{
    TxProtocol txpkt =
    {
        .canid = (PGN126996_PRI<<26)|(PGN126996_NUM<<8)|(NMEA2K_THIS_ADDR<<0),
        .len = sizeof(uint64_t)
    };
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126996_LEN);
    uint8_t *pf_dat = pvPortMalloc(fastdat_len_trunc);
    uint32_t pf_ofst = 0;

    *(uint8_t *)(pf_dat + pf_ofst) = PGN126996_LEN;                     pf_ofst+=sizeof(uint8_t );
    *(uint16_t*)(pf_dat + pf_ofst) = g_pgn126996_dat.NMEANetMsgDbVer;   pf_ofst+=sizeof(uint16_t);
    *(uint16_t*)(pf_dat + pf_ofst) = g_pgn126996_dat.NMEAMfgProdCode;   pf_ofst+=sizeof(uint16_t);
    memcpy(pf_dat+pf_ofst, g_pgn126996_dat.MfgModelId, 32);             pf_ofst+=32;
    memcpy(pf_dat+pf_ofst, g_pgn126996_dat.MfgSWVerCode, 32);           pf_ofst+=32;
    memcpy(pf_dat+pf_ofst, g_pgn126996_dat.MfgModelVer, 32);            pf_ofst+=32;
    memcpy(pf_dat+pf_ofst, g_pgn126996_dat.MfgModelSerCode, 32);        pf_ofst+=32;
    *(uint8_t *)(pf_dat + pf_ofst) = g_pgn126996_dat.NMEA2KCertLvl;     pf_ofst+=sizeof(uint8_t );
    *(uint8_t *)(pf_dat + pf_ofst) = g_pgn126996_dat.LoadEq;            pf_ofst+=sizeof(uint8_t );

    putFastpktQueue(&txpkt, pf_dat, fastdat_len_trunc);

    vPortFree(pf_dat);
    return 0;
}

int32_t Pgn126208GrpFuncAck(fastpacket *pfastpkt, pgn126208_ack_dat *pack_dat)
{
    printf("%s() Enter pgn[%lu]\n",__FUNCTION__, pfastpkt->pgn);
    if(BROADCAST_DEST_ADDR == pfastpkt->des_addr) return 1;

    printf("%s() Run pgn[%lu]\n",__FUNCTION__, pfastpkt->pgn);
    TxProtocol txpkt =
    {
        .canid = (PGN126208_PRI<<26)|((PGN126208_NUM+pfastpkt->src_addr)<<8)|(NMEA2K_THIS_ADDR<<0),
        .len = sizeof(uint64_t)
    };
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126208_ACK_LEN_BASE + pack_dat->param_len);
    uint8_t *pfastpkt_dat = pvPortMalloc(PGN126208_ACK_LEN_BASE + pack_dat->param_len);
//    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

    *(pfastpkt_dat+0) = PGN126208_ACK_LEN_BASE + (pack_dat->param_len/2 + 1);
    *(pfastpkt_dat+1) = PGN126208_ACK_CODE;
    memcpy(pfastpkt_dat+2, &pack_dat->pgn_req, 3);
    *(pfastpkt_dat+5) = ((pack_dat->tx_intval_err << 4) & 0xF0) + (pack_dat->pgn_err & 0x0F);
    *(pfastpkt_dat+6) = pack_dat->param_len;
    for(uint32_t i=0; i<pack_dat->param_len; i++)
    {
        if(i%2 == 0) *(pfastpkt_dat+7+i/2)  = 0xF0 + (pack_dat->param_err[i]&0x0F);
        else         *(pfastpkt_dat+7+i/2) &= (pack_dat->param_err[i]<<4)|0x0F;
        printf("i[%ld] param_err[%d] fastpkt_dat[0x%02x]\n", i, pack_dat->param_err[i], *(pfastpkt_dat+7+i/2));
    }

    putFastpktQueue(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
    return 0;
}

int32_t Pgn126208GrpFuncReq(fastpacket *pfastpkt)
{
    printf("%s() Enter\n",__FUNCTION__);
    uint32_t tx_intval = 0;
    uint16_t tx_intval_offset = 0;
    uint8_t  param_idx = pfastpkt->dat[PGN126208_REQ_PARAM_BASE_POS];
    uint8_t  param_ptr = PGN126208_REQ_PARAM_BASE_POS + 1;
    pgn126208_ack_dat ack_dat =
    {
        (pfastpkt->dat[PGN126208_REQ_PGN_POS+2]<<16) + \
        (pfastpkt->dat[PGN126208_REQ_PGN_POS+1]<<8) + \
        (pfastpkt->dat[PGN126208_REQ_PGN_POS]),
        PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
        PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK,
        pfastpkt->dat[PGN126208_REQ_PARAM_TOTAL_POS],
        {0,}
    };

    memcpy(&tx_intval, &pfastpkt->dat[PGN126208_REQ_TX_INTVAL_POS], 4);
    memcpy(&tx_intval_offset, &pfastpkt->dat[PGN126208_REQ_TX_INTVAL_OFFSET_POS], 2);

    if(tx_intval == PGN126208_REQ_TX_INTVAL_DONOTCHANGE)
    {
        if(tx_intval_offset != PGN126208_REQ_TX_INTVAL_OFFSET_DONOTCHANGE
        && tx_intval_offset != PGN126208_REQ_TX_INTVAL_OFFSET_TXNOW)
        {
            ack_dat.tx_intval_err = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
            ack_dat.pgn_err = PGN126208_ACK_ERRORCODE_ACCESS_DENIED;
        }
    }
    else if(tx_intval == PGN126208_REQ_TX_INTVAL_RESTORE)
    {
        if(pfastpkt->des_addr != BROADCAST_DEST_ADDR)
        {
            ack_dat.tx_intval_err = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
            ack_dat.pgn_err = PGN126208_ACK_ERRORCODE_ACCESS_DENIED;
        }
    }
    else
    {
        ack_dat.tx_intval_err = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
        ack_dat.pgn_err = PGN126208_ACK_ERRORCODE_ACCESS_DENIED;
    }

    if(ack_dat.tx_intval_err == PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED)
    {
        Pgn126208GrpFuncAck(pfastpkt, &ack_dat);
        return 1;
    }

    switch(ack_dat.pgn_req)
    {
    case PGN126720_NUM:
    {
        uint8_t param_offset[] = {2, 1, 1, 1, 2, 1, 2, 2};
        uint8_t param_err_cnt = 0;
        bool    param_valid;

        for(uint32_t i=0; i<ack_dat.param_len; i++)
        {
            param_valid = true;
            switch(param_idx)
            {
            case MFG_CODE_AIRMAR_POS:
                if(MFG_CODE != (*((uint16_t*)(&pfastpkt->dat[param_ptr])) & 0x7FF)) param_valid = false;
                break;
            case IND_GRP_POS:
                if(IND_GRP != (uint8_t)(pfastpkt->dat[param_ptr] & 0x07)) param_valid = false;
                break;
            case PGN126720_05_PROP_ID_IDX:
                if(PGN126720_05_PROP_ID != pfastpkt->dat[param_ptr]) param_valid = false;
                break;
            default:
                param_valid = false;
                break;
            }

            if(param_valid == false)
            {
                ack_dat.param_err[i] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
                param_err_cnt++;
            }
            else
            {
                ack_dat.param_err[i] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
            }

            param_ptr += param_offset[param_idx-1];
            param_idx = pfastpkt->dat[param_ptr];
            param_ptr++;
        }

        if(0 < param_err_cnt)
        {
            Pgn126208GrpFuncAck(pfastpkt, &ack_dat);
            return 2;
        }

        osDelay(1);
        Pgn126720BootVer(pfastpkt);
    }
        break;
    case PGN065288_NUM:
        printf("PGN065288 requested\n");
        break;

    default:
        break;
    }

    return 0;
}

int32_t Pgn126208Proc(fastpacket *pfastpkt)
{
    uint8_t grp_func_code = pfastpkt->dat[0];

    printf("PGN126208:%02d\n", grp_func_code);

    switch(grp_func_code)
    {
    case PGN126208_REQ_CODE:
        Pgn126208GrpFuncReq(pfastpkt);
        break;
    case PGN126208_CMD_CODE:
        break;
    default:
        printf("PGN126208:%02d No vaild grp_func_code\n", grp_func_code);
        break;
    }

    return 0;
}

int32_t Pgn127502SwitchBankControl(pgn127502_dat swbnkctl)
{
    printf("%s() Called\n",__FUNCTION__);

    TxProtocol txpkt =
    {
        .canid = (PGN127502_PRI<<26)|((PGN127502_NUM)<<8)|(NMEA2K_THIS_ADDR<<0),
        .dat64 = swbnkctl.dat64,
        .len = sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

void opNMEA2K(RxProtocol rxpacket)
{
    uint32_t rxpgn = getPGN(rxpacket.canid);
    uint32_t txpgn = 0;

//    printf("pgn rx:%ld\n", rxpgn);

    //Single and Fast/Multi packet build-up.
    switch(rxpgn)
    {
    case PGN059904_NUM: // ISO Request
        memcpy(&txpgn, (uint8_t*)&rxpacket.dat[0], rxpacket.len);
        printf("pgn tx:%ld\n", txpgn);

        switch(txpgn)
        {
        case PGN060928_NUM:
            Pgn060928ISOAddrClame(rxpacket);
            break;
        case PGN065288_NUM: // Brightness control
            Pgn065288Brightness();
            break;
        case PGN126996_NUM:
            Pgn126996ProdInfo();
            break;
        case PGN127502_NUM:
            Pgn127502SwitchBankControl(g_swbnkctl_dat);
            break;
        default:
            if(BROADCAST_DEST_ADDR != getRxPS(rxpacket.canid)) Pgn059392ISOAck(rxpacket);
            break;
        }
        break;
    case PGN060160_NUM: // Multipacket data
        Pgn060160MultiPktDataRx(&rxpacket);
        break;
    case PGN060416_NUM: // Multipacket control
        Pgn060416MultiPktCtrl(rxpacket);
        break;
    case PGN065286_NUM: // Boot State Request
        chkBootStat(rxpacket);
        break;
    case PGN065288_NUM: // Boot State Request
        setLCDBrightness(rxpacket.dat[4]);
        break;
        //Fastpacket build-up.
    case PGN126208_NUM: // NMEA2K Group Function
    case PGN126720_NUM: // Various Function
        bldFastpacket(&rxpacket);
        break;
    case PGN127502_NUM: // Switch bank control
        g_swbnkctl_dat.dat64 = rxpacket.dat64;
        doSwitchBankControl(&g_swbnkctl_dat.dat64);
        Pgn127502SwitchBankControl(g_swbnkctl_dat);
        break;
    default:
        printf("Single PGNError[%ld]\n", rxpgn);
        break;
    }

    //Fastpacket.
    for(uint32_t i=0; i<FASTPACKET_ARRAY_MAX; i++)
    {
        fastpacket *pfastpkt = &g_fastpacket[i];

        //If fastpacket build-up not completely, skip to next.
        if(pfastpkt->status != FASTPACKET_BUF_AVAILABLE)
        {
            continue;
        }

        switch(pfastpkt->pgn)
        {
        case PGN126208_NUM: // NMEA2K Group Function
            Pgn126208Proc(pfastpkt);
            break;
        case PGN126720_NUM: // Various Functions
            Pgn126720Proc(pfastpkt);
            break;
        default:
            printf("Fastpacket PGNError[%ld]\n", pfastpkt->pgn);
            break;
        }

        memset(pfastpkt, 0x00, sizeof(fastpacket));
    }

    //Multipacket.
    if(g_multipacket.status == MULTIPACKET_BUF_AVAILABLE) //If multipacket build-up completely, start multipacket pgn.
    {
        switch(g_multipacket.pgn)
        {
        case PGN065240_NUM: // Set ISO Command Address
            Pgn065240ISOCmdAddr();
            break;
        default:
            printf("Multipacket PGNError[%ld]\n", g_multipacket.pgn);
            break;
        }

        memset(&g_multipacket, 0x00, sizeof(multipacket));
    }

    return;
}

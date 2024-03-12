/*
 * nmea2k_multi.c
 *
 *  Created on: Mar 11, 2024
 *      Author: ihbaek
 */

#include "eco.h"

fastpacket g_fastpacket[FASTPACKET_ARRAY_MAX];
multipacket g_multipacket;

void dumpFastPacket(const char *pfuncname, fastpacket *pfastpkt)
{
    printf("%s() fastpacket time[%ld] seq[%d] len[%d] dat[%02x", pfuncname,
            osKernelGetTickCount() - pfastpkt->timestamp, pfastpkt->seqid, pfastpkt->len, pfastpkt->dat[0]);
    for(uint32_t i = 1; i < pfastpkt->len; i++) printf(":%02x", pfastpkt->dat[i]);
    printf("]\n");
}

void dumpMultiPacket(const char *pfuncname, multipacket *pmultipkt)
{
    printf("%s() multipkt.dat[%02x",pfuncname, pmultipkt->dat[0]);
    for(uint32_t i=1; i<pmultipkt->TotalByteLen; i++) printf(":%02x", pmultipkt->dat[i]);
    printf("]\n");
}

uint8_t getFastpacketTxSeqId(void)
{
    static uint8_t fastpacket_tx_seq_id = 7;

    if(++fastpacket_tx_seq_id == 8) fastpacket_tx_seq_id = 0;

    return fastpacket_tx_seq_id << 5;
}

int32_t opFastpktQueuePut(TxProtocol *ptxpkt, uint8_t *pfastpkt_dat, uint8_t fastdat_len_trunc)
{
    uint8_t fastpkt_tx_seq_id = getFastpacketTxSeqId();

    for(uint32_t i = 0; i < (fastdat_len_trunc/7); i++)
    {
        ptxpkt->dat[0] = fastpkt_tx_seq_id + i;
        memcpy(&ptxpkt->dat[1], pfastpkt_dat + 7*i, 7);
        EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)ptxpkt, sizeof(TxProtocol));
    }

    return 0;
}

int32_t opFastpacketBuildup(RxProtocol *prxpkt)
{
    uint8_t cnt_frame, cnt_seqid;
    fastpacket *pfastpkt;

    cnt_frame = prxpkt->dat[0] & 0x1F;
    cnt_seqid = (prxpkt->dat[0] & 0xE) >> 5;

    // build-up fastpacket
    if(cnt_frame == 0)  //start build-up fastpacket.
    {
        // no fastpacket buffer ramain, abandon it.
        if     (g_fastpacket[0].status == FASTPACKET_BUF_EMPTY)
        {
            pfastpkt = &g_fastpacket[0];
        }
        else if(g_fastpacket[1].status == FASTPACKET_BUF_EMPTY)
        {
            pfastpkt = &g_fastpacket[1];
        }
        else
        {
            printf("fastpacket no buf empty\n");
            return -1;
        }

        memset(pfastpkt, 0x00, sizeof(fastpacket));

        pfastpkt->status = FASTPACKET_BUF_MERGING;
        pfastpkt->seqid = cnt_seqid;
        pfastpkt->pgn = getPGN(prxpkt->canid);
        pfastpkt->src_addr = getRxSA(prxpkt->canid);
        pfastpkt->des_addr = getRxDA(prxpkt->canid);
        memcpy(&pfastpkt->dat[0], &prxpkt->dat[2], 6);
        pfastpkt->len = prxpkt->dat[1];
        pfastpkt->timestamp = osKernelGetTickCount();

        if(cnt_frame < (pfastpkt->len)/7)
        {
//            printf("fastpacket start success\n");
            return 1;
        }
    }
    else  //continue build-up fastpacket.
    {
        if     (g_fastpacket[0].seqid == cnt_seqid)
        {
            pfastpkt = &g_fastpacket[0];
        }
        else if(g_fastpacket[1].seqid == cnt_seqid)
        {
            pfastpkt = &g_fastpacket[1];
        }
        else
        {
            printf("fastpacket seqid invalid\n");
            return -2;
        }

        if((NMEA2K_FASTPACKET_TIMEOUT_MS <= osKernelGetTickCount() - pfastpkt->timestamp)
         ||(pfastpkt->status != FASTPACKET_BUF_MERGING))
        {
            memset(pfastpkt, 0x00, sizeof(fastpacket));
            printf("timeout or reset cnt\n");
            return -3;
        }

        pfastpkt->timestamp = osKernelGetTickCount();
        memcpy(&pfastpkt->dat[6 + 7*(cnt_frame-1)], &prxpkt->dat[1], 7);

        if(cnt_frame < (pfastpkt->len)/7)
        {
//            printf("fastpacket bldup incomplete\n");
            return 2; // fastpacket not yet build-up.
        }
    }

    // fastpacket build-up success. so, process fastpacket.
//    printf("fpkt bldup OK\n");
    pfastpkt->status = FASTPACKET_BUF_AVAILABLE;
    return 0;
}

int32_t opChkMultiPktRunning(RxProtocol *prxpkt)
{
    // Invalid Source Address.
    if(g_multipacket.SA != getRxSA(prxpkt->canid)) return -1;

    // Invauld Destination Address.
    if(g_multipacket.DA != getRxDA(prxpkt->canid)) return -2;

    // Frame count mismatch.
    if(g_multipacket.CurrFrmNum != prxpkt->dat[0]) return -3;

    return 0;
}

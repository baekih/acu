/*
 * nmea2k.c
 *
 *  Created on: Mar 30, 2023
 *      Author: ihbaek
 */

#include "eco.h"

version_dat g_version_dat;

uint8_t g_switch_bank[6];
uint8_t g_lcd_img_idx = 0;

pgn060928_dat g_pgn60928_dat =
{
    NMEA2K_ID,
    PPGN_MFGCODE,
    DEV_INSTANCE,
    DEV_FUNC,
    DEV_CLASS,
    SYS_INSTANCE,
    IND_GRP,
    ISO_SELF_CFG
};

uint64_t g_pgn060928_dat_raw =   \
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

bool IsKnownPGN(uint32_t PGN)
{
    uint32_t DefSinglePGN[] = {
        126992,  // System date/time, pri=3, period=1000
        126993,  // Heartbeat, pri=7, period=60000
        127245,  // Rudder, pri=2, period=100
        127250,  // Vessel Heading, pri=2, period=100
        127251,  // Rate of Turn, pri=2, period=100
        127257,  // Attitude, pri=3, period=1000
        127488,  // Engine parameters rapid, rapid Update, pri=2, period=100
        127493,  // Transmission parameters: dynamic, pri=2, period=100
        127501,  // Binary status report, pri=3, period=NA
        127502,  // Switch bank control, pri=3, period=NA
        127505,  // Fluid level, pri=6, period=2500
        127508,  // Battery Status, pri=6, period=1500
        128259,  // Boat speed, pri=2, period=1000
        128267,  // Water depth, pri=3, period=1000
        129025,  // Lat/lon rapid, pri=2, period=100
        129026,  // COG SOG rapid, pri=2, period=250
        129283,  // Cross Track Error, pri=3, period=1000
        130306,  // Wind Speed, pri=2, period=100
        130310,  // Outside Environmental parameters, pri=5, period=500
        130311,  // Environmental parameters, pri=5, period=500
        130312,  // Temperature, pri=5, period=2000
        130313,  // Humidity, pri=5, period=2000
        130314,  // Pressure, pri=5, period=2000
        130316,  // Temperature extended range, pri=5, period=NA
        130576   // Small Craft Status (Trim Tab position), pri=2, period=200
    };

    uint32_t DefFastPGN[] = {
        126983,  // Alert, pri=2, period=1000
        126984,  // Alert Response, pri=2, period=NA
        126985,  // Alert Text, pri=2, period=10000
        126986,  // Alert Configuration, pri=2, period=NA
        126987,  // Alert Threshold, pri=2, period=NA
        126988,  // Alert Value, pri=2, period=10000
        127233,  // Alert Value, pri=3, period=NA
        127237,  // Heading/Track control, pri=2, period=250
        127489,  // Engine parameters dynamic, pri=2, period=500
        127496,  // Trip fuel consumption, vessel, pri=5, period=1000
        127497,  // Trip fuel consumption, engine, pri=5, period=1000
        127498,  // Engine parameters static, pri=5, period=NA
        127503,  // AC Input Status, pri=6, period=1500
        127504,  // AC Output Status, pri=6, period=1500
        127506,  // DC Detailed status, pri=6, period=1500
        127507,  // Charger status, pri=6, period=1500
        127509,  // Inverter status, pri=6, period=1500
        127510,  // Charger configuration status, pri=6, period=NA
        127511,  // Inverter Configuration Status, pri=6, period=NA
        127512,  // AGS configuration status, pri=6, period=NA
        127513,  // Battery configuration status, pri=6, period=NA
        127514,  // AGS Status, pri=6, period=1500
        128275,  // Distance log, pri=6, period=1000
        128520,  // Tracked Target Data, pri=2, period=1000
        129029,  // GNSS Position Data, pri=3, period=1000
        129038,  // AIS Class A Position Report, pri=4, period=NA
        129039,  // AIS Class B Position Report, pri=4, period=NA
        129040,  // AIS Class B Extended Position Report, pri=4, period=NA
        129041,  // AIS Aids to Navigation (AtoN) Report, pri=4, period=NA
        129044,  // Datum, pri=6, period=10000
        129045,  // User Datum Settings, pri=6, period=NA
        129284,  // Navigation info, pri=3, period=1000
        129285,  // Waypoint list, pri=3, period=NA
        129301,  // Time to/from Mark, pri=3, period=1000
        129302,  // Bearing and Distance between two Marks, pri=6, period=NA
        129538,  // GNSS Control Status, pri=6, period=NA
        129540,  // GNSS Sats in View, pri=6, period=1000
        129541,  // GPS Almanac Data, pri=6, period=NA
        129542,  // GNSS Pseudorange Noise Statistics, pri=6, period=1000
        129545,  // GNSS RAIM Output, pri=6, period=NA
        129547,  // GNSS Pseudorange Error Statistics, pri=6, period=NA
        129549,  // DGNSS Corrections, pri=6, period=NA
        129551,  // GNSS Differential Correction Receiver Signal, pri=6, period=NA
        129556,  // GLONASS Almanac Data, pri=6, period=NA
        129792,  // AIS DGNSS Broadcast Binary Message, pri=6, period=NA
        129793,  // AIS UTC and Date Report, pri=7, period=NA
        129794,  // AIS Class A Static data, pri=6, period=NA
        129795,  // AIS Addressed Binary Message, pri=5, period=NA
        129796,  // AIS Acknowledge, pri=7, period=NA
        129797,  // AIS Binary Broadcast Message, pri=5, period=NA
        129798,  // AIS SAR Aircraft Position Report, pri=4, period=NA
        129799,  // Radio Frequency/Mode/Power, pri=3, period=NA
        129800,  // AIS UTC/Date Inquiry, pri=7, period=NA
        129801,  // AIS Addressed Safety Related Message, pri=5, period=NA
        129802,  // AIS Safety Related Broadcast Message, pri=5, period=NA
        129803,  // AIS Interrogation PGN, pri=7, period=NA
        129804,  // AIS Assignment Mode Command, pri=7, period=NA
        129805,  // AIS Data Link Management Message, pri=7, period=NA
        129806,  // AIS Channel Management, pri=7, period=NA
        129807,  // AIS Group Assignment, pri=7, period=NA
        129808,  // DSC Call Information, pri=8, period=NA
        129809,  // AIS Class B Static Data: Part A, pri=6, period=NA
        129810,  // AIS Class B Static Data Part B, pri=6, period=NA
        129811,  // AIS Single Slot Binary Message, pri=5, period=NA
        129812,  // AIS Multi Slot Binary Message, pri=5, period=NA
        129813,  // AIS Long-Range Broadcast Message, pri=5, period=NA
        130052,  // Loran-C TD Data, pri=3, period=1000
        130053,  // Loran-C Range Data, pri=3, period=1000
        130054,  // Loran-C Signal Data, pri=3, period=1000
        130060,  // Label, pri=7, period=NA
        130061,  // Channel Source Configuration, pri=7, period=NA
        130064,  // Route and WP Service - Database List, pri=7, period=NA
        130065,  // Route and WP Service - Route List, pri=7, period=NA
        130066,  // Route and WP Service - Route/WP-List Attributes, pri=7, period=NA
        130067,  // Route and WP Service - Route - WP Name & Position, pri=7, period=NA
        130068,  // Route and WP Service - Route - WP Name, pri=7, period=NA
        130069,  // Route and WP Service - XTE Limit & Navigation Method, pri=7, period=NA
        130070,  // Route and WP Service - WP Comment, pri=7, period=NA
        130071,  // Route and WP Service - Route Comment, pri=7, period=NA
        130072,  // Route and WP Service - Database Comment, pri=7, period=NA
        130073,  // Route and WP Service - Radius of Turn, pri=7, period=NA
        130074,  // Route and WP Service - WP List - WP Name & Position, pri=7, period=NA
        130320,  // Tide Station Data, pri=6, period=1000
        130321,  // Salinity Station Data, pri=6, period=1000
        130322,  // Current Station Data, pri=6, period=1000
        130323,  // Meteorological Station Data, pri=6, period=1000
        130324,  // Moored Buoy Station Data, pri=6, period=1000
        130567,  // Watermaker Input Setting and Status, pri=6, period=2500
        130577,  // Direction Data PGN, pri=3, period=1000
        130578   // Vessel Speed Components, pri=2, period=250
    };

    uint32_t ManFastPGN[] = {
        126464,  // PGN List (Transmit and Receive), pri=6, period=NA
        126996,  // Product information, pri=6, period=NA
        126998   // Configuration information, pri=6, period=NA
    };

    uint32_t SysSinglePGN[] = {
        59392,  /* ISO Acknowledgement */
        60160,  /* Multi packet data transfer, TP.DT */
        60416,  /* Multi packet connection management, TP.CM */
        59904,  /* ISO Request */
        60928   /* ISO Address Claim */
    };

    uint32_t SysFastPGN[] = {
        65240,   /* Commanded Address*/
        126208,  /* NMEA Request/Command/Acknowledge group function */
    };

    uint32_t EcotroSinglePGN[] = {
        45312,  //ECO: Eeprom_Write
        49408,  //ECO: Eeprom_Read
        65286   //AIRMAR: Boot State Request
    };

    for(uint8_t i=0; i<sizeof(DefSinglePGN)/sizeof(uint32_t); i++)
    {
        if(PGN == DefSinglePGN[i]) return true;
    }

    for(uint8_t i=0; i<sizeof(DefFastPGN)/sizeof(uint32_t); i++)
    {
        if(PGN == DefFastPGN[i]) return true;
    }

    for(uint8_t i=0; i<sizeof(ManFastPGN)/sizeof(uint32_t); i++)
    {
        if(PGN == ManFastPGN[i]) return true;
    }

    for(uint8_t i=0; i<sizeof(SysSinglePGN)/sizeof(uint32_t); i++)
    {
        if(PGN == SysSinglePGN[i]) return true;
    }

    for(uint8_t i=0; i<sizeof(SysFastPGN)/sizeof(uint32_t); i++)
    {
        if(PGN == SysFastPGN[i]) return true;
    }

    if ((PGN == 61184) || (65280 <= PGN && PGN <= 65535)) return true; // PropSinglePGN

    if ((PGN == 126720) || (130816 <= PGN && PGN <= 131071)) return true; // PropFastPGN

    for(uint8_t i=0; i<sizeof(EcotroSinglePGN)/sizeof(uint32_t); i++)
    {
        if(PGN == EcotroSinglePGN[i]) return true;
    }

    return false;
}

uint8_t getRxPF(uint32_t canid)
{
    return (canid & 0x00FF0000) >> 16;
}

uint8_t getRxPS(uint32_t canid)
{
    return (canid & 0x0000FF00) >> 8;
}

uint8_t getRxDA(uint32_t canid)
{
    uint8_t RxPF = (canid & 0x00FF0000) >> 16;

    if(0 <= RxPF && RxPF <= 239) return (canid & 0x0000FF00) >> 8;  // PDU1
    else                         return 0xFF;                       // PDU2 Error
}

uint8_t getRxSA(uint32_t canid)
{
    return (canid & 0x000000FF) >> 0;
}

uint32_t getRxPGN(uint32_t canid)
{
    uint8_t RxPF = (canid & 0x00FF0000) >> 16;

    if(0 <= RxPF && RxPF <= 239) return (canid & 0x01FF0000) >> 8;  // PDU1
    else                         return (canid & 0x01FFFF00) >> 8;  // PDU2
}

uint32_t getPGN(uint32_t canid)
{
    if(g_multipacket.status == MULTIPACKET_BUF_AVAILABLE) return g_multipacket.pgn;
    else return getRxPGN(canid);
}

uint32_t getStr2Uint(uint8_t *pstr, uint8_t len)
{
    char *pbuf = (char*)pvPortMalloc(len + 1);
    uint32_t ret;

    memcpy(pbuf, pstr, len);
    *(pbuf+len) = 0;

    ret = strtoul(pbuf, NULL, 16);
    vPortFree(pbuf);

    return ret;
}

static int32_t opSwitchBankControl(uint8_t *prxdat)
{
    g_common_dat.bzr_vol = *prxdat;
    g_switch_bank[0] = (*(prxdat+1)>>0) & 0x03;
    g_switch_bank[1] = (*(prxdat+1)>>2) & 0x03;
    g_switch_bank[2] = (*(prxdat+1)>>4) & 0x03;
    g_switch_bank[3] = (*(prxdat+1)>>6) & 0x03;
    g_switch_bank[4] = (*(prxdat+2)>>0) & 0x03;
    g_switch_bank[5] = (*(prxdat+2)>>2) & 0x03;

    printf("bzr_vol[%02d] SwitchBank[%d:%d:%d:%d:%d:%d]\n", g_common_dat.bzr_vol,
           g_switch_bank[0],g_switch_bank[1],g_switch_bank[2],g_switch_bank[3],g_switch_bank[4],g_switch_bank[5]);

    return 0;
}

static int32_t opLCDBrightness(uint8_t *prxdat)
{
    g_common_dat.lcd_bl = *(prxdat+4);

    return 0;
}

int32_t Pgn060416CTSPostProc(void)
{
    TxProtocol txpkt =
    {
        (PGN060416_CTS_PRI << 26) | ((PGN060416_NUM + g_multipacket.SA) << 8) | (NMEA2K_THIS_ADDR << 0),
        {PGN060416_CTS_CTRL, 0, 0, 0xFF, 0xFF, 0, 0, 0},
        sizeof(uint64_t)
    };

    // TX CTS frame.
    txpkt.dat[1] = g_multipacket.TotalFrmNum;
    txpkt.dat[2] = g_multipacket.CurrFrmNum;        // next frame number wish to be sent.
    memcpy(&txpkt.dat[5], &g_multipacket.pgn, 3);

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));
    return 0;
}

int32_t Pgn060416EOMPostProc(void)
{
    TxProtocol txpkt =
    {
        (PGN060416_EOM_PRI << 26) | ((PGN060416_NUM + g_multipacket.SA) << 8) | (NMEA2K_THIS_ADDR << 0),
        {PGN060416_EOM_CTRL, 0, 0, 0xFF, 0xFF, 0, 0, 0},
        sizeof(uint64_t)
    };

    // TX CTS frame.
    txpkt.dat[1] = g_multipacket.TotalByteLen;
    txpkt.dat[2] = g_multipacket.CurrFrmNum;
    memcpy(&txpkt.dat[5], &g_multipacket.pgn, 3);

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn059392ISOAck(RxProtocol rxpkt)
{
    TxProtocol txpkt =
    {
        (PGN059392_PRI << 26) | ((PGN059392_NUM + getRxSA(rxpkt.canid)) << 8) | (NMEA2K_THIS_ADDR << 0),
        {},
        sizeof(uint64_t)
    };
    uint32_t txpgn = 0;

    memcpy(&txpgn, rxpkt.dat, rxpkt.len);

    *((uint64_t*)txpkt.dat) = \
        ((uint64_t)(txpgn)       << 40) | \
        (0xFFFFFFUL              << 16) | \
        (0xFF                    <<  8) | \
        (PGN059392_NEGATIVIE_ACK <<  0);

    txpkt.len = sizeof(uint64_t);

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn060160MultiPktDataRx(RxProtocol rxpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    int32_t ret;
    if(0 != (ret = opChkMultiPktRunning(&rxpkt))){printf("ret[%ld]\n", ret); return -1;}

    if(g_multipacket.status != MULTIPACKET_BUF_MERGING) return -2;

    memcpy(&g_multipacket.dat[(g_multipacket.CurrFrmNum - 1)*MULTIPACKET_BYTE_PER_FRAME], &rxpkt.dat[1], MULTIPACKET_BYTE_PER_FRAME);

    if(g_multipacket.CurrFrmNum == g_multipacket.TotalFrmNum) // All multipacket received. terminate and exit.
    {
        Pgn060416EOMPostProc();
        g_multipacket.status = MULTIPACKET_BUF_AVAILABLE;
        return 1;
    }

    g_multipacket.CurrFrmNum++;

    return 0;
}

int32_t Pgn060416RTS(RxProtocol rxpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    if(g_multipacket.status != MULTIPACKET_BUF_EMPTY) return -1;

    g_multipacket.status        = MULTIPACKET_BUF_MERGING;
    g_multipacket.TotalByteLen  = (rxpkt.dat[2]<< 8) + rxpkt.dat[1];
    g_multipacket.TotalFrmNum   = rxpkt.dat[3];
    g_multipacket.CurrFrmNum    = 1;
    g_multipacket.pgn           = (rxpkt.dat[7]<<16) + (rxpkt.dat[6]<< 8) + rxpkt.dat[5];
    g_multipacket.SA            = getRxSA(rxpkt.canid);
    g_multipacket.DA            = getRxDA(rxpkt.canid);
    g_multipacket.Time          = osKernelGetSysTimerCount();

#if 0
    printf("multipkt[%d:%d:%d:%ld:%d:%d:%lu]\n",
            g_multipacket.TotalByteLen,
            g_multipacket.TotalFrmNum,
            g_multipacket.CurrFrmNum,
            g_multipacket.PGN,
            g_multipacket.SA,
            g_multipacket.DA,
            g_multipacket.Time);
#endif

    Pgn060416CTSPostProc();

    return 0;
}

int32_t Pgn060416CTS(RxProtocol rxpkt)
{
    return 0;
}

int32_t Pgn060416EOM(RxProtocol rxpkt)
{
    //reserved.
    return 0;
}

int32_t Pgn060416ABT(RxProtocol rxpkt)
{
    //reserved.
    return 0;
}

int32_t Pgn060416BAM(RxProtocol rxpkt)
{
    //reserved.
    return 0;
}

int32_t Pgn060416MultiPktCtrl(RxProtocol rxpkt)
{
    uint8_t ctrl_code = rxpkt.dat[0];
    switch(ctrl_code)
    {
    case PGN060416_RTS_CTRL:
        Pgn060416RTS(rxpkt);
        break;
    case PGN060416_CTS_CTRL:
        Pgn060416CTS(rxpkt);
        break;
    case PGN060416_EOM_CTRL:
        Pgn060416EOM(rxpkt);
        break;
    case PGN060416_ABT_CTRL:
        Pgn060416ABT(rxpkt);
        break;
    case PGN060416_BAM_CTRL:
        Pgn060416BAM(rxpkt);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

int32_t Pgn060928ISOAddrClame(RxProtocol rxpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        (PGN060928_PRI << 26) | ((PGN060928_NUM + getRxSA(rxpkt.canid)) << 8) | (NMEA2K_THIS_ADDR << 0),
        {},
        sizeof(uint64_t)
    };

    *((uint64_t*)txpkt.dat) = g_pgn060928_dat_raw;

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn065240ISOCmdAddr(void)
{
//    printf("%s() Enter\n",__FUNCTION__);
//    dumpMultiPacket(__FUNCTION__, &g_multipacket);

    if(!memcmp(&g_pgn060928_dat_raw, &g_multipacket.dat[0], sizeof(uint64_t)))
    {
        printf("ISOAddrClameDat matched. new address is %d\n", g_multipacket.dat[8]);
    }
    else printf("ISOAddrClameDat diff\n");

    g_multipacket.status = MULTIPACKET_BUF_EMPTY;

    return 0;
}

int32_t Pgn065285BootStatAck(void)//	Boot State Acknowledgment
{
    printf("%s() Called\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        (PGN065285_PRI << 26) | ((PGN065285_NUM) << 8) | (NMEA2K_THIS_ADDR << 0),
        {
            PPGN_MFGCODE & 0xFF,
            (PPGN_MFGCODE>>8) & 0xFF,
            PGN065285_BOOTSTAT_RUN_BOOTLOADER | 0xF8,
            0xff,
            0xff,
            0xff,
            0xff,
            0xff
        },
        sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn065288Brightness(void)//    Brightness
{
    printf("%s() Called\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        (PGN065288_PRI << 26) | ((PGN065288_NUM) << 8) | (NMEA2K_THIS_ADDR << 0),
        {
            PPGN_MFGCODE & 0xFF,
            (PPGN_MFGCODE>>8) & 0xFF,
            0,          // Instance - fixed to 0
            0x40,       // Brightness {preset[0:Daytime 1:Dusk 4:Manual] | source[0:LCD 1:Keypad]}
            50,         // Brightness percent - 0~99
            0xff,
            0xff,
            0xff
        },
        sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn126720BootldrVer(fastpacket* pfastpkt)
{
    printf("%s() Enter\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        (PGN126720_PRI << 26) | ((PGN126720_NUM + pfastpkt->src_addr) << 8) | (NMEA2K_THIS_ADDR << 0),
        { },
        sizeof(uint64_t)
    };
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126720_05_LEN);
    uint8_t *pfastpkt_dat = pvPortMalloc(fastdat_len_trunc);
    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

    *(pfastpkt_dat+0)               = PGN126720_05_LEN;
    *(uint16_t*)(pfastpkt_dat+1)    = PPGN_MFGCODE;
    *(pfastpkt_dat+3)               = PGN126720_PID_BOOTLDR_VER;
    *(uint16_t*)(pfastpkt_dat+4)    = PGN126720_05_PRODUCTCODE;
    *(pfastpkt_dat+6)               = PGN126720_05_PROCESSORCODE_MASTER;
    *(uint16_t*)(pfastpkt_dat+7)    = PGN126720_05_BOOTVER;
    *(uint16_t*)(pfastpkt_dat+9)    = PGN126720_05_APPVER;

    opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
    return 0;
}

int32_t Pgn126720Proc(fastpacket* pfastpkt)
{
//    printf("%s() Enter\n",__FUNCTION__);
    int32_t ret = 0;
    TxProtocol txpkt =
    {
        (PGN126720_PRI << 26) | ((PGN126720_NUM + pfastpkt->src_addr) << 8) | (NMEA2K_THIS_ADDR << 0),
        { },
        sizeof(uint64_t)
    };
    uint8_t pid = pfastpkt->dat[2]; // properity id(command code)
    uint8_t sid = pfastpkt->dat[3]; // sequence id

    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126720_06_LEN);
    uint8_t *pfastpkt_dat = pvPortMalloc(fastdat_len_trunc);
    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

    *(pfastpkt_dat+0)               = PGN126720_06_LEN;
    *(uint16_t*)(pfastpkt_dat+1)    = PPGN_MFGCODE;
    *(pfastpkt_dat+3)               = PGN126720_PID_BOOTLDR_STAT;
    *(pfastpkt_dat+4)               = sid;
    *(pfastpkt_dat+5)               = PGN126720_06_PROCESSCODE_MASTER;

//    printf("pid[%d]\n",pid);

    switch(pid)
    {
    case PGN126720_PID_BOOTLDR_CMD:
    {
        uint8_t cmd = pfastpkt->dat[4];

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
        ret = 1;
        break;
    default:
        printf("PGN1276720:?? Invaild prop_id\n");
        ret = 2;
        break;
    }

    if(ret == 0) opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
    return ret;
}

int32_t Pgn126993HeartBeat(void)
{
    static uint8_t cnt_heartbeat = 0;
    TxProtocol txpkt =
    {
//        (PGN126993_PRI << 26) | ((PGN126993_NUM + BROADCAST_DEST_ADDR) << 8) | (NMEA2K_THIS_ADDR << 0),
        (PGN126993_PRI << 26) | (PGN126993_NUM << 8) | (NMEA2K_THIS_ADDR << 0),
        {},
        sizeof(uint64_t)
    };

    *((uint64_t*)txpkt.dat) = \
        (0xFFFFFFFFULL           << 32) | \
        (0x3ULL                  << 30) | \
        (PGN126993_EQUIP_STATUS  << 28) | \
        (PGN126993_CAN_STATUS_2  << 26) | \
        (PGN126993_CAN_STATUS_1  << 24) | \
        (cnt_heartbeat           << 16) | \
        (PGN126993_UPDATE_RATE   <<  0);

    if(252 < ++cnt_heartbeat) cnt_heartbeat = 0;

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

int32_t Pgn126996ProdInfo(void)
{
    TxProtocol txpkt =
    {
        (PGN126996_PRI << 26) | (PGN126996_NUM << 8) | (NMEA2K_THIS_ADDR << 0),
        {},
        sizeof(uint64_t)
    };
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126996_LEN);
    uint8_t *pfastpkt_dat = pvPortMalloc(fastdat_len_trunc);
    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

    *(pfastpkt_dat+0)            = PGN126996_LEN;
    *(uint16_t*)(pfastpkt_dat+1) = g_pgn126996_dat.NMEANetMsgDbVer;
    *(uint16_t*)(pfastpkt_dat+3) = g_pgn126996_dat.NMEAMfgProdCode;
    memcpy(pfastpkt_dat+5  ,       g_pgn126996_dat.MfgModelId,       32);
    memcpy(pfastpkt_dat+37 ,       g_pgn126996_dat.MfgSWVerCode,     32);
    memcpy(pfastpkt_dat+69 ,       g_pgn126996_dat.MfgModelVer,      32);
    memcpy(pfastpkt_dat+101,       g_pgn126996_dat.MfgModelSerCode,  32);
    *(pfastpkt_dat+133)          = g_pgn126996_dat.NMEA2KCertLvl;
    *(pfastpkt_dat+134)          = g_pgn126996_dat.LoadEq;

    opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

    vPortFree(pfastpkt_dat);
    return 0;
}

int32_t Pgn126208GrpFuncAck(fastpacket *pfastpkt, pgn126208_ack_dat *pack_dat)
{
//    printf("%s() Enter pgn[%lu]\n",__FUNCTION__, pfastpkt->pgn);
    if(BROADCAST_DEST_ADDR == pfastpkt->des_addr) return 1;

    TxProtocol txpkt =
    {
        (PGN126208_PRI << 26) | ((PGN126208_NUM + pfastpkt->src_addr) << 8) | (NMEA2K_THIS_ADDR << 0),
        { },
        sizeof(uint64_t)
    };
    uint8_t fastdat_len_trunc = FASTDAT_LEN_TRUNC(1 + PGN126208_ACK_LEN_BASE + pack_dat->param_len);
    uint8_t *pfastpkt_dat = pvPortMalloc(PGN126208_ACK_LEN_BASE + pack_dat->param_len);
    memset(pfastpkt_dat, 0xFF, fastdat_len_trunc);

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

    opFastpktQueuePut(&txpkt, pfastpkt_dat, fastdat_len_trunc);

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
        Pgn126720BootldrVer(pfastpkt);
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

int32_t Pgn127502SwitchBankControl(void)
{
    printf("%s() Called\n",__FUNCTION__);
    TxProtocol txpkt =
    {
        (PGN127502_PRI << 26) | ((PGN127502_NUM) << 8) | (NMEA2K_THIS_ADDR << 0),
        {
            0,
            0x00,
            0xfc,
            0xff,
            0xff,
            0xff,
            0xff,
            0xff
        },
        sizeof(uint64_t)
    };

    EcoQueuePut(EcoQueueNMEA2KTX1Handle, (uint8_t*)(&txpkt), sizeof(TxProtocol));

    return 0;
}

void NMEA2KInit(void)
{
    g_multipacket.pgn = 0;
}

void NMEA2KProc(RxProtocol rxpacket)
{
    uint32_t rxpgn = getPGN(rxpacket.canid);
    uint8_t  rxdat[8];
    uint8_t  rxlen = rxpacket.len;
    uint32_t txpgn;

    memcpy(rxdat, rxpacket.dat, rxlen);
    printf("pgn rx:%ld\n", rxpgn);

    //Single and Fast/Multi packet build-up.
    switch(rxpgn)
    {
    case PGN059904_NUM: // ISO Request
        memcpy(&txpgn, rxdat, rxlen);
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
            Pgn127502SwitchBankControl();
            break;
        default:
            if(BROADCAST_DEST_ADDR != getRxPS(rxpacket.canid)) Pgn059392ISOAck(rxpacket);
            break;
        }
        break;
    case PGN060160_NUM: // Multipacket data
        Pgn060160MultiPktDataRx(rxpacket);
        break;
    case PGN060416_NUM: // Multipacket control
        Pgn060416MultiPktCtrl(rxpacket);
        break;
    case PGN065286_NUM: // Boot State Request
        Pgn065285BootStatAck();
        break;
    case PGN065288_NUM: // Boot State Request
        opLCDBrightness(&rxdat[0]);
        break;
        //Fastpacket build-up.
    case PGN126208_NUM: // NMEA2K Group Function
    case PGN126720_NUM: // Various Function
        opFastpacketBuildup(&rxpacket);
        break;
    case PGN127502_NUM: // Switch bank control
        opSwitchBankControl(&rxdat[0]);
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

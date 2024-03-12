/*
 * nmea2k-ctl.c
 *
 *  Created on: Mar 12, 2024
 *      Author: ihbaek
 */

#include "eco.h"

bool isKnownPGN(uint32_t PGN)
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

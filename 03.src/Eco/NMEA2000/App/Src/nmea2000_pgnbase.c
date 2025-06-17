/*
 * nmea2000_pgnbase.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_pgnbase.h"

const uint32_t canEFFMask = 0x80000000;

NmeaPgn _rawToNmeaPgn;

uint32_t isPDU1(int PF)
{
    return (0 <= PF && PF <= 239) ? 1 : 0;
}

NmeaPgn* getRcvNmeaPgn()
{
    return &_rawToNmeaPgn;
}

NmeaPgn* CanIDToNmeaPGN(uint32_t rawCanId)
{
    _rawToNmeaPgn.mCanNumericID = rawCanId;
    _rawToNmeaPgn.mPriority     = (rawCanId & 0x1C000000) >> 26;
    _rawToNmeaPgn.mEDP          = (rawCanId & 0x02000000) >> 25;
    _rawToNmeaPgn.mDP           = (rawCanId & 0x01000000) >> 24;
    _rawToNmeaPgn.mPF           = (rawCanId & 0x00FF0000) >> 16;
    _rawToNmeaPgn.mPS           = (rawCanId & 0x0000FF00) >> 8;
    _rawToNmeaPgn.mSA           = (rawCanId & 0x000000FF);

    _rawToNmeaPgn.mPGN          = (rawCanId & 0x01FFFF00) >> 8;

    if(isPDU1(_rawToNmeaPgn.mPF)) _rawToNmeaPgn.mPGN &= 0x1FF00;

    return &_rawToNmeaPgn;
}

NmeaPgn _nmeaPGN;

NmeaPgn* getCanId(uint8_t priority, uint32_t pgn, uint8_t destAddr, uint8_t sourceAddr)
{
    _nmeaPGN.mPriority      = priority;
    _nmeaPGN.mPGN           = pgn;
    _nmeaPGN.mSA            = sourceAddr;

    _nmeaPGN.mPF            = (_nmeaPGN.mPGN & 0x0000FF00) >> 8;

    _nmeaPGN.mCanNumericID  = (_nmeaPGN.mPriority << 26) | ((_nmeaPGN.mPGN + (isPDU1(_nmeaPGN.mPF) ? destAddr : 0)) << 8) | sourceAddr | canEFFMask;

    _nmeaPGN.mEDP           = (_nmeaPGN.mCanNumericID & 0x02000000) >> 25;
    _nmeaPGN.mDP            = (_nmeaPGN.mCanNumericID & 0x01000000) >> 24;
    _nmeaPGN.mPS            = (_nmeaPGN.mCanNumericID & 0x0000FF00) >> 8;

    return &_nmeaPGN;
}


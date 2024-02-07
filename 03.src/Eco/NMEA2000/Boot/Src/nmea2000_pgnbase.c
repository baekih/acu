/*
 * nmea2000_pgnbase.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000_pgnbase.h"

/* Private typedef -----------------------------------------------------------*/
mRCANID gR_CANID;
mSCANID gS_CANID;

/* Private variables ---------------------------------------------------------*/
uint32_t mR_PGN = 0;
uint32_t mS_PGN = 0;
uint32_t mR_CANID = 0;
uint32_t mS_CANID = 0;

/* Private functions ---------------------------------------------------------*/
uint32_t is_R_PDU1()
{
	return (gR_CANID.mR_PF >= 0 && gR_CANID.mR_PF <= 239) ? true : false;
}

uint32_t CanID_NmeaPGN(uint32_t canid)
{
	uint32_t ReceivePGN = 0;
	
// ExtId(29b)	= Priority(3b) + PGN(18b) + Source Address(8b)
	mR_CANID				= canid;

// PGN(18b) 	= EDP(1b) + DP(1b) + PF(8b) + PS(8b)
// PDU1 		= PF(0~239),	PS(Destination Address)
// PDU2 		= PF(240~255),	PS(Group Extension)
	gR_CANID.mR_Priority	= (canid & 0x1C000000) >> 26;
	gR_CANID.mR_EDP			= (canid & 0x02000000) >> 25;
	gR_CANID.mR_DP			= (canid & 0x01000000) >> 24;
	gR_CANID.mR_PF			= (canid & 0x00FF0000) >> 16;
	gR_CANID.mR_PS			= (canid & 0x0000FF00) >> 8;
	gR_CANID.mR_SA			= (canid & 0x000000FF);

	mR_PGN					= (canid & 0x01FFFF00) >> 8;
	if(is_R_PDU1())
		mR_PGN &= 0x1FF00;
	ReceivePGN = mR_PGN;

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[ReceiveCanid = 0x%lx] !!\r\n", __FUNCTION__, __LINE__, canid);
#endif

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[*ReceivePGN = %ld] !!\r\n", __FUNCTION__, __LINE__, ReceivePGN);
#endif

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[mR_Priority = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_Priority);
	printf("===>> Func:%s, Line:%d \t\t\t[mR_EDP = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_EDP);
	printf("===>> Func:%s, Line:%d \t\t\t[mR_DP = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_DP);
	printf("===>> Func:%s, Line:%d \t\t\t[mR_PF = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_PF);
	printf("===>> Func:%s, Line:%d \t\t\t[mR_PS = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_PS);
	printf("===>> Func:%s, Line:%d \t\t\t[mR_SA = %ld] !!\r\n", __FUNCTION__, __LINE__, gR_CANID.mR_SA);
#endif

	return ReceivePGN;
}

uint32_t get_R_CanId()
{
	return mR_CANID;
}

uint32_t get_R_PGN()
{
	return mR_PGN;
}

uint32_t get_R_Priority()
{
	return gR_CANID.mR_Priority;
}

uint32_t get_R_EDP()
{
	return gR_CANID.mR_EDP;
}

uint32_t get_R_DP()
{
	return gR_CANID.mR_DP;
}

uint32_t get_R_PF()
{
	return gR_CANID.mR_PF;
}

uint32_t get_R_PS()
{
	return gR_CANID.mR_PS;
}

uint32_t get_R_SA()
{
	return gR_CANID.mR_SA;
}

/* ************************************************************************** */

uint32_t is_S_PDU1()
{
	return (gS_CANID.mS_PF >= 0 && gS_CANID.mS_PF <= 239) ? true : false;
}

uint32_t NmeaPGN_CanID(uint32_t priority, uint32_t pgn, uint32_t destAddr, uint32_t sourceAddr)
{
	uint32_t SendCanid = 0;
	
	mS_PGN					= pgn;

	gS_CANID.mS_Priority	= priority;
	gS_CANID.mS_EDP			= (mS_PGN & 0x00020000) >> 17;
	gS_CANID.mS_DP			= (mS_PGN & 0x00010000) >> 16;
	gS_CANID.mS_PF			= (mS_PGN & 0x0000FF00) >> 8;
	gS_CANID.mS_PS			= ((mS_PGN + (is_S_PDU1() ? destAddr : 0)) & 0x000000FF);
	gS_CANID.mS_SA			= sourceAddr;

	SendCanid				= (gS_CANID.mS_Priority << 26) | ((mS_PGN + (is_S_PDU1() ? destAddr : 0)) << 8) | sourceAddr;
	mS_CANID				= SendCanid;

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[*SendPGN = %ld] !!\r\n", __FUNCTION__, __LINE__, mS_PGN);
#endif

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[mS_Priority = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_Priority);
	printf("===>> Func:%s, Line:%d \t\t\t[mS_EDP = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_EDP);
	printf("===>> Func:%s, Line:%d \t\t\t[mS_DP = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_DP);
	printf("===>> Func:%s, Line:%d \t\t\t[mS_PF = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_PF);
	printf("===>> Func:%s, Line:%d \t\t\t[mS_PS = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_PS);
	printf("===>> Func:%s, Line:%d \t\t\t[mS_SA = %ld] !!\r\n", __FUNCTION__, __LINE__, gS_CANID.mS_SA);
	printf("===>> Func:%s, Line:%d \t\t\t[SendCanid = 0x%lx] !!\r\n", __FUNCTION__, __LINE__, SendCanid);
#endif

#if PRINTF_DEBUG_NMEA2000_PGNBASE_NON
	printf("===>> Func:%s, Line:%d \t\t\t[SendCanid = 0x%lx] !!\r\n", __FUNCTION__, __LINE__, SendCanid);
#endif
	return SendCanid;
}

uint32_t get_S_CanId()
{
	return mS_CANID;
}

uint32_t get_S_PGN()
{
	return mS_PGN;
}

uint32_t get_S_Priority()
{
	return gS_CANID.mS_Priority;
}

uint32_t get_S_EDP()
{
	return gS_CANID.mS_EDP;
}

uint32_t get_S_DP()
{
	return gS_CANID.mS_DP;
}

uint32_t get_S_PF()
{
	return gS_CANID.mS_PF;
}

uint32_t get_S_PS()
{
	return gS_CANID.mS_PS;
}

uint32_t get_S_SA()
{
	return gS_CANID.mS_SA;
}


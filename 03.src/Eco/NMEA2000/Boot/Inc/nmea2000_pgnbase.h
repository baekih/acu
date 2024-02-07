/*
 * nmea2000_pgnbase.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_PGNBASE_H_
#define NMEA2000_INC_NMEA2000_PGNBASE_H_


/* Includes ------------------------------------------------------------------*/
#include "eco.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct __RECEVIECANID
{
	uint32_t mR_Priority;
	uint32_t mR_EDP;
	uint32_t mR_DP;
	uint32_t mR_PF;
	uint32_t mR_PS;
	uint32_t mR_SA;
} mRCANID;

typedef struct __SENDCANID
{
	uint32_t mS_Priority;
	uint32_t mS_EDP;
	uint32_t mS_DP;
	uint32_t mS_PF;
	uint32_t mS_PS;
	uint32_t mS_SA;
} mSCANID;

/* Exported variables ---------------------------------------------------------*/
extern uint32_t mR_PGN;
extern uint32_t mS_PGN;
extern uint32_t mR_CANID;
extern uint32_t mS_CANID;

extern mRCANID gR_CANID;
extern mSCANID gS_CANID;

/* Exported functions --------------------------------------------------------*/
extern uint32_t CanID_NmeaPGN(uint32_t canid);
extern uint32_t get_R_CanId();
extern uint32_t get_R_PGN();
extern uint32_t get_R_Priority();
extern uint32_t get_R_EDP();
extern uint32_t get_R_DP();
extern uint32_t get_R_PF();
extern uint32_t get_R_PS();
extern uint32_t get_R_SA();

extern uint32_t NmeaPGN_CanID(uint32_t priority, uint32_t pgn, uint32_t destAddr, uint32_t sourceAddr);
extern uint32_t get_S_CanId();
extern uint32_t get_S_PGN();
extern uint32_t get_S_Priority();
extern uint32_t get_S_EDP();
extern uint32_t get_S_DP();
extern uint32_t get_S_PF();
extern uint32_t get_S_PS();
extern uint32_t get_S_SA();


#endif /* NMEA2000_INC_NMEA2000_PGNBASE_H_ */

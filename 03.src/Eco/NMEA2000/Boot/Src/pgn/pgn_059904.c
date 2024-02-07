/*
 * pgn_059904.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "pgn_059392.h"
#include "pgn_059904.h"
#include "pgn_060928.h"
#include "pgn_126996.h"

/* define ------------------------------------------------------------*/
#define PGN059904_NUM         59904
#define PGN059904_PRIORITY    6

/* typedef -----------------------------------------------------------*/
PGN059904NAME g_PGN059904NAME;

/* functions ---------------------------------------------------------*/
void PGN059904_GetFieldValue(uint32_t pgnid, uint8_t len, uint8_t *buf)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	InitializeReceNameBitPosition();
	InitializeReceNameField();

	receivePacketLength = len;
	memcpy(&receiveNMEAPackets, buf, receivePacketLength);

	g_PGN059904NAME.mPGN_being_requested = Get3ByteUInt(0);

//	printf("mPGN_being_requested[%ld] !!\r\n", g_PGN059904NAME.mPGN_being_requested);
}

uint32_t PGN059904_ProcessNameField(uint32_t pgnid, uint8_t len, uint8_t *buf)
{
//  printf("%s:%d Enter...PGNBeingRequested[%ld]\r\n",__FUNCTION__,__LINE__, g_PGN059904NAME.mPGN_being_requested);

  switch(g_PGN059904NAME.mPGN_being_requested)
  {
  case 60928:
	  PGN060928_SetInitialField(&mMyPGN060928);
	  PGN060928_ProcessNameField();
    break;
  case 126996:
//    printf("%s:%d Pass... \r\n",__FUNCTION__,__LINE__);
    PGN126996_ProcessNameField(pgnid, len, buf, REQUEST_MESSAGE_TYPE_FASTPACKET);
    break;
  case 126464:
  case 126998:
  case 130830:
    break;
  default :
    if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
    {
      PGN059392_SetFieldValue(NEGATIVIE_ACKNOWLEDGMENT, 0xFF, 0xFFFFFF, g_PGN059904NAME.mPGN_being_requested);
      PGN059392_SendNameField();
    }
    break;
  }

	return 0;
}


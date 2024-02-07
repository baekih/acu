/*
 * pgn_126208.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"
#include "fastpacketdata.h"

#include "pgn_126208.h"
#include "pgn_126208_request.h"
#include "pgn_126208_command.h"

/* define ------------------------------------------------------------*/
#define PGN126208_NUM         126208
#define PGN126208_PRIORITY    3

/* typedef -----------------------------------------------------------*/
PGN126208NAME g_PGN126208NAME;

/* Private functions ---------------------------------------------------------*/
void PGN126208_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

  g_PGN126208NAME.mGroup_Function_Code = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 0, buf);
}

void PGN126208_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

  switch(g_PGN126208NAME.mGroup_Function_Code)
  {
  case FUNCTION_CODE_REQEUST_MESSAGE:
    PGN126208REQUEST_GetFieldValue(pgnid, buf, messagetype);
    PGN126208REQUEST_ProcessNameField(pgnid, buf, messagetype);
    break;
  case FUNCTION_CODE_COMMAND_MESSAGE:
    PGN126208COMMAND_GetFieldValue(pgnid, buf, messagetype);
    PGN126208COMMAND_ProcessNameField(pgnid, buf, messagetype);
    break;
  }
}


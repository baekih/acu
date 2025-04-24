/*
 * pgn_126208.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "multipacketdata.h"
#include "fastpacketdata.h"

#include "pgn_126208.h"
#include "pgn_126208_request.h"
#include "pgn_126208_command.h"

/* Private typedef -----------------------------------------------------------*/
PGN126208NAME g_PGN126208NAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN126208_GetFieldValue(NmeaPgn* pgnId, uint16_t size, uint8_t *buf, uint32_t messagetype)
{
    uint8_t Index = 0;

    g_PGN126208NAME.mGrpFuncCode = GetBuf_1ByteUInt(size, Index, buf);
}

void PGN126208_ProcessNameField(NmeaPgn* pgnId, uint16_t size, uint8_t *pbuf, uint32_t messagetype)
{
//    printf("%s:%d Enter... \r\n",__FUNCTION__,__LINE__);
    switch(g_PGN126208NAME.mGrpFuncCode)
    {
    case FUNCTION_CODE_REQEUST_MESSAGE:
        PGN126208REQUEST_GetFieldValue(pgnId, pbuf, size, messagetype);
        PGN126208REQUEST_ProcessNameField(pgnId, pbuf, size, messagetype);
        break;
    case FUNCTION_CODE_COMMAND_MESSAGE:
        PGN126208COMMAND_GetFieldValue(pgnId, pbuf, size, messagetype);
        PGN126208COMMAND_ProcessNameField(pgnId, pbuf, size, messagetype);
        break;
    }
}

/*
 * pgn_060416.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

#include "pgn_060416.h"
#include "pgn_060416_rts.h"
#include "pgn_060416_cts.h"
#include "pgn_060416_bam.h"

/* Private typedef -----------------------------------------------------------*/
PGN060416NAME g_PGN060416NAME;

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN060416_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    g_PGN060416NAME.mGroup_Function_Code = GetBuf_1ByteUInt(len, 0, buf);
}

uint32_t PGN060416_ProcessNameField(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
    if(g_PGN060416NAME.mGroup_Function_Code == PGN060416_FUNC_CODE_RTS) {
        PGN060416RTS_GetFieldValue(pgnId, len, buf);
        PGN060416RTS_ProcessNameField(pgnId, len, buf);
    }
    else if(g_PGN060416NAME.mGroup_Function_Code == PGN060416_FUNC_CODE_CTS) {
        PGN060416CTS_GetFieldValue(pgnId, len, buf);
        PGN060416CTS_ProcessNameField(pgnId, len, buf);
    }
    else if(g_PGN060416NAME.mGroup_Function_Code == PGN060416_FUNC_CODE_EOM) {

    }
    else if(g_PGN060416NAME.mGroup_Function_Code == PGN060416_FUNC_CODE_ABORT) {

    }
    else if(g_PGN060416NAME.mGroup_Function_Code == PGN060416_FUNC_CODE_BAM) {
        PGN060416BAM_GetFieldValue(pgnId, len, buf);
        PGN060416BAM_ProcessNameField(pgnId, len, buf);
    }

    return 0;
}

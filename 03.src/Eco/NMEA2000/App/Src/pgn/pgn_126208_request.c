/*
 * pgn_126208_request.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"
#include "multipacketdata.h"
#include "fastpacketdata.h"

#include "pgn_060928.h"
#include "pgn_061184_stgf.h"
#include "pgn_065285.h"
#include "pgn_065287.h"
#include "pgn_126208.h"
#include "pgn_126208_request.h"
#include "pgn_126208_acknowledge.h"
#include "pgn_126464.h"
#include "pgn_126720.h"
#include "pgn_126993.h"
#include "pgn_126996.h"
#include "pgn_126998.h"
#include "pgn_130816.h"
#include "pgn_130822.h"
#include "pgn_130823.h"

/* Private typedef -----------------------------------------------------------*/
PGN126208REQUESTNAME g_PGN126208REQUESTNAME;

/* Private variables ---------------------------------------------------------*/
static uint32_t PGN126208REQ_paramPos = 0;

/* Private functions ---------------------------------------------------------*/
void PGN126208REQUEST_GetFieldValue(NmeaPgn* pgnId, uint8_t *buf, uint16_t size, uint32_t messagetype)
{
    uint8_t Index = 0;

    g_PGN126208REQUESTNAME.mRequest_Group_Function_Code = GetBuf_1ByteUInt(size, Index, buf);
    Index = Index+1;
    g_PGN126208REQUESTNAME.mRequested_PGN = GetBuf_3ByteUInt(size, Index, buf);
    Index = Index+3;
    g_PGN126208REQUESTNAME.mTransmission_interval = GetBuf_4ByteUInt(size, Index, buf);
    Index = Index+4;
    g_PGN126208REQUESTNAME.mTransmission_interval_offset = GetBuf_2ByteUInt(size, Index, buf);
    Index = Index+2;
    g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow = GetBuf_1ByteUInt(size, Index, buf);
    PGN126208REQ_paramPos = Index+1;

#if 0
    printf(" [mRequest_Group_Function_Code = %ld] !!\r\n", g_PGN126208REQUESTNAME.mRequest_Group_Function_Code);
    printf(" [mRequested_PGN = %ld] !!\r\n", g_PGN126208REQUESTNAME.mRequested_PGN);
    printf(" [mTransmission_interval = 0x%lx] !!\r\n", g_PGN126208REQUESTNAME.mTransmission_interval);
    printf(" [mTransmission_interval_offset = 0x%lx] !!\r\n", g_PGN126208REQUESTNAME.mTransmission_interval_offset);
    printf(" [mNumber_of_Pairs_of_Request_Parameters_to_follow = %ld] !!\r\n", g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow);
#endif

}

void PGN126208REQUEST_ProcessNameField(NmeaPgn* pgnId, uint8_t *buf, uint16_t size, uint32_t messagetype)
{
    uint32_t txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK;

    uint8_t  fieldErrorCodes[PGN126208_FIELD_ERROR_CODE_MAX] = {0,};
    uint32_t countOfFieldErrorCode = 0;

    uint32_t numOfParam = g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow;
    uint32_t numOfError = 0;

    uint32_t paramPos = 0;

    if(g_PGN126208REQUESTNAME.mTransmission_interval == 0xFFFFFFFF){
        if(g_PGN126208REQUESTNAME.mTransmission_interval_offset != 0xFFFF && g_PGN126208REQUESTNAME.mTransmission_interval_offset != 0)
        {
            txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
        }
    }
    else if(g_PGN126208REQUESTNAME.mTransmission_interval == 0xFFFFFFFE){
        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR){
            txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
        }
    }
    else{
        txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
    }

    if (g_PGN126208REQUESTNAME.mRequested_PGN == 60928) {
        if (numOfParam > 0) {
            bool     isParamValid = false;

            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

                if (paramPos == 2) {
                    uint32_t ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x7FF;
                    isParamValid = (ManufacturerCode == N2K_MFG_CODE_FURUNO);

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 2;
                }
                else if(paramPos == 3) {
                    uint32_t ISOECUInstance = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x07;
                    isParamValid = (ISOECUInstance == (g_pgn060928_curr.mDevice_Instance & 0x07));

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else if(paramPos == 4) {
                    uint32_t ISOFunctionInstance = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x1F;
                    isParamValid = (ISOFunctionInstance == ((g_pgn060928_curr.mDevice_Instance & 0xF8) >> 3));

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else if(paramPos == 5) {
                    uint32_t ISOFunction = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                    isParamValid = (ISOFunction == g_pgn060928_curr.mDevice_Function);

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else if(paramPos == 7) {
                    uint32_t ISODeviceClassInstance = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x7F;
                    isParamValid = (ISODeviceClassInstance == g_pgn060928_curr.mDevice_Class);

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else if(paramPos == 8) {
                    uint32_t SystemIntance = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x0F;
                    isParamValid = (SystemIntance == g_pgn060928_curr.mSystem_Instance);

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else if(paramPos == 9) {
                    uint32_t IndustryGroup = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x07;
                    isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                }
                else{
                    isParamValid = false;
                }

                if (isParamValid == false) {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
                    numOfError++;
                } else {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
            HAL_Delay(2);	// for certification, but no reason.

            PGN060928_SetInitialField();
            PGN060928_ProcessNameField();

            return;
        }

        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR) {
            PGN126208ACKNOWLEDGE_SetFieldValue(
                    FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                    g_PGN126208REQUESTNAME.mRequested_PGN,
                    txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                    txPriorityErrorCode,
                    countOfFieldErrorCode,
                    fieldErrorCodes
                    );
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 126464){
        if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfParam > 0) {
            paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
            PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

            if (paramPos == 1) {
                uint32_t mReceivePGNGroupFunctionCode = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

                if (mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE || mReceivePGNGroupFunctionCode == PGN_GROUP_TRANSMIT_LIST_MESSAGE) {
                    PGN126464_SetFieldValue(mReceivePGNGroupFunctionCode,
                            mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE ?
                                    mReceive_PGN_List : mTransmit_PGN_List,
                            mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE ?
                                    mReceive_PGN_List_Size : mTransmit_PGN_List_Size);
                    PGN126464_ProcessNameField(pgnId, messagetype);

                    return;
                }
                else {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
                    numOfError = 1;
                }
            }
        }

        if(txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0){
            PGN126464_SetFieldValue(PGN_GROUP_TRANSMIT_LIST_MESSAGE, mTransmit_PGN_List, mTransmit_PGN_List_Size);
            PGN126464_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);

            PGN126464_SetFieldValue(PGN_GROUP_RECEIVE_LIST_MESSAGE, mReceive_PGN_List, mReceive_PGN_List_Size);
            PGN126464_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);

            return;
        }

        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR) {
            PGN126208ACKNOWLEDGE_SetFieldValue(
                    FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                    g_PGN126208REQUESTNAME.mRequested_PGN,
                    txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                    txPriorityErrorCode,
                    countOfFieldErrorCode,
                    fieldErrorCodes
                    );
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 126996) {
        if (numOfParam > 0) {
            bool     isParamValid = false;
            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

                if (paramPos == 2) {
                    uint32_t ProductCode = GetBuf_2ByteUInt(size, PGN126208REQ_paramPos, buf);
                    PGN126208REQ_paramPos = PGN126208REQ_paramPos + 2;
                    //6
                    isParamValid = (ProductCode == mNMEAManufacturersProductCode);
                }
                else{
                    isParamValid = false;
                }

                if (isParamValid == false) {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
                    numOfError++;

                } else {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
            HAL_Delay(1);
            PGN126996_ProcessNameField(pgnId, size, buf, messagetype);

            return;
        }

        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR) {
            PGN126208ACKNOWLEDGE_SetFieldValue(
                    FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                    g_PGN126208REQUESTNAME.mRequested_PGN,
                    txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                    txPriorityErrorCode,
                    countOfFieldErrorCode,
                    fieldErrorCodes
                    );
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 126720) {
        uint32_t ManufacturerCode = 0;
        uint32_t IdentifyCode = 0;

        if(numOfParam > 0)
        {
            bool     isParamValid = false;

            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

//              printf("%s:%d numOfParam[%ld] paramPos[%ld]\r\n",__FUNCTION__,__LINE__, numOfParam, paramPos);

                switch(paramPos)
                {
                    case 1:{
                        ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_FURUNO);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 2;
                    }
                        break;
                    case 3:{ // Industry Group. 3bit fixed to 4. skip
                        uint32_t IndustryGroup = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x07;
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                    }
                        break;
                    case 4: {
                        IdentifyCode = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

                        if(IdentifyCode == 11 || IdentifyCode == 1 || IdentifyCode == 2){
                            isParamValid = true;
                        }
                        else {
                            isParamValid = false;
                        }

                        break;
                    }
                    default:
                        isParamValid = false;
                        break;
                }

                if(isParamValid == false)
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if(numOfParam == 0){
            if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
                PGN126720_FactoryTest_SetFieldValue();
                PGN126720_ProcessNameField(pgnId);
            }
        }
        else {
            if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
                if(IdentifyCode == 11){
                    PGN126720_FactoryTest_SetFieldValue();
                    PGN126720_ProcessNameField(pgnId);
                    return;
                }
                else if(IdentifyCode == 1){
                    PGN126720_MemoryClearGroup_SetFieldValue();
                    PGN126720_ProcessNameField(pgnId);
                    return;
                }
                else if(IdentifyCode == 2){
                    PGN126720_ResetGoup_SetFieldValue();
                    PGN126720_ProcessNameField(pgnId);
                    return;
                }
            }
        }

        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR)
        {
            PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                               g_PGN126208REQUESTNAME.mRequested_PGN,
                                               txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                               txPriorityErrorCode,
                                               countOfFieldErrorCode,
                                               fieldErrorCodes);
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }

    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 130822) {
        uint32_t ManufacturerCode = 0;
        uint32_t IndustryGroup = 0;

        if(numOfParam > 0)
        {
            bool     isParamValid = false;

            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

//              printf("%s:%d numOfParam[%ld] paramPos[%ld]\r\n",__FUNCTION__,__LINE__, numOfParam, paramPos);

                switch(paramPos)
                {
                    case 1:
                        ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_FURUNO);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 2;
                        break;
                    case 3:
                        IndustryGroup = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x07;
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                        break;
                    default:
                        isParamValid = false;
                        break;
                }

                if(isParamValid == false)
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
            PGN130822_ProcessNameField(pgnId);
        }
        else {
            PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                               g_PGN126208REQUESTNAME.mRequested_PGN,
                                               txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                               txPriorityErrorCode,
                                               countOfFieldErrorCode,
                                               fieldErrorCodes);
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 130823) {
        uint32_t ManufacturerCode = 0;
        uint32_t IndustryGroup = 0;

        if(numOfParam > 0)
        {
            bool     isParamValid = false;

            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf);
                PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;

//              printf("%s:%d numOfParam[%ld] paramPos[%ld]\r\n",__FUNCTION__,__LINE__, numOfParam, paramPos);

                switch(paramPos)
                {
                    case 1:
                        ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_FURUNO);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 2;
                        break;
                    case 3:
                        IndustryGroup = GetBuf_1ByteUInt(size, PGN126208REQ_paramPos, buf) & 0x07;
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                        PGN126208REQ_paramPos = PGN126208REQ_paramPos + 1;
                        break;
                    default:
                        isParamValid = false;
                        break;
                }

                if(isParamValid == false)
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if (txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0) {
            PGN130823_ProcessNameField(pgnId);
        }
        else {
            PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                               g_PGN126208REQUESTNAME.mRequested_PGN,
                                               txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                               txPriorityErrorCode,
                                               countOfFieldErrorCode,
                                               fieldErrorCodes);
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
    else if(g_PGN126208REQUESTNAME.mRequested_PGN == 61184 ||
        g_PGN126208REQUESTNAME.mRequested_PGN == 65285 ||
        g_PGN126208REQUESTNAME.mRequested_PGN == 65287 ||
        g_PGN126208REQUESTNAME.mRequested_PGN == 126998 ||
        g_PGN126208REQUESTNAME.mRequested_PGN == 130816)
    {
        if (numOfParam > 0) {
            while (numOfParam-- > 0){
                if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                numOfError++;
            }
        }
//      printf("%s:%d Pass...PGN:%ld \r\n",__FUNCTION__,__LINE__, g_PGN126208REQUESTNAME.mRequested_PGN);

        if(txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfError == 0){
            switch(g_PGN126208REQUESTNAME.mRequested_PGN)
            {
                case 61184 : PGN061184STGF_SendNameField(pgnId->mSA);
                    break;
                case 65285 : PGN065285_SendNameField();
                    break;
                case 65287 : PGN065287_FEC_SendNameField();
                    break;
                case 126998 : PGN126998_ProcessNameField(pgnId, messagetype);
                    break;
                case 130816 : PGN130816_ProcessNameField(pgnId);
                    break;
                default :
                    break;
            }
//          printf("%s:%d Pass... \r\n",__FUNCTION__,__LINE__);
            return;
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(
                FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                g_PGN126208REQUESTNAME.mRequested_PGN,
                txPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ?
                        PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                txPriorityErrorCode,
                countOfFieldErrorCode,
                fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
    }
    else {
        if(pgnId->mPS != BROADCAST_DESTINATION_ADDR) {
            PGN126208ACKNOWLEDGE_SetFieldValue(
                    FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                    g_PGN126208REQUESTNAME.mRequested_PGN,
                    PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT,
                    PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_PRIORITY_NOT_SUP,
                    0,
                    fieldErrorCodes);
            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
        }
    }
}

/*
 * pgn_126208_command.c
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
#include "pgn_126208_command.h"
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
PGN126208COMMANDNAME g_PGN126208COMMANDNAME;

/* Private variables ---------------------------------------------------------*/
static uint32_t PGN126208CMD_paramPos = 0;

/* Private functions ---------------------------------------------------------*/
void PGN126208COMMAND_GetFieldValue(NmeaPgn* pgnId, uint8_t *buf, uint16_t size, uint32_t messagetype)
{
    uint8_t Index = 0;

    g_PGN126208COMMANDNAME.mCommand_Group_Function_Code = GetBuf_1ByteUInt(size, Index, buf);
    Index = Index+1;
    g_PGN126208COMMANDNAME.mCommanded_PGN = GetBuf_3ByteUInt(size, Index, buf);
    Index = Index+3;
    g_PGN126208COMMANDNAME.mPriority_Setting = (GetBuf_1ByteUInt(size, Index, buf) & 0xF);
    g_PGN126208COMMANDNAME.mNMEA_Reserved = (GetBuf_1ByteUInt(size, Index, buf) & 0xF) >> 4;
    Index = Index+1;
    g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow = GetBuf_1ByteUInt(size, Index, buf);
    PGN126208CMD_paramPos = Index+1;

#if 0
    printf(" [mCommand_Group_Function_Code = %ld] !!\r\n", g_PGN126208COMMANDNAME.mCommand_Group_Function_Code);
    printf(" [mCommanded_PGN = %ld] !!\r\n", g_PGN126208COMMANDNAME.mCommanded_PGN);
    printf(" [mPriority_Setting = %ld] !!\r\n", g_PGN126208COMMANDNAME.mPriority_Setting);
    printf(" [mNumber_of_Pairs_of_Commanded_Parameters_to_follow = %ld] !!\r\n", g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow);
#endif
}

void PGN126208COMMAND_ProcessNameField(NmeaPgn* pgnId, uint8_t *buf, uint16_t size, uint32_t messagetype)
{
    uint32_t txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK;

    uint8_t  fieldErrorCodes[PGN126208_FIELD_ERROR_CODE_MAX];
    uint32_t countOfFieldErrorCode = 0;

    uint32_t numOfParam = g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow;
    uint32_t numOfError = 0;

    uint32_t paramPos;

    bool     isParamValid = false;

    if (pgnId->mPS == BROADCAST_DESTINATION_ADDR) return;

    if (g_PGN126208COMMANDNAME.mPriority_Setting < 8) {
        txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
    }

    if(g_PGN126208COMMANDNAME.mCommanded_PGN == 60928)
    {
        if (numOfParam> 0){
            while (numOfParam-- > 0){
                isParamValid = true;

                paramPos = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                PGN126208CMD_paramPos += 1;

                if(paramPos == 3)
                {
                    g_pgn060928_curr.mDevice_Instance = (g_pgn060928_curr.mDevice_Instance & 0xF8) | (GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x07);
                    PGN126208CMD_paramPos += 1;
                }
                else if(paramPos == 4)
                {
                    g_pgn060928_curr.mDevice_Instance = (g_pgn060928_curr.mDevice_Instance & 0x07) | ((GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x1F) << 3);
                    PGN126208CMD_paramPos += 1;
                }
                else if(paramPos == 8)
                {
                    g_pgn060928_curr.mSystem_Instance = (GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x0F);
                    PGN126208CMD_paramPos += 1;
                }
                else
                {
                    isParamValid = false;
                }

                if (isParamValid == false)
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);

        if (g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfError == 0)
        {
            HAL_Delay(1);

            PGN060928_SetInitialField();
            PGN060928_SendAddressClaim();

//          g_spayload_curr.addrclame.DevInstance = g_pgn060928_curr.mDevice_Instance;
//          g_spayload_curr.addrclame.SysInstance = g_pgn060928_curr.mSystem_Instance;
        }
    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 61184)
    {
        if (numOfParam> 0){
            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;

                switch(paramPos)
                {
                    case 1:{
                        uint32_t ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_FURUNO);

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 2;
                    }
                        break;
                    case 3:{
                        uint32_t IndustryGroup = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x07;
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 4:{
                        isParamValid = false;

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 5:{
                        isParamValid = false;

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    default:
                        isParamValid = false;
                        break;
                }

                if (isParamValid == false)
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 65287)
    {
        uint8_t access_level = g_access_level;

        if (numOfParam> 0){
            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;

                switch(paramPos)
                {
                    case 1:{
                        uint32_t ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_AIRMAR);

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 2;
                    }
                        break;
                    case 3:{
                        uint32_t IndustryGroup = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 4:{
                        uint32_t format_code = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                        isParamValid = (format_code == 1);

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 5:{
                        access_level = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);

                        isParamValid = true;
                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 7:{
                        isParamValid = true;

                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    default:
                        isParamValid = false;
                        break;
                }

                if (isParamValid == false)
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);

        if (g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfError == 0)
        {
            g_access_level = access_level;
            printf("g_access_level[%d]\r\n", g_access_level);
            PGN065287_Airmar_SendNameField(pgnId);
        }

    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 126464)
    {
        if (numOfParam> 0){
            while (numOfParam-- > 0)
            {
                isParamValid = false;

                if (isParamValid == false)
                {
                  fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                  numOfError++;
                }
                else
                {
                  fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           &fieldErrorCodes[0]);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);

    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 126720)
    {
        uint8_t indentify_code = 0;
        uint8_t control_code = 0xFF;
        uint64_t name_field = 0;

//		printf("numOfParam[%ld]\r\n",numOfParam);

        if (numOfParam> 0){
            while (numOfParam-- > 0)
            {
                paramPos = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;

                switch(paramPos)
                {
                    case 1:{
                        uint32_t ManufacturerCode = GetBuf_2ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x7FF;
                        isParamValid = (ManufacturerCode == N2K_MFG_CODE_AIRMAR) | (ManufacturerCode == N2K_MFG_CODE_FURUNO);
                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 2;
                    }
                        break;
                    case 3:{
                        uint32_t IndustryGroup = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf) & 0x07;
                        isParamValid = (IndustryGroup == g_pgn060928_curr.mIndustry_Group);
                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;
                    }
                        break;
                    case 4:{
                        indentify_code = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                        PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;

                        if(indentify_code == 11 || indentify_code == 1 || indentify_code == 2){
                            isParamValid = true;
                        }
                        else {
                            isParamValid = false;
                        }
                    }
                        break;
                    case 5:{
                        if(indentify_code == 1 || indentify_code == 2){
                            control_code = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);

                            PGN126208CMD_paramPos = PGN126208CMD_paramPos + 1;

                            isParamValid = (control_code == 0);
                        }
                        else if(indentify_code == 11){
                            isParamValid = true;
                        }
                    }
                        break;
                    case 6:{
                        if(indentify_code == 1 || indentify_code == 2){
                          name_field  = *((uint64_t *)(buf + PGN126208CMD_paramPos));

                          PGN126208CMD_paramPos += 8;

                          isParamValid = (name_field == my_name_field);
                        }
                        else {
                            isParamValid = false;
                        }
                    }
                        break;
                    default:
                        isParamValid = false;
                        break;
                }

                if (isParamValid == false)
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
                else
                {
                    fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        if (g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfError == 0)
        {
//          printf("indentify_code[%d]\r\n", indentify_code);

            if(indentify_code == 11){
                PGN126720_FactoryTest_SetFieldValue();
                PGN126720_ProcessNameField(pgnId);
            }
            else if(indentify_code == 1){
                PGN126720_MemoryClearGroup_SetFieldValue();
                PGN126720_ProcessNameField(pgnId);
//              g_spayload_curr = g_spayload_org;
            }
            else if(indentify_code == 2){
                PGN126720_ResetGoup_SetFieldValue();
                PGN126720_ProcessNameField(pgnId);

                HAL_Delay(100);

                NVIC_SystemReset();
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);

    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 126998)
    {
        if (numOfParam> 0){
            while (numOfParam-- > 0){
                isParamValid = true;

                paramPos = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                PGN126208CMD_paramPos += 1;

                if(paramPos == 1)
                {
                    uint8_t fieldBuf[72];

                    uint8_t countBytes = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                    PGN126208CMD_paramPos += 1;

                    if(countBytes <= 72){
                        fieldBuf[0] = countBytes;

                        memcpy(fieldBuf + 1, buf + PGN126208CMD_paramPos, countBytes - 1);
                        memcpy(installationDescription1, fieldBuf, countBytes);

                        PGN126208CMD_paramPos += (countBytes - 1);
                    }
                    else {
                        printf("PGN126998 #1 size over - %d\r\n", countBytes);
                        isParamValid = false;
                    }
                }
                else if(paramPos == 2)
                {
                    uint8_t fieldBuf[72];

                    uint8_t countBytes = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                    PGN126208CMD_paramPos += 1;

                    if(countBytes <= 72){
                        fieldBuf[0] = countBytes;

                        memcpy(fieldBuf + 1, buf + PGN126208CMD_paramPos, countBytes - 1);
                        memcpy(installationDescription2, fieldBuf, countBytes);

                        PGN126208CMD_paramPos += (countBytes - 1);
                    }
                    else {
                        printf("PGN126998 #2 size over - %d\r\n", countBytes);
                        isParamValid = false;
                    }
                }
                else if(paramPos == 3)
                {
                    uint8_t fieldBuf[72];

                    uint8_t countBytes = GetBuf_1ByteUInt(size, PGN126208CMD_paramPos, buf);
                    PGN126208CMD_paramPos += 1;

                    if(countBytes <= 72){
                        fieldBuf[0] = countBytes;

                        memcpy(fieldBuf + 1, buf + PGN126208CMD_paramPos, countBytes - 1);
                        memcpy(menufacturerDescription, fieldBuf, countBytes);

                        PGN126208CMD_paramPos += (countBytes - 1);
                    }
                    else {
                        printf("PGN126998 #3 size over - %d\r\n", countBytes);
                        isParamValid = false;
                    }
                }
                else{
                    isParamValid = false;
                }

                if (isParamValid == false)
                {
                  fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                  numOfError++;
                }
                else
                {
                  fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
                }
            }
        }

        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                           txPriorityErrorCode,
                                           countOfFieldErrorCode,
                                           fieldErrorCodes);

        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);

        if (g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfError == 0)
        {
            PGN126998_ProcessNameField(pgnId, REQUEST_MESSAGE_TYPE_FASTPACKET);
        }
    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 65285 ||
            g_PGN126208COMMANDNAME.mCommanded_PGN == 65287 ||
            g_PGN126208COMMANDNAME.mCommanded_PGN == 130816 ||
            g_PGN126208COMMANDNAME.mCommanded_PGN == 130822 ||
            g_PGN126208COMMANDNAME.mCommanded_PGN == 130823)
    {
            if (numOfParam > 0) {
                while (numOfParam-- > 0){
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
            }

            PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                               g_PGN126208COMMANDNAME.mCommanded_PGN,
                                               PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                               txPriorityErrorCode,
                                               countOfFieldErrorCode,
                                               fieldErrorCodes);

            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
    }
    else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 126993)
    {
            if (numOfParam > 0) {
                while (numOfParam-- > 0){
                    if(countOfFieldErrorCode < PGN126208_FIELD_ERROR_CODE_MAX) fieldErrorCodes[countOfFieldErrorCode++] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
                    numOfError++;
                }
            }

            PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                               g_PGN126208COMMANDNAME.mCommanded_PGN,
                                               PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT,
                                               txPriorityErrorCode,
                                               countOfFieldErrorCode,
                                               fieldErrorCodes);

            PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
    }
    else {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208COMMANDNAME.mCommanded_PGN,
                                           PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT,
                                           txPriorityErrorCode,
                                           0,
                                           fieldErrorCodes);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnId, buf, messagetype);
    }
}

/*
 * pgn_126208_command.c
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

#include "pgn_060928.h"
#include "pgn_065287.h"
#include "pgn_126208.h"
#include "pgn_126208_command.h"
#include "pgn_126208_acknowledge.h"

/* define ------------------------------------------------------------*/
#define PGN126208COMMAND_NUM         126208
#define PGN126208COMMAND_PRIORITY    3

/* typedef -----------------------------------------------------------*/
PGN126208COMMANDNAME g_PGN126208COMMANDNAME;

/* variables ---------------------------------------------------------*/
uint32_t PGN126208Command_parameterPostion = 0;

/* functions ---------------------------------------------------------*/
void PGN126208COMMAND_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	if(messagetype == REQUEST_MESSAGE_TYPE_FASTPACKET)
	{
		g_PGN126208COMMANDNAME.mCommand_Group_Function_Code = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 0, buf);
		g_PGN126208COMMANDNAME.mCommanded_PGN = GetBuf_3ByteUInt(mFastPacket_Total_Message_Byte_Size, 1, buf);
		g_PGN126208COMMANDNAME.mPriority_Setting = (GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 4, buf) & 0xF);
		g_PGN126208COMMANDNAME.mNMEA_Reserved = (GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 4, buf) & 0xF) >> 4;
		g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 5, buf);
		PGN126208Command_parameterPostion = 6;
	}

#if 0
	printf(" [mCommand_Group_Function_Code = %ld] !!\r\n", g_PGN126208COMMANDNAME.mCommand_Group_Function_Code);
	printf(" [mCommanded_PGN = %ld] !!\r\n", g_PGN126208COMMANDNAME.mCommanded_PGN);
	printf(" [mPriority_Setting = %ld] !!\r\n", g_PGN126208COMMANDNAME.mPriority_Setting);
	printf(" [mNumber_of_Pairs_of_Commanded_Parameters_to_follow = %ld] !!\r\n", g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow);
#endif

}

void PGN126208COMMAND_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

	uint32_t txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK;
	uint8_t  fieldErrorCodes[3] = {0,};
	uint32_t numOfParam = g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow;
	uint32_t numOfFalse = 0;
	uint32_t numOfCount = 0;

	uint32_t parameterPostionValue = 0;
	uint8_t  parameterValidation = true;

	uint32_t time_delay = boot_delay_time / 1000;
	uint32_t access_level = g_access_level;
	uint32_t access_seed = g_access_seed;

	if(get_R_PS() == BROADCAST_DESTINATION_ADDR) return;

	if(g_PGN126208COMMANDNAME.mPriority_Setting < 8)
	{
		txPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
	}

	if(messagetype == REQUEST_MESSAGE_TYPE_FASTPACKET)
	{
		if(g_PGN126208COMMANDNAME.mCommanded_PGN == 60928)
		{
			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0)
			{
//				printf(" [numOfParam = %ld] !!\r\n", numOfParam);
				while(numOfParam-- > 0)
				{
					parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
					PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

					if(parameterPostionValue == 3)
					{
						mDevice_Intance = (mDevice_Intance & 0xF8) | (GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x07);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;
					}
					else if(parameterPostionValue == 4)
					{
						mDevice_Intance = (mDevice_Intance & 0x07) | (GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x1F);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;
					}
					else if(parameterPostionValue == 8)
					{
						mSystem_Instance = (GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x0F);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;
					}
					else parameterValidation = false;

					if(parameterValidation == false)
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
						numOfCount++;
						numOfFalse++;
//						printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
					}
					else
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
						numOfCount++;
//						printf(" [numOfCount = %ld] !!\r\n", numOfCount);
					}
				}
#if 0
				printf(" [txPriorityErrorCode = %ld] !!\r\n", txPriorityErrorCode);
				printf(" [mDevice_Intance = %ld] !!\r\n", mDevice_Intance);
				printf(" [mSystem_Intance = %ld] !!\r\n", mSystem_Intance);
#endif
			}

			PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                         g_PGN126208COMMANDNAME.mCommanded_PGN,
                                         PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                         txPriorityErrorCode,
                                         numOfCount,
                                         &fieldErrorCodes[0]);
			PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);

			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfFalse == 0)
			{
			    osDelay(1);
				PGN060928_SetInitialField(&mMyPGN060928);
				PGN060928_ProcessNameField();
//				MyNMEAInfo.SaveNMEAInfo();
			}
		}
		else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 65287)
		{
			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0)
			{
//				printf(" [numOfParam = %ld] !!\r\n", numOfParam);
				while(numOfParam-- > 0)
				{
					parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
					PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

					if (parameterPostionValue == 1)
					{
						uint32_t ManufacturerCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x7FF;
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 2;

						if(ManufacturerCode == 135) parameterValidation = true;
						else parameterValidation = false;
					}
					else if(parameterPostionValue == 3)
					{
						uint32_t IndustryGroup = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x07;
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

						if (IndustryGroup ==  mIndustry_Group) parameterValidation = true;
						else parameterValidation = false;
					}
					else if(parameterPostionValue == 4)
					{
						uint32_t pid = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

						if (pid ==  1) parameterValidation = true;
						else parameterValidation = false;
					}
					else if(parameterPostionValue == 5)
					{
						access_level = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);

						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;
					}
					else if(parameterPostionValue == 7)
					{
						access_seed = GetBuf_4ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);

						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 4;
					}
					else parameterValidation = false;

					if(parameterValidation == false)
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
						numOfCount++;
						numOfFalse++;
//						printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
					}
					else
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
						numOfCount++;
//						printf(" [numOfCount = %ld] !!\r\n", numOfCount);
					}
				}
			}

			PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
										 g_PGN126208COMMANDNAME.mCommanded_PGN,
										 PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
										 txPriorityErrorCode,
										 numOfCount,
										 &fieldErrorCodes[0]);
			PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);

			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfFalse == 0)
			{
				g_access_level = access_level;
				g_access_seed = calculate_level_2_key(access_seed, 0x64C257DA, 67);

				PGN065287_Airmar_SendNameField(pgnid);

				printf("g_access_level[%ld] g_access_seed[0x%lx]\r\n", g_access_level, g_access_seed);
			}
		}
		else if(g_PGN126208COMMANDNAME.mCommanded_PGN == 126720)
		{
			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0)
			{
//				printf(" [numOfParam = %ld] !!\r\n", numOfParam);
				while(numOfParam-- > 0)
				{
					parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
					PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

					if (parameterPostionValue == 1)
					{
						uint32_t ManufacturerCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x7FF;
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 2;

						if(ManufacturerCode == 135) parameterValidation = true;
						else parameterValidation = false;
				    }
					else if(parameterPostionValue == 3)
					{
						uint32_t IndustryGroup = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf) & 0x07;
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

						if (IndustryGroup ==  mIndustry_Group) parameterValidation = true;
						else parameterValidation = false;
					}
					else if(parameterPostionValue == 4)
					{
						uint32_t pid = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 1;

						if (pid ==  4) parameterValidation = true;
						else parameterValidation = false;
					}
					else if(parameterPostionValue == 5)
					{
						time_delay = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, PGN126208Command_parameterPostion, buf);
						PGN126208Command_parameterPostion = PGN126208Command_parameterPostion + 2;
					}
					else parameterValidation = false;

					if(parameterValidation == false)
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_ACCESS_DENIED;
						numOfCount++;
						numOfFalse++;
//						printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
					}
					else
					{
						fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
						numOfCount++;
//						printf(" [numOfCount = %ld] !!\r\n", numOfCount);
					}
				}
			}

			PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                         g_PGN126208COMMANDNAME.mCommanded_PGN,
                                         PGN126208_ACK_ERRORCODE_NO_ERROR_ACK,
                                         txPriorityErrorCode,
                                         numOfCount,
                                         &fieldErrorCodes[0]);
			PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);

			if(g_PGN126208COMMANDNAME.mNumber_of_Pairs_of_Commanded_Parameters_to_follow > 0 && numOfFalse == 0)
			{
				boot_delay_time = time_delay * 1000;

				printf("time_delay[%ld]sec\r\n", time_delay);
			}
		}
		else
		{
			PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                         g_PGN126208COMMANDNAME.mCommanded_PGN,
                                         PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT,
                                         txPriorityErrorCode,
                                         0,
                                         &fieldErrorCodes[0]);
			PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
		}
	}
}


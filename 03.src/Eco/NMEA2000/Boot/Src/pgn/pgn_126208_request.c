/*
 * pgn_126208_request.c
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
#include "pgn_126208_request.h"
#include "pgn_126208_acknowledge.h"
#include "pgn_126464.h"
#include "pgn_126720.h"
#include "pgn_126996.h"

/* define ------------------------------------------------------------*/
#define PGN126208REQUEST_NUM         126208
#define PGN126208REQUEST_PRIORITY    3

/* typedef -----------------------------------------------------------*/
PGN126208REQUESTNAME g_PGN126208REQUESTNAME;

/* variables ---------------------------------------------------------*/
uint32_t parameterPostion = 0;

/* functions ---------------------------------------------------------*/
void ProcessFASTPacket(void)
{
  return;
}

void PGN126208REQUEST_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

  g_PGN126208REQUESTNAME.mRequest_Group_Function_Code = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 0, buf);
  g_PGN126208REQUESTNAME.mRequested_PGN = GetBuf_3ByteUInt(mFastPacket_Total_Message_Byte_Size, 1, buf);
  g_PGN126208REQUESTNAME.mTransmission_interval = GetBuf_4ByteUInt(mFastPacket_Total_Message_Byte_Size, 4, buf);
  g_PGN126208REQUESTNAME.mTransmission_interval_offset = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, 8, buf);
  g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 10, buf);
  parameterPostion = 11;

//  printf(" [mRequest_Group_Function_Code = %ld] !!\r\n", g_PGN126208REQUESTNAME.mRequest_Group_Function_Code);
//  printf(" [mRequested_PGN = %ld] !!\r\n", g_PGN126208REQUESTNAME.mRequested_PGN);
//  printf(" [mTransmission_interval = 0x%lx] !!\r\n", g_PGN126208REQUESTNAME.mTransmission_interval);
//  printf(" [mTransmission_interval_offset = 0x%lx] !!\r\n", g_PGN126208REQUESTNAME.mTransmission_interval_offset);
//  printf(" [mNumber_of_Pairs_of_Request_Parameters_to_follow = %ld] !!\r\n", g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow);
}

void PGN126208REQUEST_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter... MsgType[%ld] ReqPGN[%ld]\r\n",__FUNCTION__,__LINE__, messagetype, g_PGN126208REQUESTNAME.mRequested_PGN);

	uint32_t txIntervalPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK;
	uint8_t  fieldErrorCodes[7] = {0,};
	uint32_t numOfParam = g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow;
	uint32_t numOfFalse = 0;
	uint32_t numOfCount = 0;

	uint32_t parameterPostionValue = 0;
	uint8_t  parameterValidation = false;

	uint32_t ManufacturerCode = 1855;
	uint32_t ISOECUInstance;				// Device Instance Lower
	uint32_t ISOFunctionInstance;			// Device Instance Upper
	uint32_t ISOFunction;					// Device Function
	uint32_t ISODeviceClassInstance;		// Device Class
	uint32_t SystemIntance;
	uint32_t IndustryGroup;
	uint32_t ProprietaryID = 0;

	uint32_t mReceivePGNGroupFunctionCode;

	uint32_t ProductCode;	//mNMEAManufacturersProductCode = 6

	if(g_PGN126208REQUESTNAME.mTransmission_interval == 0xFFFFFFFF)
	{
		if (g_PGN126208REQUESTNAME.mTransmission_interval_offset != 0xFFFF && g_PGN126208REQUESTNAME.mTransmission_interval_offset != 0)
		{
			txIntervalPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
		}
	}
	else if(g_PGN126208REQUESTNAME.mTransmission_interval == 0xFFFFFFFE)
	{
		if (get_R_PS() != BROADCAST_DESTINATION_ADDR)
		{
			txIntervalPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
		}
	}
	else
	{
		txIntervalPriorityErrorCode = PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED;
	}

	switch(messagetype)
	{
  case REQUEST_MESSAGE_TYPE_FASTPACKET:
      //printf(" [mMerged_FastPacket = ");  for (int i = 0; i < mFastPacket_Total_Message_Byte_Size; i++) printf("0x%02x ", buf[i]);  printf("]\r\n");
      //printf(" [messagetype = REQUEST_MESSAGE_TYPE_FASTPACKET] !!\r\n");
      //printf(" [g_PGN126208REQUESTNAME.mRequested_PGN = %ld] !!\r\n", g_PGN126208REQUESTNAME.mRequested_PGN);

    if (g_PGN126208REQUESTNAME.mRequested_PGN == 60928)
    {
      if (g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow > 0)
      {
//          printf(" [numOfParam = %ld] !!\r\n", numOfParam);
        while (numOfParam-- > 0)
        {
          parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
          parameterPostion = parameterPostion + 1;

          if (parameterPostionValue == 2)
          {
            ManufacturerCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x7FF;
            parameterPostion = parameterPostion + 2;
            if(ManufacturerCode == 1855 || ManufacturerCode == 135) parameterValidation = true;           //3f07
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 3)
          {
            ISOECUInstance = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x07;
            parameterPostion = parameterPostion + 1;
            if (ISOECUInstance == (mDevice_Intance & 0x07)) parameterValidation = true;      //00
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 4)
          {
            ISOFunctionInstance = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x1F;
            parameterPostion = parameterPostion + 1;
            if(ISOFunctionInstance == ((mDevice_Intance & 0xF8) >> 3))parameterValidation = true; //00
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 5)
          {
            ISOFunction = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
            parameterPostion = parameterPostion + 1;
            if(ISOFunction == mDevice_Function) parameterValidation = true;             //82
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 7)
          {
            ISODeviceClassInstance = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x7F;
            parameterPostion = parameterPostion + 1;
            if(ISODeviceClassInstance == mDevice_Class) parameterValidation = true;          //78
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 8)
          {
            SystemIntance = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x0F;
            parameterPostion = parameterPostion + 1;
            if (SystemIntance == mSystem_Instance) parameterValidation = true;            //00
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 9)
          {
            IndustryGroup = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x07;
            parameterPostion = parameterPostion + 1;
            if (IndustryGroup ==  mIndustry_Group) parameterValidation = true;             //04
            else parameterValidation = false;
          }
          else parameterValidation = false;

          if(parameterValidation == false)
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
            numOfCount++;
            numOfFalse++;
//              printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
          }
          else
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
            numOfCount++;
//              printf(" [numOfCount = %ld] !!\r\n", numOfCount);
          }
        }
#if 0
        printf(" [txIntervalPriorityErrorCode = %ld] !!\r\n", txIntervalPriorityErrorCode);
        printf(" [ManufacturerCode = %ld] !!\r\n", ManufacturerCode);
        printf(" [ISOECUInstance = %ld] !!\r\n", ISOECUInstance);
        printf(" [ISOFunctionInstance = %ld] !!\r\n", ISOFunctionInstance);
        printf(" [ISOFunction = %ld] !!\r\n", ISOFunction);
        printf(" [ISODeviceClassInstance = %ld] !!\r\n", ISODeviceClassInstance);
        printf(" [SystemIntance = %ld, %ld] !!\r\n", SystemIntance, mSystem_Instance);
        printf(" [IndustryGroup = %ld] !!\r\n", IndustryGroup);
#endif
      }

      if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfFalse == 0)
      {
        osDelay(1);

        PGN060928NAME pGN060928 = mMyPGN060928;
        pGN060928.mManufacturer_Code = ManufacturerCode;

        PGN060928_SetInitialField(&pGN060928);
        PGN060928_ProcessNameField();

        return;
      }
#if 0
      printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
#endif
      if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
      {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208REQUESTNAME.mRequested_PGN,
                                           txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ?
                                               PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                           txIntervalPriorityErrorCode,
                                           numOfCount,
                                           &fieldErrorCodes[0]);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
      }
    }
    else if (g_PGN126208REQUESTNAME.mRequested_PGN == 65287)
	{
	  if (g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow > 0)
	  {
//        printf(" [numOfParam = %ld] !!\r\n", numOfParam);
      while (numOfParam-- > 0)
      {
        parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
        parameterPostion = parameterPostion + 1;

        if (parameterPostionValue == 1)
        {
          ManufacturerCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x7FF;
          parameterPostion = parameterPostion + 2;

          parameterValidation = (ManufacturerCode == 135);
        }
        else if(parameterPostionValue == 3)
        {
          IndustryGroup = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x07;
          parameterPostion = parameterPostion + 1;

          parameterValidation = (IndustryGroup ==  mIndustry_Group);
        }
        else if(parameterPostionValue == 4)
        {
          uint32_t format_code = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
          parameterPostion = parameterPostion + 1;

          parameterValidation = (format_code == 1);
        }
        else parameterValidation = false;

        if(parameterValidation == false)
        {
          fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
          numOfCount++;
          numOfFalse++;
  //        printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
        }
        else
        {
          fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
          numOfCount++;
  //        printf(" [numOfCount = %ld] !!\r\n", numOfCount);
        }
      }
	  }

	  if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfFalse == 0)
	  {
      PGN065287_Airmar_SendNameField(pgnid);

      return;
	  }
#if 0
	  printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
#endif
	  if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
	  {
      PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                         g_PGN126208REQUESTNAME.mRequested_PGN,
                                         txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ?
                                             PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                             txIntervalPriorityErrorCode,
                                         numOfCount,
                                         &fieldErrorCodes[0]);
      PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
	  }
	}
    else if(g_PGN126208REQUESTNAME.mRequested_PGN == 126464)
    {
      if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow > 0)
      {
        parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
        parameterPostion = parameterPostion + 1;
        if(parameterPostionValue == 1)
        {
          mReceivePGNGroupFunctionCode = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
          parameterPostion = parameterPostion + 1;

          if(mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE || mReceivePGNGroupFunctionCode == PGN_GROUP_TRANSMIT_LIST_MESSAGE)
          {
            PGN126464_SetFieldValue(mReceivePGNGroupFunctionCode,
                                    mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE ?
                                    mReceive_PGN_List : mTransmit_PGN_List,
                                    mReceivePGNGroupFunctionCode == PGN_GROUP_RECEIVE_LIST_MESSAGE ?
                                    Receive_PGN_List_Unit : Transmit_PGN_List_Unit);
            PGN126464_ProcessNameField(pgnid, buf, messagetype);
            return;
          }
          else
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
            numOfCount++;
            numOfFalse = 1;
          }
        }
      }

      if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfFalse == 0)
      {
        PGN126464_SetFieldValue(PGN_GROUP_TRANSMIT_LIST_MESSAGE, mTransmit_PGN_List, Transmit_PGN_List_Unit);
        PGN126464_ProcessNameField(pgnid, buf, messagetype);

        PGN126464_SetFieldValue(PGN_GROUP_RECEIVE_LIST_MESSAGE, mReceive_PGN_List, Receive_PGN_List_Unit);
        PGN126464_ProcessNameField(pgnid, buf, messagetype);

        return;
      }
#if 0
      printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
#endif
      if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
      {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208REQUESTNAME.mRequested_PGN,
                                           txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                           txIntervalPriorityErrorCode,
                                           numOfCount,
                                           &fieldErrorCodes[0]);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
      }
    }
    else if(g_PGN126208REQUESTNAME.mRequested_PGN == PGN126720_NUM)
    {
      if(g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow > 0)
      {
//        printf("%s:%d numOfParam[%ld]\r\n",__FUNCTION__,__LINE__, numOfParam);
        while (numOfParam-- > 0)
        {
          parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
          parameterPostion = parameterPostion + 1;

          if(parameterPostionValue == 1)
          {
            ManufacturerCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x7FF;
            parameterPostion = parameterPostion + 2;
            if(ManufacturerCode == mManufacturer_Code) parameterValidation = true;           //3f07
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 3)
          {
            IndustryGroup = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf) & 0x07;
            parameterPostion = parameterPostion + 1;
            if (IndustryGroup ==  mIndustry_Group) parameterValidation = true;             //04
            else parameterValidation = false;
          }
          else if(parameterPostionValue == 4)
          {
            ProprietaryID = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
            parameterPostion = parameterPostion + 1;
            if(ProprietaryID == PGN126720_BOOT_PROP_BOOTLDR_VER) parameterValidation = true;
            else parameterValidation = false;
          }
          else parameterValidation = false;

          if(parameterValidation == false)
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
            numOfCount++;
            numOfFalse++;
//            printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
          }
          else
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
            numOfCount++;
//              printf(" [numOfCount = %ld] !!\r\n", numOfCount);
          }
        }
      }

      if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfFalse == 0)
      {
        osDelay(1);
        PGN126720_SetNameField(ProprietaryID, messagetype);
        PGN126720_ProcessNameField(pgnid, buf, messagetype);
        return;
      }

      printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
      if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
      {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208REQUESTNAME.mRequested_PGN,
                                           txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                           txIntervalPriorityErrorCode,
                                           numOfCount,
                                           &fieldErrorCodes[0]);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
      }
    }
    else if(g_PGN126208REQUESTNAME.mRequested_PGN == PGN126996_NUM)
    {
      if(g_PGN126208REQUESTNAME.mNumber_of_Pairs_of_Request_Parameters_to_follow > 0)
      {
//          printf(" [numOfParam = %ld] !!\r\n", numOfParam);
        while (numOfParam-- > 0)
        {
          parameterPostionValue = GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
          parameterPostion = parameterPostion + 1;

          if(parameterPostionValue == 2)
          {
            ProductCode = GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, parameterPostion, buf);
            parameterPostion = parameterPostion + 2;
            if(ProductCode == PGN126996_NMEA2000_MFG_PRODUCT_CODE) parameterValidation = true;
            else parameterValidation = false;
          }
          else parameterValidation = false;

          if(parameterValidation == false)
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM;
            numOfCount++;
            numOfFalse++;
//              printf(" [numOfCount = %ld, numOfFalse = %ld] !!\r\n", numOfCount, numOfFalse);
          }
          else
          {
            fieldErrorCodes[numOfCount] = PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK;
            numOfCount++;
//              printf(" [numOfCount = %ld] !!\r\n", numOfCount);
          }
        }
#if 0
        printf(" [txIntervalPriorityErrorCode = %ld] !!\r\n", txIntervalPriorityErrorCode);
        printf(" [ProductCode = %ld] !!\r\n", ProductCode);
#endif
      }

      if(txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK && numOfFalse == 0)
      {
        osDelay(1);
        PGN126996_ProcessNameField(pgnid, mFastPacket_Total_Message_Byte_Size, buf, messagetype);
        return;
      }

#if 0
      printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
#endif
      if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
      {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208REQUESTNAME.mRequested_PGN,
                                           txIntervalPriorityErrorCode == PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK ? PGN126208_ACK_ERRORCODE_NO_ERROR_ACK : PGN126208_ACK_ERRORCODE_ACCESS_DENIED,
                                           txIntervalPriorityErrorCode,
                                           numOfCount,
                                           &fieldErrorCodes[0]);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
      }
    }
    else  //No PGN matched.
    {
#if 0
      printf(" [get_R_PS = %ld] !!\r\n", get_R_PS());   // 110
#endif
      if(get_R_PS() != BROADCAST_DESTINATION_ADDR)
      {
        PGN126208ACKNOWLEDGE_SetFieldValue(FUNCTION_CODE_ACKNOWLEDGE_MESSAGE,
                                           g_PGN126208REQUESTNAME.mRequested_PGN,
                                           PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT,
                                           PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_PRIORITY_NOT_SUP,
                                           0,
                                           &fieldErrorCodes[0]);
        PGN126208ACKNOWLEDGE_ProcessNameField(pgnid, buf, messagetype);
      }
    }
    break;
  case REQUEST_MESSAGE_TYPE_BAM_PACKET:
    break;
  case REQUEST_MESSAGE_TYPE_RTSCTS_PACKET:
    break;
	}
}


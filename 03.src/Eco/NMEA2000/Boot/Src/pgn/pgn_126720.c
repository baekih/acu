/*
 * pgn_126720.c
 *
 *  Created on: 2022. 8. 8.
 *      Author: ihbaek
 */


/*
 * pgn_126464.c
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */


/* Includes ------------------------------------------------------------------*/
#include "common.h"

#include "nmea2000.h"
#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "fastpacketdata.h"

#include "pgn_126720.h"

/* define ------------------------------------------------------------*/
#define SREC_BASE     8
#define CNT_LEN       1
#define ADR_LEN       4
#define CHECKSUM_LEN  1

/* typedef -----------------------------------------------------------*/
PGN126720BASENAME     g_PGN126720BASENAME;
PGN126720CMDNAME      g_PGN126720CMDNAME;
PGN126720STATUSNAME   g_PGN126720STATUSNAME;
PGN126720VERNAME      g_PGN126720VERNAME;
PGN126720DATBASENAME  g_PGN126720DATBASENAME;
PGN126720DATS0NAME    g_PGN126720DATS0NAME;
PGN126720DATS3NAME    g_PGN126720DATS3NAME;
PGN126720DATS7NAME    g_PGN126720DATS7NAME;

/* functions ---------------------------------------------------------*/
uint8_t Ascii2Uint8(uint8_t *pbuf)
{
  const char buf[3] = {*pbuf, *(pbuf+1), 0};

  return (uint8_t)strtoul(buf, NULL, 16);
}

uint16_t Ascii2Uint16(uint8_t *pbuf)
{
  const char buf[5] = {*pbuf, *(pbuf+1), *(pbuf+2), *(pbuf+3), 0};

  return (uint16_t)strtoul(buf, NULL, 16);
}

uint32_t Ascii2Uint32(uint8_t *pbuf)
{
  const char buf[9] = {*pbuf, *(pbuf+1), *(pbuf+2), *(pbuf+3), *(pbuf+4), *(pbuf+5), *(pbuf+6), *(pbuf+7), 0};

  return (uint32_t)strtoul(buf, NULL, 16);
}


void PGN126720_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
  uint8_t *pbuf_srec = NULL;
  uint8_t checksum = 0;
//  printf("%s:%d Enter...\r\n",__FUNCTION__,__LINE__);

  g_PGN126720BASENAME.mMFGCode = (uint16_t)(GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, 0, buf) & 0x07FF);
  g_PGN126720BASENAME.mIndGrp  = (uint8_t)((GetBuf_2ByteUInt(mFastPacket_Total_Message_Byte_Size, 0, buf) >> 13) & 0x0003);
  g_PGN126720BASENAME.mPropID  = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 2, buf));

//  printf("%s:%d mPropID[%d]\r\n",__FUNCTION__,__LINE__, g_PGN126720BASENAME.mPropID);

  switch(g_PGN126720BASENAME.mPropID)
  {
  case PGN126720_BOOT_PROP_BOOTLDR_CMD:
    g_PGN126720CMDNAME.mSID = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 3, buf));
    g_PGN126720CMDNAME.mCMD = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 4, buf));
    g_PGN126720CMDNAME.mDAT = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 5, buf));
    break;
  case PGN126720_BOOT_PROP_BOOTLDR_DATA:
    g_PGN126720DATBASENAME.mSID = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 3, buf));
    g_PGN126720DATBASENAME.mTYP = (uint8_t)(GetBuf_1ByteUInt(mFastPacket_Total_Message_Byte_Size, 7, buf));

    pbuf_srec = malloc(mFastPacket_Total_Message_Byte_Size - SREC_BASE);
    memset(pbuf_srec, 0x00, mFastPacket_Total_Message_Byte_Size - SREC_BASE);
    memcpy(pbuf_srec, &buf[SREC_BASE], mFastPacket_Total_Message_Byte_Size - SREC_BASE);

    g_PGN126720DATBASENAME.mCNT = Ascii2Uint8(pbuf_srec);

    switch(g_PGN126720DATBASENAME.mTYP)
    {
    case '0':
      g_PGN126720DATS0NAME.mBootVer = Ascii2Uint16(pbuf_srec + 22);
      g_PGN126720DATS0NAME.mAppVer  = Ascii2Uint16(pbuf_srec + 26);
      break;
    case '3':
      g_PGN126720DATS3NAME.mADR = Ascii2Uint32(pbuf_srec + CNT_LEN*2);
      g_PGN126720DATS3NAME.mLEN = g_PGN126720DATBASENAME.mCNT - (ADR_LEN + CHECKSUM_LEN);
      for(uint8_t uicnt = 0; uicnt < g_PGN126720DATS3NAME.mLEN ; uicnt++)
      {
        g_PGN126720DATS3NAME.mDAT[uicnt] = Ascii2Uint8(pbuf_srec + (uicnt + CNT_LEN + ADR_LEN)*2);
      }
      break;
    case '7':
      g_PGN126720DATS7NAME.mADR = Ascii2Uint32(pbuf_srec + CNT_LEN*2);
      break;
    }

    for(uint8_t uicnt = 0; uicnt < g_PGN126720DATBASENAME.mCNT; uicnt++) checksum += Ascii2Uint8(pbuf_srec + 2*uicnt);
    checksum = 0xFF - checksum;
    g_PGN126720DATBASENAME.mChecksum = Ascii2Uint8(pbuf_srec + 2*g_PGN126720DATBASENAME.mCNT);

    free(pbuf_srec);
    break;
  case PGN126720_BOOT_PROP_APP_LAUNCH:
    break;
  case PGN126720_BOOT_PROP_BOOTLDR_VER:
  default:
    break;
  }
}

void PGN126720_SetNameField(uint32_t _ProprietaryID, uint32_t messagetype)
{
  if(messagetype != REQUEST_MESSAGE_TYPE_FASTPACKET) return;

  g_PGN126720BASENAME.mPropID = _ProprietaryID;

  return;
}

void PGN126720_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype)
{
//  printf("%s:%d Enter... MsgType[%ld]\r\n",__FUNCTION__,__LINE__, messagetype);
  InitializeSendNameBitPosition();
  InitializeSendNameField();

  Add2ByteUInt( PPGN_AIRMAR_MFGCODE );

  switch(g_PGN126720BASENAME.mPropID)
  {
  case PGN126720_BOOT_PROP_BOOTLDR_VER:
    g_PGN126720BASENAME.mPropID = PGN126720_BOOT_PROP_BOOTLDR_VER;
    g_PGN126720VERNAME.mProductCode = PGN126720_5VER_PRODUCTCODE;
    g_PGN126720VERNAME.mProcessorCode = PGN126720_5VER_PROCESSORCODE_MASTER;
    g_PGN126720VERNAME.mBootVer = g_common_dat.bootver;
    g_PGN126720VERNAME.mAppVer = g_common_dat.appver;

    Add1ByteUInt( g_PGN126720BASENAME.mPropID );
    Add2ByteUInt( g_PGN126720VERNAME.mProductCode );
    Add1ByteUInt( g_PGN126720VERNAME.mProcessorCode );
    Add2ByteUInt( g_PGN126720VERNAME.mBootVer );
    Add2ByteUInt( g_PGN126720VERNAME.mAppVer );
    break;
  case PGN126720_BOOT_PROP_BOOTLDR_CMD:
    g_PGN126720BASENAME.mPropID = PGN126720_BOOT_PROP_BOOTLDR_STAT;
    g_PGN126720STATUSNAME.mSID = g_PGN126720CMDNAME.mSID;
    g_PGN126720STATUSNAME.mProcessorCode = PGN126720_6STATUS_PROCESSCODE_1MASTER;
    switch(g_PGN126720CMDNAME.mCMD)
    {
      case PGN126720_10CMD_0LOCK_FLASH:
        printf("LockFlash.\r\n");
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_0LOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_1UNLOCK_FLASH:
        printf("UnlockFlash.\r\n");
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_2ERASE_FLASH:
        g_flash_source_addr = get_R_SA();
        printf("EraseFlash. source_addr[%d]\r\n", g_flash_source_addr);
//BIH        if(FLASHIF_OK != (ret = IAP_Erase())) printf("%s() EraseFlash Fail. ret[%ld]\r\n",__FUNCTION__, ret);
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_3PROGRAM_FLASH:
        printf("InitProgramFlash.\r\n");
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_4VERIFY_FLASH:
        printf("VerifyFlash.\r\n");
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_3UNLOCKVERIFY;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_5ABORT_FLASH:
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_0LOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_6CHANGE_PROCESSOR:
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_0LOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_254OUTOFRANGE:
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_0LOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
      case PGN126720_10CMD_255DONTCHANGE:
        g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_0LOCKED;
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
        break;
    }

    Add1ByteUInt( g_PGN126720BASENAME.mPropID );
    Add1ByteUInt( g_PGN126720STATUSNAME.mSID );
    Add1ByteUInt( g_PGN126720STATUSNAME.mProcessorCode );
    Add1ByteUInt( ((g_PGN126720STATUSNAME.mStatusLastOp << 3) & 0xF8) | (g_PGN126720STATUSNAME.mProgrammingMode &0x07) );

    break;
  case PGN126720_BOOT_PROP_BOOTLDR_DATA:
    g_PGN126720BASENAME.mPropID = PGN126720_BOOT_PROP_BOOTLDR_STAT;
    g_PGN126720STATUSNAME.mSID  = g_PGN126720DATBASENAME.mSID;
    g_PGN126720STATUSNAME.mProgrammingMode = PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM;

    switch(g_PGN126720DATBASENAME.mTYP)
    {
    case '0':
      printf("StartFlash.\r\n");
      g_common_dat.isUpdateFlashIdle = UPLOAD_STATE_FLASH_ING;
      g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
      break;
    case '3':
      // Write Binary to Flash.
//      printf("%s:%d WriteToFlash... \r\n",__FUNCTION__,__LINE__);
      if(0 != g_PGN126720DATS3NAME.mADR%0x20)
      {
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_5BUFOVERFLOW;
        break;
      }

#if 0 //BIH
      if(FLASHIF_OK == IAP_Write(g_PGN126720DATS3NAME.mADR, (uint64_t*)g_PGN126720DATS3NAME.mDAT, g_PGN126720DATS3NAME.mLEN/8))
      {
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
      }
      else
      {
        g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_2PROGERR;
      }
#endif
      break;
    case '5':
      g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_0NOERR;
      break;
    case '7':
      printf("FinishFlash.\r\n");
      g_common_dat.isUpdateFlashIdle = UPLOAD_STATE_FLASH_OK;
      g_PGN126720STATUSNAME.mStatusLastOp    = PGN126720_6STATUS_STATUSOFOP_1PROGSUCCESS;
      break;
    }

    Add1ByteUInt( g_PGN126720BASENAME.mPropID );
    Add1ByteUInt( g_PGN126720STATUSNAME.mSID );
    Add1ByteUInt( g_PGN126720STATUSNAME.mProcessorCode );
    Add1ByteUInt( ((g_PGN126720STATUSNAME.mStatusLastOp << 3) & 0xF8) | (g_PGN126720STATUSNAME.mProgrammingMode &0x07) );
    break;
  case PGN126720_BOOT_PROP_MASTER_RESET:
  case PGN126720_BOOT_PROP_APP_LAUNCH:
    printf("Launch APP...\r\n");
    g_nvic_reset = true;
//    HAL_NVIC_SystemReset();
    break;
  }

  SendNonSingleFrame(NmeaPGN_CanID(PGN126720_PRIORITY, PGN126720_NUM, get_R_SA(), localSourceAddr),
                     sendPacketLength, sendNMEAPackets, messagetype);
}


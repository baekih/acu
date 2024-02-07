/*
 * pgn126720.h
 *
 *  Created on: 2022. 8. 8.
 *      Author: ihbaek
 */

#ifndef INC_PGN_PGN_126720_H_
#define INC_PGN_PGN_126720_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* define ------------------------------------------------------------*/
#define PGN126720_NUM                                 126720
#define PGN126720_PRIORITY                            6

#define PGN126720_BOOT_PROP_MASTER_RESET              1
//#define PGN126720_BOOT_PROP_BOOTLDR_LAUNCH           2
#define PGN126720_BOOT_PROP_APP_LAUNCH                3
//#define PGN126720_BOOT_PROP_STARTUP_MON_TIMEOUT      4
#define PGN126720_BOOT_PROP_BOOTLDR_VER               5
#define PGN126720_BOOT_PROP_BOOTLDR_STAT              6
#define PGN126720_BOOT_PROP_BOOTLDR_CMD               10
#define PGN126720_BOOT_PROP_BOOTLDR_DATA              11

#define PGN126720_5VER_PRODUCTCODE                    10571 //Only this number work well.
#define PGN126720_5VER_PROCESSORCODE_MASTER           1
#define PGN126720_5VER_PROCESSORCODE_SLAVE            2
#define PGN126720_5VER_BOOTVER                        1000
#define PGN126720_5VER_APPVER                         1010

#define PGN126720_6STATUS_PROCESSCODE_1MASTER         PGN126720_5VER_PROCESSORCODE_MASTER
#define PGN126720_6STATUS_PROCESSCODE_2SLAVE          PGN126720_5VER_PROCESSORCODE_SLAVE

#define PGN126720_6STATUS_PROGRAMMODE_0LOCKED         0
#define PGN126720_6STATUS_PROGRAMMODE_1UNLOCKED       1
#define PGN126720_6STATUS_PROGRAMMODE_2UNLOCKPROGRAM  2
#define PGN126720_6STATUS_PROGRAMMODE_3UNLOCKVERIFY   3

#define PGN126720_6STATUS_STATUSOFOP_0NOERR           0
#define PGN126720_6STATUS_STATUSOFOP_1PROGSUCCESS     1
#define PGN126720_6STATUS_STATUSOFOP_2PROGERR         2
#define PGN126720_6STATUS_STATUSOFOP_5BUFOVERFLOW     5
#define PGN126720_6STATUS_STATUSOFOP_6ERASEERR        6
#define PGN126720_6STATUS_STATUSOFOP_7CHKSUMERR       7
#define PGN126720_6STATUS_STATUSOFOP_8HDRERR          8
#define PGN126720_6STATUS_STATUSOFOP_9SRECORDERR      9
#define PGN126720_6STATUS_STATUSOFOP_10SIDERR         10
#define PGN126720_6STATUS_STATUSOFOP_11FLASHERR       11
#define PGN126720_6STATUS_STATUSOFOP_12FLASHNOERR     12
#define PGN126720_6STATUS_STATUSOFOP_30OTHERERR       30
#define PGN126720_6STATUS_STATUSOFOP_31DATNOAVAIL     31

#define PGN126720_10CMD_0LOCK_FLASH                   0
#define PGN126720_10CMD_1UNLOCK_FLASH                 1
#define PGN126720_10CMD_2ERASE_FLASH                  2
#define PGN126720_10CMD_3PROGRAM_FLASH                3
#define PGN126720_10CMD_4VERIFY_FLASH                 4
#define PGN126720_10CMD_5ABORT_FLASH                  5
#define PGN126720_10CMD_6CHANGE_PROCESSOR             6
#define PGN126720_10CMD_254OUTOFRANGE                 254
#define PGN126720_10CMD_255DONTCHANGE                 255

#define PGN126720_11DAT_DAT_MAX                       32

/* typedef -----------------------------------------------------------*/
typedef struct __PGN126720BASENAME
{
  uint16_t mMFGCode;           // 11 bits
  uint8_t  mIndGrp;            // 3 bits
  uint8_t  mPropID;            // 5 bits
} PGN126720BASENAME;

typedef struct __PGN126720CMDNAME
{
  uint8_t  mSID;               // 8 bits
  uint8_t  mCMD;               // 8 bits
  uint8_t  mDAT;               // 8 bits
} PGN126720CMDNAME;

typedef struct __PGN126720STATUSNAME
{
  uint8_t  mSID;               // 8 bits
  uint8_t  mProcessorCode;     // 8 bits
  uint8_t  mProgrammingMode;   // 3 bits
  uint8_t  mStatusLastOp;      // 5 bits
} PGN126720STATUSNAME;

typedef struct __PGN126720VERNAME
{
  uint16_t mProductCode;       // 16 bits
  uint8_t  mProcessorCode;     // 8 bits
  uint16_t mBootVer;           // 16 bits
  uint16_t mAppVer;            // 16 bits
} PGN126720VERNAME;

typedef struct __PGN126720DATBASENAME
{
  uint8_t  mSID;                          // 8 bit
  uint8_t  mTYP;                          // 8 bit
  uint8_t  mCNT;                          // 8 bit
  uint8_t  mChecksum;                     // 8 bit
} PGN126720DATBASENAME;

typedef struct __PGN126720DATS0NAME
{
  uint16_t mBootVer;
  uint16_t mAppVer;
} PGN126720DATS0NAME;

typedef struct __PGN126720DATS3NAME
{
  uint32_t mADR;                          // 16 bit
  uint8_t  mDAT[PGN126720_11DAT_DAT_MAX]; // 32 byte
  uint8_t  mLEN;
} PGN126720DATS3NAME;

typedef struct __PGN126720DATS7NAME
{
  uint32_t mADR;                          // 32 bit
} PGN126720DATS7NAME;

/* functions --------------------------------------------------------*/
void PGN126720_GetFieldValue(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);
void PGN126720_SetNameField(uint32_t _ProprietaryID, uint32_t messagetype);
void PGN126720_ProcessNameField(uint32_t pgnid, uint8_t *buf, uint32_t messagetype);

#endif /* INC_PGN_PGN_126720_H_ */

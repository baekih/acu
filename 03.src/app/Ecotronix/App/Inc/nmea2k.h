/*
 * nmea2k.h
 *
 *  Created on: Mar 23, 2023
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_NMEA2K_H_
#define APPLICATION_USER_ECOTRONIX_INC_NMEA2K_H_

#define MFG_CODE_FURUNO                     (1855UL)
#define MFG_CODE_AIRMAR                     (135UL)

#ifdef BOOTLOADER
#define MFG_CODE                            MFG_CODE_AIRMAR
#else
#define MFG_CODE                            MFG_CODE_FURUNO
#endif
#define PPGN_MFGCODE                        ((0x04 << 13) | (0x3 << 11) | (MFG_CODE << 0))  // 0x9F3F
#define PPGN_MFGCODE_SIZE                   (2U)

#define NMEA2K_ID                           (688ULL)
#define DEV_INSTANCE_LO                     (0ULL)
#define DEV_INSTANCE_HI                     (0ULL)
#define DEV_INSTANCE                        (0ULL)
#define DEV_FUNC                            (130ULL)
#define NMEA_RSV                            (0ULL)
#define DEV_CLASS                           (110ULL)
#define SYS_INSTANCE                        (0ULL)
#define IND_GRP                             (4ULL)
#define ISO_SELF_CFG                        (1ULL)

#define MFG_CODE_AIRMAR_POS                 (1U)
#define IND_GRP_POS                         (3U)

#define NMEA2K_FASTPACKET_TIMEOUT_MS        (750UL)
#define NMEA2K_RSV_1BYTE                    (0xFFU)
#define NMEA2K_RSV_2BYTE                    (0xFFFFUL)
#define NMEA2K_RSV_4BYTE                    (0xFFFFFFFFULL)

#define NMEA_NET_MSG_DB_VER                 (2100UL)
#define NMEA_MFG_PROD_CODE                  (6U)
#define MFG_MODEL_ID                        "FI-DIN"
#define MFG_SW_VER_CODE                     "00.01:00.01"
#define MFG_MODEL_VER                       "1"
#define MFG_MODEL_SER_CODE                  "0000"
#define MFG_NMEA2K_CERT_LVL                 (2U)
#define LOAD_EQ                             (8U)

#define FASTPACKET_LEN                      (223U)
#define FASTPACKET_LEN_SIZE                 (1U)
#define FASTPACKET_ARRAY_MAX                (2U)
#define FASTPACKET_BUF_EMPTY                (0U)
#define FASTPACKET_BUF_MERGING              (1U)
#define FASTPACKET_BUF_AVAILABLE            (2U)
#define FASTPACKET_SEQ_ID_GET               (0U)
#define FASTPACKET_SEQ_ID_SET               (1U)
#define FASTPACKET_SEQ_ID_REL               (2U)
#define FASTPACKET_SEQ_ID_MAX               (8U)
#define FASTPACKET_SEQ_ID_EMPTY             (-1)
#define FASTPACKET_SEQ_ID_RET_ERR           (-1)

#define MULTIPACKET_LEN                     (1785UL)
#define MULTIPACKET_BYTE_PER_FRAME          (7U)
#define MULTIPACKET_BUF_EMPTY               (0U)
#define MULTIPACKET_BUF_MERGING             (1U)
#define MULTIPACKET_BUF_AVAILABLE           (2U)

#define PGN059392_NUM                       (59392UL)
#define PGN059392_PRI                       (6UL)
#define PGN059392_POSITIVIE_ACK             0x00
#define PGN059392_NEGATIVIE_ACK             0x01

#define PGN059904_NUM                       (59904UL)
#define PGN059904_PRI                       (6UL)

#define PGN060160_NUM                       (60160UL)
#define PGN060160_PRI                       (6UL)

#define PGN060928_NUM                       (60928UL)
#define PGN060928_PRI                       (6UL)

#define PGN060416_NUM                       (60416UL)
#define PGN060416_RTS_CTRL                  (16U)
#define PGN060416_RTS_PRI                   (6U)
#define PGN060416_CTS_CTRL                  (17U)
#define PGN060416_CTS_PRI                   (7U)
#define PGN060416_EOM_CTRL                  (19U)
#define PGN060416_EOM_PRI                   (7U)
#define PGN060416_ABT_CTRL                  (255U)
#define PGN060416_ABT_PRI                   (7U)
#define PGN060416_BAM_CTRL                  (32U)
#define PGN060416_BAM_PRI                   (7U)

#define PGN065240_NUM                       (65240UL)
#define PGN065240_PRI                       (6UL)
#define PGN065240_MULTI_FRAME_NUM           (2U)

#define PGN065285_NUM                       (65285UL)
#define PGN065285_PRI                       (7UL)
#define PGN065285_BOOTSTAT_RUN_BOOTLOADER   (1UL)

#define PGN065286_NUM                       (65286UL)

#define PGN065288_NUM                       (65288UL)
#define PGN065288_PRI                       (7UL)

#define PGN126208_NUM                                   (126208UL)
#define PGN126208_PRI                                   (3U)
#define PGN126208_REQ_CODE                              (0U)
#define PGN126208_CMD_CODE                              (1U)
#define PGN126208_ACK_CODE                              (2U)
#define PGN126208_REQ_PGN_POS                           (1U)
#define PGN126208_REQ_TX_INTVAL_POS                     (4U)
#define PGN126208_REQ_TX_INTVAL_DONOTCHANGE             (0xFFFFFFFF)
#define PGN126208_REQ_TX_INTVAL_RESTORE                 (0xFFFFFFFE)
#define PGN126208_REQ_TX_INTVAL_TURNOFFTX               (0x00000000)
#define PGN126208_REQ_TX_INTVAL_OFFSET_POS              (8U)
#define PGN126208_REQ_TX_INTVAL_OFFSET_DONOTCHANGE      (0xFFFF)
#define PGN126208_REQ_TX_INTVAL_OFFSET_TXNOW            (0x0000)
#define PGN126208_REQ_PARAM_TOTAL_POS                   (10U)
#define PGN126208_REQ_PARAM_BASE_POS                    (11U)

#define PGN126208_ACK_LEN_BASE                          (6U)

#define PGN126208_ACK_ERRORCODE_NO_ERROR_ACK                       0x0
#define PGN126208_ACK_ERRORCODE_PGN_NOT_SUPPORT                    0x1
#define PGN126208_ACK_ERRORCODE_PGN_TEMPORARILY_NOT_AVAILABLE      0x2
#define PGN126208_ACK_ERRORCODE_ACCESS_DENIED                      0x3
#define PGN126208_ACK_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED          0x4
#define PGN126208_ACK_ERRORCODE_DEFINER_TAG_NOT_SUPPORTED          0x5
#define PGN126208_ACK_ERRORCODE_READ_WRITE_NOT_SUPPORTED           0x6

#define PGN126208_TX_ACK_ERRORCODE_NO_ERROR_ACK                    0x0
#define PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_PRIORITY_NOT_SUP    0x1
#define PGN126208_TX_ACK_ERRORCODE_TX_INTERVAL_CALC_ERROR          0x2
#define PGN126208_TX_ACK_ERRORCODE_ACCESS_DENIED                   0x3
#define PGN126208_TX_ACK_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED       0x4

#define PGN126208_PARAM_ERRORCODE_NO_ERROR_ACK                     0x0
#define PGN126208_PARAM_ERRORCODE_INVALID_REQUEST_CMD_PARAM_FLD    0x1
#define PGN126208_PARAM_ERRORCODE_TEMPORARILY_AVAILABLE_COMPLY     0x2
#define PGN126208_PARAM_ERRORCODE_OUT_OF_RANGE_PARAM               0x3
#define PGN126208_PARAM_ERRORCODE_ACCESS_DENIED                    0x4
#define PGN126208_PARAM_ERRORCODE_REQUEST_CMD_NOT_SUPPORTED        0x5
#define PGN126208_PARAM_ERRORCODE_READ_WRITE_NOT_SUPPORTED         0x6

#define PGN126720_NUM                       (126720UL)
#define PGN126720_PRI                       (7UL)
#define PGN126720_PID_MASTER_RESET              1
#define PGN126720_PID_BOOTLDR_LAUNCH            2
#define PGN126720_PID_APP_LAUNCH                3
#define PGN126720_PID_STARTUP_MON_TIMEOUT       4
#define PGN126720_PID_BOOTLDR_VER               5
#define PGN126720_PID_BOOTLDR_STAT              6
#define PGN126720_PID_BOOTLDR_CMD               10
#define PGN126720_PID_BOOTLDR_DAT               11

#define PGN126720_05_PROP_ID                (5U)
#define PGN126720_05_PROP_ID_IDX            (4U)
#define PGN126720_05_LEN                    (10U)
#define PGN126720_05_PRODUCTCODE                    (10571U) //Only this number work well.
#define PGN126720_05_PROCESSORCODE_MASTER           (1U)
#define PGN126720_05_PROCESSORCODE_SLAVE            (2U)
#define PGN126720_05_BOOTVER                        (0001UL)
#define PGN126720_05_APPVER                         (0001UL)

#define PGN126720_06_LEN                    (6U)
#define PGN126720_06_PROCESSCODE_MASTER         PGN126720_05_PROCESSORCODE_MASTER
#define PGN126720_06_PROCESSCODE_SLAVE          PGN126720_05_PROCESSORCODE_SLAVE

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

#define PGN126720_CMD_0LOCK_FLASH                       (0U)
#define PGN126720_CMD_1UNLOCK_FLASH                     (1U)
#define PGN126720_CMD_2ERASE_FLASH                      (2U)
#define PGN126720_CMD_3READY_FLASH                      (3U)
#define PGN126720_CMD_4VERIFY_FLASH                     (4U)
#define PGN126720_CMD_5ABORT_FLASH                      (5U)
#define PGN126720_CMD_6CHANGE_PROCESSOR                 (6U)
#define PGN126720_CMD_254OUTOFRANGE                     (254U)
#define PGN126720_CMD_255DONTCHANGE                     (255U)

#define PGN126720_11DAT_SREC_BASE_PTR                   (8U)
#define PGN126720_11DAT_TYP_0START_FLASH                 (0U)
#define PGN126720_11DAT_TYP_3WRITE_FLASH                 (3U)
#define PGN126720_11DAT_TYP_5COUNT_FLASH                 (5U)
#define PGN126720_11DAT_TYP_7END_FLASH                   (7U)
#define PGN126720_11DAT_TYP_SIZE                        (1U)
#define PGN126720_11DAT_LEN_SIZE                        (2U)
#define PGN126720_11DAT_ADR_SIZE                        (8U)
#define PGN126720_11DAT_CHKSUM_SIZE                     (2U)
#define PGN126720_11DAT_DAT_MAX                         (32U)

#define PGN126993_NUM                           (126993UL)
#define PGN126993_PRI                           (7UL)
#define PGN126993_UPDATE_RATE                   (60000ULL)
#define PGN126993_CAN_STATUS_1                  (0ULL)
#define PGN126993_CAN_STATUS_2                  (0ULL)
#define PGN126993_EQUIP_STATUS                  (0ULL)

#define PGN126996_NUM                           (126996UL)
#define PGN126996_PRI                           (6U)
#define PGN126996_LEN                           (134U)
#define PGN126996_NMEA_MFG_PROD_CODE_POS        (2U)

#define PGN127502_NUM                       (127502UL)
#define PGN127502_PRI                       (3UL)

#define BROADCAST_DEST_ADDR                     (255U)
#define NMEA2K_THIS_ADDR                        (110U)

#pragma pack(push,1)
typedef struct __fastpacket
{
    uint8_t  status;
    uint8_t  seqid;
    uint32_t pgn;
    uint8_t  src_addr;
    uint8_t  des_addr;
    uint8_t  dat[FASTPACKET_LEN];
    uint8_t  len;
    uint32_t timestamp;
} fastpacket ;

typedef struct _multipacket
{
    uint8_t  status;
    uint16_t TotalByteLen;
    uint8_t  TotalFrmNum;
    uint8_t  CurrFrmNum;
    uint32_t pgn;
    uint8_t  SA;
    uint8_t  DA;
    uint32_t Time;
    uint8_t  dat[MULTIPACKET_LEN];
} multipacket;

typedef struct __version_dat
{
    uint16_t  boot_ver;
    uint16_t  app_ver;
} version_dat ;

typedef struct __pgn060928_dat
{
    uint32_t NMEA2KNum;                    // 21 bits
    uint16_t MfgCode;                      // 11 bits
    uint8_t  DevInstance;                  // 3  bits
    uint8_t  DevFunc;                      // 8  bits
    uint8_t  DevClass;                     // 7  bits
    uint8_t  SysInstance;                  // 4  bits
    uint8_t  IndustryGrp;                  // 3  bits
    uint8_t  ISOSelfConfig;                // 1  bits
} pgn060928_dat;

typedef struct __pgn126208_dat
{
    uint8_t GrpFuncCode;                // 8  bits
} pgn126208_dat;

typedef struct __pgn126208_req_dat
{
    uint32_t ReqPGN;                    // 24 bits
//    uint32_t TxInterval;                // 32 bits
//    uint16_t TxIntervalOffset;          // 16 bits
    uint8_t  NumPairReqParamFollow;     // 8  bits
} pgn126208_req_dat;

typedef struct __pgn126208_cmd_dat
{
    uint32_t CmdPGN;                    // 24 bits
    uint8_t  PriSetting;                // 4  bits
    uint8_t  NumPairCmdParamFollow;     // 8  bits
    uint8_t  FieldNumFirstCmdParam;     // 8  bits
    uint8_t  VarFirstCmdParam;          // 8  bits
    uint8_t  VarNumFieldNum6Repeated;   // 8  bits
    uint8_t  VarNumFieldNum7Repeated;   // 8  bits
} pgn126208_cmd_dat;

typedef struct __pgn126208_ack_dat
{
    uint32_t pgn_req;                   // 24 bits
    uint8_t  pgn_err;                   // 4  bits
    uint8_t  tx_intval_err;             // 4  bits
    uint8_t  param_len;                 // 8  bits
    uint8_t  param_err[7];              // 8*7 bit array.
} pgn126208_ack_dat;

typedef struct _pgn126996_dat
{
    uint16_t NMEANetMsgDbVer;
    uint16_t NMEAMfgProdCode;
    uint8_t  MfgModelId[32];
    uint8_t  MfgSWVerCode[32];
    uint8_t  MfgModelVer[32];
    uint8_t  MfgModelSerCode[32];
    uint8_t  NMEA2KCertLvl;
    uint8_t  LoadEq;
} pgn126996_dat ;
#pragma pack(pop)

#define FASTDAT_LEN_TRUNC(LEN) ((LEN+1)%7 ? ((LEN+1)/7)*7 + 7 : ((LEN+1)/7)*7)

uint8_t  getRxPF(uint32_t canid);
uint8_t  getRxPS(uint32_t canid);
uint8_t  getRxSA(uint32_t canid);
uint32_t getRxPGN(uint32_t canid);
bool     IsKnownPGN(uint32_t PGN);

void NMEA2KProc(RxProtocol rxpacket);
void NMEA2KInit(void);

int32_t Pgn126993HeartBeat(void);

extern uint8_t g_lcd_bl;
extern uint8_t g_bzr_vol;
extern uint8_t g_switch_bank[6];
extern uint8_t g_lcd_img_idx;

#endif /* APPLICATION_USER_ECOTRONIX_INC_NMEA2K_H_ */

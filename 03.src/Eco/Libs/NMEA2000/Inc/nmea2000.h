/*
 * nmea2000.h
 *
 *  Created on: Feb 3, 2022
 *      Author: eco_kj
 */

#ifndef NMEA2000_INC_NMEA2000_H_
#define NMEA2000_INC_NMEA2000_H_

//==============================================================================
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private struct ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
//==============================================================================

/* Includes ------------------------------------------------------------------*/
#include "eco.h"
#include <gui/common/DataBase.hpp>

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_NAME_FEILD_SIZE				8
#define BROADCAST_DESTINATION_ADDR		255

#define _OFFICE_NAME_                       "2023 by ECOTRONIX Corp."
#define _MODEL_ID_VERTICAL_                 "FI-70x"
#define _MODEL_ID_HORIZONTAL_               "FI-70H"
#define _MODEL_ID_                          _MODEL_ID_VERTICAL_
#define _HW_VERSION_                        30
#define _BOOT_VERSION_                      94
#define _APP_VERSION_                       95

#define MFG_CODE_AIRMAR                     (135)
#define MFG_CODE_FURUNO                     (1855)

#define PPGN_AIRMAR_MFGCODE                 ((0x04 << 13) | (0x3 << 11) | (MFG_CODE_AIRMAR << 0))  // 0x9887
#define PPGN_FURUNO_MFGCODE                 ((0x04 << 13) | (0x3 << 11) | (MFG_CODE_FURUNO << 0))  // 0x9F3F

typedef struct __PGNCounter
{
	uint32_t PGN;
	uint32_t value;
} PGNCounter;

extern uint8_t g_access_level; // temp.

/* Exported variables --------------------------------------------------------*/
extern uint32_t MAX_HIGH_SOURCE_ADDR;
extern uint32_t ADDRESS_CLAIM_FAIL_ADDR;
extern uint32_t localSourceAddr;
extern uint32_t savedSourceAddr;

extern uint32_t mDataLastReceivedTime;

#define CAN_RX_BUF_MAX     128
#define CAN_TX_BUF_MAX     3

typedef struct _NmeaPgn_
{
  uint32_t mCanNumericID;

  uint32_t mPriority;
  uint32_t mEDP;
  uint32_t mPGN;
  uint32_t mDP;
  uint32_t mPF;
  uint32_t mPS;
  uint32_t mSA;
} NmeaPgn;

typedef struct __RxProtocol
{
    uint32_t canid;
    uint8_t  dat[8];
    uint8_t  len;
} RxProtocol ;

/*typedef struct __TxProtocol
{
    uint32_t canid;
    uint8_t  dat[8];
    uint8_t  len;
} TxProtocol ;*/

typedef struct __TxProtocol
{
  CAN_TxHeaderTypeDef TxHeader;
  uint8_t TxData[8];
  uint32_t TxMailbox;
} TxProtocol;

/*
typedef struct __RxProtocol
{
  CAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[8];
} RxProtocol;*/

extern uint8_t g_switch_bank[6];
extern uint8_t g_lcd_img_idx;

extern RxProtocol g_RxCan[CAN_RX_BUF_MAX];   // Rx array

extern uint16_t  rxCanLastIndex;
extern uint16_t  rxCanFirstIndex;

extern TxProtocol g_TxCan[CAN_TX_BUF_MAX];	// Tx array

extern uint8_t txCanBufferCount;

/* Exported functions --------------------------------------------------------*/
void NMEA2000_Open(void);
void NMEA2000_126993_heartbeat(void);

void NMEA2000_SendParseMessages(NmeaPgn* pngId, uint32_t len, uint8_t *buf, uint8_t isFastPacket);
void SendNonSingleFrame(NmeaPgn* pngId, uint32_t len, uint8_t *buf, uint32_t messagetype);
void NMEA2000_ReceiveParseMessages(uint32_t canId, uint8_t *buf, uint8_t len);

void ProcessNMEA2000MultiPacket(uint32_t proc_pgn_number, NmeaPgn* pgnId, uint16_t size, uint8_t *buf, uint32_t messagetype);

#ifdef __cplusplus
}
#endif

#endif /* NMEA2000_INC_NMEA2000_H_ */

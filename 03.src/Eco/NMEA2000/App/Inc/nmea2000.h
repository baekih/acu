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

#include "nmea2000_deviceinfo.h"
#include "nmea2000_pgnbase.h"
#include "nmea2000_namebase.h"

#include "multipacketdata.h"
#include "fastpacketdata.h"

#include "pgn_059904.h"
#include "pgn_060160.h"
#include "pgn_060416.h"
#include "pgn_060416_rts.h"
#include "pgn_060416_bam.h"
#include "pgn_060928.h"
#include "pgn_061184_stgf.h"
#include "pgn_065240.h"
#include "pgn_065285.h"
#include "pgn_065286.h"
#include "pgn_065287.h"
#include "pgn_126208.h"
#include "pgn_126993.h"
#include "pgn_126720.h"
#include "pgn_127245.h"
#include "pgn_127250.h"
#include "pgn_127258.h"
#include "pgn_128259.h"
#include "pgn_128267.h"
#include "pgn_129025.h"
#include "pgn_129026.h"
#include "pgn_129029.h"
#include "pgn_129283.h"
#include "pgn_129284.h"
#include "pgn_129285.h"
#include "pgn_130306.h"
#include "pgn_130310.h"
#include "pgn_130816.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_NAME_FEILD_SIZE                 8
#define BROADCAST_DESTINATION_ADDR          255

#define _OFFICE_NAME_                       "2025 by ECOTRONIX Corp."
#define _MODEL_ID_VERTICAL_                 "ECO-ACU"
#define _MODEL_ID_HORIZONTAL_               "ECO-ACU"
#define _MODEL_ID_                          _MODEL_ID_VERTICAL_

#define N2K_MFG_CODE_AIRMAR                 (135)
#define N2K_MFG_CODE_FURUNO                 (1855)

#define PPGN_AIRMAR_MFGCODE                 ((0x04 << 13) | (0x3 << 11) | (N2K_MFG_CODE_AIRMAR << 0))  // 0x9887
#define PPGN_FURUNO_MFGCODE                 ((0x04 << 13) | (0x3 << 11) | (N2K_MFG_CODE_FURUNO << 0))  // 0x9F3F

#define CAN_BUF_RX_MAX                      256
#define CAN_BUF_TX_MAX                      3

#define N2K_TX_DELAY_MS                     1

#define N2K_UNIQUE_NUMBER                   1048577
#define N2K_DEVICE_INSTANCE                 1
#define N2K_SYSTEM_INSTANCE                 1
#define N2K_DEVICE_CLASS                    40      //40:rudder control
#define N2K_DEVICE_FUNCTION                 150     //150:auto rudder
#define N2K_INDUSTRY_GROUP                  4       //4:marine
#define N2K_ISO_SELF_CONFIG                 1       //1:reserved

#define N2K_ADDR_DEFAULT                    110
#define N2K_ADDR_MAX_HIGH_SOURCE            252
#define N2K_ADDR_CLAIM_FAIL                 254

#define N2K_DATA_NOT_AVAILABLE_INT16        32767
#define N2K_DO_NOT_CHANGE_INT16             32766
#define N2K_OUT_OF_ORDER_INT16              32765

/* Private function prototypes -----------------------------------------------*/
typedef struct __RxProtocol
{
    uint32_t canid;
    uint8_t  dat[8];
    uint8_t  len;
} RxProtocol ;

typedef struct __TxProtocol
{
    CAN_TxHeaderTypeDef TxHeader;
    uint8_t TxData[8];
    uint32_t TxMailbox;
} TxProtocol;

typedef struct __CANBuffer
{
    RxProtocol rx[CAN_BUF_RX_MAX];
    uint16_t   rx_idx_head;
    uint16_t   rx_idx_tail;
    TxProtocol tx[CAN_BUF_TX_MAX];
    uint8_t    tx_idx;
} CANBuffer;

/* Exported variables --------------------------------------------------------*/
extern uint8_t g_access_level; // temp.
extern CANBuffer g_canbuf;
extern uint32_t g_n2k_addr_local;
extern uint32_t g_n2k_addr_saved;
extern bool  g_n2k_is_addr_claiming;
extern uint32_t g_n2k_last_addr_claim_time;

extern uint8_t g_switch_bank[6];
extern uint8_t g_lcd_img_idx;
extern rudder g_rudder;

/* Exported functions --------------------------------------------------------*/
void NMEA2000_Open(void);
void NMEA2000_126993_heartbeat(void);

void NMEA2000_SendParseMessages(NmeaPgn* pngId, uint32_t len, uint8_t *buf, uint8_t isFastPacket);
void ProcessNMEA2000MultiPacket(uint32_t proc_pgn_number, NmeaPgn* pgnId, uint16_t size, uint8_t *buf, uint32_t messagetype);

void SendNonSingleFrame(NmeaPgn* pngId, uint32_t len, uint8_t *buf, uint32_t messagetype);

void runN2KCANRXBuffer(void);
void chkN2KLastAddrClaimTime(void);


#ifdef __cplusplus
}
#endif

#endif /* NMEA2000_INC_NMEA2000_H_ */

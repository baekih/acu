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

#define CAN_RX_BUF_MAX     128
#define CAN_TX_BUF_MAX     3

typedef struct __PGNCounter
{
	uint32_t PGN;
	uint32_t value;
} PGNCounter;

/* Exported variables --------------------------------------------------------*/
extern uint32_t BROADCAST_DESTINATION_ADDR;
extern uint32_t MAX_HIGH_SOURCE_ADDR;
extern uint32_t ADDRESS_CLAIM_FAIL_ADDR;
extern uint32_t localSourceAddr;

extern uint32_t mDataLastReceivedTime;

extern RxProtocol g_RxCan[CAN_RX_BUF_MAX];   // Rx array

extern uint16_t  rxCanLastIndex;
extern uint16_t  rxCanFirstIndex;

extern TxProtocol g_TxCan[CAN_TX_BUF_MAX];  // Tx array

extern uint8_t txCanBufferCount;

/* Exported functions --------------------------------------------------------*/
void NMEA2000_Open(void);

void NMEA2000_SendParseMessages(uint32_t canid, uint32_t len, uint8_t *buf, uint8_t isFastPacket);
void SendNonSingleFrame(uint32_t canid, uint8_t len, uint8_t *buf, uint32_t messagetype);
void NMEA2000_ReceiveParseMessages(uint32_t canid, uint8_t len, uint8_t *buf);


#endif /* NMEA2000_INC_NMEA2000_H_ */

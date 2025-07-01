/*
 * pgn_126993.h
 *
 *  Created on: 2022. 7. 19.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_126993_H_
#define NMEA2000_INC_PGN_PGN_126993_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN126993_PGN                                   126993
#define PGN126993_PRIORITY                              7

#define PGN126993_FIELD_MIN                             5
#define PGN126993_FIELD_MAX                             5
#define PGN126993_UPDATE_RATE                           60000
#define PGN126993_HEARTBEAT_CNT_MAX                     252
#define PGN126993_CAN_CTLR_STATE_CLASS1_ERR_ACTIVE      0x0
#define PGN126993_CAN_CTLR_STATE_CLASS1_ERR_PASSIVE     0x1
#define PGN126993_CAN_CTLR_STATE_CLASS1_BUS_OFF         0x2
#define PGN126993_CAN_CTLR_STATE_CLASS1_NOT_AVAILABLE   0x3
#define PGN126993_CAN_CTLR_STATE_CLASS2_DAT             0x3
#define PGN126993_EQUIP_STATUS_OPERAIONAL               0x0
#define PGN126993_EQUIP_STATUS_FAULT                    0x1
#define PGN126993_EQUIP_STATUS_RESERVED                 0x2
#define PGN126993_EQUIP_STATUS_NOT_AVAILABLE            0x3

/* Private typedef -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
uint32_t PGN126993_ProcessNameField(uint8_t, uint8_t, uint8_t);


#endif /* NMEA2000_INC_PGN_PGN_126993_H_ */

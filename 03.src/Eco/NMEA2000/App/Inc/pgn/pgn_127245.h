/*
 * pgn_127245.h
 *
 *  Created on: 2025. 5. 7.
 *      Author: yhk
 */

#ifndef NMEA2000_INC_PGN_PGN_127245_H_
#define NMEA2000_INC_PGN_PGN_127245_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN127245_PRIORITY                  2
#define PGN127245_PGN                       127245

/* Private typedef -----------------------------------------------------------*/
typedef struct __rudder
{
    uint8_t instance;
    uint8_t direction_order;
    int16_t angle_order;
    int16_t position;
} rudder;

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN127245_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN127245_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_127245_H_ */

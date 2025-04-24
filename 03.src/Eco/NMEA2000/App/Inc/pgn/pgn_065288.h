/*
 * pgn_065288.h
 *
 *  Created on: 2022. 7. 5.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_065288_H_
#define NMEA2000_INC_PGN_PGN_065288_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN065288_PGN                     65288
#define PGN065288_PRIORITY                7
#define PGN065288_FIELD_MIN               3
#define PGN065288_FIELD_MAX               9
#define PGN065288_BRIGHT_INSTANCE         0
#define PGN065288_BRIGHT_SOURCE           1
#define PGN065288_BRIGHT_COLOR_PALETTE    15

/* Private typedef -----------------------------------------------------------*/
typedef struct __PGN065288NAME
{
    uint16_t  m65288Manufacturer_Code;      // 11 bits
    uint8_t   m65288NMEA_Reserved1;         // 2  bits
    uint8_t   m65288Industry_Group;         // 3  bits
    uint8_t   m65288Brightness_Instance;    // 8  bits
    uint8_t   m65288Brightness_Source;      // 4  bits
    uint8_t   m65288Brightness_Preset;      // 4  bits
    uint8_t   m65288Brightness_Percentage;  // 8  bits
    uint8_t   m65288Color_palette;          // 4  bits
    uint32_t  m65288Reserved;               // 20 bits
} PGN65288NAME;

/* Exported variables --------------------------------------------------------*/
extern uint8_t pgn65288_datalen[];
/* Exported functions --------------------------------------------------------*/
uint32_t PGN065288_ProcessNameField(uint8_t, uint8_t);


#endif /* NMEA2000_INC_PGN_PGN_065288_H_ */

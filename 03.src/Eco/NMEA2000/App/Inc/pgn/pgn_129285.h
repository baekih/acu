/*
 * pgn_129285.h
 *
 *  Created on: 2025. 4. 25.
 *      Author: ihbaek
 */

#ifndef NMEA2000_INC_PGN_PGN_129285_H_
#define NMEA2000_INC_PGN_PGN_129285_H_


/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define PGN129285_PGN                       129285
#define PGN129285_PRIORITY                  6

/* Private typedef -----------------------------------------------------------*/
#pragma pack(push,1)
typedef struct __route
{
    uint16_t id;
    uint8_t  name[31];
    uint8_t  name_len;
    uint8_t  name_code;
} s_route;

typedef struct __waypoint
{
    uint16_t id;
    uint8_t  name[31];
    uint8_t  name_len;
    uint8_t  name_code;
    int32_t  latitude;
    int32_t  longitude;
} s_waypoint;
#pragma pack(pop)

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void PGN129285_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf);
void PGN129285_ProcessNameField(NmeaPgn* pgnId);


#endif /* NMEA2000_INC_PGN_PGN_129284_H_ */

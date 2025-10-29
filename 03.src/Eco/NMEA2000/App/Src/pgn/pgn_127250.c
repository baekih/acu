/*
 * pgn_127250.c
 *
 *  Created on: 2024. 1. 24.
 *      Author: yhk
 */

/* Includes ------------------------------------------------------------------*/
#include "nmea2000.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void PGN127250_GetFieldValue(NmeaPgn* pgnId, uint8_t len, uint8_t *buf)
{
//    uint8_t     SequenceID             =  GetBuf_1ByteUInt(len, 0, buf);                    // 8  bits
      g_boat.heading_sensor_reading = GetBuf_2ByteUInt(len, 1, buf);                      // 16 bits
//    uint16_t    Deviation              =  GetBuf_2ByteUInt(len, 3, buf);                    // 16 bits
//    uint16_t    Variation              =  GetBuf_2ByteUInt(len, 5, buf);                    // 16 bits
//    uint8_t     HeadingSensorReference = (GetBuf_1ByteUInt(len, 7, buf) & 0x03);            // 2  bits
//    uint8_t     NMEAReserved           = (GetBuf_1ByteUInt(len, 7, buf) & 0xC0) >> 6;       // 6  bits

      // calculate heading true
      if(isHeadingValid(g_boat.heading_sensor_reading))
      {
          int32_t heading_true = (int32_t)g_boat.heading_sensor_reading + (int32_t)g_boat.heading_variation;

          if(360 < roundRADem4toDEG((float)heading_true))
          {
              g_boat.heading_true = (uint16_t)(heading_true - 360);
          }
          else if(roundRADem4toDEG(heading_true) < 0)
          {
              g_boat.heading_true = (uint16_t)(heading_true + 360);
          }
          else
          {
              g_boat.heading_true = (uint16_t)heading_true;
          }
      }
      else
      {
          g_boat.heading_true = N2K_DATA_NOT_AVAILABLE_UINT16;
      }

}

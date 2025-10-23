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
      g_boat.heading_sensor_reading_em4 = GetBuf_2ByteUInt(len, 1, buf);                      // 16 bits
//    uint16_t    Deviation              =  GetBuf_2ByteUInt(len, 3, buf);                    // 16 bits
//    uint16_t    Variation              =  GetBuf_2ByteUInt(len, 5, buf);                    // 16 bits
//    uint8_t     HeadingSensorReference = (GetBuf_1ByteUInt(len, 7, buf) & 0x03);            // 2  bits
//    uint8_t     NMEAReserved           = (GetBuf_1ByteUInt(len, 7, buf) & 0xC0) >> 6;       // 6  bits

#if 0 //test only
    if(140*174 <= g_ship.curr.heading_sensor_reading) g_ship.curr.heading_sensor_reading += -140*174;
    else g_ship.curr.heading_sensor_reading += (360-140)*174;
//    if(320*174 <= g_ship.curr.heading_sensor_reading) g_ship.curr.heading_sensor_reading += -320*174;
//    else g_ship.curr.heading_sensor_reading += (360-320)*174;
    if(360*174 <= g_ship.curr.heading_sensor_reading) g_ship.curr.heading_sensor_reading -= 360*174;
#endif
/*
    printf("(%d)", pgnId->mSA);
    printf("Receive Vessel Heading - %3d,", SequenceID);
    printf("mHeadingSensorReading(%1.4f),", (float)HeadingSensorReading/10000);
    printf("mVariation(%1.4f),", (float)Deviation/10000);
    printf("mDeviation(%1.4f),", (float)Variation/10000);
    printf("mHeadingSensorReference(%d)\n", HeadingSensorReference);
    printf("mHeadingSensorReading : %3.1f,", (((float)HeadingSensorReading/10000) / (2*M_PI)) * 360);
    printf("mDeviation : %3.1f,", (((float)Deviation/10000) / M_PI) * 180);
    printf("mVariation : %3.1f\n", (((float)Variation/10000) / M_PI) * 180);
*/
}

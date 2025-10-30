/*
 * app.h
 *
 *  Created on: Jan 29, 2024
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_APP_H_
#define APPLICATION_USER_ECOTRONIX_INC_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* define --------------------------------------------------------------------*/
#define VERSION     "0.91"

/* struct --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
typedef struct __rudder
{
    uint8_t instance;
    uint8_t direction_order;
    int16_t angle_order;
    int16_t position;
} rudder;

typedef struct __ship_speed
{
    uint16_t through_water;
    uint16_t over_ground;
    uint8_t  water_reference_type;
    uint8_t direction;
} ship_speed;

typedef struct __ship_course
{
    uint8_t cog_reference;
    uint16_t over_ground;
} ship_course;

typedef struct __ship_position
{
    int32_t latitude;
    int32_t longitude;
    uint8_t gnss_method;
} ship_position;

typedef struct __ship_xte
{
    uint8_t mode;
    int32_t val;
} ship_xte;

typedef struct __ship_wind
{
    uint16_t speed;
    uint16_t direction;
    uint8_t reference;
} ship_wind;

typedef struct __ship_rudder
{
    int16_t cur;
    int16_t tgt;
} ship_rudder;

typedef struct __ship_param
{
    ship_speed speed;
    ship_course course;
    ship_position position;
    ship_xte xte;
    ship_wind wind;
    uint32_t water_depth;
    uint16_t transducer_offset;
} ship_param;

typedef struct __ship_status
{
    ship_param curr;
    ship_param prev;
} ship_status;

typedef struct _boat_status
{
    uint16_t heading_sensor_reading;
    uint16_t heading_target;
    int16_t  heading_variation;
    uint16_t heading_true;
    int32_t  rate_of_turn;
    uint16_t speed_through_water;
    uint16_t speed_over_ground;
    uint8_t  rudder_instance;
    uint8_t  rudder_direction_order;
    int16_t  rudder_angle_order;
    int16_t  rudder_position;
} boat_status;

/* macro ---------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
extern rudder g_rudder;
extern ship_status g_ship;
extern boat_status g_boat;

/* function prototypes -------------------------------------------------------*/
int32_t roundRADem4toDEG(float val);
int32_t roundDEGtoRADem4(float val);
bool isRADem4Valid(int32_t val);
float roundRotRad2Deg(int32_t val);
float roundRudderRad2Deg(int16_t val);
bool isHeadingValid(uint16_t val);
bool isRotValid(int32_t val);
bool isRudderValid(int16_t val);
bool isVariationValid(int16_t val);

void runEcoTaskDefault(void *argument);
void runEcoTaskUART(void *argument);
void runEcoTaskNMEA2KRx(void *argument);
void runEcoTaskNMEA2KTx(void *argument);
void touchgfx_taskEntry(void);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATION_USER_ECOTRONIX_INC_APP_H_ */

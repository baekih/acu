/*
 * control.h
 *
 *  Created on: Jul 22, 2025
 *      Author: ihbaek
 */

#ifndef APPLICATION_USER_ECOTRONIX_INC_CONTROL_H_
#define APPLICATION_USER_ECOTRONIX_INC_CONTROL_H_

/* Includes ------------------------------------------------------------------*/

/* define --------------------------------------------------------------------*/
#define NB              (-3)
#define NM              (-2)
#define NS              (-1)
#define ZO              (0)
#define PS              (1)
#define PM              (2)
#define PB              (3)

#define FUZZY_GAIN_HDG  (1.0)
#define FUZZY_GAIN_ROT  (1.0)
#define RUD_GAIN        (1.0)
#define RUD_ORDER_MAX   (20.0)
#define RUD_ORDER_MIN   (-20.0)

#define K_H             (5.0)
#define K_R             (1.0)
#define RUD_MAX         (15.0)

/* struct --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
typedef struct __fuzzy_hdg
{
    float a;
    float b;
} fuzzy_hdg;

typedef struct __fuzzy_rot
{
    float a;
    float b;
} fuzzy_rot;

typedef struct __fuzzy_hdg_err_rule
{
    int a;
    int b;
} fuzzy_hdg_err_rule;

typedef struct __fuzzy_rot_err_rule
{
    int a;
    int b;
} fuzzy_rot_err_rule;

typedef struct __fuzzy_rule
{
    float a;
    float b;
} fuzzy_rule;

typedef struct __fuzzy_var
{
    fuzzy_hdg hdg;
    fuzzy_rot rot;
    fuzzy_hdg_err_rule hdg_err_rule;
    fuzzy_rot_err_rule rot_err_rule;
    fuzzy_rule rule;
} fuzzy_var;

typedef struct __fuzzy_control
{
    float rud_order;
} fuzzy_control;

/* macro ---------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
extern fuzzy_control g_fuzzy_control;

/* function prototypes -------------------------------------------------------*/
float calFuzzy(float hdg_err, float rot_err);
float calPI(float hdg_err, float rot_err);

#endif /* APPLICATION_USER_ECOTRONIX_INC_CONTROL_H_ */

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
#define NB          (-3)
#define NM          (-2)
#define NS          (-1)
#define ZO          (0)
#define PS          (1)
#define PM          (2)
#define PB          (3)

/* struct --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
typedef struct __fuzzy_y
{
    float a;
    float b;
} fuzzy_y;

typedef struct __fuzzy_dy
{
    float a;
    float b;
} fuzzy_dy;

typedef struct __fuzzy_err_rule
{
    int a;
    int b;
} fuzzy_err_rule;

typedef struct __fuzzy_derr_rule
{
    int a;
    int b;
} fuzzy_derr_rule;

typedef struct __fuzzy_rule
{
    float a;
    float b;
} fuzzy_rule;

typedef struct __fuzzy_var
{
    fuzzy_y y;
    fuzzy_dy dy;
    fuzzy_err_rule erule;
    fuzzy_derr_rule derule;
    fuzzy_rule rule;
} fuzzy_var;

typedef struct __fuzzy_control
{
    float gain_hdg;
    float gain_d_hdg;
} fuzzy_control;

/* macro ---------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
extern fuzzy_control g_fuzzy_control;

/* function prototypes -------------------------------------------------------*/
float calFuzzy(float err, float d_err);

#endif /* APPLICATION_USER_ECOTRONIX_INC_CONTROL_H_ */

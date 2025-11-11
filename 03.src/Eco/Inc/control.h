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

#define FUZZY_GAIN_HDG      (1.0)
#define FUZZY_GAIN_ROT      (1.0)
#define RUD_FUZZY_GAIN      (1.0)
//#define RUD_FUZZY_HDG_ADJ   (0.2)
#define RUD_FUZZY_HDG_ADJ   (0.1)
//#define RUD_FUZZY_HDG_ADJ   (0.05)
#define RUD_FUZZY_ROT_ADJ   (0.2)
#define RUD_ORDER_MAX       (20.0)
#define RUD_ORDER_MIN       (-20.0)

#define DEG2RAD         (M_PI/180.0)
#define RAD2DEG         (180.0/M_PI)

#define K_HDG           (0.5)
#define K_ROT           (0.2)
#define RUD_MAX         (15.0)

/* struct --------------------------------------------------------------------*/

/* typedef -------------------------------------------------------------------*/
typedef struct _fuzzy_hdg
{
    float a;
    float b;
} fuzzy_hdg;

typedef struct _fuzzy_rot
{
    float a;
    float b;
} fuzzy_rot;

typedef struct _fuzzy_hdg_err_rule
{
    int a;
    int b;
} fuzzy_hdg_err_rule;

typedef struct _fuzzy_rot_err_rule
{
    int a;
    int b;
} fuzzy_rot_err_rule;

typedef struct _fuzzy_rule
{
    float aa;
    float ab;
    float ba;
    float bb;
} fuzzy_rule;

typedef struct _fuzzy_weight
{
    float aa;
    float ab;
    float ba;
    float bb;
} fuzzy_weight;

typedef struct _fuzzy_val
{
    float aa;
    float ab;
    float ba;
    float bb;
} fuzzy_val;

typedef struct _fuzzy
{
    fuzzy_hdg hdg;
    fuzzy_rot rot;
    fuzzy_hdg_err_rule hdg_err_rule;
    fuzzy_rot_err_rule rot_err_rule;
    fuzzy_rule rule;
    fuzzy_weight weight;
    fuzzy_val val;
    float weight_tot;
} fuzzy;

typedef struct _control
{
    float rud_order;
} control;

/* macro ---------------------------------------------------------------------*/

/* variables -----------------------------------------------------------------*/
extern control g_control;

/* function prototypes -------------------------------------------------------*/
float calFuzzy(float hdg_err, float rot_err);
float calPID(float hdg_err, float rot_err);

#endif /* APPLICATION_USER_ECOTRONIX_INC_CONTROL_H_ */

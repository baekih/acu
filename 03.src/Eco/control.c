/*
 * control.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ihbaek
 */

#include "eco.h"

#define CTL_IHBAEK

fuzzy_var g_fuzzy_var;
fuzzy_control g_fuzzy_control;

void calHDGErrInference(float hdg_err)
{
    if(hdg_err < -5.0)
    {
        g_fuzzy_var.hdg.a = 1.0;
        g_fuzzy_var.hdg.b = 1.0;
        g_fuzzy_var.hdg_err_rule.a = NB;
        g_fuzzy_var.hdg_err_rule.b = NB;
    }
    else if(-5.0 <= hdg_err && hdg_err <  -3.0)
    {
        g_fuzzy_var.hdg.a =  0.5*hdg_err + 2.5;
        g_fuzzy_var.hdg.b = -0.5*hdg_err - 1.5;
        g_fuzzy_var.hdg_err_rule.a = NM;
        g_fuzzy_var.hdg_err_rule.b = NM;
    }
    else if(-3.0 <= hdg_err && hdg_err <  -1.0)
    {
        g_fuzzy_var.hdg.a =  0.5*hdg_err + 1.5;
        g_fuzzy_var.hdg.b = -0.5*hdg_err - 0.5;
        g_fuzzy_var.hdg_err_rule.a = NS;
        g_fuzzy_var.hdg_err_rule.b = NS;
    }
    else if(-1.0 <= hdg_err && hdg_err <  0.0)
    {
        g_fuzzy_var.hdg.a =  hdg_err + 1.0;
        g_fuzzy_var.hdg.b = -hdg_err;
        g_fuzzy_var.hdg_err_rule.a = ZO;
        g_fuzzy_var.hdg_err_rule.b = ZO;
    }
    else if( 0.0 <= hdg_err && hdg_err <=  1.0)
    {
        g_fuzzy_var.hdg.a =  hdg_err;
        g_fuzzy_var.hdg.b = -hdg_err + 1.0;
        g_fuzzy_var.hdg_err_rule.a = ZO;
        g_fuzzy_var.hdg_err_rule.b = ZO;
    }
    else if( 1.0 <  hdg_err && hdg_err <=  3.0)
    {
        g_fuzzy_var.hdg.a =  0.5*hdg_err - 0.5;
        g_fuzzy_var.hdg.b = -0.5*hdg_err + 1.5;
        g_fuzzy_var.hdg_err_rule.a = PS;
        g_fuzzy_var.hdg_err_rule.b = PS;
    }
    else if( 3.0 <  hdg_err && hdg_err <=  5.0)
    {
        g_fuzzy_var.hdg.a =  0.5*hdg_err - 1.5;
        g_fuzzy_var.hdg.b = -0.5*hdg_err + 2.5;
        g_fuzzy_var.hdg_err_rule.a = PM;
        g_fuzzy_var.hdg_err_rule.b = PM;
    }
    else if( 5.0 <  hdg_err)
    {
        g_fuzzy_var.hdg.a = 1.0;
        g_fuzzy_var.hdg.b = 1.0;
        g_fuzzy_var.hdg_err_rule.a = PB;
        g_fuzzy_var.hdg_err_rule.b = PB;
    }
//    printf("hdg_err[%f] rule[%d:%d]\r\n", hdg_err, g_fuzzy_var.hdg_err_rule.a, g_fuzzy_var.hdg_err_rule.b);
//    printf("hdg_err[%6.2f][%6.2f][%6.2f]\r\n", hdg_err, g_fuzzy_var.hdg.a, g_fuzzy_var.hdg.b);
}

void calROTErrInference(float rot_err)
{
    if(rot_err < -5.0)
    {
        g_fuzzy_var.rot.a = 1.0;
        g_fuzzy_var.rot.b = 1.0;
        g_fuzzy_var.rot_err_rule.a = NB;
        g_fuzzy_var.rot_err_rule.b = NB;
    }
    else if(-5.0 <= rot_err && rot_err <  -3.0)
    {
        g_fuzzy_var.rot.a =  0.5*rot_err + 2.5;
        g_fuzzy_var.rot.b = -0.5*rot_err - 1.5;
        g_fuzzy_var.rot_err_rule.a = NM;
        g_fuzzy_var.rot_err_rule.b = NM;
    }
    else if(-3.0 <= rot_err && rot_err <  -1.0)
    {
        g_fuzzy_var.rot.a =  0.5*rot_err + 1.5;
        g_fuzzy_var.rot.b = -0.5*rot_err - 0.5;
        g_fuzzy_var.rot_err_rule.a = NS;
        g_fuzzy_var.rot_err_rule.b = NS;
    }
    else if(-1.0 <= rot_err && rot_err <   0.0)
    {
        g_fuzzy_var.rot.a =  rot_err + 1.0;
        g_fuzzy_var.rot.b = -rot_err;
        g_fuzzy_var.rot_err_rule.a = ZO;
        g_fuzzy_var.rot_err_rule.b = ZO;
    }
    else if( 0.0 <= rot_err && rot_err <=  1.0)
    {
        g_fuzzy_var.rot.a =  rot_err;
        g_fuzzy_var.rot.b = -rot_err + 1.0;
        g_fuzzy_var.rot_err_rule.a = ZO;
        g_fuzzy_var.rot_err_rule.b = ZO;
    }
    else if( 1.0 <  rot_err && rot_err <=  3.0)
    {
        g_fuzzy_var.rot.a =  0.5*rot_err - 0.5;
        g_fuzzy_var.rot.b = -0.5*rot_err + 1.5;
        g_fuzzy_var.rot_err_rule.a = PS;
        g_fuzzy_var.rot_err_rule.b = PS;
    }
    else if( 3.0 <  rot_err && rot_err <=  5.0)
    {
        g_fuzzy_var.rot.a =  0.5*rot_err - 1.5;
        g_fuzzy_var.rot.b = -0.5*rot_err + 2.5;
        g_fuzzy_var.rot_err_rule.a = PM;
        g_fuzzy_var.rot_err_rule.b = PM;
    }
    else if( 5.0 <  rot_err)
    {
        g_fuzzy_var.rot.a = 1.0;
        g_fuzzy_var.rot.b = 1.0;
        g_fuzzy_var.rot_err_rule.a = PB;
        g_fuzzy_var.rot_err_rule.b = PB;
    }
//    printf("rot_err[%f] rule[%d:%d]\r\n", rot_err, g_fuzzy_var.rot_err_rule.a, g_fuzzy_var.rot_err_rule.b);
}

int calAfterInferenceRule(int hdg_err_rule, int rot_err_rule)
{
    switch(hdg_err_rule)
    {
    case NB:
        switch(rot_err_rule)
        {
        case NB: return NB;
        case NM: return NB;
        case NS: return NB;
        case ZO: return NB;
        case PS: return NM;
        case PM: return NS;
        case PB: return ZO;
        }
        break;
    case NM:
        switch(rot_err_rule)
        {
        case NB: return NB;
        case NM: return NB;
        case NS: return NB;
        case ZO: return NM;
        case PS: return NS;
        case PM: return ZO;
        case PB: return PS;
        }
        break;
    case NS:
        switch(rot_err_rule)
        {
        case NB: return NB;
        case NM: return NB;
        case NS: return NM;
        case ZO: return NS;
        case PS: return ZO;
        case PM: return PS;
        case PB: return PM;
        }
        break;
    case ZO:
        switch(rot_err_rule)
        {
        case NB: return NB;
        case NM: return NM;
        case NS: return NS;
        case ZO: return ZO;
        case PS: return PS;
        case PM: return PM;
        case PB: return PB;
        }
        break;
    case PS:
        switch(rot_err_rule)
        {
        case NB: return NM;
        case NM: return NS;
        case NS: return ZO;
        case ZO: return PS;
        case PS: return PM;
        case PM: return PB;
        case PB: return PB;
        }
        break;
    case PM:
        switch(rot_err_rule)
        {
        case NB: return NS;
        case NM: return ZO;
        case NS: return PS;
        case ZO: return PM;
        case PS: return PB;
        case PM: return PB;
        case PB: return PB;
        }
        break;
    case PB:
        switch(rot_err_rule)
        {
        case NB: return ZO;
        case NM: return PS;
        case NS: return PM;
        case ZO: return PB;
        case PS: return PB;
        case PM: return PB;
        case PB: return PB;
        }
        break;
    }

    return 0; //never reach. avoid warning only.
}

float convertRule2Val(int rule)
{
    switch(rule)
    {
    case NB: return -15.0;
    case NM: return -10.0;
    case NS: return  -5.0;
    case ZO: return   0.0;
    case PS: return   5.0;
    case PM: return  10.0;
    case PB: return  15.0;
    }

    return 0.0; //never reach. avoid warning only.
}

float calPI(float hdg_err, float rot_err)  //PID control
{
    float rud_val = K_H * hdg_err + K_R * rot_err;

    if(fabs(rud_val) > RUD_MAX) rud_val = copysignf(RUD_MAX, rud_val);

    return rud_val;
}

float calFuzzy(float hdg_err, float rot_err)
{
//    printf("%s()\r\n",__FUNCTION__);

    float aft_r1_val_a, aft_r1_val_b;
    float aft_val;
    int rule_a, rule_b;

    calHDGErrInference(hdg_err);
    calROTErrInference(rot_err);

    if     (g_fuzzy_var.hdg.a <= -1.0) g_fuzzy_var.hdg.a = -1.0;
    else if( 1.0 <= g_fuzzy_var.hdg.a) g_fuzzy_var.hdg.a =  1.0;
    if     (g_fuzzy_var.hdg.b <= -1.0) g_fuzzy_var.hdg.b = -1.0;
    else if( 1.0 <= g_fuzzy_var.hdg.b) g_fuzzy_var.hdg.b =  1.0;

    if     (g_fuzzy_var.rot.a <= -1.0) g_fuzzy_var.rot.a = -1.0;
    else if( 1.0 <= g_fuzzy_var.rot.a) g_fuzzy_var.rot.a =  1.0;
    if     (g_fuzzy_var.rot.b <= -1.0) g_fuzzy_var.rot.b = -1.0;
    else if( 1.0 <= g_fuzzy_var.rot.b) g_fuzzy_var.rot.b =  1.0;

//    printf("hdg.a[%f] b[%f] rot.a[%f] b[%f]\r\n", g_fuzzy_var.hdg.a, g_fuzzy_var.hdg.b, g_fuzzy_var.rot.a, g_fuzzy_var.rot.b);
//    printf("hdg_err[%6.2f] hdg.a[%6.2f] b[%6.2f]\r\n", hdg_err, g_fuzzy_var.hdg.a, g_fuzzy_var.hdg.b);
#if 1
    g_fuzzy_var.rule.a = fminf(g_fuzzy_var.hdg.a, g_fuzzy_var.rot.a);
    g_fuzzy_var.rule.b = fminf(g_fuzzy_var.hdg.b, g_fuzzy_var.rot.b);

    aft_r1_val_a = g_fuzzy_var.rule.a * convertRule2Val(rule_a = calAfterInferenceRule(g_fuzzy_var.hdg_err_rule.a, g_fuzzy_var.rot_err_rule.a));
    aft_r1_val_b = g_fuzzy_var.rule.b * convertRule2Val(rule_b = calAfterInferenceRule(g_fuzzy_var.hdg_err_rule.b, g_fuzzy_var.rot_err_rule.b));
#else
    g_fuzzy_var.rule.a = g_fuzzy_var.hdg.a;
    g_fuzzy_var.rule.b = g_fuzzy_var.hdg.b;

    aft_r1_val_a = g_fuzzy_var.rule.a * convertRule2Val(g_fuzzy_var.hdg_err_rule.a);
    aft_r1_val_b = g_fuzzy_var.rule.b * convertRule2Val(g_fuzzy_var.hdg_err_rule.b);
#endif

    if     (0 <= hdg_err) aft_val = fmaxf(aft_r1_val_a, aft_r1_val_b);
    else if(hdg_err <  0) aft_val = fminf(aft_r1_val_a, aft_r1_val_b);

#if 0
    printf("hdg_err[%6.2f] rule_ab[%6.2f:%6.2f] aft_ab_val[%6.2f:%6.2f:%6.2f]\r\n",
           hdg_err, g_fuzzy_var.rule.a, g_fuzzy_var.rule.b,
           aft_r1_val_a, aft_r1_val_b, aft_val);
#else
//    printf("err[%6.2f:%6.2f] rule_ab[%2d:%2d] aft_val[%6.2f]\r\n",
//           hdg_err, rot_err, rule_a, rule_b, aft_val);
#endif
//    printf("err hdg:rot[%4.2f:%4.2f] aft[%4.2f] aft_r1 a[%4.2f] b[%4.2f]\r\n", hdg_err, rot_err, aft_val, aft_r1_val_a, aft_r1_val_b);

    return aft_val;
}

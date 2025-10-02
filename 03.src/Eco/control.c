/*
 * control.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ihbaek
 */

#include "eco.h"

#define CTL_IHBAEK

fuzzy g_fuzzy;
control g_control;

void calHDGErrInference(float hdg_err)
{
    if(hdg_err < -5.0)
    {
        g_fuzzy.hdg.a = 1.0;
        g_fuzzy.hdg.b = 0.0;
        g_fuzzy.hdg_err_rule.a = NB;
        g_fuzzy.hdg_err_rule.b = NB;
    }
    else if(-5.0 <= hdg_err && hdg_err <  -3.0)
    {
        g_fuzzy.hdg.a = -0.5*hdg_err - 1.5;
        g_fuzzy.hdg.b =  0.5*hdg_err + 2.5;
        g_fuzzy.hdg_err_rule.a = NB;
        g_fuzzy.hdg_err_rule.b = NM;
    }
    else if(-3.0 <= hdg_err && hdg_err <  -1.0)
    {
        g_fuzzy.hdg.a = -0.5*hdg_err - 0.5;
        g_fuzzy.hdg.b =  0.5*hdg_err + 1.5;
        g_fuzzy.hdg_err_rule.a = NM;
        g_fuzzy.hdg_err_rule.b = NS;
    }
    else if(-1.0 <= hdg_err && hdg_err <=  1.0)
    {
        g_fuzzy.hdg.a = -0.5*hdg_err + 0.5;
        g_fuzzy.hdg.b =  0.5*hdg_err + 0.5;
        g_fuzzy.hdg_err_rule.a = NS;
        g_fuzzy.hdg_err_rule.b = PS;
    }
    else if( 1.0 <  hdg_err && hdg_err <=  3.0)
    {
        g_fuzzy.hdg.a = -0.5*hdg_err + 1.5;
        g_fuzzy.hdg.b =  0.5*hdg_err - 0.5;
        g_fuzzy.hdg_err_rule.a = PS;
        g_fuzzy.hdg_err_rule.b = PM;
    }
    else if( 3.0 <  hdg_err && hdg_err <=  5.0)
    {
        g_fuzzy.hdg.a = -0.5*hdg_err + 2.5;
        g_fuzzy.hdg.b =  0.5*hdg_err - 1.5;
        g_fuzzy.hdg_err_rule.a = PM;
        g_fuzzy.hdg_err_rule.b = PB;
    }
    else if( 5.0 <  hdg_err)
    {
        g_fuzzy.hdg.a = 0.0;
        g_fuzzy.hdg.b = 1.0;
        g_fuzzy.hdg_err_rule.a = PB;
        g_fuzzy.hdg_err_rule.b = PB;
    }
//    printf("hdg_err[%f] rule[%d:%d]\r\n", hdg_err, g_fuzzy_var.hdg_err_rule.a, g_fuzzy_var.hdg_err_rule.b);
//    printf("hdg_err[%6.2f][%6.2f][%6.2f]\r\n", hdg_err, g_fuzzy_var.hdg.a, g_fuzzy_var.hdg.b);
}

void calROTErrInference(float rot_err)
{
    if(rot_err < -5.0)
    {
        g_fuzzy.rot.a = 1.0;
        g_fuzzy.rot.b = 0.0;
        g_fuzzy.rot_err_rule.a = NB;
        g_fuzzy.rot_err_rule.b = NB;
    }
    else if(-5.0 <= rot_err && rot_err <  -3.0)
    {
        g_fuzzy.rot.a = -0.5*rot_err - 1.5;
        g_fuzzy.rot.b =  0.5*rot_err + 2.5;
        g_fuzzy.rot_err_rule.a = NB;
        g_fuzzy.rot_err_rule.b = NM;
    }
    else if(-3.0 <= rot_err && rot_err <  -1.0)
    {
        g_fuzzy.rot.a = -0.5*rot_err - 0.5;
        g_fuzzy.rot.b =  0.5*rot_err + 1.5;
        g_fuzzy.rot_err_rule.a = NM;
        g_fuzzy.rot_err_rule.b = NS;
    }
    else if(-1.0 <= rot_err && rot_err <=  1.0)
    {
        g_fuzzy.rot.a = -0.5*rot_err + 0.5;
        g_fuzzy.rot.b =  0.5*rot_err + 0.5;
        g_fuzzy.rot_err_rule.a = NS;
        g_fuzzy.rot_err_rule.b = PS;
    }
    else if( 1.0 <  rot_err && rot_err <=  3.0)
    {
        g_fuzzy.rot.a = -0.5*rot_err + 1.5;
        g_fuzzy.rot.b =  0.5*rot_err - 0.5;
        g_fuzzy.rot_err_rule.a = PS;
        g_fuzzy.rot_err_rule.b = PM;
    }
    else if( 3.0 <  rot_err && rot_err <=  5.0)
    {
        g_fuzzy.rot.a = -0.5*rot_err + 2.5;
        g_fuzzy.rot.b =  0.5*rot_err - 1.5;
        g_fuzzy.rot_err_rule.a = PM;
        g_fuzzy.rot_err_rule.b = PB;
    }
    else if( 5.0 <  rot_err)
    {
        g_fuzzy.rot.a = 0.0;
        g_fuzzy.rot.b = 1.0;
        g_fuzzy.rot_err_rule.a = PB;
        g_fuzzy.rot_err_rule.b = PB;
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
    case NB: return -20.0;
    case NM: return -14.0;
    case NS: return  -7.0;
    case ZO: return   0.0;
    case PS: return   7.0;
    case PM: return  14.0;
    case PB: return  20.0;
    }

    return 0.0; //never reach. avoid warning only.
}

float calPID(float hdg_err, float rot_err)  //PID control
{
    float rud_val = K_H * hdg_err + K_R * rot_err;

    if(fabs(rud_val) > RUD_MAX) rud_val = copysignf(RUD_MAX, rud_val);

    return rud_val;
}

float calFuzzy(float hdg_err, float rot_err)
{
//    printf("%s()\r\n",__FUNCTION__);

    float aft_val;

    calHDGErrInference(hdg_err);
    calROTErrInference(rot_err);

    g_fuzzy.weight.aa = fminf(g_fuzzy.hdg.a, g_fuzzy.rot.a);
    g_fuzzy.weight.ab = fminf(g_fuzzy.hdg.a, g_fuzzy.rot.b);
    g_fuzzy.weight.ba = fminf(g_fuzzy.hdg.b, g_fuzzy.rot.a);
    g_fuzzy.weight.bb = fminf(g_fuzzy.hdg.b, g_fuzzy.rot.b);

    g_fuzzy.rule.aa = calAfterInferenceRule(g_fuzzy.hdg_err_rule.a, g_fuzzy.rot_err_rule.a);
    g_fuzzy.rule.ab = calAfterInferenceRule(g_fuzzy.hdg_err_rule.a, g_fuzzy.rot_err_rule.b);
    g_fuzzy.rule.ba = calAfterInferenceRule(g_fuzzy.hdg_err_rule.b, g_fuzzy.rot_err_rule.a);
    g_fuzzy.rule.bb = calAfterInferenceRule(g_fuzzy.hdg_err_rule.b, g_fuzzy.rot_err_rule.b);

    g_fuzzy.val.aa = convertRule2Val(g_fuzzy.rule.aa);
    g_fuzzy.val.ab = convertRule2Val(g_fuzzy.rule.ab);
    g_fuzzy.val.ba = convertRule2Val(g_fuzzy.rule.ba);
    g_fuzzy.val.bb = convertRule2Val(g_fuzzy.rule.bb);

    g_fuzzy.weight_tot = g_fuzzy.weight.aa + g_fuzzy.weight.ab + g_fuzzy.weight.ba + g_fuzzy.weight.bb;

    if(0.01 < g_fuzzy.weight_tot)
    {
        aft_val = g_fuzzy.weight.aa*g_fuzzy.val.aa + g_fuzzy.weight.ab*g_fuzzy.val.ab + g_fuzzy.weight.ba*g_fuzzy.val.ba + g_fuzzy.weight.bb*g_fuzzy.val.bb;
        aft_val = aft_val/g_fuzzy.weight_tot;
    }
    else
        aft_val = 0.0;

//    printf("err hdg:rot[%4.2f:%4.2f] aft[%4.2f] aft_r1 a[%4.2f] b[%4.2f]\r\n", hdg_err, rot_err, aft_val, aft_r1_val_a, aft_r1_val_b);

    return aft_val*DEG2RAD;
}

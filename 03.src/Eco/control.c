/*
 * control.c
 *
 *  Created on: Jul 22, 2025
 *      Author: ihbaek
 */

#include "eco.h"

fuzzy_var g_fuzzy_var;
fuzzy_control g_fuzzy_control =
{
    .gain_hdg = 0.0,
    .gain_d_hdg = 0.0
};

void calErrInference(float err)
{
    if(err < -5.0)
    {
        g_fuzzy_var.y.a = 1.0;
        g_fuzzy_var.y.b = 1.0;
        g_fuzzy_var.erule.a = NB;
        g_fuzzy_var.erule.b = NB;
    }
    else if(-5.0 <= err && err <= -3.0)
    {
        g_fuzzy_var.y.a =  0.5*err + 2.5;
        g_fuzzy_var.y.b = -0.5*err - 1.5;
        g_fuzzy_var.erule.a = NM;
        g_fuzzy_var.erule.b = NM;
    }
    else if(-3.0 <  err && err <= -1.0)
    {
        g_fuzzy_var.y.a =  0.5*err + 1.5;
        g_fuzzy_var.y.b = -0.5*err - 0.5;
        g_fuzzy_var.erule.a = NS;
        g_fuzzy_var.erule.b = NS;
    }
    else if(-1.0 <  err && err <=  0.0)
    {
        g_fuzzy_var.y.a =  err + 1.0;
        g_fuzzy_var.y.b = -err;
        g_fuzzy_var.erule.a = ZO;
        g_fuzzy_var.erule.b = ZO;
    }
    else if( 0.0 <  err && err <=  1.0)
    {
        g_fuzzy_var.y.a =  err + 1.0;
        g_fuzzy_var.y.b = -err;
        g_fuzzy_var.erule.a = ZO;
        g_fuzzy_var.erule.b = ZO;
    }
    else if( 1.0 <  err && err <=  3.0)
    {
        g_fuzzy_var.y.a =  0.5*err - 0.5;
        g_fuzzy_var.y.b = -0.5*err + 1.5;
        g_fuzzy_var.erule.a = PS;
        g_fuzzy_var.erule.b = PS;
    }
    else if( 3.0 <  err && err <=  5.0)
    {
        g_fuzzy_var.y.a =  0.5*err - 1.5;
        g_fuzzy_var.y.b = -0.5*err + 3.5;
        g_fuzzy_var.erule.a = PM;
        g_fuzzy_var.erule.b = PM;
    }
    else if( 5.0 <  err)
    {
        g_fuzzy_var.y.a = 1.0;
        g_fuzzy_var.y.b = 1.0;
        g_fuzzy_var.erule.a = PB;
        g_fuzzy_var.erule.b = PB;
    }

}

void calDErrInference(float d_err)
{
    if(d_err < -5.0)
    {
        g_fuzzy_var.dy.a = 1.0;
        g_fuzzy_var.dy.b = 1.0;
        g_fuzzy_var.derule.a = NB;
        g_fuzzy_var.derule.b = NB;
    }
    else if(-5.0 <= d_err && d_err <= -3.0)
    {
        g_fuzzy_var.dy.a =  0.5*d_err + 2.5;
        g_fuzzy_var.dy.b = -0.5*d_err - 1.5;
        g_fuzzy_var.derule.a = NM;
        g_fuzzy_var.derule.b = NM;
    }
    else if(-3.0 <  d_err && d_err <= -1.0)
    {
        g_fuzzy_var.dy.a =  0.5*d_err + 1.5;
        g_fuzzy_var.dy.b = -0.5*d_err - 0.5;
        g_fuzzy_var.derule.a = NS;
        g_fuzzy_var.derule.b = NS;
    }
    else if(-1.0 <  d_err && d_err <=  0.0)
    {
        g_fuzzy_var.dy.a =  d_err + 1.0;
        g_fuzzy_var.dy.b = -d_err;
        g_fuzzy_var.derule.a = ZO;
        g_fuzzy_var.derule.b = ZO;
    }
    else if( 0.0 <  d_err && d_err <=  1.0)
    {
        g_fuzzy_var.dy.a =  d_err + 1.0;
        g_fuzzy_var.dy.b = -d_err;
        g_fuzzy_var.derule.a = ZO;
        g_fuzzy_var.derule.b = ZO;
    }
    else if( 1.0 <  d_err && d_err <=  3.0)
    {
        g_fuzzy_var.dy.a =  0.5*d_err - 0.5;
        g_fuzzy_var.dy.b = -0.5*d_err + 1.5;
        g_fuzzy_var.derule.a = PS;
        g_fuzzy_var.derule.b = PS;
    }
    else if( 3.0 <  d_err && d_err <=  5.0)
    {
        g_fuzzy_var.dy.a =  0.5*d_err - 1.5;
        g_fuzzy_var.dy.b = -0.5*d_err + 3.5;
        g_fuzzy_var.derule.a = PM;
        g_fuzzy_var.derule.b = PM;
    }
    else if( 5.0 <  d_err)
    {
        g_fuzzy_var.dy.a = 1.0;
        g_fuzzy_var.dy.b = 1.0;
        g_fuzzy_var.derule.a = PB;
        g_fuzzy_var.derule.b = PB;
    }

}

int calAfterInferenceRule(int err_rule, int d_err_rule)
{
    switch(err_rule)
    {
    case NB:
        switch(d_err_rule)
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
        switch(d_err_rule)
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
        switch(d_err_rule)
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
        switch(d_err_rule)
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
        switch(d_err_rule)
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
        switch(d_err_rule)
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
        switch(d_err_rule)
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

    return err_rule; //never reach. avoid warning only.
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

float calFuzzy(float err, float d_err)
{
//    printf("%s() \r\n",__FUNCTION__);

    float aft_r1_val_a, aft_r1_val_b;
    float aft_val;

    calErrInference(err);
    calDErrInference(d_err);

    if     (g_fuzzy_var.y.a <= -1.0) g_fuzzy_var.y.a = -1.0;
    else if( 1.0 <= g_fuzzy_var.y.a) g_fuzzy_var.y.a =  1.0;

    if     (g_fuzzy_var.y.b <= -1.0) g_fuzzy_var.y.b = -1.0;
    else if( 1.0 <= g_fuzzy_var.y.b) g_fuzzy_var.y.b =  1.0;

    if     (g_fuzzy_var.dy.a <= -1.0) g_fuzzy_var.dy.a = -1.0;
    else if( 1.0 <= g_fuzzy_var.dy.a) g_fuzzy_var.dy.a =  1.0;

    if     (g_fuzzy_var.dy.b <= -1.0) g_fuzzy_var.dy.b = -1.0;
    else if( 1.0 <= g_fuzzy_var.dy.b) g_fuzzy_var.dy.b =  1.0;

    g_fuzzy_var.rule.a = fminf(g_fuzzy_var.y.a, g_fuzzy_var.dy.a);
    g_fuzzy_var.rule.b = fminf(g_fuzzy_var.y.b, g_fuzzy_var.dy.b);

    aft_r1_val_a = g_fuzzy_var.rule.a * convertRule2Val(calAfterInferenceRule(g_fuzzy_var.erule.a, g_fuzzy_var.derule.a));
    aft_r1_val_b = g_fuzzy_var.rule.b * convertRule2Val(calAfterInferenceRule(g_fuzzy_var.erule.b, g_fuzzy_var.derule.b));

    if     (0 <= err) aft_val = fmaxf(aft_r1_val_a, aft_r1_val_b);
    else if(err <  0) aft_val = fmaxf(aft_r1_val_a, aft_r1_val_b);

    return aft_val;
}

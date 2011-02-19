/**************************************************************************
 *                                                                        *
 *           Copyright (C) 1995, NASA Ames Neuro-Engineering Team         *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  proprietary  information of NASA Ames Neuro-Engineering Team, and     *
 *  are protected by Federal copyright law.  They  may used only for      *
 *  non-commercial purposes unless prior written agreement is obtained    *
 *  from NASA. This copyright notice must be included in any distributed  *
 *  copy of this code.                                                    *
 *                                                                        *
 **************************************************************************/

/*
 *  limit.c
 */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

double limit(double value, double min, double max)
{
    double new_value;

    if (value > max)
        new_value = max;
    else if (value < min)
        new_value = min;
    else
        new_value = value;
        
    return(new_value);
}

/****************************************************************************/

float flimit(float value, float min, float max)
{
    float new_value;

    if (value > max)
        new_value = max;
    else if (value < min)
        new_value = min;
    else
        new_value = value;
        
    return(new_value);
}

/****************************************************************************/

double limit_angle(double value, double max)
{
    double new_value;

    if (value > max)
        new_value = value - 2.0*max;
    else if (value < -max)
        new_value = value + 2.0*max;
    else
        new_value = value;
        
    return(new_value);
}

/****************************************************************************/

float flimit_angle(float value, float max)
{
    float new_value;

    if (value > max)
        new_value = value - 2.0*max;
    else if (value < -max)
        new_value = value + 2.0*max;
    else
        new_value = value;
        
    return(new_value);
}


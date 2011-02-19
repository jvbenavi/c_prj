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
 *  roundz.c
 */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

int roundz(double value)
{
    int new_value;

    if (value > 0.0)
        new_value = (int)(value + 0.5);
    else if (value < 0.0)
        new_value = (int)(value - 0.5);
    else
        new_value = 0;

    return(new_value);
}

/****************************************************************************/

int froundz(float value)
{
    int new_value;

    if (value > 0.0)
        new_value = (int)(value + 0.5);
    else if (value < 0.0)
        new_value = (int)(value - 0.5);
    else
        new_value = 0;

    return(new_value);
}

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
 *  lag.c
 */

/*
 *  First order lag routine
 *
 *    This routine uses the Tustin difference equation
 *    for a first order lag of tau.
 *
 *      y/x = 1/(1 + tau*S)
 *
 *      y = (dt(x+xold) - y(dt-2tau)) / (dt+2tau)
 *
 *    CALLING SEQUENCE :
 *
 *      lag(x,xold,&y,tau,dt)
 *
 *    FORMAL PARAMETERS :
 *
 *      xold = x
 *      y = yold
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

void lag(double x, double xold, double *y, double tau, double dt)
{
    double tt;
 
    tt = 2. * tau;
 
    (*y) = (dt*(x+xold) - (*y)*(dt-tt)) / (dt+tt);
 
    return;
}

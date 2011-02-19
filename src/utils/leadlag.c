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
 *  leadlag.c
 */

/*
 *  First order leadlag routine
 *
 *    FUNCTIONAL DESCRIPTION :
 *
 *      This routine provides a general purpose model of any
 *      first order lead-lag function of the form
 *
 *        y/x = (a + bS) / (c + dS)
 *
 *      using the Tustin difference equation
 *
 *        y = (x(a*dt+b*2) + xold(a*dt-b*2) - y(c*dt-d*2))
 *            --------------------------------------------
 *         		(c*dt+d*2)
 *
 *      Note:   Either 'a' or 'b' can be zero if desired.
 *              Both 'c' and 'd' must be non-zero.
 *
 *    CALLING SEQUENCE :
 *
 *      leadlag(x,xold,&y,a,b,c,d,dt)
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

void leadlag(double x, double xold, double *y,
             double a, double b, double c, double d, double dt)
{
    double c1;
    double c2;
    double c3;
    double c4;
    double adt;
    double cdt;
    double tb;
    double td;
 
    adt = a * dt;
    cdt = c * dt;
    tb = 2. * b;
    td = 2. * d;

    c1 = adt + tb;
    c2 = adt - tb;
    c3 = cdt - td;
    c4 = cdt + td;

    (*y) = (c1 * x + c2 * xold - c3 * (*y)) / c4;
 
    return;
}

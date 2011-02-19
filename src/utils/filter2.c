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
 *  filter2.c
 */

/*
 *  Second order filter routine
 *
 *    FUNCTIONAL DESCRIPTION :
 *
 *      This routine provides a general purpose model of any
 *      second order filter of the form
 *
 *        y/u = (b2*s*s + b1*s + b0) / (s*s + a1*s + a0)
 *
 *      using phase variable representation
 *
 *    CALLING SEQUENCE :
 *
 *      filter(u,&xdd,&xd,&x,&y,b2,b1,b0,a1,a0,dt)
 *
 *    NOTES :
 *
 *      To initialize at a steady state:
 *        xdd = 0.0
 *        xd = 0.0
 *        x = u/a0
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

void filter2(double u, double *xdd, double *xd, double *x, double *y,
double b2, double b1, double b0, double a1, double a0, double dt)
{
    double dt2;
	double xddp;
	double xdp;

	dt2 = dt/2.0;
	xddp = (*xdd);
	xdp = (*xd);

	(*xdd) = u - a1*(*xd) - a0*(*x);
	(*xd) += dt2 * (3.0*(*xdd) - xddp);
    (*x) += dt2 * (3.0*(*xd) - xdp);

    (*y) = b2*(*xdd) + b1*(*xd) + b0*(*x);
 
    return;
}

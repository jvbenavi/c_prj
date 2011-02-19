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
 *  zlfi1u.c
 */

/*%
%##  One dimensional table lookup routine
%##    John Kaneshige
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-0710
%##    jkaneshige@mail.arc.nasa.gov
%
%##  Major sections include:
%
%##  Modification Log
%##    1. written___6/01|____JTK (Ported from ZLFI1U.f)
%##    2. 1st mod_______|_______
%##    3. 2nd mod_______|_______
%
%##  Comments
%##
%##    FUNCTIONAL DESCRIPTION :
%## 
%##        the array "tbl" must be of the following format !!!!!!!!!!
%## 
%##        tbl(1) = 1            - the no. of dimensions to the table
%##                                NOTE: If this is not 1, the routine
%##                                will error off.
%## 
%##        tbl(2) = no. of breakpoints for this table
%## 
%##        tbl(3) = first independent variable
%##           .          \
%##           .           \
%##           .            independent variables
%##           .           /
%##           .          /
%##        tbl(2+no. of brkpts) = last independent variable
%## 
%##        tbl(3+no. of brkpts) = first dependent value
%##           .          \
%##           .           \
%##           .            dependent values
%##           .           /
%##           .          /
%##        tbl(2+2*no. of brkpts) = last dependent value
%## 
%## 
%##    Note: This routine limits the output values to the table limits
%##          and does not extrapolate for inputs outside the range of
%##          the table.
%## 
%##    CALLING SEQUENCE :
%## 
%##             y = zlfi1u(tbl,x)
%## 
%##    FORMAL PARAMETERS :
%## 
%##           x   = input independent variable
%##           y   = output dependent variable
%##           tbl = the array of breakpoints and dependent values
% 
%*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float zlfi1u (float *tbl, float x)
{
    int j, k;
    int nbpt;
    float zlfi1u;
 
    /* Check for proper table format */
 
    if (tbl[(1)-1] != 1.)
    {
        printf("ATTEMPT TO USE ZLFI1U WITH WRONG FORMATTED TABLE");
        exit(1);
    }
 
    /* Using no. of brkpts set pointers to iv's and dv's */
 
    nbpt = tbl[(2)-1];
    j = 3;                /* iv pointer */
    k = 3 + nbpt;          /* dv pointer */
 
    /*
     *  Test if x is within table range. If not, set output to limit values.
     *  If so, loop to bound x between brkpts and interpolate.
     */
 
    if (x <= tbl[j-1])
        zlfi1u = tbl[k-1];
    else if (x >= tbl[(2+nbpt)-1])
        zlfi1u = tbl[(2+2*nbpt)-1];
    else
    {
        j = j + 1;
        k = k + 1;
        while (x > tbl[j-1])
        {
            j = j + 1;
            k = k + 1;
        }
        zlfi1u = tbl[(k-1)-1] + 
                 (x-tbl[(j-1)-1]) *
                 (tbl[k-1]-tbl[(k-1)-1]) / 
                 (tbl[j-1]-tbl[(j-1)-1]);
    }
       
    return(zlfi1u);
}

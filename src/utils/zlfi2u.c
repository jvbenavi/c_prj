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
 *  zlfi2u.c
 */

/*%
%##  Two dimensional table lookup routine
%##    John Kaneshige
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-0710
%##    jkaneshige@mail.arc.nasa.gov
%
%##  Major sections include:
%
%##  Modification Log
%##    1. written___6/01|____JTK (Ported from ZLFI2U.f)
%##    2. 1st mod_______|_______
%##    3. 2nd mod_______|_______
%
%##  Comments
%##
%##    FUNCTIONAL DESCRIPTION :
%##
%##    Notes:
%## 
%##      1. The routine limits the output values to the table limits
%##         and does not extrapolate for inputs outside the range of
%##         the table.
%## 
%##      2. The breakpoints are referred to as nb1 for independent
%##         variable 1 and nb2 for independent variable 2.
%##
%##      3. If the first value in the table is not 2, then the
%##         routine will error off.
%##  
%##      4. Temporary terms are used to signal bases of each section
%##         of breakpoints or dependent variables to achieve greater
%##         execution time efficiency. Note that the base location
%##         for one section is also the maximum location for the 
%##         previous section.
%##
%##      5. The array "tbl" MUST be of the following format!!!!!!!!!!!
%## 
%##         tbl(1)           = 2
%##
%##         tbl(2)           = nb1
%##
%##         tbl(3)           = nb2
%## 
%##         tbl(4)           = 1st breakpoint for x
%##            .       \
%##            .        \
%##            .                   list of breakpoints for x increasing values
%##            .        /
%##            .       /
%##         tbl(3+nb1)       = last breakpoint for x
%## 
%##         tbl(4+nb1)       = 1st breakpoint for y
%##            .       \
%##            .        \
%##            .                   list of breakpoints for y increasing values
%##            .        /
%##            .       /
%##         tbl(3+nb1+nb2)   = last breakpoint for y
%##
%##         tbl(4+nb1+nb2)   = f(x1,y1) - 1st dependent value
%##            .
%##            .
%##            .                   list of dependent values varying x then y
%##            .
%##            .
%##         tbl(3+nb1+nb2+nb1*nb2) = f(xnb1,ynb2) - last dependent value
%##         
%##         tbl(4+nb1+nb2+nb1*nb2) = index to iv1 brkpt last pass
%## 
%##         tbl(5+nb1+nb2+nb1*nb2) = index to iv2 brkpt last pass
%## 
%##    CALLING SEQUENCE :
%## 
%##           f = zlfi2u(tbl,x,y)
%## 
%##    FORMAL PARAMETERS :
%## 
%##           x   = input variable 1
%##           y   = input variable 2
%##           f   = outout dependent variable
%##           tbl = the array of breakpoints and dependent values
% 
%*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float zlfi2u (float *tbl, float x, float y)
{
    int i, j, k;
    int ii, jj;
    int m11, m12, m21, m22;
    int nb1, nb2;
    int jb, jb1, kb, ipb, jpb;
    int im1, jm1;
    float xx, yy;
    float dx, dx1, dx2;
    float dy, dy1, dy2;
    float zlfi2u;
 
    /* Check for proper table format */
 
    if (tbl[(1)-1] != 2.)
    {
        printf("ATTEMPT TO USE ZLFI2U WITH WRONG FORMATTED TABLE");
        exit(1);
    }
 
    /* Set the terms for number of breakpoints and set base terms */
 
    nb1 = tbl[(2)-1];          /* number of breakpoints for iv1 */
    nb2 = tbl[(3)-1];          /* number of breakpoints for iv2 */
    jb  = 3 + nb1;             /* index to base of iv2 breakpoint list */
    jb1 = jb + 1;              /* index to first breakpoint in iv2 list */
    kb  = jb + nb2;            /* index to base of dv list */
    ipb = kb + nb1 * nb2 + 1;  /* index to loc of index to iv1 brkpt last pass */
    jpb = ipb + 1;             /* index to loc of index to iv2 brkpt last pass */
 
    /*
     *  Test for x within the table range. If outside, set x to the limit
     *  value and set the x pointer (i). If inside, loop to bound x beginning
     *  with the breakpoint used last pass and set the pointer (i).
     */ 

    if (x <= tbl[(4)-1])
    {
        i = 5;
        xx = tbl[(4)-1];
    }
    else if (x >= tbl[jb-1])
    {
        i = jb;
        xx = tbl[jb-1];
    }
    else
    {
        i = tbl[ipb-1];
        xx = x;
        if (x >= tbl[i-1])
        {
            i = i + 1;
            while (x > tbl[i-1])
            {
                i = i + 1;
            }
        }
        else
        {
            i = i - 1;
            while (x < tbl[i-1])
            {
                i = i - 1;
            }
            i = i + 1;
        }
    }
    /*tbl[ipb-1] = i;*/
 
    /*
     *  Test for y within the table range. If outside, set y to the limit
     *  value and set the y pointer (j). If inside, loop to bound y beginning
     *  with the breakpoint used the last pass and set the pointer (j).
      */

    if (y <= tbl[jb1-1])
    {
        j = jb1 + 1;
        yy = tbl[jb1-1];
    }
    else if (y >= tbl[kb-1])
    {
        j = kb;
        yy = tbl[kb-1];
    }
    else
    {
        j = tbl[jpb-1];
        yy = y;
        if (y >= tbl[j-1])
        {
            j = j + 1;
            while (y > tbl[j-1]) j = j + 1;
        } 
        else
        {
            j = j - 1;
            while (y <= tbl[j-1]) j = j - 1;
        }
    }
    /*tbl[jpb-1] = j;*/
 
    /* Determine x and y difference terms for use in interpolation */
 
    im1 = i - 1;
    dx = tbl[i-1] - tbl[im1-1];
    dx1 = xx - tbl[im1-1];
    dx2 = tbl[i-1] - xx;
    jm1 = j - 1;
    dy = tbl[j-1] - tbl[jm1-1];
    dy1 = yy - tbl[jm1-1];
    dy2 = tbl[j-1] - yy;

    /*
     *  Get the bounding function pointers for use in interpolation
     *  The formula for locating f(x,y) at i,j is
     *  m22 = kb + ii + (jj - 1) * nb1
      */

    ii = i - 3;
    jj = j - jb;
    m11 = kb + (ii-1) + (jj-2)*nb1;
    m21 = m11 + 1;
    m12 = m11 + nb1;
    m22 = m12 + 1;

    /* Solve the interpolation function for the output */
 
    zlfi2u = (dy1 * (dx1 * tbl[m22-1] + dx2 * tbl[m12-1]) +
              dy2 * (dx1 * tbl[m21-1] + dx2 * tbl[m11-1])) / (dx * dy);
     
    return(zlfi2u);
}

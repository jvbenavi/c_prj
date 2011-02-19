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
 *  zlfi3u.c
 */

/*%
%##  Three dimensional table lookup routine
%##    John Kaneshige
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-0710
%##    jkaneshige@mail.arc.nasa.gov
%
%##  Major sections include:
%
%##  Modification Log
%##    1. written___6/01|____JTK (Ported from ZLFI3U.f)
%##    2. 1st mod_______|_______
%##    3. 2nd mod_______|_______
%
%##  Comments
%##
%##    FUNCTIONAL DESCRIPTION :
%##
%##    Notes:
%## 
%##     1. The routine limits the output values to the table limits
%##        and does not extrapolate for inputs outside the range of
%##        the table.
%##
%##     2. The breakpoints are referred to as nb1, nb2 and nb3 for
%##        the first, second and third variables.
%##
%##     3. If the first value in the table is not 3, then the
%##        routine will error off.
%##
%##     4. To save execution time whereever possible, many temporary
%##        terms are used within the routine which may not be clear
%##        unless some steps are taken to transform them.
%##
%##     5. Temporary terms are used to signal bases of each section
%##        of breakpoints or dependent variables. Note that the base
%##        location for one section is also the maximum location for
%##        the previous section.
%##
%##     6. The array "tbl" MUST be of the following format !!!!!!!!!!!
%##
%##        tbl(1)             = 3
%##
%##        tbl(2)             = nb1
%##
%##        tbl(3)             = nb2
%##
%##        tbl(4)             = nb3
%##     
%##        tbl(5)             = first breakpoint for x
%##           .      \
%##           .                    x breakpoints in increasing value
%##           .      /
%##        tbl(4+nb1)         = last breakpoint for x
%##
%##        tbl(5+nb1)         = first breakpoint for y
%##           .      \
%##           .                    y breakpoints in increasing value
%##           .      /
%##        tbl(4+nb1+nb2)     = last breakpoint for y
%##
%##        tbl(5+nb1+nb2)     = first breakpoint for z
%##           .      \
%##           .                    z breakpoints in increasing value
%##           .      /
%##        tbl(4+nb1+nb2+nb3) = last breakpoint for z
%##
%##        tbl(5+nb1+nb2+nb3) = f(x1,y1,z1) - first dependent value
%##           .      \
%##           .       \
%##           .                    dependent values varying x then y then z
%##           .       /
%##           .      /
%##        tbl(4+nb1+nb2+nb3  = f(xnb1,ynb2,znb3) - last dependent value
%##                +nb1*nb2*nb3)
%##
%##        tbl(5+nb1+nb2+nb3  = index to iv1 brkpt last pass
%##                +nb1*nb2*nb3)
%##
%##        tbl(6+nb1+nb2+nb3  = index to iv2 brkpt last pass
%##                +nb1*nb2*nb3)
%##
%##        tbl(7+nb1+nb2+nb3  = index to iv3 brkpt last pass
%##                +nb1*nb2*nb3)
%## 
%##    CALLING SEQUENCE :
%##
%##          f = zlfi3u(tbl,x,y,z)
%##
%##    FORMAL PARAMETERS :
%##
%##           x   = input variable 1
%##           y   = input variable 2
%##           z   = input variable 3
%##           f   = output dependent variable
%##           tbl = array of breakpoints and dependent values
%
%*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float zlfi3u (float *tbl, float x, float y, float z)
{
    int i, j, k;
    int ii, jj, kk;
    int m111, m112, m121, m122, m211, m212, m221, m222;
    int nb1, nb2, nb3, nb12;
    int jb, jb1, kb, kb1, lb, lb1;
    int ipb, jpb, kpb;
    int im1, jm1, km1;
    float xx, yy, zz;
    float dx, dx1, dx2;
    float dy, dy1, dy2;
    float dz, dz1, dz2;
    float zlfi3u;
 
    /* Check for proper table format */
 
    if (tbl[(1)-1] != 3.)
    {
        printf("ATTEMPT TO USE ZLFI3U WITH WRONG FORMATTED TABLE");
        exit(1);
    }
 
    /*
     * Set the terms for number of breakpoints
     * Also set terms for breakpoint and dependent sections bases
     */

    nb1 = tbl[(2)-1];                  /* number of breakpoints for iv1 */
    nb2 = tbl[(3)-1];                  /* number of breakpoints for iv2 */
    nb12 = nb1 * nb2;
    nb3 = tbl[(4)-1];                  /* number of breakpoints for iv3 */
    jb = 4 + nb1;                      /* index to base of iv2 list */
    jb1 = jb + 1;                      /* index to first breakpoint of iv2 list */
    kb = jb + nb2;                     /* index to base of iv3 list */
    kb1 = kb + 1;                      /* index to first breakpoint of iv3 list */
    lb = kb + nb3;                     /* index to base of dv list */
    ipb = lb + nb12 * nb3 + 1;         /* index to loc of iv1 brkpt index */
    jpb = ipb + 1;                     /* index to loc of iv2 brkpt index */
    kpb = jpb + 1;                     /* index to loc of iv3 brkpt index */

    /*
     * Test for x within the table range. If outside, set xx to the limit
     * value and set the x pointer (i). If inside, loop to bound x beginning
     * with the breakpoint used last pass and set xx to x and set the pointer (i).
     */

    if (x <= tbl[(5)-1])
    {
        i = 6;
        xx = tbl[(5)-1];
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
            while (x > tbl[i-1]) i = i + 1;
        }
        else
        {
            i = i - 1;
            while (x <= tbl[i-1]) i = i - 1;
            i = i + 1;
        }
    }
 
    /*tbl[ipb-1] = i;*/

    /*
     * Test for y within the table range. If outside, set yy to the limit
     * value and set the y pointer (j). If inside, loop to bound y beginning
     * with the breakpoint used the last pass and set yy to y and set the 
     * pointer (j).
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
            j = j + 1;
        }
    }

    /*tbl[jpb-1] = j;*/

    /*
     * Test for z within the table range. If outside, set zz to the limit
     * value and set the z pointer (k). If inside, loop to bound z beginning
     * with the breakpoint used the last pass and set zz to z and set the 
     * pointer (k).
     */

    if (z <= tbl[kb1-1])
    {
        k = kb1 + 1;
        zz = tbl[kb1-1];
    }
    else if (z >= tbl[lb-1])
    {
        k = lb;
        zz = tbl[lb-1];
    }
    else
    {
        k = tbl[kpb-1];
        zz = z;
        if (z >= tbl[k-1])
        {
            k = k + 1;
            while (z > tbl[k-1]) k = k + 1;
        }
        else
        {
            k = k - 1;
            while (z <= tbl[k-1]) k = k - 1;
            k = k + 1;
        }
    }

    /*tbl[kpb-1] = k;*/

    /* Determine the x, y, and z difference terms for use in interpolation */

    im1 = i - 1;
    dx  = tbl[i-1] - tbl[im1-1];
    dx1 = xx - tbl[im1-1];
    dx2 = tbl[i-1] - xx;
    jm1 = j - 1;
    dy  = tbl[j-1] - tbl[jm1-1];
    dy1 = yy - tbl[jm1-1];
    dy2 = tbl[j-1] - yy;
    km1 = k - 1;
    dz  = tbl[k-1] - tbl[km1-1];
    dz1 = zz - tbl[km1-1];
    dz2 = tbl[k-1] - zz;

    /*
     * Get the bounding functions for use in the interpolation
     * The formula for locating f(x,y,z) at i,j,k is
     * m222 = lb + ii + (jj - 1) * nb1 + (kk - 1) * nb1 * nb2
     */

    ii = i - 4;
    jj = j - jb;
    kk = k - kb;
       
    m111 = lb + (ii-1) + (jj-2)*nb1 + (kk-2)*nb12;
    m211 = m111 + 1;
    m121 = m111 + nb1;
    m221 = m121 + 1;
    m112 = m111 + nb12;
    m212 = m112 + 1;
    m122 = m112 + nb1;
    m222 = m122 + 1;

    /* Solve the interpolation function for the output */

    zlfi3u = (dz1 * (dy1 * (dx1 * tbl[m222-1] + dx2 * tbl[m122-1]) +
              dy2 * (dx1 * tbl[m212-1] + dx2 * tbl[m112-1])) +
              dz2 * (dy1 * (dx1 * tbl[m221-1] + dx2 * tbl[m121-1]) +
              dy2 * (dx1 * tbl[m211-1] + dx2 * tbl[m111-1])))
             / (dx * dy * dz);
     
    return(zlfi3u);
}

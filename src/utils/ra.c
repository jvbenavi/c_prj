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
/* 08/05/2005 M. Steglinski Changed code for ra180 & ra360
//
// 12/15/2005 M. Steglinski Corrected a bug in ra360.
*/
/*
 *  ra.c
 */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

/* This is general purpose function that resolves an
// input angle into it's value limited between -180 and
// +180 deg.
*/

double ra180(double angle)
{
    double new_angle;
    double rem;
    double quot;

    quot = angle / 360.;
    rem  = (quot - (int)quot) * 360.;
    if (fabs(rem) > 180.)
    {
      if (rem >= 0.)
      {
        new_angle = rem - 360.;
      }
      else
      {
        new_angle = rem + 360.;
      }
    }
    else
    {
      new_angle = rem;
    }

	/*
    // while (new_angle > 180.0)  new_angle -= 360.0;
    // while (new_angle < -180.0) new_angle += 360.0;
	*/
        
    return(new_angle);
}


/****************************************************************************/

/*
// This is general purpose function that resolves an
// input angle into it's value limited between 0.0 and
// +360.0 deg.
*/

double ra360(double angle)
{
    double new_angle;
    double rem;
    double quot;


    quot = angle / 360.;
    rem  = (quot - (int)quot) * 360.;

    if (rem < 0.) /* MS 12/15/2005 */
    {
      rem = rem + 360.;
    }
    new_angle = rem;
	/*
    // while (new_angle > 360.0) new_angle -= 360.0;
    // while (new_angle < 0.0)   new_angle += 360.0;
	*/
        
    return(new_angle);
}


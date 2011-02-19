/*======================================================================
C
C			  ****  SODINV  ****
C
C
C	FILE :SODINV.FOR 	Version :1.0 		Created : 11-6-90
C
C
C	NOTES :
C		This routine ....
C
C
C	PARAMETERS :
C		param_1, param_2, ...
C
C
C	CALLING SEQUENCE :
C		routine_a --> routine_b --> ...
C
C
C	FUNCTIONS CALLED :
C		routine_x
C
C
C	AUTHOR :
C		Your Name; Sterling Federal Systems, Inc.
C
C
C	REVISIONS :
C		11-6-90		I. LEVRAM		Brief Description
C
C         04/01/98  M. Steglinski	Port to Win32. Note: This is a workaround
C                                    for the DEC compiler which doesn't like 
C                                    an explicit constant as an argument to the
C                                    intrinsic function SIGN.
C		07-14-04	J. Kaneshige	Ported to C
C
C========================================================================*/

/***********************************************************************
C                                                                      *
C      LIBRARY      : GPF.A                                            *
C      AUTHOR       : MIKE IVAN                                        *
C      CODING DATE  : 17 FEB 87                                        *
C      VERSION      : A                                                *
C      RELEASE DATE : 1 JUN 87                                         *
C                                                                      *
C                               REVISIONS                              *
C                                                                      *
C    REV/E# DATE       NAME         PR #   DESCRIPTION                 *
C --!------!----------!------------!------!----------------------------*
C    A00    16 MAR 87  WALT WILEY          ORIGINAL RELEASE.           *
C    A01    31 AUG 87  BENTON PARRIS       CHANGED LARGE CONSTANT      *
C                                          1.0E-38 TO 1.0D-38 TO BE    *
C                                          COMPATIBLE WITH ALLIANT     *
C    C00    04 NOV 88  BEN PARRIS     0725 FIX SIGNS FOR BEARINGS.     *
C           12 SEP 89  H. RADFAR      ADDED NO IMPLICIT                *
C***********************************************************************
C:EJECT
C***********************************************************************
C                                                                      *
C PREAMBLE:                                                            *
C                                                                      *
C THE SODINV SUBROUTINE USES THE INVERSE OF SODANO'S EQUATIONS.  FROM  *
C THE LATITUDE AND LONGITUDE OF TWO POINTS, THE GREAT CIRCLE DISTANCE  *
C AND THE BEARING FROM POINT 1 TO POINT 2 IS CALCULATED.  OPTIONALLY,  *
C THE BEARING FROM POINT 2 TO POINT 1 MAY ALSO BE CALCULATED.          *
C SPHERICAL EARTH GEOMETRY IS ASSUMED.                                 *
C                                                                      *
C***********************************************************************
      IMPLICIT NONE
C                                                                      *
C CALLING SEQUENCE:                                                    *
C                                                                      *
C  CALL SODINV (LAT1, LON1, LAT2, LON2, RE, FLBG21, GCD, BRG12, BRG21) *
C                                                                      *
C        INPUT ARGUMENTS                                               *
C                                                                      *
C             NAME      TYPE      DEFINITION                           *
C             ------    -------   ------------------------------------ *
C             LAT1      REAL      LATITUDE OF POINT 1 (DEGREES)        *
C             LON1      REAL      LONGITUDE OF POINT 1 (DEGREES)       *
C             LAT2      REAL      LATITUDE OF POINT 2 (DEGREES)        *
C             LON2      REAL      LONGITUDE OF POINT 2 (DEGREES)       *
C             RE        REAL      EARTH'S RADIUS AT LAT1 (N. MILES)    *
C             FLBG21    INTEGER   FLAG SET TO 1 TO CALCULATE BRG21     *
C                                                                      *
C        OUTPUT ARGUMENTS                                              *
C                                                                      *
C             NAME      TYPE      DEFINITION                           *
C             ------    -------   ------------------------------------ *
C             GCD       REAL      GREAT CIRCLE DISTANCE FROM POINT 1 TO*
C                                 POINT 2 (N. MILES)                   *
C             BRG12     REAL      BEARING FROM POINT 1 TO POINT 2 (DEG)*
C             BRG21     REAL      BEARING FROM POINT 2 TO POINT 1 (DEG)*
C                                                                      *
C USAGE:                                                               *
C        CALLED FROM:  ANYWHERE                                        *
C                                                                      *
C                                                                      *
C        CALLS:  RA360                                                 *
C                                                                      *
C***********************************************************************
C:EJECT
C***********************************************************************
C                                                                      *
C INPUTS: NONE                                                         *
C                                                                      *
C PARAMETERS:                                                          *
C                                                                      *
C   DTOR              CONV. DEGREES TO RADIANS                         *
C                     (.01745329520) RAD/DEG                           *
C   RTOD              CONV. RADIANS TO DEGREES                         *
C                     (57.295779513) DEG/RAD                           *
C                                                                      *
C                                                                      *
C INTERNAL VARIABLES:                                                  *
C                                                                      *
C   BRG12             OUTPUT ARGUMENT                                  *
C   BRG21             OUTPUT ARGUMENT                                  *
C   BSGN12            SIGN VARIABLE FOR BSG12                          *
C   BSGN21            SIGN VARIABLE FOR BSG21                          *
C   CBRG12            COSINE OF BRG12                                  *
C   CBRG21            COSINE OF BRG21                                  *
C   CDLON             COSINE OF DLON                                   *
C   CL1               COSINE OF LATITUDE 1                             *
C   CL2               COSINE OF LATITUDE 2                             *
C   CRORE             COSINE OF RORE                                   *
C   DLON              LONGITUDE 2 - LONGITUDE 1  (RADIANS)             *
C   DLOND             LONGITUDE 2 - LONGITUDE 1  (DEGREES)             *
C   FLBG21            INPUT ARGUMENT                                   *
C   GCD               OUTPUT ARGUMENT                                  *
C   LAT1              INPUT ARGUMENT                                   *
C   LAT1R             LATITUDE OF POINT 1  (RADIANS)                   *
C   LAT2              INPUT ARGUMENT                                   *
C   LAT2R             LATITUDE OF POINT 2  (RADIANS)                   *
C   LN1               RA360(LON1)                                      *
C   LN2               RA360(LON2)                                      *
C   LON1              INPUT ARGUMENT                                   *
C   LON2              INPUT ARGUMENT                                   *
C   RE                INPUT ARGUMENT                                   *
C   RORE              RATIO OF GCD TO RE                               *
C   SL1               SINE OF LATITUDE 1                               *
C   SL2               SINE OF LATITUDE 2                               *
C   SRORE             SINE OF RORE                                     *
C                                                                      *
C                                                                      *
C OUTPUTS:  NONE                                                       *
C                                                                      *
C                                                                      *
C***********************************************************************/

#include <math.h>
#include <stdio.h>

/* global constants */
#include "params.h" 

/* function declarations */
#include "utils.h"

void sodinv(double lat1, double lon1, double lat2, double lon2, double re,
            int flbg21, double *gcd, double *brg12, double *brg21)
{

	/* local variables */
	double ln1, ln2, bsgn12, bsgn21, dlond;
	double lat1r, lat2r, sl1, cl1, sl2, cl2;
	double cdlon, srore, crore, rore;
	double dlon, cbrg12, cbrg21;

	/* process inputs */
	lat1r = lat1 * D2R;
	lat2r = lat2 * D2R;
	sl1 = sin(lat1r);
	cl1 = cos(lat1r);
	sl2 = sin(lat2r);
	cl2 = cos(lat2r);
	ln1 = limit_angle(lon1,180.0) + 180.0;	/* 0 to 360 */
	dlond = lon2 - lon1;
	dlon = dlond * D2R;
	cdlon = cos(dlon);

	/* great circle distance between points 1 and 2 */
	crore = sl2 * sl1 + cl2 * cl1 * cdlon;
	crore = limit(crore, -1.0, 1.0);
	rore = acos(crore);
	srore = sin(rore);
	*gcd = re * rore;

	/* bearing from point 1 to point 2 */
	if (*gcd >= 1.0e-38)
	{
		cbrg12 = (sl2 - sl1 * crore) / (cl1 * srore);
		cbrg12 = limit(cbrg12, -1.0, 1.0);
		/* note: the acos function returns an angle between 0 and 180 degrees */
		*brg12 = acos(cbrg12) * R2D;
		if (dlond > 180.0)
		{
			if (dlond >= 0.0)
				bsgn12 = -1.0;
			else
				bsgn12 = 1.0;
		}
		else if (dlond < 180.0)
		{
			if (dlond >= 0.0)
				bsgn12 = 1.0;
			else
				bsgn12 = -1.0;
		}
		else
		{
			bsgn12 = 1.0;
		}

		*brg12 = bsgn12 * (*brg12);

		/* bearing from point 2 to point 1 */
		if (flbg21 == 1)
		{
			bsgn21 = -bsgn12;
			cbrg21 = (sl1 - sl2 * crore) / (cl2 * srore);
			cbrg21 = limit(cbrg21, -1.0, 1.0);
			*brg21 = acos(cbrg21) * R2D;
			*brg21 = bsgn21 * (*brg21);
		}
	}
	else
	{
		/* no bearings if great circle distance is zero */
	  /* because brg12 and brg21 may not both be used by the user
             user may pass a NULL character for one ... make sure you 
             don't write to a NULL pointer -SFC */
	  if(brg12 != NULL)
		*brg12 = 0.0;
          if(brg21 != NULL)
		*brg21 = 0.0;

		*gcd = 0.0;
	}

	return;
}

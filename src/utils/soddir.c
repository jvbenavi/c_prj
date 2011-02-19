/*======================================================================
C
C			  ****  SODDIR  ****
C
C
C	FILE :SODDIR.FOR 	Version :1.0 		Created :11-6-90
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
C		10-20-94	J. Kaneshige	Convert bearing between +/- 180
C						before sign check.
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
C    C00    04 NOV 88  BEN PARRIS    0725  RA180(LON2)                 *
C           12 SEP 89  H. RADFAR     1310CA  ADDED NO IMPLICIT         *
C***********************************************************************
C:EJECT
C***********************************************************************
C                                                                      *
C PREAMBLE:                                                            *
C                                                                      *
C THE SODDIR SUBROUTINE USES SODANO'S EQUATIONS DIRECTLY.  FROM        *
C ONE POINT LOCATION (LAT1/LON1), A GREAT CIRCLE DISTANCE AND THE      *
C BEARING FROM THAT POINT, THE LOCATION (LAT2/LON2) OF THE OTHER POINT *
C IS CALCULATED.  OPTIONALLY, THE BEARING FROM THE CALCULATED POINT MAY*
C ALSO BE CALCULATED.  SPHERICAL GEOMETRY IS USED.                     *
C                                                                      *
C***********************************************************************
      IMPLICIT NONE
C                                                                      *
C CALLING SEQUENCE:                                                    *
C                                                                      *
C  CALL SODDIR (LAT1, LON1, GCD, BRG12, RE, FLBG21, LAT2, LON2, BRG21) *
C                                                                      *
C        INPUT ARGUMENTS                                               *
C                                                                      *
C             NAME      TYPE      DEFINITION                           *
C             ------    -------   ------------------------------------ *
C             LAT1      REAL      LATITUDE OF POINT 1 (DEGREES)        *
C             LON1      REAL      LONGITUDE OF POINT 1 (DEGREES)       *
C             GCD       REAL      GREAT CIRCLE DIST. FROM PT 1 TO PT 2 *
C                                 (NAUTICAL MILES)                     *
C             BRG12     REAL      BEARING FROM POINT 1 TO POINT 2      *
C                                 (DEGREES)                            *
C             RE        REAL      EARTH'S RADIUS AT LAT1 (N. MILES)    *
C             FLBG21    INTEGER   FLAG SET TO 1 TO CALCULATE BRG21     *
C                                                                      *
C        OUTPUT ARGUMENTS                                              *
C                                                                      *
C             NAME      TYPE      DEFINITION                           *
C             ------    -------   ------------------------------------ *
C             LAT2      REAL      LATITUDE OF POINT 2 (DEGREES)        *
C             LON2      REAL      LONGITUDE OF POINT 2 (DEGREES)       *
C             BRG21     REAL      BEARING FROM POINT 2 TO POINT 1      *
C                                 (DEGREES)                            *
C                                                                      *
C USAGE:                                                               *
C        CALLED FROM:  ANYWHERE                                        *
C                                                                      *
C                                                                      *
C        CALLS:  RA180                                                  *
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
C   BRG12             INPUT ARGUMENT                                   *
C   BRG21             OUTPUT ARGUMENT                                  *
C   BSIGN             BEARING SIGN                                     *
C   CBRG21            COSINE OF BEARING FROM POINT 2 TO POINT 1        *
C   CDLON             COSINE OF DIFFERENCE IN LONGITUDES               *
C   CL1               COSINE OF LATITUDE 1                             *
C   CL2               COSINE OF LATITUDE 2                             *
C   CRORE             COSINE OF RORE                                   *
C   FLBG21            INPUT ARGUMENT                                   *
C   GCD               INPUT ARGUMENT                                   *
C   LAT1              INPUT ARGUMENT                                   *
C   LAT1R             LATITUDE OF POINT 1 (RADIANS)                    *
C   LAT2              OUTPUT ARGUMENT                                  *
C   LAT2R             LATITUDE OF POINT 2 (RADIANS)                    *
C   LON1              INPUT ARGUMENT                                   *
C   LON2              OUTPUT ARGUMENT                                  *
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

void soddir(double lat1, double lon1, double gcd, double brg12, double re,
            int flbg21, double *lat2, double *lon2, double *brg21)
{

	/* local variables */
	double lat1r, rore, sl1, cl1, crore, sl2, lat2r, srore, cl2;
	double cdlon, cbrg21, bsign, lon2_temp;

	/* process inputs */
	lat1r = lat1 * D2R;
	rore = gcd/re;
	srore = sin(rore);
	crore = cos(rore);
	sl1 = sin(lat1r);
	cl1 = cos(lat1r);
	if (limit_angle(brg12, 180.0) >= 0.0)
		bsign = 1.0;
	else
		bsign = -1.0;

	/* latitude of point 2 */
	sl2 = sl1 * crore + cl1 * srore * cos(brg12*D2R);
	sl2 = limit(sl2, -1.0, 1.0);
	lat2r = asin(sl2);
	cl2 = cos(lat2r);
	*lat2 = lat2r * R2D;

	/* longitude of point 2 */
	cdlon = (crore - sl2 * sl1) / (cl1 * cl2);
	cdlon = limit(cdlon, -1.0, 1.0);
	lon2_temp = lon1 + bsign * acos(cdlon) * R2D;
	*lon2 = limit_angle(lon2_temp, 180.0);

	/* bearing from point 2 to point 1 */
	if (flbg21 == 1)
	{
		cbrg21 = (sl1 - crore * sl2) / (srore * cl2);
		cbrg21 = limit(cbrg21, -1.0, 1.0);
		*brg21 = -bsign * acos(cbrg21) * R2D;
	}

	return;
}

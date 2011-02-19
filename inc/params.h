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
 *  params.h
 */

#ifndef PARAMS_H
#define PARAMS_H

/* To negate trimming a failure */
#define DAMAGE_NO_TRIM

/* booleans */
#define TRUE       1
#define FALSE      0

/* constants */
#define NULL_CHAR  '\0'
#define TINY 1.0e-8 

/* conversions */
#define PI         3.14159265358979323846
#define TWOPI      2.*PI
#define R2D        (180./PI)
#define D2R        (PI/180.)
#define F2M        0.3048
#define M2F        (1./F2M)
#define NM2FT      6076.115486
#define FT2NM      (1./NM2FT)
#define KTS2FPS    (NM2FT/3600.)	// 1.6877
#define FPS2KTS    (1./KTS2FPS) 	// 0.5925
#define LBF2N      4.4482216152605
#define N2LBF	   (1./LBF2N)
#define LBM2KG	   0.45359237
#define KG2LBM	   (1./LBM2KG)
#define SLG2KG     14.5939029
#define KG2SLG     (1./SLG2KG)

/* inputs */
#define NONE 0
#define NOT_SPECIFIED -1

/* environment (atmosphere) */
#define GRAVSL   32.174     /* earth's gravity at sea level-feet per second squared */
#define DENSL    0.0023769  /* earth's density at sea level-slugs per cubic foot    */
#define PRESL    2116.22    /* earth's pressure at sea level-pounds per square foot */
#define TEMSL    288.16     /* earth's temperature at sea level-degrees Kelvin      */
#define DELCON   5.255745   /* earth's pressure ratio constant                      */

/* fms */
#define MAX_WP 100         // MS 02/28/2005
#define MAX_GP 400         // MS 02/28/2005 
#define NOMINAL_BANK 27.    /* Nominal bank angle in turn [deg] MS 07/07/2004 */
#define BANK_RAD PI*NOMINAL_BANK/180.
/* TURN_CONST = KTS2FPS*3600.0/GRAV, where 3600 = NM2KTS */
/* TURN_CONST = 1.0/(GRAV*FPS2KTS*3600), where 3600 = HRS2SEC => NM */
#define TURN_CONST   0.0000145719 /* Constant used to compute turn radious in nm
                                     from ground speed in kts  MS 07/07/2004 */
#define EARTH_RADIUS 3441.1 /* Earth radius [nm]  MS 07/07/2004 */

#define MAX_FILE_LINE_SIZE  4096

#endif /* PARAMS_H */


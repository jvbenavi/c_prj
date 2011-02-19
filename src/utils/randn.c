/*
 * randn.c
 */

/* Sample from a normal distribution using the polar form of the Box-Muller
 * transformation. Requires a call to a uniform random number generator. This
 * uses mt19937, the mersenne twister.
 */

#include <math.h>
#include "utils.h"

double randn( double m, double s ) { /* mean m, std dev s */

	double X, Y, L, d1;
	static double d2;
	static int have_dev = 0;    /* use the deviate generated on the last call */

	if ( have_dev ) 
	{
		d1 = d2;
		have_dev = 0;
	} 
	else 
	{
		do {
			X = 2. * genrand_real1() - 1.;
			Y = 2. * genrand_real1() - 1.;
			L = X * X + Y * Y;
		} while ( L >= 1. );

		L = sqrt( -2. * log( L ) / L );
		d1 = X * L;
		d2 = Y * L;
		have_dev = 1;
	}

	return ( m + d1 * s );
}

#include "utils.h"

/* output a uniform random number on the interval [a,b] using mt19937 */
/* for a real on interval (0 1) use genrand_real3()                   */
double randu(double a, double b) {
	/* mersenne twister, real on interval [0 1] */
	return (b - a) * genrand_real1() + a;
}

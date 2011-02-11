
#include "func.h"

int globalvar1 = 1;

int func1(int in1, int in2) {

	globalvar1 = in2;

	return (in1 + 2*globalvar1);
}


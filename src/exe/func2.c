
#include <stdio.h>

#include "func.h"

extern int globalvar1;

int func2(int in1, int in2) {

	printf("%d\n",globalvar1);

	return (in1 + 4*in2);

}


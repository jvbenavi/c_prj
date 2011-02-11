#include <stdio.h>

#include "util.h"

extern int globalvar1;

int util_file(const int * const intp)
{
    printf("Inside util_file() \n");
	printf("int value: %d\n",*intp);
	//*intp = 5;
	//intp = &globalvar1;
	//*intp = 13;
	printf("int value: %d\n",*intp);

	return(1);
}


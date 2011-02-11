/**
\file prj_main.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "func.h"

void usage() {
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "   -v : Debug\n");
	fprintf(stderr, "   -h : help (this text)\n");
}

int main(int argc, char **argv) {
	int result;
	int i; 
	int rv;
	int *intp;
	int myint;
	static char uinput[200];

	strcpy(uinput,"");

	myint = 4;
	intp = &myint;

	result = 0;

	/* command line arguments */
	if (argc > 1) {
		for (i=1; i<argc; i++) {
			printf("arg[%d]: %s\n", i, argv[i]);
			strcat(uinput, argv[i]); /* Fill uinput with command line input */
			strcat(uinput, " ");
		}
	} else {
		result = 0;
		usage();
	}

	printf("Inside main() of %s\n", argv[0]);

	// cmnd = icommand("cmnd1 cmnd2 cmnd3",uinput,"");

	/* use a function from each object file that is in the library */
	util_file(intp);
    util_net();
    util_math();

	rv = func1(1, 2);
	printf("%d\n",rv);
	rv = func2(1, 2);
	printf("%d\n",rv);

	return(result);
}

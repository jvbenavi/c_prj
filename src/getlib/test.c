#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getlib.h"

#define cmd1 1
#define cmd2 2
#define cmd3 3

int main(int argc, char **argv) {
	int result = 0;
	int i;
	int cmd;
	static char uinput[1000];

    printf("Inside main()\n");
    
	/* command line arguments */
    printf("%s\n",argv[0]);
	if (argc > 1) {
		for (i=1; i<argc; i++) {
			printf("argv[%d]: %s\n",i,argv[i]);
			strcat(uinput,argv[i]);
			strcat(uinput," ");
		}
	} else {
		result = 1;
		printf("Usage: \n");
	}

	cmd = icommand("cmd1 cmd2 cmd3",uinput,"");
	switch (cmd) {
	  case cmd1:
		printf("this is cmd1\n");
	  break;
	  case cmd2:
		printf("this is cmd2\n");
	  break;
	  case cmd3:
		printf("this is cmd3\n");
	  break;

	}

	return(result);
}

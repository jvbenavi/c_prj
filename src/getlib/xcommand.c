#define _CRT_SECURE_NO_DEPRECATE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getlib.h"
#include "getliba.h"

int xcommand(char prompt[], char uinput[], char a_default[], int restoreUinput )
{
 static int icmnd;
 int status;
 char *tempUinput;

 tempUinput = (char *) calloc(strlen(uinput)+1,sizeof(char));
 strcpy(tempUinput,uinput);

 icmnd = get_Command(prompt,uinput,a_default,0);
 status = get_Error();
 
 if (status==1) icmnd=0;
 if ((status==1)&&(restoreUinput))  strcpy(uinput,tempUinput);

 free(tempUinput);

 return(icmnd);
}


#define _CRT_SECURE_NO_DEPRECATE 

#include <stdlib.h>
#include <stdio.h>
#include "getlib.h"

static char get_cinteger[10];

char *cinteger(int inumber)
{
 sprintf(get_cinteger, "%d", inumber);
 return(get_cinteger);
}

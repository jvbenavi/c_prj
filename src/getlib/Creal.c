#define _CRT_SECURE_NO_DEPRECATE 

#include <stdlib.h>
#include <stdio.h>
#include "getlib.h"

char get_creal[20];

char *cReal( double real)
 {
  sprintf(get_creal,"%g",real);
  return(get_creal);
 }

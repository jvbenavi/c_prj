#define _CRT_SECURE_NO_DEPRECATE 

#include <string.h>
#include "getlib.h"

char get_clogical[4];

char *clogical( int alogical )
 {
  if (alogical)
   strcpy(get_clogical,"Yes");
  else
   strcpy(get_clogical,"No");

  return(get_clogical);
 }

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "getlib.h"
#include "getliba.h"

int icommand(char prompt[], char uinput[], char a_default[] )
{
 static int icmnd
       ;

 do
  {
   icmnd = get_Command(prompt,uinput,a_default,1);
  } while (get_Error());


 return(icmnd);
}



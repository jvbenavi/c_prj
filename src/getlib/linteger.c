#define _CRT_SECURE_NO_DEPRECATE 

#include <string.h>
#include "error.h"
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include "getlib.h"
#include "getliba.h"

unsigned long linteger(char prompt[] , char uinput[], char deft[])
 {
  static int status
        ,cinteger_len
        ,i
        ;
        
  unsigned long ainteger;

  static char cinteger[nb_line_len]
         ,iprompt[prompt_len]
         ,a_default[default_len]
         ,nb_line[nb_line_len]
         ;
 strcpy(a_default,deft);

  strcpy(iprompt,prompt);
  do
   {
    status = 0;
    do
     {
      get_NonblankLine(nb_line,iprompt,uinput,a_default);
     } while ( string_GetItem(cinteger,nb_line)== get_noLine ) ;
    strcpy(uinput,nb_line);

    ainteger = atol(cinteger);
    if (ainteger == 0 )
     {
      cinteger_len = strlen(cinteger);
      for ( i=0 ; ( i <= cinteger_len ) && ( status == 0 ) ; i++ )
       if ( strcspn( cinteger + i ,"0123456789") !=0 )
    {
     status = -1;
     get_SignalGetlibError(get_notAnInt,cinteger);
    }
     }
   } while ( status == -1 );
  return(ainteger);
}

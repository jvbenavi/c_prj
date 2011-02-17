#define _CRT_SECURE_NO_DEPRECATE 

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "getlib.h"
#include "getliba.h"

double real(char prompt[] , char uinput[], char a_default[])
 {
  static int status
        ,creal_len
        ,i
        ;

  static double areal
          ;
  static char creal[nb_line_len]
         ,iprompt[prompt_len]
         ,nb_line[nb_line_len]
         ;

  strcpy(iprompt,prompt);
  do
   {
    status = 0 ;
    do
     {
      get_NonblankLine(nb_line,iprompt,uinput,a_default);
     } while ( string_GetItem(creal,nb_line)== get_noLine ) ;
    strcpy(uinput,nb_line);

    areal = atof(creal);
    if (areal == 0.0 )
     {
      creal_len = strlen(creal);
      for ( i=0 ; ( i <= creal_len ) && ( status == 0 ) ; i++ )
       if ( strcspn( creal + i ,"0123456789.e-+") !=0 )
        {
         status = -1;
         get_SignalGetlibError(get_notAReal,creal);
        }
     }
   } while ( status == -1 );
  return(areal);
}


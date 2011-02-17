#define _CRT_SECURE_NO_DEPRECATE 

#include <math.h> 
#include "getlib.h"
#include "getliba.h"
#include <string.h>

/*char *strcpy(char *, const char *);*/
int strcmpi(char *convert_str, char *compare_str);


int logical(char prompt[] , char uinput[], char a_default[])
 {
  static int alogical
        ;

  static char clogical[nb_line_len]
         ,iprompt[prompt_len]
         ,nb_line[nb_line_len]
         ;

  alogical = 2 ;
  strcpy(iprompt,prompt);
  do
   {
    do
     {
      get_NonblankLine(nb_line,iprompt,uinput,a_default);
     } while ( string_GetItem(clogical,nb_line)== get_noLine ) ;
     strcpy(uinput,nb_line);

     if ( strcmpi( clogical , "y" ) == 0 )		alogical = true;
     if ( strcmpi( clogical , "yes" ) == 0 )	alogical = true;
     if ( strcmpi( clogical , "t" ) == 0 )		alogical = true;
     if ( strcmpi( clogical , "true" ) == 0 )	alogical = true;
     if ( strcmpi( clogical , "on" ) == 0 )		alogical = true;
     if ( strcmpi( clogical , "n" ) == 0 )		alogical = false;
     if ( strcmpi( clogical , "no" ) == 0 )		alogical = false;
     if ( strcmpi( clogical , "f" ) == 0 )		alogical = false;
     if ( strcmpi( clogical , "false" ) == 0 )	alogical = false;
     if ( strcmpi( clogical , "off" ) == 0 )	alogical = false;
     if ( alogical == 2 )
       get_SignalGetlibError(get_notALogical,clogical);
   } while ( alogical == 2 );
  return(alogical);
}

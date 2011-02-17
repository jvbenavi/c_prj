#define _CRT_SECURE_NO_DEPRECATE 

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "getlib.h"
#include "getliba.h"

int get_Hex(int *, char *);

int integer(char prompt[] , char uinput[], char a_default[])
 {
  static int status
        ,cinteger_len
        ,i
        ,ainteger
        ;

  static char cinteger[nb_line_len]
         ,iprompt[prompt_len]
         ,nb_line[nb_line_len]
         ;

  strcpy(iprompt,prompt);
  do
   {
    status = 0;
    do
     {
      get_NonblankLine(nb_line,iprompt,uinput,a_default);
     } while ( string_GetItem(cinteger,nb_line)== get_noLine ) ;
    strcpy(uinput,nb_line);

    ainteger = atoi(cinteger);
    if (ainteger == 0 )
     {
      cinteger_len = strlen(cinteger);
      for ( i=0 ; ( i <= cinteger_len ) && ( status == 0 ) ; i++ ) /* Check to see if cinteger is a zero or an error (from the return of atoi) */
       if ( strcspn( cinteger + i ,"0123456789") !=0 ) status = -1;
       
      if (status == -1) status=get_Hex(&ainteger,cinteger);
      if (status == -1) get_SignalGetlibError(get_notAnInt,cinteger);
     }
   } while ( status == -1 );
  return(ainteger);
}


int get_Hex(int *ainteger, char *cinteger)
{
 int status=0
    ,not_hex=1
    ;

#ifdef NOWARN_UNUSED_PARAM
	cinteger = cinteger;
#endif
 
 *ainteger=10;
 
 if (not_hex) status=-1;
 
 return (status);
}         

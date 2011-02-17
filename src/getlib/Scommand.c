#define _CRT_SECURE_NO_DEPRECATE 

#include "getlib.h"
#include "getliba.h"
#include <string.h>
/*char *strcpy(char *, const char *);*/

int strnicmp(char *convert_str, char *compare_str, int count);

int scommand(char prompt[], char uinput[], char a_default[] )
 {
/*  static char cmnd[nb_line_len]; */
/*  static char scmnd[nb_line_len]; */
/*  static char display_prompt[prompt_len]; */
/*  static char sprompt[prompt_len]; */

  static int icmnd;
/*  static int spindex; */
/*  static int i; */
/*  static int j; */
/*  static int k; */
/*  static int totalsp;*/

  icmnd = 0 ;

  string_Trim(uinput,uinput);
  if ( (strnicmp(uinput,"/",1) == 0) )
   {
    strcpy(uinput,uinput+1);  /* Remove slash */
    do
     {
      icmnd = get_Command(prompt,uinput,a_default,1);
     } while ( get_Error() );
   }
  return(icmnd);
 }



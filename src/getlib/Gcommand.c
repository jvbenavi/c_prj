#define _CRT_SECURE_NO_DEPRECATE 

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "getlib.h"
#include "getliba.h"

int get_Command(char *prompt, char *uinput, char *a_default, int processError )
{
/* static int status;*/
 static char cmnd[nb_line_len]
        ,scmnd[nb_line_len]
        ,display_prompt[prompt_len]
        ,sprompt[prompt_len]
        ,nb_line[nb_line_len]
        ;

 static int icmnd
       ,spindex
       ,i
       ,j
       ,k
       ,totalsp
       ;

 get_errorNumber = get_normal;

 strcpy( display_prompt, "Select one of : " );
 strcat( display_prompt, prompt );
 do
  {
   get_NonblankLine(nb_line,display_prompt,uinput,a_default);
  } while ( ( string_GetItem(cmnd,nb_line)== get_noLine)) ;

 totalsp = 0;

 icmnd = 0;
 strcpy(uinput,nb_line);

 strcpy(scmnd, " ");
 strcat(scmnd, cmnd);

 strcpy( sprompt, " ");
 strcat( sprompt, prompt);
 strcat( sprompt, " ");

 i=string_Index(sprompt,scmnd);

 j=string_Index(sprompt+i+1,scmnd);
 strcat(scmnd," ");
 k = string_Index(sprompt,scmnd);
 if ( ( (j != -1) && (k != -1) ) || (j==-1) )
  {
   if ( i != -1)
    {
     do
      {
       spindex=string_Index(sprompt," ");
       totalsp = totalsp + spindex + 1;
       icmnd++;
       strcpy(sprompt,sprompt + spindex + 1);
      } while (totalsp <= i) ;
    }
   else get_errorNumber = get_notUnique;
  }
 else get_errorNumber = get_noCommand;


 if (processError) if (get_errorNumber != get_normal) get_SignalGetlibError(get_errorNumber,cmnd);


 return(icmnd);
}



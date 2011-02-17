#define _CRT_SECURE_NO_DEPRECATE 

#include <string.h>
#include "getlib.h"
#include "getliba.h"

char *filename(char prompt[], char uinput[], char a_default[])
{
 static char iprompt[prompt_len]
        ,filename[nb_line_len]
        ,nb_line[nb_line_len]
        ;

 strcpy(iprompt,prompt);
 do
  {
   get_NonblankLine(nb_line,iprompt,uinput,a_default);
  } while ( ( string_GetItem(filename,nb_line)== get_noLine)) ;
 strcpy(uinput,nb_line);

  return(filename);
}

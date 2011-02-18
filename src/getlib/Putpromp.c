#define _CRT_SECURE_NO_DEPRECATE 

#include <stdio.h>
#include <string.h>

void get_PutPrompt(char *prompt, char *a_default)
 {
  static char string[1000]; /* (JVB) changing from 255 to 1000 for avoiding memory overwite for
  prompts over 255 */

   if ( strlen(a_default) != 0 )
    sprintf(string,"\n%s [%s] ? ",prompt,a_default);
   else
    sprintf(string,"\n%s ? ",prompt);

  printf("%s",string);

 }

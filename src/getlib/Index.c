#define _CRT_SECURE_NO_DEPRECATE 

#include "getliba.h"
#include <string.h>

/*typedef unsigned int size_t;*/
/*char *strcpy(char *, const char *);*/
char *strlwr(char *convert_str);
/*size_t strlen(const char *);*/
/*size_t strcspn(const char *, const char *);*/


int string_Index(char *string1 , char *pattern1)
 {
  static char string[nb_line_len]
         ,pattern[nb_line_len]
         ,temp[2]
         ;

  static int index
        ,totali
        ,status
        ,i
        ,ii
        ,itemp
        ;

  static size_t pattern_lenght;

  static char *istring;

  status = -1;
  totali = 0;
  strcpy(string,string1);
  strcpy(pattern,pattern1);

  strlwr(string);
  strlwr(pattern);

  pattern_lenght = strlen(pattern);

  if ( strlen(string) >= pattern_lenght )
   {
    istring = string;
    strcpy(temp ," ");

    do
     {
      status = 0;
      *temp = *pattern;
      ii = strcspn(istring,temp);
      istring = istring + ii;
      totali= totali + ii;
      itemp = 0;

      for (i=1;i<(int)pattern_lenght;i++)
       {
        *temp = pattern[i];
        istring++;
        ii = strcspn(istring,temp);
        itemp++;
        if ((ii != 0)||(strlen(istring)==0))
         {
          status = -1 ;
          *temp = *pattern ;
          index = strcspn(istring,temp);
          totali = totali + index + itemp;
          istring = istring + index;
          break ;
         }
       }
     }  while ( strlen(istring) >= pattern_lenght && status != 0);
   }
  if ( status == -1 ) totali = -1;
  return(totali);
 }

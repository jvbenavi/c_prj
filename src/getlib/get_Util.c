#include <stdio.h>
#include <ctype.h>
#include <string.h>
/*
typedef unsigned int size_t;
int strncmp(const char *, const char *, size_t);
*/

#ifndef _WIN32
char *strlwr(char *convert_str);
int strcmpi(char *convert_str, char *compare_str);
int strnicmp(char *convert_str, char *compare_str, int count);

char *strlwr(char *convert_str)
{
 int i = 0;

 while (convert_str[i] != 0)
  {
    if (isupper(convert_str[i]))
      convert_str[i] = convert_str[i] + 32; 
    i++;
  }

 return(convert_str);
  
}

int strcmpi(char *convert_str, char *compare_str)
{

 char *str;

 str = strlwr(&convert_str[0]);
 
 if (str[0] == compare_str[0])
   return(0);
 else
   return(1);

}

int strnicmp(char *convert_str, char *compare_str, int count)
{
 return(strncmp(strlwr(convert_str),strlwr(compare_str),count));
}
#endif

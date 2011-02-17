/*typedef unsigned int size_t;*/
/*size_t strlen(const char *);*/
/*char *strcpy(char *, const char *);*/
#define _CRT_SECURE_NO_DEPRECATE 

#include <string.h>

int strnicmp(char *convert_str, char *compare_str, int count);

int string_Trim( char *destination , char *source )
{ 
 static int length
	   ,status
	   ,i
	   ;

 status = 0;

 length = strlen(source);
 for ( i=0 ; (i <= length) && ( (strnicmp(source + i," ",1) == 0 ) || (strnicmp(source + i,"	",1) == 0 ) ); i++) ;
 strcpy(destination, source + i);
 return(status);
}


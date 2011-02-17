#define _CRT_SECURE_NO_DEPRECATE 

#include <string.h>
#include "getlib.h"
#include "getliba.h"

int string_GetItem(char *item, char *input)
{
 int space
	 ,tab
     ,quote
     /*,comma*/
	 ,status = 0
     ,i
	 ;

 char temp[nb_line_len];
 char Quote[2] = {'"','\0'};

/* Remove spaces and tabs from front of input */
 string_Trim(input,input);

 space = string_Index( input , " " );
 tab = string_Index( input , "	" );
 quote = string_Index( input , Quote );
/* comma = string_Index( input , "," );*/

 /* If no input then set item to NULL*/ 
 if ( (space == -1)&&(tab == -1)&&(quote == -1)/*&&(comma == -1)*/ )
  {
   strcpy(item,"");
   status = get_noItem;
  }
 else
  {
   if (quote==0)
    {
     i=string_Index( input+1 , Quote );
     if (i==-1)
      {
       strcpy(item,"");
       status = get_noQuote;
      }
     else
      {
       strncpy( item , input+1 , i );
       item[i+1] = 0;
       strcpy( temp, input + 2 + i );
       strcpy( input , temp );
      }
    }
   else
    {
     if (tab == -1) tab=10000;
     if (space == -1) space=10000;
/*   if (comma == -1) comma=1000;*/

   /* Find min */
     i=tab;
     if (space<i) i=space;
/*   if (comma<i) i=comma;*/

     strncpy( item , input , i );
     item[i] = 0;
     strcpy( temp, input + i );
     strcpy( input , temp );

    }
  }
  
 return(status);
}



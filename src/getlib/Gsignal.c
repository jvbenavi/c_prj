#include <stdio.h>
#include "error.h"
#include "getliba.h"
#include "getnb.h"

extern FILE *indirect_file_ptr[p_max_indirect_files];
extern int indirect_file_count;

void get_SignalGetlibError(int status, char *message)
{
/* static char warnning[255];*/
/* static char information[255];*/

 char nextLine[nb_line_len];

 int i;
 
 switch (status)
  {
   case  1 : printf("\n$GETLIB-E-FNF , file %s not found.",message);
         break;
   case  2 : printf("\n$GETLIB-E-TOMINF , to many indirect files.");
         break;
   case  3 : printf("\n$GETLIB-E-NOLINE");
         break;
   case  4 : printf("\n$GETLIB-E-NOTUNIQ , Command %s is not unique.",message);
         break;
   case  5 : printf("\n$GETLIB-E-NOCMND , Command %s not found.",message);
         break;
   case  6 : printf("\n$GETLIB-E-??? , Error code 6");
         break;
   case 10 : printf("\n$GETLIB-E-NOPREPROCCMND , Preprocessor does not exist.");
         break;
   case 11 : printf("\n%%%%GETLIB-E-NOTINT, %s is not an integer number.",message);
         break;
   case 12 : printf("\n%%%%GETLIB-E-NOTREAL, %s is not a real number.",message);
         break;
   case 13 : printf("\n%%%%GETLIB-E-NOTLOG, %s is not a logical.",message);
         break;
   case 15 : printf("\n%%%%GETLIB-E-LABNOTF, On_error label not found.");
         break;
   case 16 : printf("\n%%%%GETLIB-E-NOQUOTE, Closing quotation mark missing.");
         break;
  }
    
 if ( indirect_file_count != 0)
  {
   printf("\n$GETLIB-I-CLINF , closing all %d indirect files.",indirect_file_count);

   if ( fgets(nextLine,nb_line_len,indirect_file_ptr[indirect_file_count]) != NULL )
	   printf("\n$GETLIB-I-NextLine , %s",nextLine);
   if ( fgets(nextLine,nb_line_len,indirect_file_ptr[indirect_file_count]) != NULL )
	   printf("$GETLIB-I-NextLine , %s",nextLine);
   if ( fgets(nextLine,nb_line_len,indirect_file_ptr[indirect_file_count]) != NULL )
	   printf("$GETLIB-I-NextLine , %s",nextLine);

   for ( i = 1 ; i < indirect_file_count ; i++)
     fclose(indirect_file_ptr[i]);
   indirect_file_count = 0;
  }
 
 printf("\a\a");

}



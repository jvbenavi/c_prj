#define _CRT_SECURE_NO_DEPRECATE 

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "getlib.h"
#include "getliba.h"
#include "getnb.h"

int get_Preprocessor( char *uinput , FILE *indirect_file_ptr);


extern FILE *indirect_file_ptr[p_max_indirect_files];
extern int indirect_file_count;

int get_NonblankLine(char *nonblankLine, char *prompt, char *input, char *a_default )
{
 static int status
       ,i
       ,nb_line_length   
       ,argument_length
       ,once=1
       ;

 static char file_name[64]
        ,save_a_default[default_len]
        ,save_prompt[prompt_len]        
        ,indirect_file_aurguments[p_max_indirect_files][p_max_indirect_file_arguments][64]
        ,argument_index[2]
        ;
 char nb_line_substring[nb_line_len],preprocessor_input[nb_line_len];
       
 if (once)      
  {
   once = 0;
   get_externalError=0;
   get_flag=0;
  }

 status = get_normal;
 string_Trim(nonblankLine,input);
 nb_line_length = strlen(nonblankLine); 
 
/* Check for external error */
 if (get_externalError)
  {
   get_externalError=0; 
   if ( indirect_file_count != 0)
    {
     printf("\a\n$GETLIB-I-CLINF , closing all %d indirect files.",indirect_file_count);
     for ( i = 1 ; i < indirect_file_count ; i++)
       fclose(indirect_file_ptr[i]);
     indirect_file_count = 0;
    }
  }

/* Get a non blank line (nbl) */
a:
 if ( strlen(nonblankLine) == 0 )
  {
   if (indirect_file_count != 0 )
    {
     do
      {
       if ( get_verbose ) get_PutPrompt(prompt,a_default);
       if ( fgets(nonblankLine,nb_line_len,indirect_file_ptr[indirect_file_count]) == NULL )
        {
         fclose( indirect_file_ptr[indirect_file_count] );
         indirect_file_count-- ;
         goto a ;
        }
       nb_line_length = strlen(nonblankLine);
       if ( (nb_line_length   == 1) & (strlen(a_default)!=0) )
         strcpy(nonblankLine,a_default);
       else
         strcpy(nonblankLine + nb_line_length - 1, "");
       string_Trim(nonblankLine,nonblankLine);
      } while ( strlen(nonblankLine) == 0 );
     if ( get_verbose ) 
      {
       if (get_expandArguments)
        {
         /* Substitute indirect file arguments */
         i = strcspn( nonblankLine , "%" );
         nb_line_length = strlen(nonblankLine); 
         while( i != nb_line_length )
          {      
           strcpy( nb_line_substring , nonblankLine + i + 2 );    
           strncpy( argument_index , nonblankLine + i + 1 , 1 ); 
           argument_index[1] = 0;
           argument_length = strlen(indirect_file_aurguments[indirect_file_count][atoi(argument_index)]);
           strcpy( nonblankLine + i , indirect_file_aurguments[indirect_file_count][atoi(argument_index)] ); 
           strcpy( nonblankLine + i + argument_length , nb_line_substring );
           i = strcspn( nonblankLine , "%" );
           nb_line_length = strlen(nonblankLine); 
          }
        }
       get_PutPrompt(nonblankLine,"");
      }
     /* Substitute indirect file arguments */
     i = strcspn( nonblankLine , "%" );
     nb_line_length = strlen(nonblankLine); 
     while( i != nb_line_length )
      {      
       strcpy( nb_line_substring , nonblankLine + i + 2 );    
       strncpy( argument_index , nonblankLine + i + 1 , 1 ); 
       argument_index[1] = 0;
       argument_length = strlen(indirect_file_aurguments[indirect_file_count][atoi(argument_index)]);
       strcpy( nonblankLine + i , indirect_file_aurguments[indirect_file_count][atoi(argument_index)] ); 
       strcpy( nonblankLine + i + argument_length , nb_line_substring );
       i = strcspn( nonblankLine , "%" );
       nb_line_length = strlen(nonblankLine); 
      }
    }
   else
    {
     do
      {
       get_PutPrompt(prompt,a_default);
       get_Input(nonblankLine);
       nb_line_length = strlen(nonblankLine);
       if ( (nb_line_length == 1) & (strlen(a_default)!=0) )
         strcpy(nonblankLine,a_default);
       else
         strcpy(nonblankLine + nb_line_length - 1, "");
       string_Trim(nonblankLine,nonblankLine);
      } while ( strlen(nonblankLine) == 0 );
      /* Logging code starts here.  It will only log what is type in, not what is executed in scripts */
     if (get_log)
      {
       if (get_logPtr==NULL) get_logPtr=fopen(get_logFilename,"w");
       fprintf(get_logPtr,"%s\n",nonblankLine);
      }         
    }
  }

 /* Open indirect command file */    

 if ( (nonblankLine[0] == '@') || (nonblankLine[0] == '$') )
  {
   indirect_file_count++;
   strcpy(nonblankLine , nonblankLine + 1 );
   status = string_GetItem( file_name , nonblankLine  );
   i=1;
   while ( ( status = string_GetItem( indirect_file_aurguments[indirect_file_count][i] , nonblankLine ) ) != get_noItem )
     i++;
   
   strcpy(nonblankLine,"");
   if ( indirect_file_count > p_max_indirect_files )
    {
     status = get_maxIndirectFiles;
     for ( i = 1 ; i <= p_max_indirect_files ; i++)
       fclose(indirect_file_ptr[i]);
     indirect_file_count = 0;
     get_SignalGetlibError(status,"");
    }
   if (( indirect_file_ptr[indirect_file_count] = fopen(file_name,"r+") ) == NULL )
    {
     status = get_fileNotFound;
     for ( i = 1 ; i < indirect_file_count ; i++)
       fclose(indirect_file_ptr[i]);
     indirect_file_count = 0;
     get_SignalGetlibError(status,file_name);
    }
   goto a;
  }

 /* Execute system command */

  if ( nonblankLine[0] == '~' )
   {
    status = system(nonblankLine + 1);
    if ( status == -1 )
     {
      if ( errno == ENOMEM ) printf("\n$SYS-F-ENOMEM, not enough memory.");
      if ( errno == ENOENT ) printf("\n$SYS-F-ENOENT, cannot find COMMAND.COM.");
     }
    strcpy(nonblankLine, "");
    strcpy(input,"");
    goto a;
   }

 /* Ignore comments */

  if ( (nonblankLine[0] == '!') || (nonblankLine[0] == ':') )
   {
     strcpy(nonblankLine, "");
     strcpy(input,"");
     goto a;
   }

 /* Execute preprocessor command */

  if ( nonblankLine[0] == '#' )
   {
     strcpy(save_a_default,a_default);
     strcpy(save_prompt,prompt);
     strcpy(preprocessor_input,nonblankLine + 1);
     status = get_Preprocessor(preprocessor_input,indirect_file_ptr[indirect_file_count]);
     strcpy(nonblankLine,preprocessor_input);
     strcpy(prompt, save_prompt);
     strcpy(a_default,save_a_default);
     strcpy(input,"");
     goto a;               
   }
         
 return(status);

}

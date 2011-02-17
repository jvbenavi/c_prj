#define _CRT_SECURE_NO_DEPRECATE 

#include <stdio.h>
#include <string.h>
#include "version.h"
#include "preproc.h"
#include "getlib.h"
#include "getliba.h"

int get_Preprocessor( char *uinput , FILE *indirect_file_ptr)
{
 char *afilename 
     ,label[132]
     ,goto_label_line[nb_line_len]
     ,goto_label[132]
     ;
 int cmnd
    ,status
    ,goto_label_line_length
    ;

 status = 0;
 get_preproc = TRUE;

 cmnd=icommand("Verbose Pause Type Bell Expand Open Close On_flag Getlib_version",uinput,"Getlib_version");
 switch (cmnd)
  {
   case 1 : get_verbose = logical("Do you want the script files printed out",uinput,clogical(get_verbose));
            break;
   case 2 : printf("Press any key to continue");
            getchar();
            break;      
   case 3 : printf("\n%s",uinput);
            strcpy(uinput,"");
            break;
   case 4 : printf("\a");
            break;
   case 5 : get_expandArguments = logical("Do you want the pass parameters expanded for display",uinput,clogical(get_expandArguments));
            break;
   case 6 : fclose(get_logPtr);
            afilename = filename("Enter log file name",uinput,get_logFilename);
            strcpy(get_logFilename,afilename);
            get_logPtr = NULL;            
            get_log = TRUE;
            break;
   case 7 : fclose(get_logPtr);
            get_log = FALSE;
            break;
   case 8 : string_GetItem(label,uinput); /* Strip off on_error command */  
            if (string_GetItem(label,uinput)== get_noLine ) 
             {
              get_SignalGetlibError(get_noLabel,"");
              break;
             }
            
            if (get_flag==TRUE)
             {
              do
               {
                do
                 {
                  if ( fgets(goto_label_line,nb_line_len,indirect_file_ptr) == NULL )
                   {
                    get_SignalGetlibError(get_noLabel,"");
                    break;
                   }
                  goto_label_line_length = strlen(goto_label_line);
                  strcpy(goto_label_line + goto_label_line_length - 1, "");
                  string_Trim(goto_label_line,goto_label_line);
                  string_GetItem(goto_label,goto_label_line);
                 } while ( goto_label[0] != ':' );      /* Looking for a label */
               } while (strcmp(goto_label+1,label)==0); /* Check label found */
             }

            break;    
   case 9 : printf(ver);
            break;
   default: status = get_noPreprocCmnd;
            break;
  }
 get_preproc = FALSE;
 return(status);
}

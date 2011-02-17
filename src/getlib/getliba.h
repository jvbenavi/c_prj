#include "error.h"

#define nb_line_len 2048
#define prompt_len 255
#define default_len 50

void get_Input(char *input);  /*** Look this over do I really need it? */

int get_Command(char *prompt, char *uinput, char *a_default, int processError  );
int string_GetItem(char *item, char *input);
void get_PutPrompt(char *prompt, char *a_default);
void get_SignalGetlibError(int status, char *message);
int get_NonblankLine(char *nonblankLine, char *prompt, char *input, char *a_default );
int get_Error();


int string_Trim( char *destination , char *source );
int string_Index(char *string1 , char *template1);

extern int get_errorNumber;
extern int get_externalError;
extern int get_flag;

int icommand( char prompt[] , char uinput[], char a_default[] );
int xcommand(char prompt[], char uinput[], char a_default[], int restoreUinput );
int scommand( char prompt[] , char uinput[], char a_default[] );
int integer( char prompt[] , char uinput[], char a_default[] );
unsigned long linteger( char prompt[] , char uinput[], char a_default[] );
int logical( char prompt[] , char uinput[], char a_default[] );
double real( char prompt[] , char uinput[], char a_default[] );
char *filename( char prompt[] , char uinput[], char a_default[] );
char *filename_ext(char prompt[], char uinput[], char a_default[], char *extension);
char *cReal( double areal );
char *cinteger( int ainteger );
char *clogical( int alogical );

#include "getlib.h"
#include "getliba.h"

int get_Error(void)
{
 static int status;

 status = 0;

 if (get_errorNumber != get_normal) status = 1;

 return(status);
}

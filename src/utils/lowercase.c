#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifndef WIN32
#include <unistd.h>
#endif

/* converts string to lower case */
/* return converted string */
char *lowercase(char *ptr)
{
    char *ps;

    if (!ptr) return ptr;
    
    /* strip off from the front */
    for (ps = ptr; *ps; ps++) 
    {
        if (isalpha(*ps)) *ps=tolower(*ps);
    }

    return ptr;
}



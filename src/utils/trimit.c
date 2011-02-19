/* frpm common/work/sysutil/ltnames.c */
/* 01/23/2003 M. Steglinski Port to Windows */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#ifndef WIN32
#include <unistd.h>
#endif

/* remove whitespace from front and back of a string */
/* assumes that it can work with the passed string   */
/* return trimmed string */
char *trimit(char *ptr)
{
    char *ps, *pe;

    if (!ptr) return ptr;
    /* strip off from the front */
    for (ps = ptr; *ps; ps++) if (!isspace(*ps)) break;
    for (pe = ps; *pe; pe++); pe--;                         /* go to end */
    for (; pe > ps; pe--)     if (!isspace(*pe)) break;
    *(pe + 1) = '\0';

    return ps;
}



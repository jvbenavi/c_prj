/**************************************************************************
 *                                                                        *
 *           Copyright (C) 1995, NASA Ames Neuro-Engineering Team         *
 *                                                                        *
 *  These coded instructions, statements, and computer programs  contain  *
 *  proprietary  information of NASA Ames Neuro-Engineering Team, and     *
 *  are protected by Federal copyright law.  They  may used only for      *
 *  non-commercial purposes unless prior written agreement is obtained    *
 *  from NASA. This copyright notice must be included in any distributed  *
 *  copy of this code.                                                    *
 *                                                                        *
 **************************************************************************/

/*
 *  strsub.c
 */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

char *strsub(char *string, char *search, char *replace)
{
    char *pdest;
    int rlength;
    int slength;
    int i;

    pdest=strstr( string, search );
    if (pdest!=NULL)
    {
        rlength = strlen(replace);
        slength = strlen(search);

        if (slength<rlength)
        {
        }
        else if (slength>rlength)
        {
        }
        else
        {
            for (i=0; i<rlength; i++)
                pdest[i]=replace[i];
        }
    }

    return(string);
}

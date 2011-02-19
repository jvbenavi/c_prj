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
 *  ztable_load.c
 */

/*%
%##  Loads Data Tables (for zlfi*u lookup routines)
%##    John Kaneshige
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-0710
%##    jkaneshige@mail.arc.nasa.gov
%
%##  Major sections include:
%
%##  Modification Log
%##    1. written___6/01|____JTK
%##    2. 1st mod_______|_______
%##    3. 2nd mod_______|_______
%
%##  Comments
%
%*/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global constants */
#include "params.h"

/* function declarations */
#include "utils.h"

#define MAX_VARS 3

float *ztable_load(FILE *fptr, char tblname[])
{
    float *tbl;

    int i;
    int ztable_loaded = FALSE;
    int found_table = FALSE;
    int found_tbl = FALSE;
    int num_vars = 0;
    int found_vars = 0;
    int found_func = FALSE;
    int found_dims = 0;
    int var_dims[MAX_VARS];
    int dcalc;
    int tot_size;
    int num_data = 0;
    int found_data = 0;

    char bufr[132];
    char word[132];
    int slen;
    int wlen;
    char *cptr;             /* inline comment string pointer */
    char *sptr;             /* string pointer */

    /* initialize variable dimension vector */
    for (i=0; i<MAX_VARS; i++) var_dims[i] = 0; 

    while (!ztable_loaded && fgets(bufr, sizeof(bufr), fptr))
    {
        sptr = bufr;
        /* replace inline comment with null character */
        if (cptr = strchr(sptr, '!')) cptr[0] = '\0';
        
        while (!ztable_loaded && (strlen(sptr) > 0))
        {
            /* skip over spaces, tabs, and commas */
            slen = strlen(sptr);
            while (slen >= 0 && (isspace(sptr[0]) || (sptr[0] == ',')))
            {
                sptr++;
                slen--;
            }
            /* get next word */
            wlen = 0;
            while ((wlen < slen) && !(isspace(sptr[wlen]) || (sptr[wlen] == ','))) wlen++;
            strncpy(word, sptr, wlen);
            word[wlen] = '\0';
            sptr += wlen;

            if (wlen <= 0)
            {
                continue;
            }
            /* find desired table */
            else if (!found_table)
            {
                if (strcmp(word, tblname)==0)
                    found_table = TRUE;
                else
                {
				    /* skip table */
                    /*printf("Error parsing table %s\n", tblname);*/
                    /*exit(1);*/
                }
            }
            /* find desired TBL */
            else if (!found_tbl)
            {
                if (strcmp(word, "TBL")==0)
                    found_tbl = TRUE;
                else
                {
                    printf("Error parsing table %s\n", tblname);
                    exit(1);
                }
            }
            /* find number of table variables */
            else if (num_vars == 0)
            {
                num_vars = atoi(word);
            }
            /* skip over variable names */        
            else if (found_vars < num_vars)
            {
                found_vars++;
            }
            /* skip over function name */
            else if (!found_func)
            {
                 found_func = TRUE;
            }
            /* find variable diminsions */
            else if (found_dims < num_vars)
            {
                var_dims[found_dims] = atoi(word);
                found_dims++;
            }
            /* read data */
            else
            {
                /* setup data vector */
                if (num_data == 0)
                {
                    dcalc = 1;
                    for (i=0; i<num_vars; i++)
                    {
                        num_data += var_dims[i];
                        dcalc *= var_dims[i];
                    }
                    num_data += dcalc;
                    tbl = (float *)malloc((unsigned) (1+num_vars+num_data+num_vars)*sizeof(float));
                    tbl[0] = num_vars;
                    for (i=0; i<num_vars; i++) tbl[i+1] = var_dims[i];
                    tbl[1+num_vars+num_data+0] = num_vars + 2;
                    for (i=1; i<num_vars; i++)
                    {
                        tbl[1+num_vars+num_data+i] = tbl[1+num_vars+num_data+i-1]
                                                   + var_dims[i-1];
                    }
                }
                /* get data */
                tbl[1+num_vars+found_data] = (float) atof(word);
                found_data++;
                if (found_data == num_data) ztable_loaded = TRUE;
            }
        }
    }

    if (!ztable_loaded)
    {
        printf("Error parsing table %s\n", tblname);
        exit(1);
    }

    return(tbl);
}

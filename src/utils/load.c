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
 *  load.c
 */

/*%
%##  Load Routines
%##    Don Soloway
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-6558
%##    don@infi.net
%
%##  Major sections include:
%##    load(), loads vector from file
%##    loadm(), loads matrix from file
%##    load3(), loads 3-D matrix from file
%##    load4(), loads 4-D matrix from file
%##    sload(), loads vector from string
%##    sloadm(), loads matrix from string
%##    sload3(), loads 3-D matrix from string
%##    sload4(), loads 4-D matrix from string
%
%##  Modification Log
%##    1. written__??/??|____DIS
%##    2. 1st mod___8/96|____KAL (convert to doubles)
%##    3. 2nd mod___2/01|____JTK (FLTz Version)
%##    4. 3rd mod_______|_______
%
%##  Comments
%##    This program was developed by Don Soloway and Pam Haley at NASA Langley
%##    Research Center.  It is distributed to individuals for their use only and
%##    is not to be distributed to anyone else with out written permission of
%##    NASA Langley.  Any questions or problems with the code should be addressed
%##    to Don Soloway, phone 804-864-6681, e-mail don@isrl6.larc.nasa.gov
%
%*/

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

void load(char *filename, double *vec)
{
    FILE *fptr;
    int row;

    fptr=fopen(filename,"r");
    if (fptr==0)
    {
        printf("load: fptr = %x ; %s ; %x\n",fptr,filename,vec); 
        exit(1);
    }

    for (row=1; row<=vec[0]; row++)
    {
        fscanf(fptr,"%le",&vec[row]);
    }
    fclose(fptr);
}

/****************************************************************************/

void loadm(char *filename, double **mat)
{
    FILE *fptr;
    int row,col;

    fptr=fopen(filename,"r");
    if (fptr==0)
    {
        printf("loadm: fptr = %x ; %s ; %x\n",fptr,filename,mat); 
        exit(1);
    }

    for (row=1; row<=mat[1][0]; row++)
    {
        for (col=1; col<=mat[0][1]; col++)
        {
            fscanf(fptr,"%le",&mat[row][col]);
        }
    }
    fclose(fptr);
}

/****************************************************************************/

void load3(char *filename, double ***mat)
{
    FILE *fptr;
    int i,j,k;
 
    fptr=fopen(filename,"r");
    if (fptr==0)
    {
        printf("load3: fptr = %x ; %s ; %x\n",fptr,filename,mat); 
        exit(1);
    }

    for (i=1; i<=(int)mat[1][0][0]; i++) 
    {
        for (j=1; j<=(int)mat[0][1][0]; j++) 
        {
            for (k=1; k<=(int)mat[0][0][1]; k++) 
            {
                fscanf(fptr,"%le",&mat[i][j][k]);
            }
        }
    }
}

/****************************************************************************/

void load4(char *filename, double ****mat)
{
    FILE *fptr;
    int i,j,k,l;
 
    fptr=fopen(filename,"r");
    if (fptr==0)
    {
        printf("load4: fptr = %x ; %s ; %x\n",fptr,filename,mat); 
        exit(1);
    }

    for (i=1; i<=(int)mat[1][0][0][0]; i++) 
    {
        for (j=1; j<=(int)mat[0][1][0][0]; j++) 
        {
            for (k=1; k<=(int)mat[0][0][1][0]; k++) 
            {
                for (l=1; l<=(int)mat[0][0][0][1]; l++) 
                {
                    fscanf(fptr,"%le",&mat[i][j][k][l]);
                }
            }
        }
    }
}

/****************************************************************************/

void sload(char *string, double *vec)
{
    char *sptr;
    int row;
 
    sptr = string;
    for (row=1; row<=vec[0]; row++)
    {
        while (isspace(sptr[0])) sptr++;
        sscanf(sptr,"%le",&vec[row]);
        while (!isspace(sptr[0])) sptr++;
    }
}

/****************************************************************************/

void sloadm(char *string, double **mat)
{
    char *sptr;
    int row,col;
 
    sptr = string;
    for (row=1; row<=mat[1][0]; row++)
    {
        for (col=1; col<=mat[0][1]; col++)
        {
            while (isspace(sptr[0])) sptr++;
            sscanf(sptr,"%le",&mat[row][col]);
            while (!isspace(sptr[0])) sptr++;
        }
    }
}

/****************************************************************************/

void sload3(char *string, double ***mat)
{
    char *sptr;
    int i,j,k;
 
    sptr = string;
    for (i=1; i<=(int)mat[1][0][0]; i++) 
    {
        for (j=1; j<=(int)mat[0][1][0]; j++) 
        {
            for (k=1; k<=(int)mat[0][0][1]; k++) 
            {
                while (isspace(sptr[0])) sptr++;
                sscanf(sptr,"%le",&mat[i][j][k]);
                while (!isspace(sptr[0])) sptr++;
            }
        }
    }
}

/****************************************************************************/

void sload4(char *string, double ****mat)
{
    char *sptr;
    int i,j,k,l;
 
    sptr = string;
    for (i=1; i<=(int)mat[1][0][0][0]; i++) 
    {
        for (j=1; j<=(int)mat[0][1][0][0]; j++) 
        {
            for (k=1; k<=(int)mat[0][0][1][0]; k++) 
            {
                for (l=1; l<=(int)mat[0][0][0][1]; l++) 
                {
                    while (isspace(sptr[0])) sptr++;
                    sscanf(sptr,"%le",&mat[i][j][k][l]);
                    while (!isspace(sptr[0])) sptr++;
                }
            }
        }
    }
}

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
 *  vector.c
 */

/*%
%##  Vector Routines
%##    Don Soloway
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-6558
%##    don@infi.net
%
%##  Major sections include:
%##    vector(), allocates memory for vector
%##    free_vector(), frees memory for vector
%##    vectorn(), allocates memory for full vector
%##    free_vectorn(), frees memory for full vector
%##    ivector(), allocates memory for integer vector
%##    free_ivector(), frees memory for integer vector
%##    ivectorn(), allocates memory for full integer vector
%##    free_ivectorn(), frees memory for full integer vector
%##    vector_ptr(), allocates memory for vector pointer
%##    free_vector_ptr(), frees memory for vector pointer
%##    vector_set(), sets vector
%##    vector_kronecker(), computes kronecker product
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

double *vector(int low, int high, char *message)
{
    double *vec;

    if ((high-low)<0) return('\0');

    vec = (double *)calloc((unsigned) (high-low+1),sizeof(double)); 

    if (!vec)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    return(vec-low);
}

/*******************************************************************************
 
 Function: void free_vector(double *vec, int low, int high)

 Parameters: vec - pointer to vector to be freed
         low  - Low index for vector
         High - High index for vector

 Description: Free memory allocated to vec with a lenght of high-low.

 Returns: Void

 Call: free

 Local Variables:

*******************************************************************************/

void free_vector(double *vec, int low, int high)
{
#ifdef NOWARN_UNUSED_PARAM
    high = high;
#endif
    free((char*) (vec+low));
} 

/****************************************************************************/

double *vectorn(int high, char *message)
{
    double *vec;
    int i;

    vec = (double *)malloc((unsigned) (high+1+1)*sizeof(double));

    if (!vec)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    vec[0]=(double)(high);

    for (i=1; i<=high; i++)
    {
        vec[i]=0.0F;
    }

    return(vec);
}

/****************************************************************************/

void free_vectorn(double *vec)
{
    free((char*) (vec));
} 
     
/*******************************************************************************
 
 Function: Int *ivector(int low, int high)

 Parameters: low  - Low index for vector
         High - High index for vector

 Description: Allocate memory for a vector lenght high-low, having
          a lower index of low and upper inder of high.

 Returns: A pointer to the allocated vector.

 Call: malloc

 Local Variables: *vec - pointer to malloced vector.

*******************************************************************************/

int *ivector(int low, int high, char *message)
{
    int *vec;

    vec = (int *)malloc((unsigned) (high-low+1)*sizeof(int));

    if (!vec)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    return(vec-low);
}

/*******************************************************************************
 
 Function: void free_ivector(int *vec, int low, int high)

 Parameters: vec - pointer to vector to be freed
         low  - Low index for vector
         High - High index for vector

 Description: Free memory allocated to vec with a lenght of high-low.

 Returns: Void

 Call: free

 Local Variables:

*******************************************************************************/

void free_ivector(int *vec, int low, int high)
{
#ifdef NOWARN_UNUSED_PARAM
    high = high;
#endif
    free((char*) (vec+low));
}

/****************************************************************************/
 
int *ivectorn(int high, char *message)
{
    int *vec;
    int i;

    vec = (int *)malloc((unsigned) (high+1)*sizeof(int));

    if (!vec)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    vec[0]=high;

    for (i=1; i<=high; i++)
    {
        vec[i]=0;
    }

    return(vec);
}
           
/****************************************************************************/

void free_ivectorn(int *vec)
{
    free((char*) (vec));
}

/*******************************************************************************
 
 Function: double **vector_ptr(int low, int high)

 Parameters: low  - Low index for vector
             High - High index for vector

 Description: Allocate memory for a pointer to doubleing vector.  The pointer lenght 
          high-low, having a lower index of low and upper inder of high.

 Returns: A pointer to the allocated array of vector pointers.  Note: one needs to allocate
          the memory for each pointer iin the array.  One could use the vector fuction for this.

 Call: calloc
       NNET_Signal

 Local Variables: **m

*******************************************************************************/

double **vector_ptr(int low, int high, char *message)
{
    double **m;
    int i;

    if ((high-low)<0) return('\0');

    m = (double **) calloc((unsigned) (high-low+1), sizeof(double*));

    if (!m)
       printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    m -= low;

    for (i = low; i <= high; i++)
        m[i]=NULL;

    return m;
}

/****************************************************************************/

void free_vector_ptr(double **m, int row_low, int row_high)
{
    int i;

    for(i=row_high; i>=row_low; i--) 
        free((char*) (m[i]));
}

/****************************************************************************/

double *vector_set(double *out, double value)
{
    int i; 

    if (out==NULL) printf("\n%%UTILS-E-FATAL, %s\n","vector_set - Return pointer is NULL, can't continue.");

    for (i=1; i<=out[0]; i++)
        out[i]=value;  
    return(out);
} 

/****************************************************************************/

int vector_kronecker(double *a, double *b, double *out)
{
    int i, j, k; 

    if ((int)(a[0]*b[0])!=(int)out[0])
        return (1); /* Number of elements in a*b must equal number of elements in c */

    k=0;
    for (i=1; i<=(int)a[0]; i++)
    {
        for (j=1; j<=(int)b[0]; j++)
        {
            out[k] = a[i]*b[j];
            k++;
        }
    }

    return(0);
} 

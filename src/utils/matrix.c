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
 *  matrix.c
 */

/*%
%##  Matrix Routines
%##    Don Soloway
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-6558
%##    don@infi.net
%
%##  Major sections include:
%##    matrix(), allocates memory for matrix
%##    free_matrix(), frees memory for matrix

%##    matrixn(), allocates memory for full matrix
%##    free_matrixn(), frees memory for full matrix
%##    matrix3(), allocates memory for full 3-D matrix
%##    free_matrix3(), frees memory for full 3-D matrix
%##    matrix4(), allocates memory for full 4-D matrix
%##    free_matrix4(), frees memory for full 4-D matrix
%##    imatrix(), allocates memory for integer matrix
%##    free_imatrix(), frees memory for integer matrix
%##    imatrixn(), allocates memory for full integer matrix
%##    free_imatrixn(), frees memory for full integer matrix
%##    matrix_set(), sets matrix
%##    matrix_set3(), sets 3-D matrix
%##    matrix_set4(), sets 4-D matrix
%##    matrix_add(), matrix addition
%##    matrix_sub(), matrix subtraction
%##    matrix_mult(), matrix multiplication
%##    Matrix_VecMult(),  matrix vec multiplication
%##    matrix_inv(), matrix inversion
%##    ludcmp(), ludcmp
%##    lubksb(), lubksb
%
%##  Modification Log
%##    1. written__??/??|____DIS
%##    2. 1st mod___8/96|____KAL (convert to doubles)
%##    3. 2nd mod___2/01|____JTK (FLTz Version)
%##    4. 3rd mod_______|_______
%##    5. 4th mod__12/08|____JVB (added Matrix_VecMult)
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

/*******************************************************************************
 
 Function: double **matrix(int low, int high)

 Parameters: low  - Low index for vector
         High - High index for vector

 Description: Allocate memory for a vector length high-low, having
          a lower index of low and upper inder of high.

 Returns: A pointer to the allocated vector.

 Call: malloc

 Local Variables: *vec - pointer to malloced vector.

*******************************************************************************/

double **matrix(int row_low, int row_high, int col_low, int col_high, char *message)
{
    int i;
    double **m;

    m = (double **) malloc((unsigned) (row_high-row_low+1)*sizeof(double*));

    if (!m)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    m -= row_low;

    for (i = row_low; i <= row_high; i++)
    {
        m[i] = (double *) malloc((unsigned) (col_high-col_low+1)*sizeof(double));

        if (!m[i])
            printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

        m[i] -= col_low;
    }

    return m;
}

/*******************************************************************************
 
 Function: void free_matrix(double *vec, int low, int high)

 Parameters: vec - pointer to vector to be freed
         low  - Low index for vector
         High - High index for vector

 Description: Free memory allocated to vec with a lenght of high-low.

 Returns: Void

 Call: free

 Local Variables:

*******************************************************************************/

void free_matrix(double **m, int nrl, int nrh, int ncl, int nch)
{
    int i;

#ifdef NOWARN_UNUSED_PARAM
    nch = nch;
#endif
    for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
    free((char*) (m+nrl));
}

/****************************************************************************/

double ***matrix3(int d1, int d2, int d3, char *message)
{
	double ***m;
	int i,j;

	m = (double ***) malloc((unsigned)(d1+1) *sizeof(double**));
	for(i=0; i < d1+1;  i++ )
	{
		m[i] = (double **) malloc((unsigned)(d2+1) *sizeof(double*));
		for(j=0; j < d2+1;  j++ )
		{
			m[i][j] = (double *) malloc((unsigned)(d3+1) *sizeof(double));
		}
	}

	m[0][0][0] = -9999.9999;   /*i think this slot is never,ever used - so mark it wildly */
	m[1][0][0]=(double)d1;
	m[0][1][0]=(double)d2;
	m[0][0][1]=(double)d3;

	return m;
}

/****************************************************************************/

void free_matrix3(double ***m)
{
	int i=0,j=0;
	int d1, d2;
  
	d1 = m[1][0][0];
	d2 = m[0][1][0];
  
	for(i=0; i < d1+1;  i++ )
	{
		for(j=0; j < d2+1;  j++ )
		{
			free( m[i][j] );
		}
		free( m[i] );
	}
	free( m );
}

/****************************************************************************/

double ****matrix4(int d1, int d2, int d3, int d4, char *message)
{
	double ****m;
	int i,j,k;

	m = (double ****) malloc((unsigned)(d1+1) *sizeof(double***));
	for(i=0; i < d1+1;  i++ )
	{
		m[i] = (double ***) malloc((unsigned)(d2+1) *sizeof(double**));
		for(j=0; j < d2+1;  j++ )
		{
			m[i][j] = (double **) malloc((unsigned)(d3+1) *sizeof(double*));
			for(k=0; k < d3+1;  k++ )
			{
				m[i][j][k] = (double *) malloc((unsigned)(d4+1) *sizeof(double));
			}
		}
	}

	m[0][0][0][0] = -9999.9999;   /*i think this slot is never,ever used - so mark it wildly */
	m[1][0][0][0]=(double)d1;
	m[0][1][0][0]=(double)d2;
	m[0][0][1][0]=(double)d3;
	m[0][0][0][1]=(double)d4;

	return m;
}

/****************************************************************************/

void free_matrix4(double ****m)
{
	int i=0,j=0,k=0;
	int d1, d2, d3;
  
	d1 = m[1][0][0][0];
	d2 = m[0][1][0][0];
	d3 = m[0][0][1][0];
  
	for(i=0; i < d1;  i++ )
	{
		for(j=0; j < d2;  j++ )
		{
			for(k=0; k < d3;  k++ )
			{
				free( m[i][j][k] );
			}
			free( m[i][j] );
		}
		free( m[i] );
	}
	free( m );
}

/****************************************************************************/

double **matrixn(int row_high, int col_high, char *message)
{
    double **m;

    m=matrix(0,row_high,0,col_high,message);

    m[1][0]=(double)row_high;
    m[0][1]=(double)col_high;
    
    return m;
}

/****************************************************************************/

void free_matrixn(double **m)
{
    free_matrix(m, 0, (int) m[1][0], 0, (int) m[0][1]);
}

/****************************************************************************/

int **imatrix(int row_low, int row_high, int col_low, int col_high, char *message)
{
    int i;
    int **m;

    m = (int **) malloc((unsigned) (row_high-row_low+1)*sizeof(int*));

    if (!m)
        printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

    m -= row_low;

    for (i = row_low; i <= row_high; i++)
    {
        m[i] = (int *) calloc((unsigned) (col_high-col_low+1),sizeof(int));

        if (!m[i])
            printf("\n%%UTILS-E-NOMEM, not enough memory, Called from %s\n",message);

        m[i] -= col_low;
    }

    return m;
}

/****************************************************************************/

void free_imatrix(int **m, int row_low, int row_high, int col_low)
{
    int i;

    for(i=row_high;i>=row_low;i--) free((char*) (m[i]+col_low));
    free((char*) (m+row_low));
}

/****************************************************************************/

int **imatrixn(int row_high, int col_high, char *message)
{
    int **m;
    int i;

    m=imatrix(0,row_high,0,col_high,message);

    for (i=1; i<=col_high; i++)
        m[i][0]=row_high;

    for (i=1; i<=row_high; i++)
        m[0][i]=col_high;

    return m;
}

/****************************************************************************/

void free_imatrixn(int **m)
{
    free_imatrix(m, 0, m[1][0], 0);
}                         
                                                     
/****************************************************************************/

int matrix_set(double **out, double value)
{
    int row,col;

	for (row=1; row<=out[1][0]; row++)
	{
		for (col=1; col<=out[0][1]; col++)
		{
			out[row][col] = value;
		}
	}

    return(0);
}

/****************************************************************************/

int matrix_set3(double ***out, double value)
{
	int i,j,k;

	for (i=1; i<=out[1][0][0]; i++) 
	{
		for (j=1; j<=out[0][1][0]; j++) 
		{
			for (k=1; k<=out[0][0][1]; k++)
			{
				out[i][j][k] = value;
			}
		}
	}

    return(0);
}


/****************************************************************************/

int matrix_set4(double ****out, double value)
{
	int i,j,k,l;

	for (i=1; i<=out[1][0][0][0]; i++) 
	{
		for (j=1; j<=out[0][1][0][0]; j++) 
		{
			for (k=1; k<=out[0][0][1][0]; k++)
			{
				for (l=1; l<=out[0][0][0][1]; l++)
				{
					out[i][j][k][l] = value;
				}
			}
		}
	}

    return(0);
}

/****************************************************************************/

int matrix_add(double **a, double **b, double **c)
{
    /* a=b+c */

    int row,col;

    if ((int)b[1][0]!=(int)c[1][0])
        return (1); /* Number of elements of a row in b must equal number of elements in a row of c */

    if ((int)b[0][1]!=(int)c[0][1])
        return (2); /* Number of elements of a column in b must equal number of elements in a column of c */

    if ((int)a[1][0]!=(int)c[1][0])
        return (3); /* Number of elements of a row in a must equal number of elements in a row of c */

    if ((int)a[0][1]!=(int)c[0][1])
        return (4); /* Number of elements of a column in a must equal number of elements in a column of c */

    for ( row = 1 ; row<=(int)b[1][0] ; row++)
        for ( col = 1 ; col<=(int)b[0][1] ; col++)
            a[row][col] = b[row][col]+c[row][col];

    return(0);
}

/****************************************************************************/

int matrix_sub(double **a, double **b, double **c)
{
    /* a=b-c */

    int row,col;

    if ((int)b[1][0]!=(int)c[1][0])
        return (1); /* Number of elements of a row in b must equal number of elements in a row of c */

    if ((int)b[0][1]!=(int)c[0][1])
        return (2); /* Number of elements of a column in b must equal number of elements in a column of c */

    if ((int)a[1][0]!=(int)c[1][0])
        return (3); /* Number of elements of a row in a must equal number of elements in a row of c */

    if ((int)a[0][1]!=(int)c[0][1])
        return (4); /* Number of elements of a column in a must equal number of elements in a column of c */

    for ( row = 1 ; row<=(int)b[1][0] ; row++)
        for ( col = 1 ; col<=(int)b[0][1] ; col++)
            a[row][col] = b[row][col]-c[row][col];

    return(0);
}

/****************************************************************************/

int matrix_mult(double **a, double **b, double **c)
{
    int i,row,col;

    if ((int)b[0][1]!=(int)c[1][0])
        return (1); /* Number of columns in b must equal number of rows in c */

    if ((int)a[1][0]<(int)b[1][0])
        return (2); /* Index out of matrix a's row */

    if ((int)a[0][1]<(int)c[0][1])
        return (3); /* Index out of matrix a's column */

    for ( row = 1 ; row<=(int)b[1][0] ; row++)
        for ( col = 1 ; col<=(int)c[0][1] ; col++)
        {
            a[row][col]=0.0F;
            for ( i = 1 ; i<=(int)b[0][1] ; i++)
                a[row][col] = a[row][col] + b[row][i]*c[i][col];
        }

    return(0);
}

/****************************************************************************/
/** FIXME - bad return values on error conditions */
double *Matrix_VecMult(double *a,double **b,double *c)
{
        int i,row;

        if ((int)b[1][0]!=(int)c[0])
                printf("Matrix_VecMult - Number of columns in B must equal number of rows in C");

        if ((int)a[0]!=(int)b[0][1])
                printf("Matrix_VecMult - Number of rows in A must does not equal number of columns in B");

        for ( row = 1 ; row<=(int)b[0][1] ; row++)
        {
                a[row]=0.0;
                for ( i = 1 ; i<=(int)b[1][0] ; i++)
                        a[row] = a[row] + b[row][i]*c[i];
        }

        return(a);
}

/****************************************************************************/

int matrix_inv(double **minv, double **m)
{
    int i,j,*indx;
    double d
        ,**a
        ,*col
        ;

    if ((int)m[0][1]!=(int)m[1][0])
        return (1); /* m needs to be square */

    indx=ivector(1,(int)m[0][1],"inv - indx");
    col=vector(1,(int)m[0][1],"inv - col");
    a=matrix(1,(int)m[1][0],1,(int)m[0][1],"inv - a");

    for (i=1; i<=(int)m[0][1]; i++)
        for (j=1; j<=(int)m[1][0]; j++)
            a[i][j]=m[i][j];

    /* From Numerical Recipes in C */
    ludcmp(a,(int)m[0][1],indx,&d);

    for (j=1; j<=(int)m[0][1]; j++)
    {
        for (i=1; i<=(int)m[0][1]; i++)
            col[i]=0.0F;
        col[j]=1.0F;
        lubksb(a,(int)m[0][1],indx,col);
        for (i=1; i<=(int)m[0][1]; i++)
            minv[i][j]=col[i];
    }

    free_ivector(indx,1,(int)m[0][1]);
    free_vector(col,1,(int)m[0][1]);
    free_matrix(a,1,(int)m[0][1],1,(int)m[1][0]);

    return(0);
}

/****************************************************************************/

#define TINY 1.0e-20;

void ludcmp(double **a, int n, int *indx, double *d)
{
    int i,imax,j,k;
    double big,dum,sum,temp;
    static double *vv;
    static int once=1;

    if (once)
    {
        vv=vector(1,n,"ludcmp");
        once=0;
    }

    *d=1.0;
    for (i=1;i<=n;i++)
    {
        big=0.0;
        for (j=1;j<=n;j++)
            if ((temp=fabs(a[i][j])) > big) big=temp;
        if (big == 0.0) printf("Singular matrix in routine LUDCMP\n");
        vv[i]=1.0/big;
    }
    for (j=1;j<=n;j++)
    {
        for (i=1;i<j;i++)
        {
            sum=a[i][j];
            for (k=1;k<i;k++) sum -= a[i][k]*a[k][j];
            a[i][j]=sum;
        }
        big=0.0;
        for (i=j;i<=n;i++)
        {
            sum=a[i][j];
            for (k=1;k<j;k++)
                sum -= a[i][k]*a[k][j];
            a[i][j]=sum;
            if ( (dum=vv[i]*fabs(sum)) >= big)
            {
                big=dum;
                imax=i;
            }
        }
        if (j != imax)
        {
            for (k=1;k<=n;k++)
            {
                dum=a[imax][k];
                a[imax][k]=a[j][k];
                a[j][k]=dum;
            }
            *d = -(*d);
            vv[imax]=vv[j];
        }
        indx[j]=imax;
        if (a[j][j] == 0.0) a[j][j]=TINY;
        if (j != n)
        {
            dum=1.0/(a[j][j]);
            for (i=j+1;i<=n;i++) a[i][j] *= dum;
        }
    }
    /*free_vector(vv,1,n);*/
}

#undef TINY

/****************************************************************************/

void lubksb(double **a, int n, int *indx, double *b)
{
    int i,ii=0,ip,j;
    double sum;

    for (i=1;i<=n;i++)
    {
        ip=indx[i];
        sum=b[ip];
        b[ip]=b[i];
        if (ii)
            for (j=ii;j<=i-1;j++) sum -= a[i][j]*b[j];
        else if (sum) ii=i;
        b[i]=sum;
    }
    for (i=n;i>=1;i--)
    {
        sum=b[i];
        for (j=i+1;j<=n;j++) sum -= a[i][j]*b[j];
        b[i]=sum/a[i][i];
    }
}

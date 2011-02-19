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

/*! 
 Author       
 -     Jonathan S. Barlow
 -     MS: 269-1
 -     Moffett Field, CA. 94035
 -     (650) 604-3378
 -     jonathan.s.barlow@nasa.gov
 
 Major sections include:
 -	matrix_subtraction
 -	postmultiply_diagonal_matrix
 -	inverse_matrix_nXn
 Modification Log
 -     1. written__11/08|____JSB 
 -     2. 1st mod__12/08|____JSB - Add inverse_matrix_nXn and sub functions
 -     3. 2nd mod_______|_______
 
 Comments
 -    
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* Local Function Definitions */
void cholesky ( double OrigMat[], double UMat[], int *fault , int n );
void forward ( double UMat[], double Result[], double RightMat[], int RightMatRows, int RightMatCols);
void backsubs ( double UMat[], double Result[], double RightMat[], int RightMatRows, int RightMatCols);
void lutxFullPivot ( double OrigMat[], double L[], double U[], double RightMat[], double LeftMat[], int OMatRows, int OMatCols, int RMatRows, int RMatCols, int LMatRows, int LMatCols);
void max_sub_matrix ( double Mat[], int max_row[], int max_col[], int NumRows, int NumCols, int MatCols, int StartRow, int StartCol);
void swap_row_matrix ( double Mat[], int row1, int row2, int OMatCols);
void swap_col_matrix ( double Mat[], int col1, int col2, int OMatCols, int OMatRows);


/**************************************************************************************/
/* The Remainder of this file is matrix utilities for row major matrices used by RLS */
/**************************************************************************************/

/**************************************************************************************/
void matrix_subtraction(double FirstMat[], double SecMat[], double AddMat[], int FMRows, int FMCols )
{
	int i, j, k;
	
	for(i = 0; i < FMRows; i = i + 1)
		for(j = 0; j < FMCols; j = j + 1)
			AddMat[i * FMCols + j] = FirstMat[i * FMCols + j] - SecMat[i * FMCols + j];
	

	return;
}

void postmultiply_diagonal_matrix(double FirstMat[], double DiagMat[], double Product[], int FMRows, int FMCols )
{
	int i, j, k;
	
	for(i = 0; i < FMRows; i = i + 1)
		for(j = 0; j < FMCols; j = j + 1)
			Product[i * FMCols + j] = FirstMat[i * FMCols + j] * DiagMat[j];
	
	return;
}

void copy_sub_matrix(double OrigMat[], double ToMat[], int OMCols, int TMCols, int NumRows, int NumCols, int ORowStart, int OColStart, int TRowStart, int TColStart )
{
	/* Copies a sub-matrix of size NumRows by NumCols from OrigMat to ToMat.
	   The widths of OrigMat and ToMat are OMCols and TMCols, respectively.
	   (ORowStart,OColStart) and (TRowStart,TColStart) are the starting indices for OrigMat and ToMat respectively. */
	int i,j,k;
	
	for(i = 0; i < NumRows; i = i + 1)
		for(j = 0; j < NumCols; j = j + 1)
			ToMat[ ( i + TRowStart ) * TMCols + j + TColStart ] = OrigMat[ ( i + ORowStart ) * OMCols + j + OColStart];
			
	return;
}

void generic_sub_matrix_multiplication(double FirstMat[], double SecMat[], double ReturnMat[], int FMCols, int SMCols, int NumRows, int NumFMCols, int NumSMCols, int FMRowStart, int FMColStart, int SMRowStart, int SMColStart )
{
	/* This routine has no error checking for matrix sizes. Make sure that matrices are appropriately allocated */
	
	int i, j, k;
	
	/* Perform the multiplication */
	for(i = 0; i < NumRows; i = i + 1)
	{
		for( j = 0; j < NumSMCols; j = j + 1)
		{
			ReturnMat[i * NumSMCols + j] = 0.0;
			for( k = 0; k< NumFMCols; k = k + 1)
			{
				ReturnMat[ i * NumSMCols + j ] += ( FirstMat[ ( i + FMRowStart ) * FMCols + k + FMColStart ] * SecMat[ ( k + SMRowStart ) * SMCols + j + SMColStart ] ) ;
			}
		}
	}
	
	return;
}


void inverse_matrix_nXn(double OrigMat[], double MatInv[] , int n) 
{
	double *UMat = malloc(n*n*sizeof(double));
	double *IMat = malloc(n*n*sizeof(double));
	double *YMat = malloc(n*n*sizeof(double));
	double *RightMat = malloc(n*n*sizeof(double));
	double *LeftMat = malloc(n*n*sizeof(double));
	double *L = malloc(n*n*sizeof(double));
	double *U = malloc(n*n*sizeof(double));
	double div;
	double cond;
	double normMat;
	double normMatInv;
	double normSum;
	double normSumInv;
	int fail,i,j,pd;
	
	if( UMat == NULL || IMat == NULL || YMat == NULL || RightMat == NULL || LeftMat == NULL || L == NULL || U == NULL )
	{
	fprintf(stdout,"Memory Allocation Failed, inverse_matrix_nXn");
	exit(1);
	}
	
	for( i = 0 ; i < n ; i++)/*initialize matrices*/
	{
		for( j = 0 ; j < n ; j++)
		{
			IMat[i*n +j] = 0.0;
			RightMat[i*n +j] = 0.0;
			LeftMat[i*n +j] = 0.0;
			UMat[i*n +j] = 0.0;
			YMat[i*n +j] = 0.0;
			if(i==j)/*initialize matrices*/
			{
				IMat[i*n +j] = 1.0;
				RightMat[i*n +j] = 1.0;
				LeftMat[i*n +j] = 1.0;
			}
		}
	}
	
	/* [R,fail] = chol(A); */
	cholesky ( OrigMat, UMat, &fail , n );
	if( !fail )
	{
		/* Positive definite */
        pd=1;
		
		/* YMat = forward(UMat,IMat); UMat is upper triangular, although normally it is written UMat' */
        forward(UMat,YMat,IMat,n,n);
		
		/*MatInv = backsubs(UMat,YMat); */
		backsubs(UMat,MatInv,YMat,n,n);
		
	}
	else
	{
		pd=-1;
		lutxFullPivot ( OrigMat, L, U, RightMat, LeftMat, n, n, n, n, n, n);
		/*YMat = forward(L,RightMat); */
		forward(L,YMat,RightMat,n,n);
		
		/*RightMat = backsubs(UMat,YMat); RightMat used as temporary variable */
		backsubs(U,RightMat,YMat,n,n);
		
		/*MatInv = LeftMat*RightMat; */
		generic_matrix_multiplication( LeftMat , RightMat , MatInv , n , n , n );
	}
	/*This is error checking to see if the inverse works*/
	normMat = 0.0;
	normMatInv = 0.0;
	for( i = 0 ; i < n ; i++ )
	{
		normSum = 0.0;
		normSumInv = 0.0;
		for( j = 0 ; j < n ; j++ )
		{
			normSum = normSum + fabs( OrigMat[ i * n + j ] );
			normSumInv = normSumInv + fabs( MatInv[ i * n + j ] );
		}
		if( normSum > normMat )
		{
			normMat = normSum;
		}
		if( normSumInv > normMatInv )
		{
			normMatInv = normSumInv;
		}
	}
	cond = normMat * normMatInv ;
	if(cond > 1.0E+12)/*This is based on the Matlab tolerance for the pinv function.*/
	{
		fprintf(stdout,"Warning: Matrix ill-conditioned, condition number=%f, inverse may be inaccurate, testing inverse\n",cond);
		generic_matrix_multiplication( OrigMat , MatInv , YMat , n , n , n );
		
		for( i = 0 ; i < n*n ; i++)
		{
			div=1;
			if( fabs( OrigMat[ i ] ) > fabs( MatInv[ i ] ) )
			{ 
				if( fabs( OrigMat[ i ] ) > 1) div = OrigMat[ i ];
				else div = 1.0;
			}
			else
			{
				if( fabs( MatInv[ i ] ) > 1) div = MatInv[i];
				else div = 1.0;
			}
			if( (fabs((IMat[i] - YMat[i])/div)>1e-8) || !(MatInv[i]==MatInv[i]))
			{
				fprintf(stdout,"Matrix Inverse Failed\n");
				if(!(MatInv[i]==MatInv[i]))
				{
					fprintf(stdout,"NaN detected\n");
				}
				else
				{
					fprintf(stdout,"Ident=%f, Prod=%f, Orig=%f, Inv=%f, Diff=%f, div=%f, Diff/div=%f, Value=%f, pd=%d\n",IMat[i]*1e9,YMat[i]*1e9,OrigMat[i]*1e9,MatInv[i]*1e9,(IMat[i] - YMat[i]),div,(IMat[i] - YMat[i])/div,fabs((IMat[i] - YMat[i])/div)*1e9,pd);
				}
				print_matrix(OrigMat,n,n,"Original");
				print_matrix(MatInv,n,n,"Inverse");
				print_matrix(YMat,n,n,"Product");
				exit(1);
			}
		}
	}	
	free(UMat);
	free(IMat);
	free(YMat);
	free(RightMat);
	free(LeftMat);
	free(L);
	free(U);
}

void forward ( double UMat[], double Result[], double RightMat[], int RightMatRows, int RightMatCols)
{
	/*    FORWARD forward solves UMat'*Result=RightMat                                   */
	/*    UMat is n by n upper triangular and square, RightMat is n by any size			 */
	/*    Result is the same size as RightMat											 */
	/*    Written using UMat upper triangular instead of LMat lower triangular to		 */
	/*	  simplify variable passing from inverse_matrix_nXn								 */
	int k, j, i;
	double temp;
	copy_matrix( RightMat , Result , RightMatRows , RightMatCols );
	for( k = 0 ; k < RightMatRows ; k++ )
	{
		for( i = 0 ; i < RightMatCols ; i++ )
		{
			temp=0.0;
			for( j = 0 ; j < k ; j++)
			{
				temp = temp + UMat[j*RightMatRows+k]*Result[j*RightMatCols+i];
			}
			Result[k*RightMatCols+i] = (Result[k*RightMatCols+i] - temp)/UMat[k*RightMatRows+k];
		}
	}
}

void backsubs ( double UMat[], double Result[], double RightMat[], int RightMatRows, int RightMatCols)
{
	/*    BACKSUBS backsolves UMat*Result=RightMat										 */
	/*    UMat is upper triangular and square, RightMat is n by any size				 */
	/*    Result is the same size as RightMat											 */
	/*    Written for upper triangular for inverse_matrix_nXn							 */
	int k, j, i;
	double temp;
	copy_matrix( RightMat , Result , RightMatRows , RightMatCols );
	for( k = RightMatRows-1 ; k >= 0 ; k-- )
	{
		for( i = 0 ; i < RightMatCols ; i++ )
		{
			temp=0.0;
			for( j = k+1 ; j < RightMatRows ; j++)
			{
				temp = temp + UMat[k*RightMatRows+j]*Result[j*RightMatCols+i];
			}
			Result[k*RightMatCols+i] = (Result[k*RightMatCols+i] - temp)/UMat[k*RightMatRows+k];
		}
	}	
}

void lutxFullPivot ( double OrigMat[], double L[], double U[], double RightMat[], double LeftMat[], int OMatRows, int OMatCols, int RMatRows, int RMatCols, int LMatRows, int LMatCols)
{
	int i,j,k;
	int max_row[1];
	int max_col[1];
	double Mat[ OMatRows * OMatCols ];
	copy_matrix( OrigMat, Mat, OMatRows, OMatCols);
	for( k = 0 ; k < OMatCols - 1 ; k++)
	{
		/*Find largest element below diagonal in k-th column*/
		max_sub_matrix ( Mat, max_row, max_col, OMatRows-k, OMatCols-k, OMatCols, k, k);
		
		/*Skip elimination if column is zero*/
		if( Mat[max_row[0] * OMatCols + max_col[1] ] != 0 )
		{
			/*Swap pivot row*/
			
			if( k != max_row[0] )
			{
				swap_row_matrix ( Mat, k, max_row[0], OMatCols);
				swap_row_matrix ( RightMat, k, max_row[0], RMatCols);
			}
			/*Swap pivot column*/
			
			if( k != max_col[0] )
			{
				swap_col_matrix ( Mat, k, max_col[0], OMatCols, OMatRows);
				swap_col_matrix ( LeftMat, k, max_col[0], LMatCols, LMatRows);
			}
			/*Compute Multipliers*/
			for( i = k + 1 ; i < OMatCols ; i++)
			{
				Mat[ i * OMatRows + k ] = Mat[ i * OMatRows + k ] / Mat[ k * OMatRows + k ];
				for ( j = k + 1 ; j < OMatCols ; j++)
				{
					/*Update the remainder of the matrix*/
					Mat[ i * OMatRows + j ] = Mat[ i * OMatRows + j ] - Mat[ i * OMatRows + k ] * Mat[ k * OMatRows + j ];
				}
			}
		}
	}
	/*Separate result*/
	for( i = 0 ; i < OMatRows ; i++)
	{
		for( j = 0 ; j < OMatCols ; j++)
		{
			if( j > i )
			{
				U[i*OMatRows + j]=Mat[i*OMatRows + j];
				L[j*OMatRows + i]=0.0;/*written for L upper-triangular*/
			}
			else
			{
				if(i==j)
				{
					U[i*OMatRows + j]=Mat[i*OMatRows + j];
					L[j*OMatRows + i]=1.0;/*written for L upper-triangular*/
				}
				else
				{
					U[i*OMatRows + j]=0.0;
					L[j*OMatRows + i]=Mat[i*OMatRows + j];/*written for L upper-triangular*/
				}
			}
		}
	}
}

void swap_row_matrix ( double Mat[], int row1, int row2, int OMatCols)
{
	int i;
	double temp;
	for( i = 0 ; i < OMatCols ; i++ )
	{
		temp=Mat[row1 * OMatCols + i];
		Mat[row1 * OMatCols + i] = Mat[row2 * OMatCols + i];
		Mat[row2 * OMatCols + i] = temp;
	}
}

void swap_col_matrix ( double Mat[], int col1, int col2, int OMatCols, int OMatRows)
{
	int i;
	double temp;
	for( i = 0 ; i < OMatRows ; i++ )
	{
		temp=Mat[i * OMatCols + col1];
		Mat[i * OMatCols + col1] = Mat[i * OMatCols + col2];
		Mat[i * OMatCols + col2] = temp;
	}
}

void max_sub_matrix ( double Mat[], int max_row[], int max_col[], int NumRows, int NumCols, int MatCols, int StartRow, int StartCol)
{
/*	Finds the maximum value of a sub-matrix.  Written for any continuous rectangular*
*	sub-matrix of Mat.  Mat is MatCols wide, and the submatrix is NumRows by NumCols*
*	starting at the point defined by StartRow and StartCol.  The row and column of	*
*	the maximum value are returned in max_row and max_col.							*/
	int i,j,k;
	int max;
	
	max_row[0]=StartRow;
	max_col[0]=StartCol;
	for(i = 0; i < NumRows; i = i + 1)
		for(j = 0; j < NumCols; j = j + 1)
		{
			if(Mat[ ( i + StartRow ) * MatCols + j + StartCol]>max)
			{
				max=Mat[ ( i + StartRow ) * MatCols + j + StartCol];
				max_row[0]=StartRow;
				max_col[0]=StartCol;
			}
		}			
	return;
}


void cholesky ( double OrigMat[], double UMat[], int *fault , int n)

/*    CHOLESKY computes the Cholesky factorization of a matrix.						*
*    For a positive definite symmetric matrix A, UMat is an upper triangular matrix	*
*    such that A = U' * U.															*/
{
	double accuracy = 1.0E-15;/*9;*/
	int i;
	int col, row;
	double w;
	double x;
	
	*fault = 0;
		
  	/* col=column number */
	for ( col = 0; col < n; col++ )
	{
		x = accuracy * accuracy * OrigMat[col*n+col];
		/* row=row number */
		for ( row = 0; row <= col; row++ )
		{
			w = OrigMat[row*n+col];
			
			for ( i = 0; i < row; i++ )
			{
				w = w - UMat[i*n+row] * UMat[i*n+col];
			}
			
			if ( row == col ) 
			{
				break;
			}
			
			if ( UMat[row*n+row] != 0.0 )
			{
				UMat[row*n+col] = w / UMat[row*n+row];
			}
			else
			{
				UMat[row*n+col] = 0.0;
				
				if ( fabs( x * OrigMat[row*n+col] ) < w * w )
				{
					*fault = 2;
					return;
				}
			}
		}
		/* End of row, estimate relative accuracy */
		if ( fabs( w ) <= fabs( accuracy * OrigMat[row*n+col] ) )
		{
			UMat[row*n+col] = 0.0;
		}
		else
		{
			if ( w < 0.0 )
			{
				*fault = 2;
				return;
			}
			UMat[row*n+col] = sqrt( w );
		}
	}
	return;
}

/**************************************************************************************/


/*===========================================================================*
* common.c
* Utility functions commonly used by MEX source files
*
* Copyright 2008 Levente Hunyadi
* All rights reserved.
*===========================================================================*/

/* This file is not intended to be compiled directly. Instead, include it in
 * other MEX source files using the #include preprocessor directive:
 *
 *    #include "common.c"
 */

/* MEX functions throw MatLab exceptions with the following scheme:
 *
 *    package_name:function_name:exception_type
 *
 * where
 *
 * - package_name  is the package that contains the function, e.g. math
 *
 * - function_name  is the source file name or, equivalently, the identifier
 *   with which the function is invoked from MatLab
 *
 * - exception_type  is one of
 *   - ArgumentCountMismatch  if too many or too few input or output arguments
 *     are given
 *   - ArgumentTypeMismatch  if the arguments are not of the expected type
 *   - DimensionMismatch  if arguments are not of the expected dimensions
 *   - InvalidValue  if any argument is of or contains an inappropriate value
 *   - OutOfMemory
 */

#include "mexutil.h"

/**
* Test whether array is two-dimensional.
*/
__INLINE__ bool is2d(const mxArray* arr) {
	mwSize d = mxGetNumberOfDimensions(arr);

	return d == 2;
}

__INLINE__ bool is3d(const mxArray* arr) {
	return mxGetNumberOfDimensions(arr) == 3;
}

/**
* Test whether array is a square matrix.
*/
__INLINE__ bool issquare(const mxArray* arr) {
	mwSize m = mxGetM(arr);
	mwSize n = mxGetN(arr);

	return m == n;
}

/**
* Test whether array is a vector shape.
*/
__INLINE__ bool isvector(const mxArray* arr) {
	mwSize m = mxGetM(arr);
	mwSize n = mxGetN(arr);

	return m == 1 || n == 1;
}

/**
* Test whether array represents a scalar.
*/
__INLINE__ bool isscalar(const mxArray* arr) {
	mwSize m = mxGetM(arr);
	mwSize n = mxGetN(arr);

	return m == 1 && n == 1;
}

/**
* Test whether array is empty.
*/
__INLINE__ bool isnonempty(const mxArray* arr) {
	mwSize m = mxGetM(arr);
	mwSize n = mxGetN(arr);

	return m*n > 0;  /* safe for sparse arrays */
}

/**
* Number of elements in array.
*/
__INLINE__ mwSize numel(const mxArray* arr) {
	if (mxIsSparse(arr)) {
		mwSize m = mxGetM(arr);
		mwSize n = mxGetN(arr);
		return m*n;
	} else {
		return mxGetNumberOfElements(arr);
	}
}

/**
* Number of nonzero elements in numeric array.
*/
mwSize nonzerocount(const mxArray* arr) {
	if (mxIsCell(arr)) {
		mwSize n = mxGetNumberOfElements(arr);
		mwSize c = 0;
		mwIndex ix;

		for (ix = 0; ix < n; ix++) {
			const mxArray* cell = mxGetCell(arr, ix);
			if (cell != NULL) {
				c += nonzerocount(cell);
			}
		}
		return c;
	} else if (mxIsStruct(arr)) {
		mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Structure arrays are not supported.");
		return 0;
	} else if (mxIsSparse(arr)) {
		mwIndex* jc = mxGetJc(arr);
		mwSize n = mxGetN(arr);  /* number of columns */
		return jc[n];  /* number of nonzero elements */
	} else {  /* filter zero elements */
		mwSize n = mxGetNumberOfElements(arr);
		mwSize c = 0;
		const double* pr = mxGetPr(arr);
		const double* pi = mxGetPi(arr);
		mwSize i;

		if (pr == NULL) {
			mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Operation supported only on numeric arrays.");
		}

		if (pi != NULL) {  /* real and imaginary part */
			for (i = 0; i < n; i++) {
				if (isnonzero(*(pr++)) || isnonzero(*(pi++))) {
					c++;
				}
			}
		} else {  /* real part only */
			for (i = 0; i < n; i++) {
				if (isnonzero(*(pr++))) {
					c++;
				}
			}
		}
		return c;
	}
}

/**
* Test if any element in an array is a complex number.
*/
bool isanycomplex(const mxArray* arr) {
	if (mxIsCell(arr)) {
		mwIndex n = mxGetNumberOfElements(arr);
		mwIndex ix;

		for (ix = 0; ix < n; ix++) {
			const mxArray* cell = mxGetCell(arr, ix);
			if (cell != NULL && isanycomplex(cell)) {
				return true;
			}
		}
		return false;
	} else if (mxIsStruct(arr)) {
		mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Structure arrays are not supported.");
		return false;
	} else {
		return mxIsComplex(arr);
	}
}

/**
* Test if all elements in an array can be interpreted as integers.
*/
bool isallinteger(const mxArray* arr) {
	if (mxIsCell(arr)) {
		mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Cell arrays are not supported.");
		return false;
	} else if (mxIsStruct(arr)) {
		mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Structure arrays are not supported.");
		return false;
	} else if (mxIsSparse(arr)) {
		mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Sparse integer arrays are not supported.");
		return false;
	} else {
		mwSize n = mxGetNumberOfElements(arr);
		const double* pr = mxGetPr(arr);
		const double* pi = mxGetPi(arr);
		mwSize i;

		if (pr == NULL) {
			mexErrMsgIdAndTxt(__ARGTYPEMISMATCH__, "Operation supported only on numeric arrays.");
		}

		if (pi != NULL) {  /* real and imaginary part */
			for (i = 0; i < n; i++) {
				if (!isinteger(*(pr++)) || !isinteger(*(pi++))) {
					return false;
				}
			}
		} else {  /* real part only */
			for (i = 0; i < n; i++) {
				if (!isinteger(*(pr++))) {
					return false;
				}
			}
		}
		return true;
	}
}

/**
* Prints a matrix M to the MatLab standard output.
*/
void printmatrix(double* M, int rows, int cols) {
	mxArray* matrix = mxCreateDoubleMatrix(rows, cols, mxREAL);	
	double* matrixcell = mxGetPr(matrix);
	int i;
	/* memcpy(matrixcells, M, rows*cols*sizeof(double)); */
	for (i = 0; i < rows*cols; i++) {
		*(matrixcell++) = *(M++);
	}
	mexCallMATLAB(0, NULL, 1, &matrix, "disp");
}

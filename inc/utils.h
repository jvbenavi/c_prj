/* utils.h */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

/* deadband routines (deadband.c) */
double deadband(double value, double min, double max);

/* interp routines (interp.c) */
double interp1(double *x, double *y, double a);
double interp2(double *x, double *y, double **z, double a, double b);
double interp3(double *x, double *y, double *z, double ***D,  double a, double b, double c);
double interp4(double *w, double *x, double *y, double *z, double ****D,  double a, double b, double c, double d);

/* filter routines (lag.c, leadlag.c, filter2.c) */
void lag(double x, double xold, double *y, double tau, double dt);
void leadlag(double x, double xold, double *y,
             double a, double b, double c, double d, double dt);
void filter2(double u, double *xdd, double *xd, double *x, double *y,
double b2, double b1, double b0, double a1, double a0, double dt);

/* limit routines (limit.c) */
double limit(double value, double min, double max);
float  flimit(float value, float min, float max);
double limit_angle(double value, double max);
float  flimit_angle(float value, float max);

/* load routines (load.c) */
void   load (char *, double *);
void   loadm (char *, double **);
void   load3 (char *, double ***);
void   load4 (char *, double ****);
void   sload (char *, double *);
void   sloadm (char *, double **);
void   sload3 (char *, double ***);
void   sload4 (char *, double ****);

/* matrix routines (matrix.c) */
void   free_imatrix(int **m, int row_low,int row_high,int col_low);
void   free_imatrixn(int **m);
int    **imatrix(int row_low,int row_high,int col_low,int col_high, char *message);
int    **imatrixn(int row_high,int col_high, char *message);

void   free_matrix(double **, int, int, int, int); 
void   free_matrixn(double **); 
void   free_matrix3(double ***); 
void   free_matrix4(double ****); 
double **matrix(int, int, int, int, char *);
double **matrixn(int, int, char *);
double ***matrix3(int, int, int, char *);
double ****matrix4(int, int, int, int, char *);

int    matrix_set(double **, double);
int    matrix_set3(double ***, double);
int    matrix_set4(double ****, double);

int    matrix_add(double **,double **,double **);
int    matrix_sub(double **,double **,double **);
int    matrix_mult(double **,double **,double **);
double *Matrix_VecMult(double *a,double **b,double *c);
int    matrix_inv(double **, double **);

void   ludcmp(double **a,int n,int *indx,double *d);
void   lubksb(double **a,int n,int *indx,double *b);

/* round angles (ra.c) */
double ra180(double angle);
double ra360(double angle);

/* sodano's equations (soddir.c, sodinv.c) */
void soddir(double lat1, double lon1, double gcd, double brg12, double re,
            int flbg21, double *lat2, double *lon2, double *brg21);
void sodinv(double lat1, double lon1, double lat2, double lon2, double re,
            int flbg21, double *gcd, double *brg12, double *brg21);

/* uniform random number generator mt19937, mersenne twister (mt19937.c) */
void init_genrand(unsigned long s);
void init_by_array(unsigned long init_key[], int key_length);
unsigned long genrand_int32(void);
long genrand_int31(void);
double genrand_real1(void);
double genrand_real2(void);
double genrand_real3(void);
double genrand_res53(void);

/* random number routines that use mt19937 */
double randu(double a, double b);  /* randu.c */
double randn(double m, double s);  /* randn.c */

/* roundz routines (round.c) */
int roundz(double value);
int froundz(float value);

/* matlab ported routines (fix.c, sign.c) */
int fix(double in);
int sign(double in);

/* lowercase routine (lowercase.c) */
char *lowercase(char *);

/* strsub routines (strsub.c) */
char *strsub(char *string, char *search, char *replace);

/* trimit routine (trimit.c) */
char *trimit(char *);

/* vector routines (vector.c) */
double *vector(int ,int, char *);
void   free_vector(double *, int, int);
double *vectorn(int, char *);
void   free_vectorn(double *); 
int    *ivector(int, int, char *);
void   free_ivector(int *, int, int);
int    *ivectorn(int, char *);
void   free_ivectorn(int *vec);  
double **vector_ptr(int, int, char *);
void   free_vector_ptr(double **m, int row_low, int row_high);
double *vector_set(double *out, double value);
int    vector_kronecker(double *a, double *b, double *out);

/* vector operators (vector_operators.c) */
double *Vector_Subtract(double *difference,double *in2,double *in1);
double *Vector_Sum(double *sum,double *in2,double *in1);
double *Vector_VectorMin(double *min, double *in1, double *in2);
double *Vector_VectorMax(double *max, double *in1, double *in2);
double  Vector_Max(double *in);
double  Vector_Min(double *in);
int     Vector_IMax(int *in);
int     Vector_IMin(int *in);
double  Vector_Dot(double *in1,double *in2);
double  Vector_RMS(double *arg1, double *arg2);
double *Vector_Mux2(double *outx, double *arg1, double *arg2);
double *Vector_Mux(double *out,  ...);
double *Vector_Switch(double *out, double *in1, double *in2, int fswitch);
double *Vector_Round(double *output, double *input);
double *Vector_VectorSet(double *out, double *in);

/* table lookup routines (zlfi1u.c, zlfi2u.c zlfi3u.c, ztable_load.c) */
float zlfi1u(float tbl[], float x);  /* 1 Dim Function Interpolator */
float zlfi2u(float tbl[], float x, float y);  /* 2 Dim Function Interpolator */
float zlfi3u(float tbl[], float x, float y, float z);  /* 3 Dim Function Interpolator */
float *ztable_load(FILE *fptr, char tblname[]);  /* Loads N Dim data tables */

#endif /* UTILS_H */

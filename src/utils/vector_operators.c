#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

/******************************************************************************/
/*                                                                            */
/*  Library: Vector operators                                                 */
/*                                                                            */
/*  Description:                                                              */
/*                                                                            */
/*  Operators:
		double *Vector_Subtract(double *difference,double *in2,double *in1)
		double *Vector_Sum(double *sum,double *in2,double *in1)
		double  Vector_Max(double *in);
		double  Vector_Min(double *in);
		int     Vector_IMax(int *in);
		int     Vector_IMin(int *in);
		double  Vector_Dot(double *in1,double *in2)
		double *Vector_Mux(double *out,  ...)
		double *Vector_Switch(double *out, double *in1, double *in2, int fswitch)
		double *Vector_Round(double *out, double *in)
                                                                              */
/*                                                                            */
/******************************************************************************/

double *Vector_Subtract(double *difference,double *in2,double *in1)
{
    int n;

    if (difference==NULL) difference=vectorn((int)in1[0],"Vector_Subtract - difference");

    if ( ((int)in1[0] != (int)in2[0]) || ((int)difference[0] != (int)in2[0]) ) 
    {
        printf("\n%%UTILS-E-FATAL, %s\n","Vector_Subtract");
    }

    for (n = 1; n <= difference[0]; n++)
        difference[n] = in2[n] - in1[n];

    return(difference);
}             

/******************************************************************************/

double *Vector_Sum(double *sum,double *in2,double *in1)
{
 int n;

 if (in1==NULL)
 {
   if (sum==NULL) sum=vectorn((int)in2[0],"Vector_Sum - sum");

   if ( ((int)sum[0] != (int)in2[0]) ) printf("\n%%UTILS-E-FATAL, %s\n","Vector_Sum");

   for (n = 1; n <= sum[0]; n++)
     sum[n] = in2[n];
 }

 else if (in2==NULL)
 {
   if (sum==NULL) sum=vectorn((int)in1[0],"Vector_Sum - sum");

   if ( ((int)sum[0] != (int)in1[0]) ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Sum");

   for (n = 1; n <= sum[0]; n++)
     sum[n] = in1[n];
 }

 else
 {
   if (sum==NULL) sum=vectorn((int)in1[0],"Vector_Sum - sum");

   if ( ((int)in1[0] != (int)in2[0]) || ((int)sum[0] != (int)in2[0]) )
   {
     printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Sum");
   }

   for (n = 1; n <= sum[0]; n++)
     sum[n] = in2[n] + in1[n];
 }

 return(sum);
}             

/******************************************************************************/

double *Vector_VectorMin(double *min, double *in1, double *in2)
{
 int n;
 
 if (min==NULL) min=vectorn((int)in1[0],"Vector_Min - min");

 if ( ((int)in1[0] != (int)in2[0]) || ((int)min[0] != (int)in2[0]) )
 {
   printf("\n%%UTILS-E-FATAL, %s\n", "Vector_VectorMin");
 }

 for (n = 1; n <= (int)in1[0]; n++)
 {
	if (in1[n] < in2[n]) 
		min[n]=in1[n];
	else
		min[n]=in2[n];
 }

 return(min);
}             

/******************************************************************************/

double *Vector_VectorMax(double *max, double *in1, double *in2)
{
 int n;

 if (max==NULL) max=vectorn((int)in1[0],"Vector_Max - max");

 if ( ((int)in1[0] != (int)in2[0]) || ((int)max[0] != (int)in2[0]) )
  {
    printf("\n%%UTILS-E-FATAL, %s\n", "Vector_VectorMax");
  }
 
 for (n = 1; n <= (int)in1[0]; n++)
 {
	if (in1[n] > in2[n]) 
		max[n]=in1[n];
	else
		max[n]=in2[n];
 }

 return(max);
}             

/******************************************************************************/

double Vector_Min(double *in)
{
 int n;
 double min;

 min = in[1];
 for (n = 2; n <= (int)in[0]; n++)
   if (min > in[n]) min=in[n];

 return(min);
}             

/******************************************************************************/

double Vector_Max(double *in)
{
 int n;
 double max;

 max = in[1];
 for (n = 2; n <= (int)in[0]; n++)
   if (max < in[n]) max=in[n];

 return(max);
}             

/******************************************************************************/

int Vector_IMin(int *in)
{
 int n;
 int min;

 min = in[1];
 for (n = 2; n <= in[0]; n++)
   if (min > in[n]) min=in[n];

 return(min);
}             

/******************************************************************************/

int Vector_IMax(int *in)
{
 int n;
 int max;

 max = in[1];
 for (n = 2; n <= in[0]; n++)
   if (max < in[n]) max=in[n];

 return(max);
}             

/******************************************************************************/

double Vector_Dot(double *in1,double *in2)
{
 int n
    ;
 double dot;

 if ((int)in1[0] != (int)in2[0]) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Dot");
 
 dot=0.0;
 for (n = 1; n <= in1[0]; n++)
   dot = dot + in1[n] * in2[n];

 return(dot);
}             

/******************************************************************************/

double Vector_RMS(double *arg1, double *arg2) 
{
	double 	ms = 0.0F;
	int		i;
	int 	dim;

	if(arg1[0] != arg2[0])
		printf("\n%%UTILS-E-FATAL, %s\n","rms vector size mismatch");

	dim = (int)arg1[0];

	for (i = 1; i <= dim; i++)
		ms += ((arg1[i] - arg2[i]) * (arg1[i] - arg2[i]));

	return(sqrt(ms / (float)dim));

}

/******************************************************************************/

double *Vector_Mux2(double *out, double *arg1, double *arg2)
{   
 int n ;

 if (out==NULL) out=vectorn((int)arg1[0]+(int)arg2[0],"Vector_Mux2 - out");
 
 for (n=1; n<=(int)arg1[0]; n++)
   out[n]=arg1[n];
 for (n=1; n<=(int)arg2[0]; n++)
   out[n+(int)arg1[0]]=arg2[n];

 return(out);
}

/******************************************************************************/

double *Vector_Mux(double *out,  ...)
{   
 int n
    ,index=0
    ;

 double *in;
 
 va_list arg_marker;

 va_start(arg_marker, out);
  
 do
  {
   in=va_arg(arg_marker,double *);  
   if ( index+(int)in[0] > (int)out[0] ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Mux");
   for (n=1; n<=(int)in[0]; n++)
     out[n+index]=in[n];
   index = index + (int)in[0];
  } while (	index != (int)out[0] );

 return(out);
}

/******************************************************************************/

double *Vector_Switch(double *out, double *in1, double *in2, int fswitch)
{
 int i;

 
 if (fswitch == 1)
 {
   if (out==NULL) out=vectorn((int)in2[0],"Vector_Switch - out");
   if ( (int)out[0] > (int)in2[0] ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Switch");
 }
 else
 {
   if (out==NULL) out=vectorn((int)in1[0],"Vector_Switch - out");
   if ( (int)out[0] > (int)in1[0] ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Switch");
 }

 for (i = 1; i <= out[0]; i++)
  {
   if (fswitch == 1)
     out[i] = in2[i];
   else
     out[i] = in1[i];
  }
  
 return(out);
}

/******************************************************************************/

double *Vector_Round(double *out, double *in)
{
 int i; 

 if (out==NULL) out=vectorn((int)in[0],"Vector_Round - out");

 if ( ((int)out[0] != (int)in[0]) ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_Round");
 
 for (i=1; i<=in[0]; i++)
  out[i]=(double)((int) (in[i]+0.5));  
 return(out);
} 

/******************************************************************************/

double *Vector_VectorSet(double *out, double *in)
{
 int i; 

 if (out==NULL) out=vectorn((int)in[0],"Vector_Switch - out");

 if ( ((int)out[0] != (int)in[0]) ) printf("\n%%UTILS-E-FATAL, %s\n", "Vector_VectorSet");

 for (i=1; i<=out[0]; i++)
  out[i]=in[i];  
 return(out);
} 


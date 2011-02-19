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
 *  interp.c
 */

/*%
%##  Interpolation Routines
%##    Don Soloway
%##    MS: 269-1
%##    Moffett Field, CA. 94035
%##    (650)604-6558
%##    don@infi.net
%
%##  Major sections include:
%##    interp1(), one-dimensional interpolation
%##    interp2(), two-dimensional interpolation
%##    interp3(), three-dimensional interpolation
%##    interp4(), four-dimensional interpolation
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

double interp1(double *x, double *y, double a)
{
    double result;
    int i;
    int xi=0;
    int xj;
    double ax;
    double t;

    xj = (int)x[0];
    ax = limit(a, x[1], x[xj]);

    /* special processing for single element vector */
    if (xj==1) return(result=y[xj]);

    i=0;
    do
    {
        i++;  
        if (x[i] > ax) xi=i-1;
        if (i >= xj) xi = xj-1;
    } while (xi==0);

    t = (ax - x[xi]) / (x[xi+1] - x[xi]);
    
    /* This is transposed from the orginal way implemented */
    result = ((1-t) * y[xi]) + (t * y[xi+1]);

    return(result);
}

/****************************************************************************/

double interp2(double *x, double *y, double **z, double a, double b)
{
    double result;
    int i;
    int xi=0, yi=0;
    int xj, yj;
    double ax, by;
    double t, u;
    double *tmp;

    xj = (int)x[0];
    yj = (int)y[0];
    ax = limit(a, x[1], x[xj]);
    by = limit(b, y[1], y[yj]);

    /* special processing for 1x1 matrix */
    if ((xj==1) && (yj==1))
    {
        result = z[1][1];
        return(result);
    }
    /* special processing for 1xN matrix */
    else if ((xj==1) && (yj!=1))
    {
        tmp=vectorn(yj,"interp2");
        for (i=1;i<=yj;i++) tmp[i]=z[1][i];
        result=interp1(y,tmp,by);
        free_vectorn(tmp);
        return(result);
    }
    /* special processing for Nx1 matrix */
    else if ((xj!=1) && (yj==1))
    {
        tmp=vectorn(xj,"interp2");
        for (i=1;i<=xj;i++) tmp[i]=z[i][1];
        result=interp1(x,tmp,ax);
        free_vectorn(tmp);
        return(result);
    }

    i=0;
    do
    {
        i++;  
        if (x[i] > ax) xi=i-1;
        if (i >= xj) xi = xj-1;
    } while (xi==0);

    i=0;
    do
    {
        i++;  
        if (y[i] > by) yi=i-1;
        if (i >= yj) yi = yj-1;
    } while (yi==0);

    t = (ax - x[xi]) / (x[xi+1] - x[xi]);
    u = (by - y[yi]) / (y[yi+1] - y[yi]);
    
    result = 
    ((1-t)*(1-u) * z[xi][yi]) + 
    ((  t)*(1-u) * z[xi+1][yi]) + 
    ((1-t)*(  u) * z[xi][yi+1]) +
    ((  t)*(  u) * z[xi+1][yi+1])
	;

    return(result);
}

/****************************************************************************/

double interp3(double *x, double *y, double *z,  double ***D, double m, double a, double b)
{
    double result;
    int i,j;
    int xi=0, yi=0, zi=0;
    int xj, yj, zj;
    double ax, by, cz;
    double t, u, v;
    double *tmp;
    double **tmp2;

    xj = (int)x[0];
    yj = (int)y[0];
    zj = (int)z[0];
    ax = limit(m, x[1], x[xj]);
    by = limit(a, y[1], y[yj]);
    cz = limit(b, z[1], z[zj]);

    /* special processing for 1x1x1 matrix */
    if ((xj==1) && (yj==1) && (zj==1)) {
        result = D[1][1][1];
        return(result);
    }
    /* special processing for 1x1xN matrix */
    else if ((xj==1) && (yj==1) && (zj!=1)) {
        tmp=vectorn(zj,"interp3");
        for (j=1;j<=zj;j++)   tmp[j]=D[1][1][j];
        result = interp1( z, tmp, cz );
        free_vectorn(tmp);
        return(result);
	}
    /* special processing for 1xNx1 matrix */
    else if ((xj==1) && (yj!=1) && (zj==1)) {
        tmp=vectorn(yj,"interp3");
        for (j=1;j<=yj;j++)   tmp[j]=D[1][j][1];
        result = interp1( y, tmp, by );
        free_vectorn(tmp);
        return(result);
	}
    /* special processing for Nx1x1 matrix */
    else if ((xj!=1) && (yj==1) && (zj==1)) {
        tmp=vectorn(xj,"interp3");
        for (i=1;i<=xj;i++)   tmp[i]=D[i][1][1];
        result = interp1( x, tmp, ax );
        free_vectorn(tmp);
        return(result);
	}
    /* special processing for 1xNxN matrix */
    else if ( (xj==1) && (yj!=1) && (zj!=1) ) {
	    tmp2=matrixn(yj,zj,"interp3");
	    for (i=1;i<=yj;i++)   for (j=1;j<=zj;j++)   tmp2[i][j]=D[1][i][j];
	    result=interp2(y,z,tmp2,by,cz);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for Nx1xN matrix */
    else if ( (xj!=1) && (yj==1) && (zj!=1) ) {
	    tmp2=matrixn(xj,zj,"interp3");
	    for (i=1;i<=xj;i++)   for (j=1;j<=zj;j++)   tmp2[i][j]=D[i][1][j];
	    result=interp2(x,z,tmp2,ax,cz);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for NxNx1 matrix */
    else if ( (xj!=1) && (yj!=1) && (zj==1) ) {
	    tmp2=matrixn(xj,yj,"interp3");
	    for (i=1;i<=xj;i++)   for (j=1;j<=yj;j++)   tmp2[i][j]=D[i][j][1];
	    result=interp2(x,y,tmp2,ax,by);
        free_matrixn(tmp2);
        return(result);
    }
    
    i=0;
    do {
        i++;  
        if (x[i] > ax) xi=i-1;
        if (i >= xj) {  xi = xj-1;   break;  }
    } while (xi==0);

    i=0;
    do {
        i++;  
        if (y[i] > by) yi=i-1;
        if (i >= yj) {  yi = yj-1;  break; }
    } while (yi==0);

    i=0;
    do {
        i++;  
        if (z[i] > cz) zi=i-1;
        if (i >= zj) { zi = zj-1;  break; }
    } while (zi==0);

    t = (ax - x[xi]) / (x[xi+1] - x[xi]);
    u = (by - y[yi]) / (y[yi+1] - y[yi]);
    v = (cz - z[zi]) / (z[zi+1] - z[zi]);
    
    result = 
	((1-t)*(1-u)*(1-v) * D[xi][yi][zi]) + 
	((  t)*(1-u)*(1-v) * D[xi+1][yi][zi]) + 
	((1-t)*(  u)*(1-v) * D[xi][yi+1][zi]) +
	((  t)*(  u)*(1-v) * D[xi+1][yi+1][zi]) +
	((1-t)*(1-u)*(  v) * D[xi][yi][zi+1]) + 
	((  t)*(1-u)*(  v) * D[xi+1][yi][zi+1]) + 
	((1-t)*(  u)*(  v) * D[xi][yi+1][zi+1]) +
	((  t)*(  u)*(  v) * D[xi+1][yi+1][zi+1])
	;

    return(result);
}

/****************************************************************************/

double interp4(double *w, double *x, double *y, double *z,  double ****D, double a, double b, double c, double d)
{
    double result;
    int i,j,k;
    int wi=0, xi=0, yi=0, zi=0;
    int wj, xj, yj, zj;
	double aw, bx, cy, dz;
	double s, t, u, v;
    double *tmp;
    double **tmp2;
	double ***tmp3;

    wj = (int)w[0];
    xj = (int)x[0];
    yj = (int)y[0];
    zj = (int)z[0];
    aw = limit(a, w[1], w[wj]);
    bx = limit(b, x[1], x[xj]);
    cy = limit(c, y[1], y[yj]);
    dz = limit(d, z[1], z[zj]);

    /* special processing for 1x1x1x1 matrix */
    if ((wj==1) && (xj==1) && (yj==1) && (zj==1)) {
        result = D[1][1][1][1];
        return(result);
    }
    /* special processing for 1x1x1xN matrix */
    else if ((wj==1) && (xj==1) && (yj==1) && (zj!=1)) {
        tmp=vectorn(zj,"interp4");
        for (j=1;j<=zj;j++)   tmp[j]=D[1][1][1][j];
        result = interp1( z, tmp, dz );
        free_vectorn(tmp);
        return(result);
    }
    /* special processing for 1x1xNx1 matrix */
    else if ((wj==1) && (xj==1) && (yj!=1) && (zj==1)) {
        tmp=vectorn(yj,"interp4");
        for (j=1;j<=yj;j++)   tmp[j]=D[1][1][j][1];
        result = interp1( y, tmp, cy );
        free_vectorn(tmp);
        return(result);
    }
    /* special processing for 1xNx1x1 matrix */
    else if ((wj==1) && (xj!=1) && (yj==1) && (zj==1)) {
        tmp=vectorn(xj,"interp4");
        for (j=1;j<=xj;j++)   tmp[j]=D[1][j][1][1];
        result = interp1( x, tmp, bx );
        free_vectorn(tmp);
        return(result);
    }
    /* special processing for Nx1x1x1 matrix */
    else if ((wj!=1) && (xj==1) && (yj==1) && (zj==1)) {
        tmp=vectorn(wj,"interp4");
        for (j=1;j<=wj;j++)   tmp[j]=D[j][1][1][1];
        result = interp1( w, tmp, aw );
        free_vectorn(tmp);
        return(result);
    }
    /* special processing for 1x1xNxN matrix */
    else if ((wj==1) && (xj==1) && (yj!=1) && (zj!=1)) {
	    tmp2=matrixn(yj,zj,"interp4");
	    for (i=1;i<=yj;i++)   for (j=1;j<=zj;j++)   tmp2[i][j]=D[1][1][i][j];
	    result=interp2(y,z,tmp2,cy,dz);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for 1xNx1xN matrix */
    else if ((wj==1) && (xj!=1) && (yj==1) && (zj!=1)) {
	    tmp2=matrixn(xj,zj,"interp4");
	    for (i=1;i<=xj;i++)   for (j=1;j<=zj;j++)   tmp2[i][j]=D[1][i][1][j];
	    result=interp2(x,z,tmp2,bx,dz);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for 1xNxNx1 matrix */
    else if ((wj==1) && (xj!=1) && (yj!=1) && (zj==1)) {
	    tmp2=matrixn(xj,yj,"interp4");
	    for (i=1;i<=xj;i++)   for (j=1;j<=yj;j++)   tmp2[i][j]=D[1][i][j][1];
	    result=interp2(x,y,tmp2,bx,cy);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for Nx1x1xN matrix */
    else if ((wj!=1) && (xj==1) && (yj==1) && (zj!=1)) {
	    tmp2=matrixn(wj,zj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=zj;j++)   tmp2[i][j]=D[i][1][1][j];
	    result=interp2(w,z,tmp2,aw,dz);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for Nx1xNx1 matrix */
    else if ((wj!=1) && (xj==1) && (yj!=1) && (zj==1)) {
	    tmp2=matrixn(wj,yj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=yj;j++)   tmp2[i][j]=D[i][1][j][1];
	    result=interp2(w,y,tmp2,aw,cy);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for NxNx1x1 matrix */
    else if ((wj!=1) && (xj!=1) && (yj==1) && (zj==1)) {
	    tmp2=matrixn(wj,xj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=xj;j++)   tmp2[i][j]=D[i][j][1][1];
	    result=interp2(w,x,tmp2,aw,bx);
        free_matrixn(tmp2);
        return(result);
    }
    /* special processing for 1xNxNxN matrix */
    else if ((wj==1) && (xj!=1) && (yj!=1) && (zj!=1)) {
	    tmp3=matrix3(xj,yj,zj,"interp4");
	    for (i=1;i<=xj;i++)   for (j=1;j<=yj;j++)   for(k=1;k<=zj;k++)   tmp3[i][j][k]=D[1][i][j][k];
	    result=interp3(x,y,z,tmp3,bx,cy,dz);
        free_matrix3(tmp3);
        return(result);
    }
    /* special processing for Nx1xNxN matrix */
    else if ((wj!=1) && (xj==1) && (yj!=1) && (zj!=1)) {
	    tmp3=matrix3(wj,yj,zj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=yj;j++)   for(k=1;k<=zj;k++)   tmp3[i][j][k]=D[i][1][j][k];
	    result=interp3(w,y,z,tmp3,aw,cy,dz);
        free_matrix3(tmp3);
        return(result);
    }
    /* special processing for NxNx1xN matrix */
    else if ((wj!=1) && (xj!=1) && (yj==1) && (zj!=1)) {
	    tmp3=matrix3(wj,xj,zj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=xj;j++)   for(k=1;k<=zj;k++)   tmp3[i][j][k]=D[i][j][1][k];
	    result=interp3(w,x,z,tmp3,aw,bx,dz);
        free_matrix3(tmp3);
        return(result);
    }
    /* special processing for NxNxNx1 matrix */
    else if ((wj!=1) && (xj!=1) && (yj!=1) && (zj==1)) {
	    tmp3=matrix3(wj,xj,yj,"interp4");
	    for (i=1;i<=wj;i++)   for (j=1;j<=xj;j++)   for(k=1;k<=yj;k++)   tmp3[i][j][k]=D[i][j][k][1];
	    result=interp3(w,x,y,tmp3,aw,bx,cy);
        free_matrix3(tmp3);
        return(result);
    }
    
    i=0;
    do {
        i++;  
        if (w[i] > aw) wi=i-1;
        if (i >= wj) {  wi = wj-1;   break;  }
    } while (wi==0);

    i=0;
    do {
        i++;  
        if (x[i] > bx) xi=i-1;
        if (i >= xj) {  xi = xj-1;   break;  }
    } while (xi==0);

    i=0;
    do {
        i++;  
        if (y[i] > cy) yi=i-1;
        if (i >= yj) {  yi = yj-1;  break; }
    } while (yi==0);

    i=0;
    do {
        i++;  
        if (z[i] > dz) zi=i-1;
        if (i >= zj) { zi = zj-1;  break; }
    } while (zi==0);

    s = (aw - w[wi]) / (w[wi+1] - w[wi]);
    t = (bx - x[xi]) / (x[xi+1] - x[xi]);
    u = (cy - y[yi]) / (y[yi+1] - y[yi]);
    v = (dz - z[zi]) / (z[zi+1] - z[zi]);

    result = 
	((1-s)*(1-t)*(1-u)*(1-v) * D[wi][xi][yi][zi]) + 
	((  s)*(1-t)*(1-u)*(1-v) * D[wi+1][xi][yi][zi]) + 
	((1-s)*(  t)*(1-u)*(1-v) * D[wi][xi+1][yi][zi]) +
	((  s)*(  t)*(1-u)*(1-v) * D[wi+1][xi+1][yi][zi]) +
	((1-s)*(1-t)*(  u)*(1-v) * D[wi][xi][yi+1][zi]) + 
	((  s)*(1-t)*(  u)*(1-v) * D[wi+1][xi][yi+1][zi]) + 
	((1-s)*(  t)*(  u)*(1-v) * D[wi][xi+1][yi+1][zi]) +
	((  s)*(  t)*(  u)*(1-v) * D[wi+1][xi+1][yi+1][zi]) +
	((1-s)*(1-t)*(1-u)*(  v) * D[wi][xi][yi][zi+1]) + 
	((  s)*(1-t)*(1-u)*(  v) * D[wi+1][xi][yi][zi+1]) + 
	((1-s)*(  t)*(1-u)*(  v) * D[wi][xi+1][yi][zi+1]) +
	((  s)*(  t)*(1-u)*(  v) * D[wi+1][xi+1][yi][zi+1]) +
	((1-s)*(1-t)*(  u)*(  v) * D[wi][xi][yi+1][zi+1]) + 
	((  s)*(1-t)*(  u)*(  v) * D[wi+1][xi][yi+1][zi+1]) + 
	((1-s)*(  t)*(  u)*(  v) * D[wi][xi+1][yi+1][zi+1]) +
	((  s)*(  t)*(  u)*(  v) * D[wi+1][xi+1][yi+1][zi+1])
	;

    return(result);
}

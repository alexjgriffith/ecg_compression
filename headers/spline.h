#ifndef SPL_H_
#define SPL_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common_functions.h"

double analyze_2_order(double d2_buffer[1000],double d1_buffer[1000],int length,int start);

int pick_knots(int threshold,int window);

double * curv1(int n,int* x, double* y,double sigma);

double curv2(int n,int * x,double *y,double sigma,double *yp, int t);

int spline_test(double sigma);

#endif

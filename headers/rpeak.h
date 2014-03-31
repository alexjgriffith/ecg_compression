#ifndef RPK_H_
#define RPK_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common_functions.h"
#include "fourier.h"

int difference(double *y,int length);

double band_pass(double w1,double w2, double order, double ripple,double i);

int cheb(double f, double order, double ripple, double i,double *filter);

int triangle_filter(double *y ,  int length);

int shannon_entropy_envelope(double *y, int length);

int normalize(double *y,int length);

int falserdetection(double * filtered_data, double *garbage, int peak,int trof,int ppeak,int f,int * FN,int *RF,int length);

#endif

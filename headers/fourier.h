#ifndef SPL_H_
#define SPL_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common_functions.h"

int test_fft(int size,double *x,double fs);

long determine_power(int length);

int fft(int dir,long m, double *x, double *y);

#endif

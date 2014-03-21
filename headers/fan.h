#ifndef FAN_H_
#define FAN_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common_functions.h"


int fan_check_value(double * su, double * sl, double point_a,double point_b,double error);

int fan_compress(double error);

int fan_decompress();

#endif

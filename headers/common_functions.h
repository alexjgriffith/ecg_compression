#ifndef COM_H_
#define COM_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "fan.h"


void split_token(char trim[256], char del[],  int i);

int load_from_stdin(double * v);

int modulate(double float_data , int size,double min,double max);

double demodulate(int int_data , int size,double min,double max);

double mean(double * data, int start, int stop);

double max(double * data, int start, int stop);

int  max_abs_location(double * data, int start, int stop);

#endif

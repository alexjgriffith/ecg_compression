#ifndef ENT_H_
#define ENT_H_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "common_functions.h"

int dpcm_compress_2nd();


typedef struct huffman_code huffman_code;

int dcpm_decompress ();

void huffman_aux_function(huffman_code point,int key);

int build_huffman_code(int length);

#endif


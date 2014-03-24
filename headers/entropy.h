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

int dpcm_decompress ();

void huffman_aux_function_fixed(huffman_code point,int key, int depth);

void huffman_aux_function_variable(huffman_code point,char key[12], int depth);

int delta_code();

int build_huffman_code(int length);

int huffman_encode(char file[20]);

int huffman_decode(char file[20]);

#endif


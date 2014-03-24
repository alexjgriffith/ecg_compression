#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include "../headers/common_functions.h"
#include "../headers/fan.h"
#include "../headers/entropy.h"
#include "../headers/spline.h"
int main(int argc,char ** argv)
{
  int ch;
  double error;
  char mode[20];
  int compress=0;
  char huffman_file[20];
  static struct option long_options[] =
    {
      {"mode", required_argument, NULL, 'm'},
      {"error", optional_argument, NULL, 'e'},
      {"huffman-file",optional_argument,NULL,'h'},
      {"decompress",no_argument,NULL,'d'},
      {NULL, 0, NULL, 0}
    };

  while ((ch = getopt_long(argc, argv, "dm:e:h:", long_options, NULL)) != -1)
    {
      switch (ch)
	{
	case 'm':
	  strcpy(mode, optarg); 
	  break;
	case 'e':
	  {
	  sscanf(optarg,"%lf",&error);
          if (error<0)
          {
           printf("imporoper error value\n");
           exit(0);
           }   
	  break;
	  }
        case 'd':
          compress=1;
	  break;
	case 'h':
	  strcpy(huffman_file,optarg);
	  break;
	}
    }

  if(!strcmp(mode,"fan"))
    {
    if(compress==0)
      {
	if(&error==NULL)
	  {
            printf("requires -e or --error value\n");
	    exit(0);
          }
      fan_compress(error);
      }
    if(compress==1)
      fan_decompress();
    }

  if (!strcmp(mode,"dpcm") && compress==0)
    {
      delta_code();
    }
  if (!strcmp(mode,"dpcm") && compress==1)
    {
      dpcm_decompress();
    }
  if (!strcmp(mode,"huffman") && compress==0)
    {
      huffman_encode(huffman_file);
    }
  if (!strcmp(mode,"make-huffman"))
    {
      build_huffman_code(400);
    }
  if(!strcmp(mode,"huffman") && compress==1)
    {
      huffman_decode(huffman_file);
    }
  if(!strcmp(mode,"spline"))
    {
      pick_knots(3,4);
    }
  return 0;
}

